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
//#include "usbh_midi_XH.h"

/* Exported function prototypes -----------------------------------------------*/
void openamp_cm7_init(void);
void Process_message(void);
void string_sendToCM4(uint32_t number);


#endif /* INC_OPENAMP_INTERFACE_H_ */
