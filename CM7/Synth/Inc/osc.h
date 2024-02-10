/*
 * 	osc.h
 *	Simple oscillator with various waveforms
 *  Created on: Jan 26, 2024
 *      Author: Xavier Halgand
 */

#ifndef OSC_H_
#define OSC_H_

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"
#include "math_tools.h"
#include "random.h"
#include "sinetable.h"

/*----------------------------------------------------------------------------*/

typedef enum
{
	ACCSIN, WTSIN, SIN5, MORPH, TRI, SAW, SQSAW, MSAW, SQUARE, WNOISE, LAST_TYPE

} OscType_t;

/*----------------------------------------------------------------------------*/
typedef struct
{
	OscType_t type;
	float_t amp;	// should be <= 1 for normal sound output
	float_t last_amp;
	float_t freq;	// Hertz
	float_t phase;	// radians
	float_t phi0;	// radians
	float_t modInd;	// Modulation Index for FM
	float_t mul;	// pitch frequency multiplier
	float_t out;	// output sample

} Oscillator_t;

/*----------------------------------------------------------------------------*/
typedef struct
{
	OscType_t type;
	float amp;	// should be <= 1 for normal sound output
	float last_amp;
	float freq;	// Hertz
	float modInd;	// Modulation Index for FM
	float mul;	// pitch frequency multiplier

} OscillatorParams_t;

/* Exported functions ------------------------------------------------------- */

void Oscillator_params_set(const OscillatorParams_t *params, Oscillator_t *op);
void Oscillator_params_save(const Oscillator_t *op, OscillatorParams_t *params);
void Osc_init(Oscillator_t *op, float_t amp, float_t freq);
void OpSetFreq(Oscillator_t *op, float_t f);
void FM_op_freq_set(Oscillator_t *op, uint8_t val);
void FM_op_modInd_set(Oscillator_t *op, uint8_t val);
float_t OpSampleCompute(Oscillator_t *op, uint8_t type);
float_t Osc_FM_sine_SampleCompute(Oscillator_t *op, float FMinput);
float_t OpSampleCompute7bis(Oscillator_t *op);

/**********************************************************************************************************************/
#endif /* OSC_H_ */
