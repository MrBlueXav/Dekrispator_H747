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
float frand_a_b(float a, float b)
{
	return (rand() / (float) RAND_MAX) * (b - a) + a;
}

//---------------------------------------------------------------------------
/**************
 * returns a random float between 0 and 1
 *****************/
float randomNum(void)
{
	return ((float) rand() / (float) RAND_MAX);
}

/*-----------------------------------------------------------------------------*/
/**************
 * returns a random integer between 0 and MIDI_MAX
 *****************/
uint8_t MIDIrandVal(void)
{
	return (uint8_t) lrintf(frand_a_b(0, MIDI_MAX));
}

/*-----------------------------------------------------------------------------*/
/**************
* Probabilistically generates triggers

 \param : prob (1 always returns 1.0, 0 always 0.0)

 \return : given a probability 0 to 1, returns 1.0 or 0.0.
 *****************/
float mayTrig(float prob)
{
	return ((float) rand() / (float) RAND_MAX) <= prob ? 1.0f : 0.0f;
}

//-------------------------------------------------------------------------------------------------------
