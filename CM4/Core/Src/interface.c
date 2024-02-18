/*
 * interface.c
 *
 *  Created on: Nov 14, 2023
 *      Author: Xavier Halgand
 */
/*------------------------------------------------------------------------------------------------------------------*/
#include "interface.h"

/*------------------------ Private macro ---------------------------------------------------------------------------*/
#define RPMSG_SERVICE_NAME 		"M4_M7_communication"
#define RX_BUFF_SIZE 			64 /* USB MIDI buffer : max received data 64 bytes */
#define CYC_MAX					(((float)AUDIO_BUFFER_SIZE / 8.0f) * ((float)FREQ_CM7 / (float)SAMPLERATE))

/*------------------------------------------------------------------------------------------------------------------*/
extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHostHS;

/*------------------------------------------------------------------------------------------------------------------*/
static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv);
void Process_ReceivedMidiDatas(void);
void midipacket_print(midi_package_t pack);

/* -----------------------------------------------------------------------------------------------------------------*/
HSEM_TypeDef *HSEM_DEBUG = HSEM;

/*-------------------------------------- Private variables ---------------------------------------------------------*/
static uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer
static uint32_t oldtick, newtick;
static volatile int message_received;
static volatile uint32_t received_number;
static int received_data_len;
static struct rpmsg_endpoint rp_endpoint;
static char string_message[100];

/*------------------------------------------------------------------------------------------------------------------*/
void openamp_init(void)
{
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
}
/*----------------------------------------------------------------------------------------------------------------*/

static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv)
{
	//received_number = *((uint32_t*) data);
	//received_data_len = len;
	message_received = 1;
	uint8_t cmd = binn_object_uint8(data, "cmd");

	switch (cmd)
	{

	case 'P': /* save patch */
		SynthPatch_t *patch = binn_object_blob(data, "patch", &received_data_len);
		break;

	case 'S': /* print string */
		char *strg = binn_object_str(data, "string");
		printf(strg);
		break;

	case 'X':
		uint32_t received_number = binn_object_uint32(data, "number");
		printf("Nombre de cycles moyen de CM7 = %lu \n", received_number);
		uint32_t occupation_cm7 = (uint32_t) roundf((100 * (float) received_number / CYC_MAX));
		printf("Taux d'occupation moyen CM7 = %lu %% \n", occupation_cm7);

		sprintf(string_message, "Average CPU load (M7) = %lu %%   ", occupation_cm7);
		UTIL_LCD_DisplayStringAt(20, 220, (uint8_t*) string_message, LEFT_MODE);
		break;

	}

	return 0;
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

/*------------------------------------------------------------------------------------------------*/

void Application_Process(void) // called in main() loop (main_cm4.c)
{
	/* Check for CM7 messages (openamp) */

	OPENAMP_check_for_message();

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
		UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "MIDI controller connected !      ", LEFT_MODE);
		Appli_state = APPLICATION_RUNNING;
	}

	if (Appli_state == APPLICATION_DISCONNECT)
	{
		BSP_LED_Off(LED_GREEN);
		UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "MIDI controller not connected...", LEFT_MODE);
		Appli_state = APPLICATION_IDLE;
		USBH_MIDI_Stop(&hUsbHostHS);
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

		//if (pack.cin_cable != 0) // if incoming midi message...
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

//
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
	}
}
