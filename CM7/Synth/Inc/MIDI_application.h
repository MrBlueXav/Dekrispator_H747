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
#include "adsr.h"
#include "drifter.h"
#include "sinetable.h"
#include "notesTables.h"
#include "drifter.h"
#include "soundGen.h"
#include "audio_conf.h"
#include "midi_types.h"

/* Exported functions ------------------------------------------------------- */
void ProcessReceivedMidiDatas(midi_package_t packet);
void Reset_notes_On(void);

/*------------------------------------------------------------------------------*/
#endif /* MIDI_APPLICATION_H_ */
