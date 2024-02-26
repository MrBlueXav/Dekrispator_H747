/*
 * interface.c
 *
 *  Created on: Nov 14, 2023
 *      Author: Xavier Halgand
 */
/*------------------------------------------------------------------------------------------------------------------*/
#include "interface.h"

#include <stdint.h>
#include <math.h>

#include "usbh_midi_XH.h"
#include "usb_host.h"
#include "stm32h747i_discovery.h"
#include "stm32_lcd.h"
#include "openamp.h"
#include "constants.h"
#include "binn.h"
#include "soundGen.h"
#include "stm32h747i_discovery_qspi.h"

/*------------------------ Private macro ---------------------------------------------------------------------------*/
#define RPMSG_SERVICE_NAME 			"M4_M7_communication"
#define RX_BUFF_SIZE 				64 /* USB MIDI buffer : max received data 64 bytes */
#define CYC_MAX						(((float)AUDIO_BUFFER_SIZE / 8.0f) * ((float)FREQ_CM7 / (float)SAMPLERATE))

/*--------- for QSPI Flash --------------------*/
#define SUBSECTOR_SIZE				8192
#define MAX_PATCH_SIZE				1024
#define PATCH_MEMORY_START_ADDRESS	0x0

/*------------------------------------------------------------------------------------------------------------------*/
extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHostHS;

/*------------------------------------------------------------------------------------------------------------------*/
static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv);
static void Process_ReceivedMidiDatas(void);
static void midipacket_print(midi_package_t pack);
static int32_t write_patch_to_memory(SynthPatch_t *patch);
static void write_initPatch_to_sector8Kbuffer(SynthPatch_t *patch);

/* -----------------------------------------------------------------------------------------------------------------*/
HSEM_TypeDef *HSEM_DEBUG = HSEM;

/*-------------------------------------- Private variables ---------------------------------------------------------*/
static uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer
static uint32_t oldtick, newtick;
static uint32_t oldtick2, newtick2;
static volatile int message_received;
//static volatile uint32_t received_number;
static struct rpmsg_endpoint rp_endpoint;
static char string_message[100];
static uint8_t sector8Kbuffer[SUBSECTOR_SIZE];
static SynthPatch_t *patch;
static char *strg;
static uint8_t messageBuffer[1024];
static volatile bool SEV_received;
//volatile char sharedBuffer[100];

/* message buffers variables in SRAM4 */
volatile uint8_t *buf_cm4_to_cm7 = (void*) BUFF_CM4_TO_CM7_ADDR;
volatile uint8_t *buf_cm7_to_cm4 = (void*) BUFF_CM7_TO_CM4_ADDR;

/*------------------------------------------------------------------------------------------------------------------*/
void openamp_init(void)
{
	SEV_received = false;

	/* Initialize the mailbox use notify the other core on new message */
	MAILBOX_Init();

	/* Initialize OpenAmp and libmetal libraries */
	if (MX_OPENAMP_Init(RPMSG_REMOTE, NULL) != HAL_OK)
		Error_Handler();

	/* create an endpoint for rmpsg communication */
	int32_t status = 0;
	status = OPENAMP_create_endpoint(&rp_endpoint, RPMSG_SERVICE_NAME, RPMSG_ADDR_ANY, rpmsg_recv_callback, NULL);
	if (status < 0)
	{
		Error_Handler();
	}

	/* Clear message buffers */
	memset((void*) buf_cm4_to_cm7, 0x00, MAX_PATCH_SIZE);
	memset((void*) buf_cm7_to_cm4, 0x00, MAX_PATCH_SIZE);
	message_received = 0;

}
/*----------------------------------------------------------------------------------------------------------------*/
static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv)
{
	memcpy(messageBuffer, data, len); /* copy data to message buffer */
	message_received = 1;
	return 0;
}

/*----------------------------------------------------------------------------------------------------------------*/
void CM7_SEV_signal(void)
{
	SEV_received = true;
}

