/*
 * rational_ratios.h
 *
 *  Created on: Dec 30, 2023
 *      Author: XavSab
 */

#ifndef RATIONAL_RATIOS_H_
#define RATIONAL_RATIOS_H_

#include <stdint.h>
#include <math.h>
#include "constants.h"

/*----------------------------------------------------------------------------
 * Produces a rational number in interval [0 ; 8] chosen in a table of 53 quotients based on 1/2,
 * 1/3, 1/4, 1/5, 1/6, 1/8 fractions.
 * CCval is the mapped midi value in [0 ; 127] to choose the ratio.
 ----------------------------------------------------------------------------*/
float rational_midi_get(uint8_t CCval);

#endif /* RATIONAL_RATIOS_H_ */
