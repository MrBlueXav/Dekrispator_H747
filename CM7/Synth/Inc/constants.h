/**
 ******************************************************************************
 * File Name          : constants.h
 * Author			  : Xavier Halgand
 * Date               :
 * Description        : Global defines for Dekrispator
 ******************************************************************************
 */
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/*--------------------------------------------------------------------------------------*/

#define SAMPLERATE              48000 // Don't modify !
#define Ts						(1.f/SAMPLERATE)  // sample period
#define _2PI                    6.283185307f
#define _PI                    	3.14159265f

#define VOL                     70 // initial output DAC volume
#define MAXVOL                  100 // maximal output DAC volume

#define MIDI_MAX				127.f 	// floating max value
#define MIDI_MAXi				127		// integer max value
#define MIDI_MID_i				64		// integer mid value

//#define _CCM_					__attribute__((section(".ccmram"))) // for use of CCM RAM (64kB)
//#define _DTCM_				__attribute__((section(".dtcm_vars")))
//#define _DTCMRAM_				__attribute__((section(".DTCMRAM_section")))
//#define _ITCMRAM_				__attribute__((section(".itcm_text")))
#define	_ITCMRAM_
#define _DTCMRAM_

#define _CCM_
#define _DTCM_

#if defined   (__GNUC__)        /* GNU Compiler */
    #define __ALIGN    __attribute__ ((aligned (4)))
#endif /* __GNUC__ */

/************************************************************************************/
#endif  /*__CONSTANTS_H__ */
