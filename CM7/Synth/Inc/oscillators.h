/**
 ******************************************************************************
 * File Name          	: oscillators.h
 * Author				: Xavier Halgand
 * Date               	:
 * Description        	:
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OSCILLATORS_H
#define __OSCILLATORS_H

/*--------------------- Global Defines ------------------------ */
#define PARTIALS_NUMBER		10 // number of computed harmonics in the additive generator

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "math_tools.h"
#include "random.h"
#include "drifter.h"
#include "sinetable.h"
#include "notesTables.h"
#include "drifter.h"
#include "blepvco.h"
/*----------------------------------------------------------------------------*/

enum timbre { MORPH_SAW = 0, SPLIT, ACC_SINE, WT_SINE, ADDITIVE, POWER_SINE, BLEPTRIANGLE, BLEPSQUARE,
	NOISE, CHORD15, CHORD135, CHORD13min5, VOICES3, DRIFTERS, FM2, BLEPSAW, LAST_SOUND };

/*----------------------------------------------------------------------------*/
typedef struct
{
	float_t 	amp;	// should be <= 1 for normal sound output
	float_t		last_amp;
	float_t 	freq;	// Hertz
	float_t 	phase;	// radians
	float_t		phi0;	// radians
	float_t		modInd;	// Modulation Index for FM
	float_t		mul;	// pitch frequency multiplier
	float_t 	out;	// output sample

} Oscillator_t;

typedef struct
{
	float_t 	amp;	// should be <= 1 for normal sound output
	float_t		last_amp;
	float_t 	freq;	// Hertz
	float_t		modInd;	// Modulation Index for FM
	float_t		mul;	// pitch frequency multiplier

} OscillatorParams_t;

/* Exported functions ------------------------------------------------------- */

void Oscillator_params_set(const OscillatorParams_t *params, Oscillator_t *op);
void Oscillator_params_save(const Oscillator_t *op, OscillatorParams_t *params);

/*-------------------------------------------------------*/
void 	osc_init(Oscillator_t * op, float_t amp, float_t freq);
/*-------------------------------------------------------*/
float 	waveCompute(uint8_t sound, float f0);
/*-------------------------------------------------------*/

float_t OpSampleCompute(Oscillator_t * op); // accurate sine
/*-------------------------------------------------------*/
float_t OpSampleCompute1(Oscillator_t * op); // basic sawtooth^2
/*-------------------------------------------------------*/
float_t OpSampleCompute2(Oscillator_t * op); // basic sawtooth
/*-------------------------------------------------------*/
float_t OpSampleCompute3(Oscillator_t * op); // sin(phi)^5
/*-------------------------------------------------------*/
float_t OpSampleCompute4(Oscillator_t * op); // Complex waveform : +/- |sin(phi)|^alpha(freq), tends to a sine at high freqs
/*-------------------------------------------------------*/
float_t OpSampleCompute5(Oscillator_t * op); // Basic Triangle
/*-------------------------------------------------------*/
float_t MorphingSaw_SampleCompute(Oscillator_t * op); // Morphing sawtooth, tends to a triangle at high freqs
/*-------------------------------------------------------*/
float_t Osc_WT_SINE_SampleCompute(Oscillator_t * op) ;// basic wave table sine
float_t Osc_FM_sine_SampleCompute(Oscillator_t * op, float FMinput); // basic wave table sine with FM
/*-------------------------------------------------------*/
float_t OpSampleCompute7bis(Oscillator_t * op); // basic wave table positive sine : 0 < output < op.amp
/*-------------------------------------------------------*/
float_t BasicSquare_SampleCompute(Oscillator_t * op); // basic square
/*-------------------------------------------------------*/
void 	AdditiveGen_newWaveform(void); //
/*-------------------------------------------------------*/
float_t AdditiveGen_SampleCompute(Oscillator_t * op); // additive sine generator
/*-------------------------------------------------------*/
float_t FM1_sampleCompute(void); // op4 -> op3 -> op2 -> op1 => sound
float_t FM2_sampleCompute(float frq); // (op2 -> op1) + (op4 -> op3) => sound
void 	FM_op_freq_set(Oscillator_t *op, uint8_t val);
void 	FM_op_modInd_set(Oscillator_t *op, uint8_t val);

/*-------------------------------------------------------*/
void 	OpSetFreq(Oscillator_t * op, float_t f);
/*-------------------------------------------------------*/
void	Drifter_centralFreq_set(uint8_t val);
float	Lin2Exp(uint8_t val, float min, float max); // input val in 0 ... 127


/****************************************************************************************************************************/
#endif /* __OSCILLATORS_H */
