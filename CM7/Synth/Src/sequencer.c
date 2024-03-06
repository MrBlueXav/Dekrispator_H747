/**
 ******************************************************************************
 * File Name          : sequencer.c
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
/*********************************************************************************************/

#include "sequencer.h"
#include "constants.h"
#include "random.h"
#include "notesTables.h"

/*--------------------------------------------------------------------------------------------*/
Sequencer_t seq _DTCMRAM_;
NoteGenerator_t noteGen _DTCMRAM_;

/*--------------------------------------------------------------------------------------------*/
#define NUMBER_OF_LENGTHS 10
#define INDEX_FOR_16_STEPS 5

static const uint8_t LENGTHS[] =
{ 3, 4, 6, 8, 12, 16, 5, 7, 11, 15 }; // 10 predefined loop lengths

/*--------------------------------------------------------------------------------------------*/
void sequencer_init(float sample_rate)
{
	seq.samplerate = sample_rate;
	seq.tempo = INIT_TEMPO;
	seq.length_index = INDEX_FOR_16_STEPS;
	seq_steptime_update(&seq);
	seq.smp_count = 0;
	seq.step_idx = 0;
	seq.gateTime = seq.steptime / 2;

	noteGen.transpose = 0;
	noteGen.automaticON = false;
	noteGen.glideON = false;
	noteGen.chRequested = false;
	noteGen.someNotesMuted = false;
	noteGen.scaleIndex = 0;
	noteGen.octaveSpread = 4;
	noteGen.rootNote = 36;
	noteGen.currentScale = (uint8_t*) MIDIscale13;

	seq_sequence_new();
}

/*--------------------------------------------------------------------------------------------*/
void Sequencer_params_set(const SequencerParams_t *params, Sequencer_t *seq, NoteGenerator_t *ng)
{
	seq->samplerate = params->samplerate;
	seq->tempo = params->tempo;
	seq->length_index = params->length_index;
	seq->gateTime = params->gateTime;
	ng->scaleIndex = params->scaleIndex;
	ng->currentScale = params->currentScale;
	ng->octaveSpread = params->octaveSpread;
	ng->rootNote = params->rootNote;
	ng->transpose = params->transpose;
	ng->automaticON = params->automaticON;
	ng->glideON = params->glideON;
	ng->chRequested = params->chRequested;
	ng->someNotesMuted = params->someNotesMuted;

	for (uint8_t i = 0; i < NUMBER_STEPS; i++)
	{
		seq->track1.note[i] = params->track1.note[i];
	}

	seq_steptime_update(seq);
	seq->smp_count = 0;
	seq->step_idx = 0;
}

/*--------------------------------------------------------------------------------------------*/
void Sequencer_params_save(const Sequencer_t *seq, const NoteGenerator_t *ng, SequencerParams_t *params)
{
	params->samplerate = seq->samplerate;
	params->tempo = seq->tempo;
	params->length_index = seq->length_index;
	params->gateTime = seq->gateTime;
	params->scaleIndex = ng->scaleIndex;
	params->currentScale = ng->currentScale;
	params->octaveSpread = ng->octaveSpread;
	params->rootNote = ng->rootNote;
	params->transpose = ng->transpose;
	params->automaticON = ng->automaticON;
	params->glideON = ng->glideON;
	params->chRequested = ng->chRequested;
	params->someNotesMuted = ng->someNotesMuted;
	for (uint8_t i = 0; i < NUMBER_STEPS; i++)
	{
		params->track1.note[i] = seq->track1.note[i];
	}
}

/*--------------------------------------------------------------------------------------------*/
void seq_steptime_update(Sequencer_t *seq)
{
	seq->steptime = lrintf(seq->samplerate * 60 / seq->tempo);
}
/*-------------------------------------------------------*/
void seq_gateTime_set(uint8_t midival)
{
	seq.gateTime = seq.steptime * ((0.9f - 0.1f) * midival / MIDI_MAX + 0.1f); // from 10% to 90% of each step duration
}

/*-------------------------------------------------------*/
void seq_transpP2(uint8_t val) // one tone up
{
	if (val == MIDI_MAXi)
	{
		if (noteGen.rootNote < (MAX_NOTE_INDEX - 12))
		{
			noteGen.transpose = 2;
		}
	}
}

/*-------------------------------------------------------*/
void seq_transpP1(uint8_t val) // one semitone up
{
	if (val == MIDI_MAXi)
	{
		if (noteGen.rootNote < (MAX_NOTE_INDEX - 12))
		{
			noteGen.transpose = 1;
		}
	}
}
/*-------------------------------------------------------*/
void seq_transpP7(uint8_t val) //
{
	if (val == MIDI_MAXi)
	{
		if (noteGen.rootNote < (MAX_NOTE_INDEX - 12))
		{
			noteGen.transpose = 7;
		}
	}
}
/*-------------------------------------------------------*/
void seq_transpM2(uint8_t val) // one tone down
{
	if (val == MIDI_MAXi)
	{
		if (noteGen.rootNote > LOWEST_NOTE)
		{
			noteGen.transpose = -2;
		}
	}
}

