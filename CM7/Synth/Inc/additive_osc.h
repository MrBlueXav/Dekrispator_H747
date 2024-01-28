/*
 *  additive_osc.h
 *
 *  Created on: Jan 27, 2024
 *      Author: Xavier Halgand
 */
/*-----------------------------------------------------------------------------------------*/
#ifndef ADDITIVE_OSC_H_
#define ADDITIVE_OSC_H_

/*-----------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "constants.h"
#include "random.h"
#include "sinetable.h"

/*-----------------------------------------------------------------------------------------*/
#define PARTIALS_NUMBER		10 // number of computed harmonics in the additive generator

/*-----------------------------------------------------------------------------------------*/
typedef struct
{
	float 	amp;						// should be <= 1 for normal sound output
	float	last_amp;
	float	freq;						// Hertz
	float 	a[PARTIALS_NUMBER + 1]; 	// additive generator amplitudes
	float 	ph[PARTIALS_NUMBER + 1]; 	// additive generator phases
	float 	out;						// output sample

} Add_oscillator_t;

/*-----------------------------------------------------------------------------------------*/
typedef struct
{
	float 	amp;						// should be <= 1 for normal sound output
	float	last_amp;
	float	freq;						// Hertz
	float 	a[PARTIALS_NUMBER + 1]; 	// additive generator amplitudes

} Add_oscillatorParams_t;

/*-----------------------------------------------------------------------------------------*/
void 	AddOsc_params_set(const Add_oscillatorParams_t *params, Add_oscillator_t *osc);
void 	AddOsc_params_save(const Add_oscillator_t *osc, Add_oscillatorParams_t *params);
void 	AddOsc_init(Add_oscillator_t *osc, float amp, float freq);
void 	AddOsc_gen_newWaveform(Add_oscillator_t *osc);	// create a new random waveform
float 	AddOsc_SampleCompute(Add_oscillator_t *osc);  // additive sine generator
void	AddOsc_freq_set(Add_oscillator_t *osc, float freq);

/*-----------------------------------------------------------------------------------------*/
#endif /* ADDITIVE_OSC_H_ */
