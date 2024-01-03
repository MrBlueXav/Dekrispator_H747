/*
 * metronome.c
 *
 *  Created on: Dec 29, 2023
 *      Author: Xavier Halgand
 */

#include "metronome.h"

/****************************************************************************************/

void Metro_params_set(const MetroParams_t *params, Metro_t *metro)
{
	metro->freq_ = params->freq_;
	metro->phs_ = 0.0f;
	metro->phs_inc_ = (_2PI * metro->freq_) / metro->sample_rate_;
}

void Metro_params_save(const Metro_t *metro, MetroParams_t *params)
{
	params->freq_ = metro->freq_;
}


/** Initializes Metro module.
 Arguments:
 - freq : frequency at which new clock signals will be generated
 Input Range: 0 ... sample_rate/2
 - sample_rate : sample rate of audio engine

 */
void metro_init(Metro_t *metro, float freq, float sample_rate)
{
	metro->freq_ = freq;
	metro->phs_ = 0.0f;
	metro->sample_rate_ = sample_rate;
	metro->phs_inc_ = (_2PI * freq) / sample_rate;
}

/** Initializes Metro module in BPM.
 Arguments:
 - bpm : frequency (expressed in beats per minute) at which new clock signals will be generated
 Input Range: 0 ... sample_rate/2
 - sample_rate : sample rate of audio engine

 */
void metro_initBPM(Metro_t *metro, float bpm, float sample_rate)
{
	metro->freq_ = bpm / 60.0f;
	metro->phs_ = 0.0f;
	metro->sample_rate_ = sample_rate;
	metro->phs_inc_ = (_2PI * metro->freq_) / sample_rate;
}

/** checks current state of Metro_t object and updates state if necessary.
 */
uint8_t metro_process(Metro_t *metro)
{
	metro->phs_ += metro->phs_inc_;
	if (metro->phs_ >= _2PI)
	{
		metro->phs_ -= _2PI;
		return 1;
	}
	return 0;
}

/** resets phase to 0
 */
void metro_reset(Metro_t *metro)
{
	metro->phs_ = 0.0f;
}

void metro_setFreq(Metro_t *metro, float freq)
{
	metro->freq_ = freq;
	metro->phs_inc_ = (_2PI * metro->freq_) / metro->sample_rate_;

}
void metro_setBPM(Metro_t *metro, float bpm)
{
	metro->freq_ = bpm / 60.0f;
	metro->phs_inc_ = (_2PI * metro->freq_) / metro->sample_rate_;
}
float metro_getFreq(Metro_t *metro)
{
	return metro->freq_;
}
float metro_getBPM(Metro_t *metro)
{
	return (metro->freq_) * 60.0f;
}
/*---------------------------------------------------------------------------------------*/
