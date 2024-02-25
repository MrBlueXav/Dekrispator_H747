/**
 ******************************************************************************
 * File Name          	: sequencer.h
 * Author				: Xavier Halgand
 * Date               	:
 * Description        	:
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SEQUENCER_H
#define __SEQUENCER_H

/*----------------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/*-----------------------------------------------------------------------------*/

#define NUMBER_STEPS 	16
#define INIT_TEMPO 		(4*80)

/*------------------------------------------------------------------------------*/
typedef struct
{
	int16_t note[NUMBER_STEPS];

} Track_t;
/*------------------------------------------------------------------------------*/
typedef struct
{
	float_t samplerate;
	Track_t track1;
	float_t tempo; 			// unit : bpm
	uint8_t length_index;	// index for number of steps of loop
	int32_t steptime; 		// unit : # of samples
	int32_t smp_count;		// sample de-counter
	int16_t step_idx;		// current step index
	int32_t gateTime; 		// desired gate on time (in number of samples) ≤ steptime
	bool	chgTempoRequested;

} Sequencer_t;

/*------------------------------------------------------------------------------*/
typedef struct
{
	int16_t scaleIndex;
	uint8_t *currentScale;
	int16_t octaveSpread;
	int16_t rootNote;
	int16_t transpose;
	bool automaticON;	// random notes ?
	bool glideON;		// glissando between notes ?
	bool chRequested;
	bool someNotesMuted;

} NoteGenerator_t;

/*------------------------------------------------------------------------------*/
typedef struct
{
	float_t samplerate;
	Track_t track1;
	float_t tempo; 			// unit : bpm
	uint8_t length_index;	// index for number of steps of loop
	int32_t gateTime; 		// desired gate on time (in samples)
	int16_t scaleIndex;
	uint8_t *currentScale;
	int16_t octaveSpread;
	int16_t rootNote;
	int16_t transpose;
	bool automaticON;		// random notes ?
	bool glideON;			// glissando between notes ?
	bool chRequested;
	bool someNotesMuted;

} SequencerParams_t;

/*--------------   Exported Functions Prototype ---------------------------*/
void sequencer_init(float sample_rate);
void Sequencer_params_set(const SequencerParams_t *params, Sequencer_t *seq, NoteGenerator_t *ng);
void Sequencer_params_save(const Sequencer_t *seq, const NoteGenerator_t *ng, SequencerParams_t *params);
void seq_steptime_update(Sequencer_t *seq);
void sequencer_process(void);
void seq_sequence_new(void);

void seq_new_seq(uint8_t val);

void seq_tempo_set(uint8_t val);
void seq_tempo_double(uint8_t val);
void seq_tempo_half(uint8_t val);

void seq_length_dec(uint8_t val);
void seq_length_inc(uint8_t val);

int16_t seq_random_note(void);

void seq_transpP1(uint8_t val);
void seq_transpP2(uint8_t val);
void seq_transpP7(uint8_t val);
void seq_transpM1(uint8_t val);
void seq_transpM2(uint8_t val);
void seq_transpM7(uint8_t val);
void seq_transpose(void);
void seq_transpUp(void); // one tone up
void seq_transpDown(void); // one tone down
void seq_transp(int8_t s, uint8_t val);

void seq_gateTime_set(uint8_t val);

void seq_chooseScale(int16_t idx);
void seq_scale_set(uint8_t val);

void seq_switchMovingSeq(uint8_t val);
void seq_switchMute(uint8_t val);
void seq_switchGlide(uint8_t val);
void seq_freqMax_set(uint8_t val);

/* User Callbacks: user has to implement these functions in his code if
 they are needed. -----------------------------------------------------------*/
void sequencer_newSequence_action(void);
void sequencer_newStep_action(void);

/******************************************************************************/
#endif

