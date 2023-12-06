/*
 * midi_types.h
 *
 *  Created on: Dec 6, 2023
 *      Author: XavSab
 */

#ifndef INC_MIDI_TYPES_H_
#define INC_MIDI_TYPES_H_

/* -------------------- Exported_Types ------------------------------------------*/

typedef enum
{
	NoteOff = 0x8, NoteOn = 0x9, PolyPressure = 0xa, CC = 0xb, ProgramChange = 0xc, Aftertouch = 0xd, PitchBend = 0xe
} midi_event_t;

/* MIDI Code Index Numbers (CIN) */
typedef enum
{
	MISCELLANEOUS = 0,
	CABLE_EVENTS,
	TWO_BYTE_MSG,
	THREE_BYTE_MSG,
	SYS_EX_START,
	SYS_EX_END_ONE_BYTE,
	SYS_EX_END_TWO_BYTES,
	SYS_EX_END_THREE_BYTES,
	NOTE_OFF,
	NOTE_ON,
	POLY_KEY_PRESS,
	CONTROL_CHANGE,
	PROGRAM_CHANGE,
	CHANNEL_PRESSURE,
	PITCH_BEND,
	SINGLE_BYTE_MSG
} MIDI_EventTypeDef;

/* Basic MIDI packet structure */
typedef struct
{
	uint8_t header;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
} MIDI_Packet;

typedef enum
{
	Chn1, Chn2, Chn3, Chn4, Chn5, Chn6, Chn7, Chn8, Chn9, Chn10, Chn11, Chn12, Chn13, Chn14, Chn15, Chn16
} midi_chn_t;

typedef union
{
	struct
	{
		uint32_t ALL;
	};
	struct
	{
		uint8_t cin_cable;
		uint8_t evnt0;
		uint8_t evnt1;
		uint8_t evnt2;
	};
	struct
	{
		uint8_t type :4;
		uint8_t cable :4;
		uint8_t chn :4; // mios32_midi_chn_t
		uint8_t event :4; // mios32_midi_event_t
		uint8_t value1;
		uint8_t value2;
	};

	// C++ doesn't allow to redefine names in anonymous unions
	// as a simple workaround, we rename these redundant names
	struct
	{
		uint8_t cin :4;
		uint8_t dummy1_cable :4;
		uint8_t dummy1_chn :4; // mios32_midi_chn_t
		uint8_t dummy1_event :4; // mios32_midi_event_t
		uint8_t currentNote :8;
		uint8_t velocity :8;
	};
	struct
	{
		uint8_t dummy2_cin :4;
		uint8_t dummy2_cable :4;
		uint8_t dummy2_chn :4; // mios32_midi_chn_t
		uint8_t dummy2_event :4; // mios32_midi_event_t
		uint8_t cc_number :8;
		uint8_t value :8;
	};
	struct
	{
		uint8_t dummy3_cin :4;
		uint8_t dummy3_cable :4;
		uint8_t dummy3_chn :4; // mios32_midi_chn_t
		uint8_t dummy3_event :4; // mios32_midi_event_t
		uint8_t program_change :8;
		uint8_t dummy3 :8;
	};
} midi_package_t;

#endif /* INC_MIDI_TYPES_H_ */
