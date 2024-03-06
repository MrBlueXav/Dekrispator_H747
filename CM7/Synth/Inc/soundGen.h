/**
 ******************************************************************************
 * File Name          	: soundGen.h
 * Author				: Xavier Halgand
 * Date               	: 2024
 * Description        	:
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOUNDGEN_H
#define __SOUNDGEN_H

/*--------------------- Global Defines ------------------------ */

#define MAX_FILTER_LFO_AMP	2.0f
#define DECAY_FACTOR		0.99975f // initial multiplying factor for the envelope decay
#define MIN_DECAY_FACTOR	0.999f
#define VIBRATO_AMP			0.01f
#define VIBRATO_FREQ		5.5f
#define MAX_VIBRATO_AMP		0.5f
#define MAX_VIBRATO_FREQ	12.f

/* Includes ------------------------------------------------------------------*/

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "binn.h"

#include "constants.h"
#include "osc.h"
#include "additive_osc.h"
#include "drift_3osc.h"
#include "MIDI_application.h"
#include "math_tools.h"
#include "random.h"
#include "sequencer.h"
#include "delay.h"
#include "chorusFD.h"
#include "phaser.h"
#include "sinetable.h"
#include "notesTables.h"
#include "resonantFilter.h"
#include "adsr.h"
#include "blepvco.h"
#include "metronome.h"
#include "rational_ratios.h"

/*------------------------------------------------------------------------------*/
typedef enum
{
	MORPH_SAW = 0,
	WT_SINE,
	ADDITIVE,
	POWER_SINE,
	BLEPTRIANGLE,
	BLEPSQUARE,
	BLEPSAW,
	VOICES3,
	CHORD15,
	FM2,
	DRIFTERS,
	CHORD135,
	CHORD13min5,
	NOISE,
	LAST_SOUND

} Timbre_t;

/*------------------------------------------------------------------------------*/
typedef struct /* Maximum patch size is currently 1024 bytes ! */
{
	uint16_t memory_location; // in 0.... LAST_PATCH

	bool desynkatorON_par;
	bool autoFilterON_par;
	bool delayON_par;
	bool phaserON_par;
	bool chorusON_par;
	Timbre_t sound_par;
	int8_t autoSound_par;

	OscillatorParams_t op1_par;
	OscillatorParams_t op2_par;
	OscillatorParams_t op3_par;
	OscillatorParams_t op4_par;
	OscillatorParams_t oscill2_par;
	OscillatorParams_t oscill3_par;
	OscillatorParams_t vibr_lfo_par;
	OscillatorParams_t filt_lfo_par;
	OscillatorParams_t filt2_lfo_par;
	OscillatorParams_t amp_lfo_par;
	OscillatorParams_t amp_lfo2_par;

	BlepOscillatorParams_t rect_osc1_par;
	BlepOscillatorParams_t rect_osc2_par;
	BlepOscillatorParams_t saw_osc_par;
	BlepOscillatorParams_t tri_osc_par;

	DriftingOscParams_t driftosc_par;

	DrifterParams_t d1_par;
	DrifterParams_t d2_par;

	Add_oscillatorParams_t addosc_par;

	ADSRParams_t adsr_par;
	ADSRParams_t adsr2_par;
	ADSRParams_t adsr3_par;

	ResonantFilterParams_t filt1_par;
	ResonantFilterParams_t filt2_par;

	SequencerParams_t seq_par;

	MetroParams_t metro1_par;
	MetroParams_t metro2_par;
	MetroParams_t metro3_par;
	float proba1_par;
	float proba2_par;
	float proba3_par;

	PhaserParams_t phaser_par;
	DelayParams_t delay_par;
	ChorusParams_t chorus_par;

} SynthPatch_t;

/*------------------------------------------------------------------------------*/
typedef struct
{
	SynthPatch_t mypatch;
	SynthPatch_t initpatch;
	uint16_t currentPatchMemory;
	bool validation;

} PatchMemoryCtl_t;

/*------------------------------------------------------------------------------*/
typedef struct
{
	bool desynkatorON_par;
	bool dekrispatorON_par; // demoModeOn
	bool synthOn_par;

	bool autoFilterON_par;
	bool delayON_par;
	bool phaserON_par;
	bool chorusON_par;

	uint16_t memory_loc_par;
	Timbre_t sound_par;
	bool sequencerIsOn_par;
	float tempo_par;
	uint8_t seq_length_par;
	bool holes_par;
	bool move_par;

	bool usb_midi_par;
	uint32_t cpu_cycles_par;

} ScreenDatas_t;