/*----------------------------------------------------------------------------------------------------------------*/
int32_t write_patch_to_memory(SynthPatch_t *patch)
{
	//uint32_t sectorAddress = PATCH_MEMORY_START_ADDRESS * (1 + (patch->memory_location) / 8); /* Find address of 8k sector in which patch will be written */
	uint32_t sectorAddress = PATCH_MEMORY_START_ADDRESS + SUBSECTOR_SIZE * ((patch->memory_location) / 8); /* Find address of 8k sector in which patch will be written */

	if (BSP_QSPI_Read(0, sector8Kbuffer, sectorAddress, SUBSECTOR_SIZE) != BSP_ERROR_NONE) /* read this 8k sector to buffer before erasing */
	{
		printf("Read error !\n");
		return BSP_ERROR_COMPONENT_FAILURE;
	}

	uint32_t patchAddress = (patch->memory_location % 8) * MAX_PATCH_SIZE; /* Find relative address of patch in the sector */
	memcpy(&sector8Kbuffer[patchAddress], patch, sizeof(*patch)); /* copy patch to buffer */

	if (BSP_QSPI_EraseBlock(0, sectorAddress, BSP_QSPI_ERASE_8K) != BSP_ERROR_NONE) /* erase sector */
	{
		printf("Erase error !\n");
		return BSP_ERROR_COMPONENT_FAILURE;
	}

	if (BSP_QSPI_Write(0, sector8Kbuffer, sectorAddress, SUBSECTOR_SIZE) != BSP_ERROR_NONE)/* write buffer with new patch to qspi flash */
	{
		printf("Write error !\n");
		return BSP_ERROR_COMPONENT_FAILURE;
	}
	return BSP_ERROR_NONE;
}

/*----------------------------------------------------------------------------------------------------------------*/
void write_initPatch_to_sector8Kbuffer(SynthPatch_t *patch) /* Fill sector8Kbuffer with 8 init patches  */
{
	for (int i = 0; i < SUBSECTOR_SIZE; i++) /* erase sector buffer */
	{
		sector8Kbuffer[i] = 0;
	}
	uint32_t patchAddress;
	for (int j = 0; j < 8; j++) /* 8 patches per buffer */
	{
		patchAddress = j * MAX_PATCH_SIZE; /* Find relative address of patch in the sector */
		memcpy(&sector8Kbuffer[patchAddress], patch, sizeof(*patch)); /* copy patch to buffer */
	}
}

