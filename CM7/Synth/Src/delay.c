/**
 ******************************************************************************
 * File Name		: delay.c
 * Author			: Xavier Halgand
 * Date				:
 * Description		: simple delay/echo effect with low pass filter in the feedback loop
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
#include "delay.h"

/*-------------------------------------------------------------------------------------------*/
/* Delay effect variables  */
static float 		delayline[DELAYLINE_LEN + 2];
static float 		*readpos _DTCMRAM_; 	// output pointer of delay line
static float 		*writepos _DTCMRAM_; 	// input pointer of delay line
static uint32_t 	shift _DTCMRAM_;		// delay in number of samples
static float 		coeff_a1 _DTCMRAM_; 	// coeff for the one pole low-pass filter in the feedback loop
											// coeff_a1 is between 0 and 1
											//	 0 : no filtering ;     1 : heavy filtering
static float 		old_dy _DTCMRAM_; 		//previous delayed sample
static float 		fdb _DTCMRAM_; 			// feedback
static float 		wet _DTCMRAM_; 			// wet/dry factor

/*-------------------------------------------------------------------------------------------*/
void Delay_init(void)
{
	/* initialize pointers positions for delay effect */
	Delay_clean();
	coeff_a1 = 0.6f;
	wet = INIT_WET;
	fdb = INIT_FEEDB;
	shift = INIT_DELAY;
	readpos = delayline;
	writepos = delayline + shift;

}
/*-------------------------------------------------------------------------------------------*/
void _ITCMRAM_ Delay_clean(void)
{
	for (int i = 0; i < DELAYLINE_LEN + 2; i++)
		delayline[i] = 0;
}

/*-------------------------------------------------------------------------------------------*/
void Delay_params_set(const DelayParams_t *params)
{
	coeff_a1 = params->coeff_a1;
	wet = params->wet;
	fdb = params->fdb;
	shift = params->shift;
	readpos = delayline;
	writepos = delayline + shift;
	Delay_clean(); // useful ??
}

/*-------------------------------------------------------------------------------------------*/
void Delay_params_save(DelayParams_t *params)
{
	params->shift = shift;
	params->coeff_a1 = coeff_a1;
	params->fdb = fdb;
	params->wet = wet;
}

/*-------------------------------------------------------------------------------------------*/
void Delay_time_inc(uint8_t val)
{
	if (val == MIDI_MAXi)
	{

		float *pos;
		if (shift < (DELAYLINE_LEN - DELTA_DELAY))
			shift += DELTA_DELAY;
		pos = writepos - shift;
		if (pos >= delayline)
			readpos = pos;
		else
			readpos = pos + DELAYLINE_LEN - 1;
	}
}
/*-------------------------------------------------------------------------------------------*/
void Delay_time_dec(uint8_t val)
{
	if (val == MIDI_MAXi)
	{

		float *pos;
		if (shift > (MIN_DELAY + DELTA_DELAY))
			shift -= DELTA_DELAY;
		pos = writepos - shift;
		if (pos >= delayline)
			readpos = pos;
		else
			readpos = pos + DELAYLINE_LEN - 1;
	}
}
/*-------------------------------------------------------------------------------------------*/
void Delay_time_set(uint8_t val)
{
	float *pos;
	shift = (uint32_t) lrintf((DELAYLINE_LEN - 1 - MIN_DELAY) / MIDI_MAX * val + MIN_DELAY);
	pos = writepos - shift;
	if (pos >= delayline)
		readpos = pos;
	else
		readpos = pos + DELAYLINE_LEN - 1;

}
/*-------------------------------------------------------------------------------------------*/
void Delay_feedback_inc(void)
{
	/* increment feedback delay */

	fdb *= 1.05f; 			//
}
/*-------------------------------------------------------------------------------------------*/
void Delay_feedback_dec(void)
{
	/* decrement feedback delay */

	fdb *= 0.95f; 			//
}
/*-------------------------------------------------------------------------------------------*/
void DelayFeedback_set(uint8_t val)
{
	fdb = val / MIDI_MAX;
}
/*-------------------------------------------------------------------------------------------*/
void DelayWet_set(uint8_t val)
{
	wet = val / MIDI_MAX;
}
/*-------------------------------------------------------------------------------------------*/
float _ITCMRAM_ Delay_compute(float x)
{
	float y, dy;

	// (*readpos) : delayed sample read at the output of the delay line
	dy = (1.f - coeff_a1) * (*readpos) + coeff_a1 * old_dy; // apply lowpass filter in the loop
	old_dy = dy;
	y = x + fdb * dy;

	y = (y > 1.0f) ? 1.0f : y; //clip too loud samples
	y = (y < -1.0f) ? -1.0f : y;
	*writepos = y; // write new computed sample at the input of the delay line

	/* update the delay line pointers : */
	writepos++;
	readpos++;

	if ((writepos - delayline) >= DELAYLINE_LEN)
		writepos = delayline; // wrap pointer

	if ((readpos - delayline) >= DELAYLINE_LEN)
		readpos = delayline;  // wrap pointer

	return (wet * y + (1 - wet) * x);

}
/**************************************************  EOF  ****************************************************/