/* Exported functions ------------------------------------------------------- */

void get_datas_for_screen(volatile ScreenDatas_t *datas);

void setGateTime(uint8_t val);
void AttTime_set(uint8_t val);
void DecTime_set(uint8_t val);
void SustLevel_set(uint8_t val);
void RelTime_set(uint8_t val);
void ADSRkeyON(void);
void ADSRkeyOFF(void);

void Synth_patch_load(const SynthPatch_t *patch);
void Soundpatch_save(uint8_t midival);
void Soundpatch_load(uint8_t midival);
void Memory_erase(uint8_t val);
void Memory_valid(uint8_t val);
void Memory_inc(uint8_t val);
void Memory_dec(uint8_t val);

uint8_t soundNumber_get(void);
void autoSound_set(int8_t val);
void RandSound1(uint8_t val);
void RandSound2(uint8_t val);

void DemoMode_toggle(uint8_t val);
void Sequencer_toggle(uint8_t val);
void Freeze_toggle(uint8_t val);
void Synth_reset(uint8_t val);
void Desynkator_toggle(uint8_t val);

/*-------------------------------------------------------*/
void FM_OP1_freq_set(uint8_t val);
void FM_OP1_modInd_set(uint8_t val);

/*-------------------------------------------------------*/
void FM_OP2_freq_set(uint8_t val);
void FM_OP2_freqMul_inc(uint8_t val);
void FM_OP2_freqMul_dec(uint8_t val);
void FM_OP2_modInd_set(uint8_t val);

/*-------------------------------------------------------*/
void FM_OP3_freq_set(uint8_t val);
void FM_OP3_freqMul_inc(uint8_t val);
void FM_OP3_freqMul_dec(uint8_t val);
void FM_OP3_modInd_set(uint8_t val);

/*-------------------------------------------------------*/
void FM_OP4_freq_set(uint8_t val);
void FM_OP4_freqMul_inc(uint8_t val);
void FM_OP4_freqMul_dec(uint8_t val);
void FM_OP4_modInd_set(uint8_t val);

/*-------------------------------------------------------*/
void AmpLFO_amp_set(uint8_t val);
void AmpLFO_freq_set(uint8_t val);

/*-------------------------------------------------------*/
void DriftOsc1_amp_set(uint8_t val);
void DriftOsc1_minFreq_set(uint8_t val);
void DriftOsc1_maxFreq_set(uint8_t val);
void DriftOsc1_centralFreq_set(uint8_t val);

//------------------------------------------------------------------------------------
void Filter1Freq_set(uint8_t val);
void Filter1Res_set(uint8_t val);
void Filter1Drive_set(uint8_t val);
void Filter1Type_set(uint8_t val);
/*-----------------------------------------------------------------------------------*/
void Filter2Freq_set(uint8_t val);
void Filter2Res_set(uint8_t val);
void Filter2Drive_set(uint8_t val);
void Filter2Type_set(uint8_t val);

void Filt1LFO_amp_set(uint8_t val);
void Filt1LFO_freq_set(uint8_t val);

void Filt2LFO_amp_set(uint8_t val);
void Filt2LFO_freq_set(uint8_t val);

void toggleVibrato(void);
void VibratoAmp_set(uint8_t val);
void VibratoFreq_set(uint8_t val);

void toggleSynthOut(void);
void SynthOut_switch(uint8_t val);

void AdditiveGen_newWaveform(void);

void incSynthOut(void);
void decSynthOut(void);
void SynthOut_amp_set(uint8_t val);

void Delay_toggle(void);
void Delay_switch(uint8_t val);

void toggleFilter(void);
void Filter_Random_switch(uint8_t val);

void Phaser_switch(uint8_t val);

void Chorus_toggle(void);
void Chorus_switch(uint8_t val);

void incDecay(void);
void decDecay(void);
void setDecayFactor(uint8_t val);

void nextSound(void);
void prevSound(void);
void Sound_set(uint8_t val);

void metro_tempo_set(uint8_t val);
void metro2_tempo_set(uint8_t val);
void metro3_tempo_set(uint8_t val);
void metro1_proba_set(uint8_t val);
void metro2_proba_set(uint8_t val);
void metro3_proba_set(uint8_t val);
void metro_reset_rq(uint8_t val);

void MagicFX(uint8_t val);
void MagicPatch(uint8_t val);

float waveCompute(Timbre_t sound, float frq);
void Synth_Init(void);
void make_sound(uint16_t *buf, uint16_t len);

#endif /* __SOUNDGEN_H */