/*------------------------------------------------------------------------------------------------*/
void Application_Process(void) // called in main() loop (main_cm4.c)
{

	if (SEV_received) /* a patch has been written by CM7 in the shared buffer */
	{
		printf("SEV signal from CM7 received !\n");
		patch = (SynthPatch_t*) buf_cm7_to_cm4;
		SEV_received = false;
	}

	/* Check for CM7 messages (openAMP) */
	if (message_received == 0)
	{
		OPENAMP_check_for_message();
	}
	if (message_received)
	{
		uint16_t loc;
		uint8_t cmd;
		uint32_t sectorAddress;

		cmd = binn_object_uint8(messageBuffer, "cmd");

		switch (cmd)
		{

		case 'D': /* request for loading a patch */
			loc = binn_object_uint16(messageBuffer, "location");
			uint32_t patchAddress = PATCH_MEMORY_START_ADDRESS + MAX_PATCH_SIZE * loc; /* Find address of memory in which patch will be read */

			if (BSP_QSPI_Read(0, (uint8_t*) buf_cm4_to_cm7, patchAddress, sizeof(*patch)) != BSP_ERROR_NONE) /* read this patch to buffer */
			{
				printf("Read error !\n");
				//return BSP_ERROR_COMPONENT_FAILURE;
			}
			else
			{
				asm("sev");
				/* inform CM7 that patch is ready in shared "buf_cm4_to_cm7" buffer */
				printf("Patch # %u loaded !\n", loc); /* hope it's been loaded ! */
				sprintf(string_message, "Patch # %u loaded !           ", loc);
				UTIL_LCD_DisplayStringAt(201, 297, (uint8_t*) string_message, LEFT_MODE);
			}
			break;

		case 'L': /* print current patch location */
			loc = binn_object_uint16(messageBuffer, "location");
			printf("Current patch location : %u\n", loc);
			sprintf(string_message, "%u ", loc);
			UTIL_LCD_DisplayStringAt(704, 428, (uint8_t*) string_message, LEFT_MODE);
			break;

		case 'P': /* save patch */
			if (patch == NULL)
				printf("patch = NULL !!!");
			else
			{
				if (write_patch_to_memory(patch) == BSP_ERROR_NONE)
				{
					printf("Patch saved in memory # %u !\n", patch->memory_location);
					sprintf(string_message, "Patch saved in memory # %u !  ", patch->memory_location);
					UTIL_LCD_DisplayStringAt(201, 297, (uint8_t*) string_message, LEFT_MODE);
					printf("Size of patch is : %d bytes.\n", sizeof(*patch));
				}
			}
			break;

		case 'R': /* request for erase memory */
			printf("Erase all patches ?\n");
			UTIL_LCD_DisplayStringAt(201, 297, (uint8_t*) "Erase all patches ?           ", LEFT_MODE);
			break;

		case 'E': /* erase memory */
			UTIL_LCD_DisplayStringAt(201, 297, (uint8_t*) "                        ", LEFT_MODE);
			if (BSP_QSPI_EraseBlock(0, PATCH_MEMORY_START_ADDRESS, BSP_QSPI_ERASE_128K) == BSP_ERROR_NONE)
			{
				printf("All patches erased ! \n");
				UTIL_LCD_DisplayStringAt(201, 297, (uint8_t*) "All patches erased !          ", LEFT_MODE);
				//patch = binn_object_blob(messageBuffer, "patch", &received_data_len);
				printf("Size of Init patch is : %d bytes.\n", sizeof(*patch));
				write_initPatch_to_sector8Kbuffer(patch);

				for (int i = 0; i < 4; i++)
				{
					sectorAddress = PATCH_MEMORY_START_ADDRESS + SUBSECTOR_SIZE * i;
					if (BSP_QSPI_Write(0, sector8Kbuffer, sectorAddress, SUBSECTOR_SIZE) != BSP_ERROR_NONE)/* write buffer with 8 init patches to qspi flash */
					{
						printf("Write error !\n");
					}
				}
				printf("Memory initialized !\n");
			}
			else
				printf("Memory can't be erased ! \n");

			break;

		case 'S': /* print any string */
			strg = binn_object_str(messageBuffer, "string");
			printf(strg);
			free(strg);
			break;

		case 'X': /* print CPU M7 load */
			uint32_t received_number = binn_object_uint32(messageBuffer, "number");
			uint32_t occupation_cm7 = (uint32_t) roundf((100 * (float) received_number / CYC_MAX));

			//printf("Nombre de cycles moyen de CM7 = %lu \n", received_number);
			//printf("Taux d'occupation moyen CM7 = %lu %% \n", occupation_cm7);

			//sprintf(string_message, "Average CPU load (M7) = %lu%%   ", occupation_cm7);
			//UTIL_LCD_DisplayStringAt(20, 220, (uint8_t*) string_message, LEFT_MODE);

			sprintf(string_message, "%lu%%", occupation_cm7);
			UTIL_LCD_DisplayStringAt(702, 60, (uint8_t*) string_message, LEFT_MODE);
			break;

		default:
			printf("Unidentified message !\n");
			break;
		}

		message_received = 0;
	}

	if (Appli_state == APPLICATION_RUNNING) /* Check for MIDI messages (if USB MIDI controller connected) */
	{
		/* start a new MIDI reception not faster than 1 per ms */
		newtick = HAL_GetTick();
		if (newtick != oldtick)
		{
			USBH_MIDI_Receive(&hUsbHostHS, MIDI_RX_Buffer, RX_BUFF_SIZE);
			oldtick = newtick;
		}
	}

	if (Appli_state == APPLICATION_READY)
	{
		BSP_LED_On(LED_GREEN);
		//UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "MIDI controller connected !      ", LEFT_MODE);
		UTIL_LCD_DisplayStringAt(610, 58, (uint8_t*) "X", LEFT_MODE);
		Appli_state = APPLICATION_RUNNING;
	}

	if (Appli_state == APPLICATION_DISCONNECT)
	{
		BSP_LED_Off(LED_GREEN);
		//UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "MIDI controller not connected...", LEFT_MODE);
		UTIL_LCD_DisplayStringAt(610, 58, (uint8_t*) "-", LEFT_MODE);
		Appli_state = APPLICATION_IDLE;
		USBH_MIDI_Stop(&hUsbHostHS);
	}

	newtick2 = HAL_GetTick();
	if ((newtick2 - oldtick2) >= 2000)
	{
		printf("CM4 is still alive ! \n");
		oldtick2 = newtick2;
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief  MIDI data receive callback. Redefinition of _weak function defined in usbh_midi_XH.c
 * @param  phost: Host handle
 * @retval None
 */
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
	BSP_LED_On(LED_BLUE);
	Process_ReceivedMidiDatas();
	BSP_LED_Off(LED_BLUE);
}

/*----------------------------------------------------------------------------------------------------------------*/
void midipacket_sendToCM7(midi_package_t packet)
{
	int32_t status = 0;
	status = OPENAMP_send(&rp_endpoint, &packet, sizeof(packet));
	if (status < 0)
	{
		Error_Handler();
	}
}

/*-----------------------------------------------------------------------------*/
void Process_ReceivedMidiDatas(void)
{
	uint16_t numberOfPackets;
	uint8_t *ptr = MIDI_RX_Buffer;
	midi_package_t pack;

	numberOfPackets = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostHS) / 4; //each USB midi package is 4 bytes long

	while (numberOfPackets--)
	{
		pack.cin_cable = *ptr;
		ptr++;
		pack.evnt0 = *ptr;
		ptr++;
		pack.evnt1 = *ptr;
		ptr++;
		pack.evnt2 = *ptr;
		ptr++;

		if (pack.ALL != 0) // if incoming midi message...
		{
			midipacket_sendToCM7(pack);
			midipacket_print(pack);
		}
	}
}
/*------------------------------------------------------------------------------------------------*/
void midipacket_print(midi_package_t pack) //cf. Teensy-MIDI-monitor
{
	uint8_t type = pack.cin;
	uint8_t channel = 1 + pack.chn;
	uint8_t data1 = pack.evnt1;
	uint8_t data2 = pack.evnt2;

	switch (type)
	{
	case NoteOff: // 0x8
		printf("Note Off, ch= %d", channel);
		printf(", note= %d", data1);
		printf(", velocity= %d", data2);
		printf("\n");
		break;

	case NoteOn: // 0x9
		printf("Note On, ch= %d", channel);
		printf(", note= %d", data1);
		printf(", velocity= %d", data2);
		printf("\n");
		break;

	case ControlChange: // 0xB
		printf("Control Change, ch= %d", channel);
		printf(", control= %d", data1);
		printf(", value= %d", data2);
		printf("\n");
		break;

	case ProgramChange: // 0xC
		printf("Program Change, ch= %d", channel);
		printf(", program= %d", data1);
		printf("\n");
		break;

	case Aftertouch: // 0xD
		printf("AfterTouch, ch= %d", channel);
		printf(", pressure= %d", data1);
		printf("\n");
		break;

	case PitchBend: // 0xE
		printf("PitchBend, ch= %d", channel);
		printf(", pitch= %d", data1 + data2 * 128);
		printf("\n");
		break;

	case PolyPressure: // 0xA
		printf("PolyKeyPress, ch= %d", channel);
		printf(", note= %d", data1);
		printf(", velocity= %d", data2);
		printf("\n");
		break;

	default:
		printf("Other MIDI packet : %X, %X, %d, %d   \n", pack.cin_cable, pack.evnt0, pack.evnt1, pack.evnt2);

	}
}

