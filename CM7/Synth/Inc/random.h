/**
 ******************************************************************************
 * File Name          : random.h
 * Author			  			: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __RANDOM_H__
#define __RANDOM_H__

/* Includes ------------------------------------------------------------------*/

#include  <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "constants.h"
#include "stm32h7xx_hal.h"
#include "main.h"


/*-Exported functions---------------------------------------------------------*/

float 	frand_a_b (float a, float b);
float 	randomNum (void);
uint8_t 	MIDIrandVal(void);
float 		mayTrig(float prob);

/************************************************************************************/
#endif
