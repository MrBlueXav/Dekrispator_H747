/**
 ******************************************************************************
 * File Name          : random.h
 * Author			  			: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __DEKR_RANDOM_H__
#define __DEKR_RANDOM_H__

/* Includes ------------------------------------------------------------------*/

#include  <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "constants.h"

/*-Exported functions---------------------------------------------------------*/

float 		frand_a_b (float a, float b);
float 		randomNum (void);
uint8_t 	MIDIrandVal(void);
float 		mayTrig(float prob);

/************************************************************************************/
#endif
