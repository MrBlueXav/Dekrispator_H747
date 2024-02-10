/*
 * 	osc.c
 *	Simple oscillator with various waveforms
 *  Created on: Jan 26, 2024
 *      Author: Xavier Halgand
 */

#include "osc.h"

/*===============================================================================================================*/

/*--------------------- Save parameters from op oscillator to params structure ----------------------------------*/

void Oscillator_params_save(const Oscillator_t *op, OscillatorParams_t *params)
{
	params->type = op->type;
	params->amp = op->amp;
	params->last_amp = op->last_amp;
	params->freq = op->freq;
	params->modInd = op->modInd;
	params->mul = op->mul;
}

/*------------------ Set op parameters using params struct ---------------------------------------------------*/

void Oscillator_params_set(const OscillatorParams_t *params, Oscillator_t *op)
{
	op->type = params->type;
	op->amp = params->amp;
	op->last_amp = params->last_amp;
	op->freq = params->freq;
	op->modInd = params->modInd;
	op->mul = params->mul;

	op->phase = 0;
	op->out = 0;
}

/*-------------------------------------------------------*/
void Osc_init(Oscillator_t *op, float_t amp, float_t freq)
{
	op->type = WTSIN;
	op->amp = amp;
	op->last_amp = amp;
	op->freq = freq;
	op->phase = 0;
	op->out = 0;
	op->modInd = 0;
	op->mul = 1;
}
/*-------------------------------------------------------*/

void OpSetFreq(Oscillator_t *op, float_t f)
{
	op->freq = f;
}

/*---------------------------------------------------------------*/
void FM_op_freq_set(Oscillator_t *op, uint8_t midival)
{
	op->freq = Lin2Exp(midival, 0.5f, 8000.f);
}

/*-------------------------------------------------------*/
void FM_op_modInd_set(Oscillator_t *op, uint8_t midival)
{
	op->modInd = (1.4f * midival / MIDI_MAX) * (1.4f * midival / MIDI_MAX);
}

/*-------------------------------------------------------*/
void Trim_phase(Oscillator_t *op) // keep oscillator phase in [0, 2pi]
{
	while (op->phase < 0)
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;
}

/*----------------------------------------------------------------------------------------------*/
float_t _ITCMRAM_ OpSampleCompute(Oscillator_t *op, OscType_t type)
{
	float_t z;

	Trim_phase(op);

	switch (type)
	{
	case ACCSIN:
		z = sinf(op->phase);
		break;

	case WTSIN:			// basic wave table sine
		z = sinetable[lrintf(ALPHA * (op->phase))];
		break;

	case SAW:			// basic sawtooth
		if (op->phase < _PI)
			z = 0.5f * (op->phase / _PI);
		else
			z = 0.5f * (op->phase / _PI - 2);
		break;

	case SQSAW: 		// basic sawtooth^2
		if (op->phase < _PI)
			z = 0.5f * (op->phase / _PI) * (op->phase / _PI);
		else
			z = 0.5f * (op->phase / _PI - 2) * (op->phase / _PI - 2);
		break;

	case SIN5:			// sin(phi)^5
		z = sinf(op->phase);
		z = z * z * z * z * z;
		break;

	case MORPH:			// Complex waveform : +/- |sin(phi)|^alpha(freq), tends to a sine at high freqs
		float x, alpha;
		x = op->freq;

		//http://www.math.ucla.edu/~ronmiech/Interpolation/HTMDOCS/Introduction/Interpolation_Applet.htm  :
		alpha = 81.096f + x * (-.037f + .582E-5f * x - .311E-9f * x * x); //alpha varies from 80 to 1 with freq from 30Hz to 8000Hz
		//alpha = 60.695f -.023f * x + .31E-5f * x*x -.141E-9f * x*x*x ;
		//alpha = (50.f-1)/(30.f-8000)*(x - 8000) + 1 ;
		//z = powf(fabsf(sinf(op->phase )), alpha) ;

		z = powf(fabsf(sinetable[lrintf(ALPHA * (op->phase))]), alpha);

		if (op->phase >= _PI)
			z = -z;
		break;

	case TRI:
		if (op->phase < _PI)
			z = -2 / _PI * op->phase + 1;
		else
			z = 2 / _PI * op->phase - 3;
		break;

	case MSAW:
		if (op->freq < 8000)
			op->phi0 = _PI / 8000 * (op->freq);
		else
			op->phi0 = _PI;
		if (op->phi0 <= .0001f)
			op->phi0 = .0001f;
		if (op->phase < op->phi0)
			z = (-2 / op->phi0 * op->phase + 1);
		else
			z = (2 * op->phase - _2PI - op->phi0) / (_2PI - op->phi0);
		break;

	case SQUARE:
		if (op->phase < _PI)
			z = 1.0f;
		else
			z = -1.0f;
		break;

	default:
		z = 0.0f;
		break;

	}

	op->out = op->amp * z;
	op->phase += _2PI * Ts * op->freq; // increment phase

	return op->out;
}

/*-------------------------------------------------------*/
float_t _ITCMRAM_ Osc_FM_sine_SampleCompute(Oscillator_t *op, float FMinput) // basic wave table sine with FM
{

	op->phase += _2PI * Ts * op->freq + op->modInd * FMinput; // increment phase
	Trim_phase(op);
	op->out = op->amp * sinetable[lrintf(ALPHA * (op->phase))];
	return op->out;
}

/*-------------------------------------------------------*/
float_t _ITCMRAM_ OpSampleCompute7bis(Oscillator_t *op) // basic wave table positive sine : 0 < output < op.amp
{
	Trim_phase(op);

	op->out = op->amp * 0.5f * (1.f + sinetable[lrintf(ALPHA * (op->phase))]);
	op->phase += _2PI * Ts * op->freq; // increment phase

	return op->out;
}

/**************************************************EOF*************************************************************/
