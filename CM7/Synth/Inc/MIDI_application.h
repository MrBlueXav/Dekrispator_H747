/*
 * MIDI_application.h
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

#ifndef MIDI_APPLICATION_H_
#define MIDI_APPLICATION_H_

/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
//#include "usbh_core.h"
//#include "usbh_midi_RS.h"
//#include "usbh_midi_XH.h"
#include "main.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "random.h"
#include "drifter.h"
#include "sinetable.h"
#include "notesTables.h"
#include "drifter.h"
#include "soundGen.h"
#include "audio_conf.h"

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


typedef union {
	struct {
		uint32_t ALL;
	};
	struct {
		uint8_t cin_cable;
		uint8_t evnt0;
		uint8_t evnt1;
		uint8_t evnt2;
	};
	struct {
		uint8_t type:4;
		uint8_t cable:4;
		uint8_t chn:4; // mios32_midi_chn_t
		uint8_t event:4; // mios32_midi_event_t
		uint8_t value1;
		uint8_t value2;
	};

	// C++ doesn't allow to redefine names in anonymous unions
	// as a simple workaround, we rename these redundant names
	struct {
		uint8_t cin:4;
		uint8_t dummy1_cable:4;
		uint8_t dummy1_chn:4; // mios32_midi_chn_t
		uint8_t dummy1_event:4; // mios32_midi_event_t
		uint8_t currentNote:8;
		uint8_t velocity:8;
	};
	struct {
		uint8_t dummy2_cin:4;
		uint8_t dummy2_cable:4;
		uint8_t dummy2_chn:4; // mios32_midi_chn_t
		uint8_t dummy2_event:4; // mios32_midi_event_t
		uint8_t cc_number:8;
		uint8_t value:8;
	};
	struct {
		uint8_t dummy3_cin:4;
		uint8_t dummy3_cable:4;
		uint8_t dummy3_chn:4; // mios32_midi_chn_t
		uint8_t dummy3_event:4; // mios32_midi_event_t
		uint8_t program_change:8;
		uint8_t dummy3:8;
	};
} midi_package_t;


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