/*-------------------------------------------------------*/
void seq_transpM1(uint8_t val) // one semitone down
{
	if (val == MIDI_MAXi)
	{
		if (noteGen.rootNote > LOWEST_NOTE)
		{
			noteGen.transpose = -1;
		}
	}
}
/*-------------------------------------------------------*/
void seq_transpM7(uint8_t val) //
{
	if (val == MIDI_MAXi)
	{
		if (noteGen.rootNote > LOWEST_NOTE)
		{
			noteGen.transpose = -7;
		}
	}
}
/*-------------------------------------------------------*/
void seq_transpUp(void) // one tone up
{
	if (noteGen.rootNote < (MAX_NOTE_INDEX - 12))
	{
		noteGen.transpose = 2;
	}
}
/*-------------------------------------------------------*/
void seq_transp(int8_t semitone, uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		noteGen.transpose = semitone;
	}
}
/*-------------------------------------------------------*/
void seq_transpDown(void) // one tone down
{
	if (noteGen.rootNote > LOWEST_NOTE)
	{
		noteGen.transpose = -2;
	}
}
/*-------------------------------------------------------*/
void seq_chooseScale(int16_t idx)
{
	uint8_t *currentScale;

	switch (idx)
	{
	case 0:
		currentScale = (uint8_t*) MIDIscale13;
		break;
	case 1:
		currentScale = (uint8_t*) MIDIscale14;
		break;
	case 2:
		currentScale = (uint8_t*) MIDIscale07;
		break;
	case 3:
		currentScale = (uint8_t*) MIDIscale08;
		break;
	case 4:
		currentScale = (uint8_t*) MIDIscale09;
		break;
	case 5:
		currentScale = (uint8_t*) MIDIscale10;
		break;
	case 6:
		currentScale = (uint8_t*) MIDIscale04;
		break;
	case 7:
		currentScale = (uint8_t*) MIDIscale01;
		break;
	case 8:
		currentScale = (uint8_t*) MIDIscale03;
		break;
	case 9:
		currentScale = (uint8_t*) MIDIscale11;
		break;
	case 10:
		currentScale = (uint8_t*) MIDIscale02;
		break;
	case 11:
		currentScale = (uint8_t*) MIDIscale06;
		break;
	case 12:
		currentScale = (uint8_t*) MIDIscale05;
		break;
	case 13:
		currentScale = (uint8_t*) MIDIscale12;
		break;
	case 14:
		currentScale = (uint8_t*) MIDIscale11;
		break;
	default:
		currentScale = (uint8_t*) MIDIscale11;
		break;
	}
	noteGen.currentScale = currentScale;
	noteGen.chRequested = true;
}
/*-------------------------------------------------------*/
void seq_nextScale(void)
{
	if (noteGen.scaleIndex < MAX_SCALE_INDEX)
	{
		noteGen.scaleIndex++;
		seq_chooseScale(noteGen.scaleIndex);
	}
}
/*-------------------------------------------------------*/
void seq_prevScale(void)
{
	if (noteGen.scaleIndex > 0)
	{
		noteGen.scaleIndex--;
		seq_chooseScale(noteGen.scaleIndex);
	}
}
/*-------------------------------------------------------*/
void seq_scale_set(uint8_t val)
{
	noteGen.scaleIndex = (uint8_t) rintf(MAX_SCALE_INDEX / MIDI_MAX * val);
	seq_chooseScale(noteGen.scaleIndex);
}
/*-------------------------------------------------------*/
void seq_automatic_or_manual(void)
{
	if (noteGen.automaticON)
		noteGen.automaticON = false;
	else
		noteGen.automaticON = true;
}
/*-------------------------------------------------------*/
void seq_switchMovingSeq(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		noteGen.automaticON = !noteGen.automaticON;
	}
}
/*-------------------------------------------------------*/
void seq_toggleGlide(void)
{
	if (noteGen.glideON)
		noteGen.glideON = false;
	else
		noteGen.glideON = true;
}
/*-------------------------------------------------------*/
void seq_switchGlide(uint8_t val)
{
	switch (val)
	{
	case MIDI_MAXi:
		noteGen.glideON = true;
		break;
	case 0:
		noteGen.glideON = false;
		break;
	}
}
/*-------------------------------------------------------*/
void seq_muteSomeNotes(void)
{
	if (noteGen.someNotesMuted)
		noteGen.someNotesMuted = false;
	else
		noteGen.someNotesMuted = true;
}
/*-------------------------------------------------------*/
void seq_switchMute(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		noteGen.someNotesMuted = !noteGen.someNotesMuted;
	}
}

