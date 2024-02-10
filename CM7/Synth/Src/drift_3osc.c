/*
 * 	drift_3osc.c
 *	A voice with 3 drifting oscillators
 *  Created on: Jan 27, 2024
 *      Author: Xavier Halgand
 */

/*--------------------------------------------------------------------------------------------------------*/
#include "drift_3osc.h"

/*--------------------------------------------------------------------------------------------------------*/
void DriftOsc_init(DriftingOsc_t *osc)
{
	Osc_init(&osc->op1, 0.8f, 587.f);
	Osc_init(&osc->op2, 0.8f, 587.f);
	Osc_init(&osc->op3, 0.8f, 587.f);
	Drifter_init(&osc->d1);
	Drifter_init(&osc->d2);
	Drifter_init(&osc->d3);
	osc->centralFreq = 440.0f;
}

/*--------------------------------------------------------------------------------------------------------*/
void DriftOsc_params_set(const DriftingOscParams_t *params, DriftingOsc_t *osc)
{
	Oscillator_params_set(&params->op1, &osc->op1);
	Oscillator_params_set(&params->op2, &osc->op2);
	Oscillator_params_set(&params->op3, &osc->op3);
	Drifter_params_set(&params->d1, &osc->d1);
	Drifter_params_set(&params->d2, &osc->d2);
	Drifter_params_set(&params->d3, &osc->d3);
	osc->centralFreq = params->centralFreq;
}

/*--------------------------------------------------------------------------------------------------------*/
void DriftOsc_params_save(const DriftingOsc_t *osc, DriftingOscParams_t *params)
{
	Oscillator_params_save(&osc->op1, &params->op1);
	Oscillator_params_save(&osc->op2, &params->op2);
	Oscillator_params_save(&osc->op3, &params->op3);
	Drifter_params_save(&osc->d1, &params->d1);
	Drifter_params_save(&osc->d2, &params->d2);
	Drifter_params_save(&osc->d3, &params->d3);
	params->centralFreq = osc->centralFreq;
}

/*--------------------------------------------------------------------------------------------------------*/
float DriftOsc_sample_compute(DriftingOsc_t *osc)
{
	float y;

	OpSetFreq(&osc->op1, 25 + osc->centralFreq * (1 + Drifter_nextSample(&osc->d1)));
	OpSetFreq(&osc->op2, 25 + osc->centralFreq * (1 + Drifter_nextSample(&osc->d2)));
	OpSetFreq(&osc->op3, 25 + osc->centralFreq * (1 + Drifter_nextSample(&osc->d3)));

	y = 0.33f
			* (OpSampleCompute(&osc->op1, MSAW) + OpSampleCompute(&osc->op2, MSAW) + OpSampleCompute(&osc->op3, MSAW));
	return y;
}

/*--------------------------------------------------------------------------------------------------------*/
void DriftOsc_freq_set(DriftingOsc_t *osc, float freq)
{
	osc->centralFreq = freq;
}

/*--------------------------------------------------------------------------------------------------------*/
void DriftOsc_centralFreq_set(DriftingOsc_t *osc, uint8_t midival)
{
	osc->centralFreq = Lin2Exp(midival, 1, 6000);
}

/*-------------------------------------------------------------------------------------------*/
void DriftOsc_amp_set(DriftingOsc_t *osc, uint8_t midival)
{
	osc->d1.gain = osc->d2.gain = osc->d3.gain = (midival / MIDI_MAX) * (midival / MIDI_MAX);

}

/*-------------------------------------------------------------------------------------------*/
void DriftOsc_minFreq_set(DriftingOsc_t *osc, uint8_t midival)
{
	osc->d1.fmin = osc->d2.fmin = osc->d3.fmin = .1f + 50 * midival / MIDI_MAX;

}

/*-------------------------------------------------------------------------------------------*/
void DriftOsc_maxFreq_set(DriftingOsc_t *osc, uint8_t midival)
{
	float x;

	x = (1 + 19 * midival / MIDI_MAX) * osc->d1.fmin;
	if (x > 10000)
		osc->d1.fmax = osc->d2.fmax = osc->d3.fmax = 10000;
	else
	{
		osc->d1.fmax = osc->d2.fmax = osc->d3.fmax = x;
	}
}
/*--------------------------------------------------------------------------------------------------------*/
