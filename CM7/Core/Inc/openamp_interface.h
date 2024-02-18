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
void send_patch_to_CM4(SynthPatch_t *patch);
void send_message_to_CM4(binn *obj);
void send_string_to_CM4(char *str);

#endif /* INC_OPENAMP_INTERFACE_H_ */