//**************************************************************************
//	case midi_SystemExclusive: // 0xF0
//		// Messages larger than usbMIDI's internal buffer are truncated.
//		// To receive large messages, you *must* use the 3-input function
//		// handler.  See InputFunctionsComplete for details.
//		print("SysEx Message // 0xF0: ");
//		printBytes(MIDI1.getSysExArray(), data1 + data2 * 256);
//		printf("\n");
//		break;
//
//	case midi_TimeCodeQuarterFrame: // 0xF1
//		printf("TimeCode // 0xF1, index=");
//		printf(data1 >> 4, DEC);
//		printf(", digit=");
//		printf(data1 & 15, DEC);
//		printf("\n");
//		break;
//
//	case midi_SongPosition: // 0xF2
//		printf("Song Position // 0xF2, beat=");
//		printf(data1 + data2 * 128);
//		printf("\n");
//		break;
//
//	case midi_SongSelect: // 0xF3
//		printf("Sond Select // 0xF3, song=");
//		printf(data1, DEC);
//		printf("\n");
//		break;
//
//	case midi_TuneRequest: // 0xF6
//		println("Tune Request // 0xF6");
//		printf("\n");
//		break;
//
//	case midi_Clock: // 0xF8
//		printf("Clock // 0xF8");
//		printf("\n");
//		break;
//
//	case midi_Start: // 0xFA
//		printf("Start // 0xFA");
//		printf("\n");
//		break;
//
//	case midi_Continue: // 0xFB
//		printf("Continue // 0xFB");
//		printf("\n");
//		break;
//
//	case midi_Stop: // 0xFC
//		printf("Stop // 0xFC");
//		printf("\n");
//		break;
//
//	case midi_ActiveSensing: // 0xFE
//		printf("Actvice Sensing // 0xFE");
//		printf("\n");
//		break;
//
//	case midi_SystemReset: // 0xFF
//		printf("System Reset // 0xFF");
//		printf("\n");
//		break;
//*************************************************************
