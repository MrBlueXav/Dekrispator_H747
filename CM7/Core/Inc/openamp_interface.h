/*
 * openamp_interface.h
 *
 *  Created on: Dec 6, 2023
 *      Author: XavSab
 */

#ifndef INC_OPENAMP_INTERFACE_H_
#define INC_OPENAMP_INTERFACE_H_

/* Includes ------------------------------------------------------------------*/
#include "openamp.h"
#include "midi_types.h"
#include "MIDI_application.h"
#include "soundGen.h"
#include "binn.h"

/* Exported function prototypes -----------------------------------------------*/
void openamp_cm7_init(void);
void Process_message(void);
void send_integer_to_CM4(uint32_t number);
void send_patch_location_to_CM4(uint16_t loc);
void send_patch_request_to_CM4(uint16_t loc);
void send_patch_to_CM4(SynthPatch_t *patch);
void send_erase_request_to_CM4(void);
void send_erase_all_patches_to_CM4(SynthPatch_t *patch);
void send_message_to_CM4(binn *obj);
void send_string_to_CM4(char *str);
void send_SEV_to_CM4(void);
void CM4_SEV_signal(void);
void send_clear_message_to_CM4(void);

#endif /* INC_OPENAMP_INTERFACE_H_ */
