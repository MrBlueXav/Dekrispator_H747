/**
 ******************************************************************************
 * File Name          : random.c
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
//-------------------------------------------------------------------------------------------------------

#include "random.h"

//---------------------------------------------------------------------------
/**************
 * returns a random float between a and b
 *****************/
float_t frand_a_b(float_t a, float_t b)
{
	return ( rand()/(float_t)RAND_MAX ) * (b-a) + a ;
}


//---------------------------------------------------------------------------
/**************
 * returns a random float between 0 and 1
 *****************/
float_t randomNum(void)
{
	float_t random = 1.0f;

	random = rand()/4294967294.0f;
	return random;
}

/*-----------------------------------------------------------------------------*/
/**************
 * returns a random integer between 0 and MIDI_MAX
 *****************/
uint8_t MIDIrandVal(void)
{
	return (uint8_t)lrintf(frand_a_b(0 , MIDI_MAX));
}

//-------------------------------------------------------------------------------------------------------
