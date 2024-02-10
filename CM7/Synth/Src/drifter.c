/**
 ******************************************************************************
 * File Name          : drifter.c
 * Author			  : Xavier Halgand
 * Date               :
 * Description        : random segment waveform generator
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

/*-------------------------------------------------------------------------------------------*/
#include "drifter.h"

/*-------------------------------------------------------------------------------------------*/
void Drifter_newSegment(Drifter_t *d);

/*-------------------------------------------------------------------------------------------*/
void Drifter_params_save(const Drifter_t *d, DrifterParams_t *params)
{
	params->fmax = d->fmax;
	params->fmin = d->fmin;
	params->gain = d->gain;
}

/*-------------------------------------------------------------------------------------------*/
void Drifter_params_set(const DrifterParams_t *params, Drifter_t *d)
{
	d->fmax = params->fmax;
	d->fmin = params->fmin;
	d->gain = params->gain;
	d->final = 0;
	Drifter_newSegment(d);
}

/*---------------------------------------------------------------------------------------------*/
void Drifter_init(Drifter_t *d)
{
	d->final = 0;
	d->fmax = 4;
	d->fmin = 2;
	d->gain = .01f;

	Drifter_newSegment(d);
}

/*-------------------------------------------------------------------------------------------*/
float Drifter_nextSample(Drifter_t *d) //
{
	d->out = d->gain * (d->slope * d->n + d->initial);
	(d->n)++;
	if (d->n >= d->length)
		{
			Drifter_newSegment(d);
		}
	return d->out;
}

/*-------------------------------------------------------------------------------------------*/
void Drifter_newSegment(Drifter_t *d) //
{
	d->n = 0;
	d->initial = d->final;
	d->minLength = 0.5f * SAMPLERATE / d->fmax ;
	d->maxLength = 0.5f * SAMPLERATE / d->fmin ;
	d->length = frand_a_b(d->minLength,d->maxLength);
	d->final = frand_a_b(-1, 1);
	d->slope = (d->final - d->initial) / d->length ;
}

/*------------------------------------------END----------------------------------------------*/