/*--------------------------------------------------------------------------------------------*/
void seq_tempo_set(uint8_t val)
{
	float ratio = (float) (seq.gateTime) / seq.steptime; // keep gate time ratio
	seq.tempo = (float) (800.f * val / MIDI_MAX + 20); // unit : step bpm
	seq_steptime_update(&seq);
	seq.gateTime = lrintf(ratio * seq.steptime);
}

/*-------------------------------------------------------*/
void seq_tempo_double(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		seq.tempo = 2 * seq.tempo;
		seq.chgTempoRequested = true;
	}
}
/*-------------------------------------------------------*/
void seq_tempo_half(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		seq.tempo = 0.5f * seq.tempo;
		seq.chgTempoRequested = true;
	}
}

/*-------------------------------------------------------*/
uint8_t seq_length_get(void)
{
	return LENGTHS[seq.length_index];
}

/*-------------------------------------------------------*/
void seq_length_dec(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		if (seq.length_index > 0)
			seq.length_index--;
	}
}

/*-------------------------------------------------------*/
void seq_length_inc(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		if (seq.length_index < (NUMBER_OF_LENGTHS - 1))
			seq.length_index++;
	}
}
/*-------------------------------------------------------*/
void seq_incMaxFreq(void)
{
	if (noteGen.octaveSpread < 8)
	{
		noteGen.octaveSpread++;
		noteGen.chRequested = true;
	}
}
/*-------------------------------------------------------*/
void seq_decMaxFreq(void)
{
	if (noteGen.octaveSpread > 0)
	{
		noteGen.octaveSpread--;
		noteGen.chRequested = true;
	}
}
/*-------------------------------------------------------*/
void seq_freqMax_set(uint8_t val)
{
	noteGen.octaveSpread = (uint8_t) (8 / MIDI_MAX * val);
	noteGen.chRequested = true;
}

/*-------------------------------------------------------*/
void seq_new_seq(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		noteGen.chRequested = true;
	}
}
/*--------------------------------------------------------------------------------------------*/
void _ITCMRAM_ seq_sequence_new(void) // make a new sequence of notes following noteGen parameters
{
	int16_t relativeNote;
	int16_t octaveShift;
	int16_t index;

	for (uint8_t i = 0; i < NUMBER_STEPS; i++)
	{
		relativeNote = noteGen.currentScale[lrintf(frand_a_b(1, noteGen.currentScale[0]))];
		octaveShift = 12 * lrintf(frand_a_b(0, noteGen.octaveSpread));
		index = noteGen.rootNote + octaveShift + relativeNote - LOWEST_NOTE;

		while (index > MAX_NOTE_INDEX)
			index -= 12;
		while (index < 0)
			index += 12;
		seq.track1.note[i] = index; // note frequency is in notesFreq[index]
	}
}

/*--------------------------------------------------------------------------------------------*/
int16_t seq_random_note(void) // returns a random midi note number through the note generator
{
	int16_t relativeNote;
	int16_t octaveShift;
	int16_t index;

	relativeNote = noteGen.currentScale[lrintf(frand_a_b(1, noteGen.currentScale[0]))];
	octaveShift = 12 * lrintf(frand_a_b(0, noteGen.octaveSpread));
	index = noteGen.rootNote + octaveShift + relativeNote - LOWEST_NOTE;

	while (index > MAX_NOTE_INDEX)
		index -= 12;
	while (index < 0)
		index += 12;

	return (index + LOWEST_NOTE);
}
/*--------------------------------------------------------------------------------------------*/
void seq_transpose(void)
{
	int16_t noteIndex;

	for (uint8_t i = 0; i < NUMBER_STEPS; i++)
	{
		noteIndex = seq.track1.note[i] + noteGen.transpose;
		while (noteIndex > MAX_NOTE_INDEX)
			noteIndex -= 12;
		while (noteIndex < 0)
			noteIndex += 12;
		seq.track1.note[i] = noteIndex;
	}
	noteGen.transpose = 0;
}

/*--------------------------------------------------------------------------------------------*/
void _ITCMRAM_ sequencer_process(void) // To be called at each sample treatment
{
	/* If we have reached a new step ....  */
	if (seq.smp_count-- <= 0)
	{
		/* If we are at the beginning of a new sequence .... */
		if (seq.step_idx == 0)
		{
			sequencer_newSequence_action();
		}
		sequencer_newStep_action();

		seq.smp_count = seq.steptime; // reload the counter
		seq.step_idx++;
		if (seq.step_idx >= LENGTHS[seq.length_index])
			seq.step_idx = 0;
	}
}
/*--------------------------------------------------------------------------------------------*/
