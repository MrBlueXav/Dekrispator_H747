/*
 * rational_ratios.c
 *
 *  Created on: Dec 30, 2023
 *      Author: XavSab
 */
#include "rational_ratios.h"

#define RATIOS_NUMB 53

const float rational_ratios[RATIOS_NUMB] = {
		1.0/8, 1.0/6, 1.0/5, 1.0/4, 1.0/3, 3.0/8, 2.0/5, 1.0/2, 		/* <= 0.5  */
		3.0/5, 5.0/8, 2.0/3, 3.0/4, 4.0/5, 5.0/6, 7.0/8,				/* <= 1.0 */
		1, 9.0/8, 7.0/6, 6.0/5, 5.0/4, 4.0/3, 11.0/8, 7.0/5, 3.0/2, 	/* <= 1.5  */
		8.0/5, 13.0/8, 5.0/3, 7.0/4, 9.0/5, 11.0/6, 15.0/8, 2,			/* <= 2  */
		17.0/8, 13.0/6, 11.0/5, 9.0/4, 7.0/3, 19.0/8, 12.0/5, 5.0/2,	/* <= 2.5  */
		13.0/5, 8.0/3, 11.0/4, 17.0/6, 3,	/* <= 3  */
		13.0/4, 3.0/5, 15.0/4, 4,			/* <= 4  */
		5, 6, 7, 8		/* <= 8  */
};

float rational_midi_get(uint8_t CCval) {
	return rational_ratios[(uint8_t)roundf (CCval / MIDI_MAX * RATIOS_NUMB)];
}
