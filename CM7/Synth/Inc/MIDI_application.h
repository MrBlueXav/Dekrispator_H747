/*
 * MIDI_application.h
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

#ifndef MIDI_APPLICATION_H_
#define MIDI_APPLICATION_H_

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "constants.h"
#include "random.h"
#include "drifter.h"
#include "sinetable.h"
#include "notesTables.h"
#include "drifter.h"
#include "soundGen.h"
#include "audio_conf.h"
#include "midi_types.h"


/*------------------------------------------------------------------------------*/
typedef enum
{
	APPLICATION_IDLE = 0,
	APPLICATION_START,
	APPLICATION_READY,
	APPLICATION_RUNNING,
	APPLICATION_DISCONNECT
}
MIDI_ApplicationTypeDef;

/*------------------------------------------------------------------------------*/

//extern USBH_HandleTypeDef hUSBHost;
extern MIDI_ApplicationTypeDef Appli_state;
extern int8_t currentNote;
extern int8_t velocity;

/* Exported functions ------------------------------------------------------- */

void MagicFX(uint8_t val);
void MagicPatch(uint8_t val);
void Reset_notes_On(void);
void MIDI_Application(void);
void ProcessReceivedMidiDatas(midi_package_t packet);

/*------------------------------------------------------------------------------*/
#endif /* MIDI_APPLICATION_H_ */
