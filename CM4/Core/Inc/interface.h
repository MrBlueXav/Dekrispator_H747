/*
 * interface.h
 *
 *  Created on: Nov 14, 2023
 *      Author: Xavier Halgand
 */
#ifndef APPLICATION_CM7_INTERFACE_H_
#define APPLICATION_CM7_INTERFACE_H_

/*-----------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <math.h>

#include "usbh_midi_XH.h"
#include "usb_host.h"
#include "stm32h747i_discovery.h"
#include "openamp.h"
#include "constants.h"

/*-----------------------------------------------------------------------------------------------------------------*/

void openamp_init(void);
void Application_Process(void);

/*-----------------------------------------------------------------------------------------------------------------*/

#endif /* APPLICATION_CM7_INTERFACE_H_ */
