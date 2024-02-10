/*
 *  additive_osc.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Xavier Halgand
 */

/*--------------------------------------------------------------------------------------*/
#include "additive_osc.h"

/*--------------------------------------------------------------------------------------*/
void AddOsc_params_set(const Add_oscillatorParams_t *params, Add_oscillator_t *osc)
{
	osc->amp = params->amp;
	osc->last_amp = params->last_amp;
	osc->freq = params->freq;
	for (uint8_t k = 0; k < PARTIALS_NUMBER + 1; k++)
	{
		osc->a[k] = params->a[k];
	}
}

/*--------------------------------------------------------------------------------------*/
void AddOsc_params_save(const Add_oscillator_t *osc, Add_oscillatorParams_t *params)
{
	params->amp = osc->amp;
	params->last_amp = osc->last_amp;
	params->freq = osc->freq;
	for (uint8_t k = 0; k < PARTIALS_NUMBER + 1; k++)
	{
		params->a[k] = osc->a[k];
	}
}

/*--------------------------------------------------------------------------------------*/
void AddOsc_init(Add_oscillator_t *osc, float amp, float freq)
{
	osc->amp = amp;
	osc->last_amp = amp;
	osc->freq = freq;
	osc->out = 0;
}

/*--------------------------------------------------------------------------------------*/
void AddOsc_freq_set(Add_oscillator_t *osc, float freq)
{
	osc->freq = freq;
}

/*--------------------------------------------------------------------------------------*/
void AddOsc_gen_newWaveform(Add_oscillator_t *osc) // create a new random waveform
{
	uint8_t k;

	osc->a[0] = osc->a[1] = 1;
	osc->ph[0] = osc->ph[1] = 0;

	for (k = 2; k <= PARTIALS_NUMBER; k++)
	{
		osc->a[k] = frand_a_b(0.001f, 0.98f);
		osc->a[0] += osc->a[k];
		osc->ph[k] = 0;
	}
	osc->a[0] = 1.f / osc->a[0];
}

/*-------------------------------------------------------------------------------------*/
float AddOsc_SampleCompute(Add_oscillator_t *osc) // additive sine generator
{
	uint8_t k = 1;
	float y = 0;

	while ((k <= PARTIALS_NUMBER) && (k * osc->freq < SAMPLERATE / 2.f))
	{
		while (osc->ph[k] >= _2PI) // keep phase in [0, 2pi]
			osc->ph[k] -= _2PI;

		y += osc->a[k] * sinetable[(uint16_t) (ALPHA * (osc->ph[k]))];
		osc->ph[k] += _2PI * Ts * k * osc->freq; // increment phase
		k++;
	}
	y = osc->a[0] * y;

	osc->out = osc->amp * y;

	return osc->out;
}
/***************************************************** EOF ******************************************/
