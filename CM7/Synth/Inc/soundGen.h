/**
 ******************************************************************************
 * File Name          	: soundGen.h
 * Author				: Xavier Halgand
 * Date               	:
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
#include "constants.h"
#include "MIDI_application.h"
#include "math_tools.h"
#include "random.h"
#include "sequencer.h"
#include "oscillators.h"
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

/* Exported functions ------------------------------------------------------- */

void setGateTime(uint8_t val);
void AttTime_set(uint8_t val);
void DecTime_set(uint8_t val);
void SustLevel_set(uint8_t val);
void RelTime_set(uint8_t val);
void ADSRkeyON(void);
void ADSRkeyOFF(void);

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

//------------------------------------------------------------------------------------
void 	Filter1Freq_set(uint8_t val);
void 	Filter1Res_set(uint8_t val);
void	Filter1Drive_set(uint8_t val);
void 	Filter1Type_set(uint8_t val);
/*-----------------------------------------------------------------------------------*/
void 	Filter2Freq_set(uint8_t val);
void 	Filter2Res_set(uint8_t val);
void	Filter2Drive_set(uint8_t val);
void 	Filter2Type_set(uint8_t val);

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


void soundGenInit(void);
void soundGenNewWave(void);
void Synth_Init(void);

void make_sound(uint16_t *buf , uint16_t len);
void OpsRandFreq(void);

#endif /* __SOUNDGEN_H */
