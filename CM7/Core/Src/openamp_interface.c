/*
 * openamp_interface.c
 *
 *  Created on: Dec 6, 2023
 *      Author: XavSab
 */

#include "openamp_interface.h"
#include "main.h"
#include "soundGen.h"

/* Private macro -------------------------------------------------------------*/
#define RPMSG_CHAN_NAME              "M4_M7_communication"

/* Private variables ---------------------------------------------------------*/
static uint32_t message;
static volatile int message_received;
static volatile int service_created;
static volatile midi_package_t received_data;
static struct rpmsg_endpoint rp_endpoint;
static binn *obj;
static volatile bool SEV_received;
static volatile uint32_t _DTCMRAM_ ticktime, oldtime;

/* message buffers variables in SRAM4 ----------------------------------------*/
volatile uint8_t *buf_cm4_to_cm7 = (void*) BUFF_CM4_TO_CM7_ADDR;
volatile uint8_t *buf_cm7_to_cm4 = (void*) BUFF_CM7_TO_CM4_ADDR;
volatile ScreenDatas_t *scr_datas = (void*) BUFF2_CM7_TO_CM4_ADDR;

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
	/* create a endpoint for rpmsg communication */
	OPENAMP_create_endpoint(&rp_endpoint, name, dest, rpmsg_recv_callback, service_destroy_cb);
	service_created = 1;
}

/*-----------------------------------------------------------------------------------------------------------------*/
void openamp_cm7_init(void)
{
	SEV_received = false;
	oldtime = 0;

	/* Initialize the mailbox use notify the other core on new message */
	MAILBOX_Init();

	/* Initialize the rpmsg endpoint to set default addresses to RPMSG_ADDR_ANY */
	rpmsg_init_ept(&rp_endpoint, RPMSG_CHAN_NAME, RPMSG_ADDR_ANY, RPMSG_ADDR_ANY, NULL, NULL);
	/* Initialize OpenAmp and libmetal libraries */
	if (MX_OPENAMP_Init(RPMSG_MASTER, new_service_cb) != HAL_OK)
		Error_Handler();

	/*
	 * The rpmsg service is initiate by the remote processor, on A7 new_service_cb
	 * callback is received on service creation. Wait for the callback
	 */
	OPENAMP_Wait_EndPointready(&rp_endpoint);

	/* Send the message to the remote CPU */
	int32_t status;
	status = OPENAMP_send(&rp_endpoint, &message, sizeof(message));
	if (status < 0)
	{
		Error_Handler();
	}

	SEV_received = false;
}

/*-------------------------------------------------------------------------------------*/
void send_message_to_CM4(binn *obj)
{
	int32_t status = 0;
	status = OPENAMP_send(&rp_endpoint, binn_ptr(obj), binn_size(obj));
	if (status < 0)
	{
		Error_Handler();
	}
}

/*-------------------------------------------------------------------------------------*/
void send_integer_to_CM4(uint32_t number)
{
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'X'); // command 'X' = send number of cpu cycles
	binn_object_set_uint32(obj, "number", number);
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*-------------------------------------------------------------------------------------*/
void send_patch_to_CM4(SynthPatch_t *patch)
{
	memcpy((void*) buf_cm7_to_cm4, patch, sizeof(*patch));
	asm("sev");
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'P'); // command 'P' = "save patch"
	//binn_object_set_blob(obj, "patch", patch, sizeof(*patch));
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*-------------------------------------------------------------------------------------*/
void send_string_to_CM4(char *str)
{
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'S'); // command 'S' = send string
	binn_object_set_str(obj, "string", str);
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*-------------------------------------------------------------------------------------*/
void send_erase_request_to_CM4(void)
{
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'R'); // command 'R' = send QSPI Flash erase request
	send_message_to_CM4(obj);
	binn_free(obj);
}
/*-------------------------------------------------------------------------------------*/
void send_erase_all_patches_to_CM4(SynthPatch_t *patch) // erase qspi flash and fill it with init patch
{
	memcpy((void*) buf_cm7_to_cm4, patch, sizeof(*patch));
	asm("sev");
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'E'); // command 'E' = "erase qspi flash with init patch"
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*-------------------------------------------------------------------------------------*/
void send_patch_location_to_CM4(uint16_t loc)
{
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'L'); // command 'L' = send current patch memory location
	binn_object_set_uint16(obj, "location", loc);
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*-------------------------------------------------------------------------------------*/
void send_patch_request_to_CM4(uint16_t loc)
{
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'D'); // command 'D' = send patch download request
	binn_object_set_uint16(obj, "location", loc);
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*-------------------------------------------------------------------------------------*/
void send_clear_message_to_CM4(void)
{
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'B');
	send_message_to_CM4(obj);
	binn_free(obj);
}
/*-------------------------------------------------------------------------------------*/
void send_screen_infos_to_CM4(void)
{
	get_datas_for_screen(scr_datas);
	obj = binn_object();
	binn_object_set_uint8(obj, "cmd", 'C');
	send_message_to_CM4(obj);
	binn_free(obj);
}

/*----------------------------------------------------------------------------------------------------------------*/
void CM4_SEV_signal(void)
{
	SEV_received = true;
}

/*-----------------------------------------------------------------------------------------------------------------*/
void Process_messages(void) // called in main() loop (in main_cm7.c)
{

	if (SEV_received == true)
	{
		//printf("SEV signal from CM7 received !\n");
		//printf((char*) buf_cm7_to_cm4);
		Synth_patch_load((SynthPatch_t*) buf_cm4_to_cm7);
		SEV_received = false;
	}

	if (message_received == 0 && service_created == 1)
	{
		OPENAMP_check_for_message();
	}

	if (message_received)
	{
		ProcessReceivedMidiDatas(received_data);
		message_received = 0;
	}

	ticktime = HAL_GetTick();
	if ((ticktime - oldtime) > 500) // refresh screen every 0.5 second maximum
	{
		oldtime = ticktime;
		send_screen_infos_to_CM4();
	}
}
