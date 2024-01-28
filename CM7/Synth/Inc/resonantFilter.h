/*
 * resonantFilter.h
 *
 *  Created on: 05.04.2012
 * ------------------------------------------------------------------------------------------------------------------------
 *  Copyright 2013 Julian Schmidt	////// Modified by Xavier Halgand
 *  Julian@sonic-potions.com
 * ------------------------------------------------------------------------------------------------------------------------
 *  This file is part of the Sonic Potions LXR drumsynth firmware.
 * ------------------------------------------------------------------------------------------------------------------------
 *  Redistribution and use of the LXR code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *       - The code may not be sold, nor may it be used in a commercial product or activity.
 *
 *       - Redistributions that are modified from the original source must include the complete
 *         source code, including the source code for all components used by a binary built
 *         from the modified sources. However, as a special exception, the source code distributed
 *         need not include anything that is normally distributed (in either source or binary form)
 *         with the major components (compiler, kernel, and so on) of the operating system on which
 *         the executable runs, unless that component itself accompanies the executable.
 *
 *       - Redistributions must reproduce the above copyright notice, this list of conditions and the
 *         following disclaimer in the documentation and/or other materials provided with the distribution.
 * ------------------------------------------------------------------------------------------------------------------------
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *   USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ------------------------------------------------------------------------------------------------------------------------
 */


#ifndef DEKRISPATOR_V2_SYNTH_RESONANTFILTER_H_
#define DEKRISPATOR_V2_SYNTH_RESONANTFILTER_H_

//----------------------------------------------------

#include <stdint.h>
#include <math.h>

#include "constants.h"
#include "math_tools.h"

//----------------------------------------------------

#define FILTER_TYPES	5 /* Number of filter types */
#define ENABLE_NONLINEAR_INTEGRATORS 	1
#define FILTER_GAIN_F 					0.88f
#define USE_SHAPER_NONLINEARITY 		0
#define MAX_FREQ						8000.f
#define MIN_FREQ						80.f

//----------------------------------------------------
enum filterTypeEnum
{
	FILTER_LP=1,
	FILTER_HP,
	FILTER_BP,
	FILTER_NOTCH,
	FILTER_PEAK
};
//----------------------------------------------------
typedef struct ResoFilterStruct
{
	uint8_t 	type; 	// filter type : LP, HP, BP...
	float 		f;		/* actual relative cutoff freq. (/samplerate) */
	float		f0; 	/* reference relative cutoff freq. */
	float 		g;  	/* embedded integrator gain (Fig 3.11), wc == wa*/
	float 		q;		/* q value calculated from setReso()*/
	float 		s1;
	float 		s2;
	float 		drive;
#if ENABLE_NONLINEAR_INTEGRATORS
	float 		zi;	//input z^(-1)
#endif

#if USE_SHAPER_NONLINEARITY
	Distortion shaper;
#endif

} ResonantFilter;

//-----------------------------------------------------------------------------------------
typedef struct
{
	uint8_t 	type; 	// filter type : LP, HP, BP...
	//float 		f;		/* actual relative cutoff freq. (/samplerate) */
	float		f0; 	/* reference relative cutoff freq. */
	float 		q;		/* q value calculated from setReso()*/
	float 		drive;

} ResonantFilterParams_t;

/*************************************************************************************************************/
void 	ResonantFilter_params_save(const ResonantFilter *filter, ResonantFilterParams_t *params);
void 	ResonantFilter_params_set(const ResonantFilterParams_t *params, ResonantFilter *filter);
void 	SVF_initialize(ResonantFilter* filter);
void 	SVF_setReso(ResonantFilter* filter, float feedback);
void 	SVF_setDrive(ResonantFilter* filter, uint8_t drive);
void 	SVF_directSetFilterValue(ResonantFilter* filter, float val);
void	SVF_refFreq_set(ResonantFilter* filter, float val);
float	SVF_refFreq_get(ResonantFilter* filter);
void 	SVF_recalcFreq(ResonantFilter* filter);
float 	SVF_calcSample(ResonantFilter* filter, float in);

#endif /* DEKRISPATOR_V2_SYNTH_RESONANTFILTER_H_ */
