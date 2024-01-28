/*
 * drift_3osc.c
 *
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
