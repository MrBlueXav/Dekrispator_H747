/*
 * metronome.h
 *
 * Creates a clock signal at a specific frequency : a "1" byte sample every 1/freq second and "0" else.
 *
 *
 *  Created on: Dec 29, 2023
 *      Author: Xavier Halgand & ElectroSmith team in DaisySP
 */

#ifndef METRONOME_H_
#define METRONOME_H_

/*----------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "constants.h"

/*----------------------------------------------------------------------------------------*/
typedef struct
{
	float freq_;
	float phs_, sample_rate_, phs_inc_; // internals

} Metro_t;

typedef struct
{
	float freq_;
} MetroParams_t;

/*---------------------------------------------------------------------------------------*/
void Metro_params_set(const MetroParams_t *params, Metro_t *metro);
void Metro_params_save(const Metro_t *metro, MetroParams_t *params);

/** Initializes Metro module.
        Arguments:
        - freq : frequency at which new clock signals will be generated
            Input Range: 0 ... sample_rate/2
        - sample_rate : sample rate of audio engine

    */
void metro_init(Metro_t *metro, float freq, float sample_rate);

/** Initializes Metro module in BPM.
        Arguments:
        - bpm : frequency (expressed in beats per minute) at which new clock signals will be generated
            Input Range: 0 ... sample_rate/2
        - sample_rate : sample rate of audio engine

    */
void metro_initBPM(Metro_t *metro, float bpm, float sample_rate);

/** checks current state of Metro_t object and updates state if necessary.
    */
uint8_t metro_process(Metro_t *metro);

/** resets phase to 0
    */
void metro_reset(Metro_t *metro);

void metro_setFreq(Metro_t *metro, float freq);
void metro_setBPM(Metro_t *metro, float freq);
float metro_getFreq(Metro_t *metro);
float metro_getBPM(Metro_t *metro);
/*---------------------------------------------------------------------------------------*/

#endif /* METRONOME_H_ */
