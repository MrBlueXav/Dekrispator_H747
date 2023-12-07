/**
 ******************************************************************************
 * @file     phaser.h
 * @author  Xavier Halgand, thanks to Ross Bencina and music-dsp.org guys !
 * @version
 * @date    december 2012
 * @brief

 ******************************************************************************
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 */


#ifndef __PHASER_H__
#define __PHASER_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "constants.h"
#include "sinetable.h"

/*----------------------------------------------------------------------------*/
typedef struct
{
	float_t 	fmin;
	float_t		fmax;
	float_t 	swrate;	// sweep rate
	float_t 	fdb;
	float_t		wet;

} PhaserParams_t;

/* Exported functions ------------------------------------------------------- */
void PhaserInit(void);
float Phaser_compute(float xin);
void Phaser_Rate_set(uint8_t val);
void Phaser_Feedback_set(uint8_t val);
void Phaser_Wet_set(uint8_t val);
void PhaserRate(float rate);
void PhaserFeedback(float fdb);
void Phaser_params_set(const PhaserParams_t *params);
void Phaser_params_save(PhaserParams_t *params);

/*---------------------------------------------------------------------------*/
#endif
