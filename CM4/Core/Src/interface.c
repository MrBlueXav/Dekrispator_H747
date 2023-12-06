/*
 * interface.c
 *
 *  Created on: Nov 14, 2023
 *      Author: Xavier Halgand
 */
#include "interface.h"

#define RX_BUFF_SIZE 64 /* USB MIDI buffer : max received data 64 bytes */

extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHostHS;

uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer
uint32_t oldtick, newtick;

/*------------------------------------------------------------------------------------------------------------------*/
static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv);
static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv);
void ProcessReceivedMidiDatas(void);

/* Private macro -------------------------------------------------------------*/
#define RPMSG_SERVICE_NAME              "midi_communication"

/* Private variables ---------------------------------------------------------*/
static volatile int message_received;
static volatile midi_package_t received_data;
static struct rpmsg_endpoint rp_endpoint;
HSEM_TypeDef *HSEM_DEBUG = HSEM;

/*----------------------------------------------------------------------------------------------------------------*/
void openamp_init(void)
{
	/* Initialize the mailbox use notify the other core on new message */
	MAILBOX_Init();

	/* Inilitize OpenAmp and libmetal libraries */
	if (MX_OPENAMP_Init(RPMSG_REMOTE, NULL) != HAL_OK)
		Error_Handler();

	/* create a endpoint for rmpsg communication */
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
	received_data = *((midi_package_t*) data);
	message_received = 1;
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

void Application_Process(void)
{
	if (Appli_state == APPLICATION_READY)
	{
		BSP_LED_On(LED_GREEN);

		if (message_received == 0)
		{
			OPENAMP_check_for_message();
		}
		if (message_received)
		{
			message_received = 0;
		}

		/* start a new reception not faster than 1 per ms */
		newtick = HAL_GetTick();
		if (newtick != oldtick)
		{
			USBH_MIDI_Receive(&hUsbHostHS, MIDI_RX_Buffer, RX_BUFF_SIZE);
			oldtick = newtick;
		}
	}

	if (Appli_state == APPLICATION_DISCONNECT)
	{
		BSP_LED_Off(LED_GREEN);
		Appli_state = APPLICATION_IDLE;
		USBH_MIDI_Stop(&hUsbHostHS);
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief  MIDI data receive callback. _weak defined in usbh_midi_XH.c
 * @param  phost: Host handle
 * @retval None
 */
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
	BSP_LED_On(LED_BLUE);
	ProcessReceivedMidiDatas();
	//USBH_MIDI_Receive(&hUSBHost, MIDI_RX_Buffer, RX_BUFF_SIZE); // start a new reception
	BSP_LED_Off(LED_BLUE);
}

/* Process MIDI packets and change the note according to key pressed
 on digital piano */
//void usbh_midi_rx_callback(USBH_HandleTypeDef *phost) {
//	MIDI_Packet *packet_p = (MIDI_Packet*) &midi_rx_buffer[0];
//	uint16_t num_of_packets;
//
//
//	num_of_packets = usbh_midi_last_rx_size(phost) / 4;
//
//	while (num_of_packets--) {
//		/* Check if MIDI message is a CC event */
//		if (GET_CIN(packet_p->header) == CONTROL_CHANGE) {
//			LCD_UsrLog("%d  :  %d, %d, %d, %d   \n", num_of_packets,
//					packet_p->header, packet_p->byte1, packet_p->byte2,
//					packet_p->byte3);
//		}
//
//		++packet_p;
//	}
//	/* Refill MIDI RX buffer */
//	usbh_midi_receive(phost, &midi_rx_buffer[0], RX_BUFFER_SIZE);
//}
/*-----------------------------------------------------------------------------*/
void ProcessReceivedMidiDatas(void)
{
	uint16_t numberOfPackets;
	uint8_t *ptr = MIDI_RX_Buffer;
	midi_package_t pack;

	numberOfPackets = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostHS) / 4; //each USB midi package is 4 bytes long
	//LCD_UsrTrace("Nombre de paquets reçus : %d   \n", numberOfPackets);

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

		if (pack.cin_cable != 0) // if incoming midi message...
		{
			//midipacket_sendToCM7(pack);
			//start_LED_On(LED_Blue, 8);
			printf("%d  :  %d, %d, %d, %d   \n",
			RX_BUFF_SIZE / 4 - numberOfPackets, pack.cin_cable, pack.evnt0, pack.evnt1, pack.evnt2);
		}
	}
}
