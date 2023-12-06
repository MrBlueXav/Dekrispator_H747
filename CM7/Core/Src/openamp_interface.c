/*
 * openamp_interface.c
 *
 *  Created on: Dec 6, 2023
 *      Author: XavSab
 */

#include "openamp_interface.h"

/* Private macro -------------------------------------------------------------*/
#define RPMSG_CHAN_NAME              "midi_communication"

/* Private variables ---------------------------------------------------------*/
static uint32_t message = 0;
static volatile int message_received;
static volatile int service_created;
static volatile midi_package_t received_data;
static struct rpmsg_endpoint rp_endpoint;

/* Private functions ---------------------------------------------------------*/

static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src, void *priv)
{
	received_data = *((midi_package_t*) data);
	message_received = 1;

	return 0;
}
/*-----------------------------------------------------------------------------------------------------------------*/
void service_destroy_cb(struct rpmsg_endpoint *ept)
{
	/* this function is called while remote endpoint as been destroyed, the
	 * service is no more available
	 */
	service_created = 0;
}
/*-----------------------------------------------------------------------------------------------------------------*/
void new_service_cb(struct rpmsg_device *rdev, const char *name, uint32_t dest)
{
	/* create a endpoint for rmpsg communication */
	OPENAMP_create_endpoint(&rp_endpoint, name, dest, rpmsg_recv_callback, service_destroy_cb);
	service_created = 1;
}

/*-----------------------------------------------------------------------------------------------------------------*/
void Process_message(void)
{
	if (message_received == 0 && service_created == 1)
	{
		OPENAMP_check_for_message();
	}
	if (message_received)
	{
		ProcessReceivedMidiDatas(received_data);
		message_received = 0;
	}
}

/*-----------------------------------------------------------------------------------------------------------------*/
void openamp_cm7_init(void)
{
	/* Initialize the mailbox use notify the other core on new message */
	MAILBOX_Init();

	/* Initialize the rpmsg endpoint to set default addresses to RPMSG_ADDR_ANY */
	rpmsg_init_ept(&rp_endpoint, RPMSG_CHAN_NAME, RPMSG_ADDR_ANY,
	RPMSG_ADDR_ANY,
	NULL, NULL);
	/* Initialize OpenAmp and libmetal libraries */
	if (MX_OPENAMP_Init(RPMSG_MASTER, new_service_cb) != HAL_OK)
		Error_Handler();

	/*
	 * The rpmsg service is initiate by the remote processor, on A7 new_service_cb
	 * callback is received on service creation. Wait for the callback
	 */
	OPENAMP_Wait_EndPointready(&rp_endpoint);

	/* Send the massage to the remote CPU */
	int32_t status;
	status = OPENAMP_send(&rp_endpoint, &message, sizeof(message));
	if (status < 0)
	{
		Error_Handler();
	}
}
/*-------------------------------------------------------------------------------------*/
