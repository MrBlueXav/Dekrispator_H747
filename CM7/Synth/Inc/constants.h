/**
 ******************************************************************************
 * File Name          : constants.h
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/*--------------------------------------------------------------------------------------*/

#define SAMPLERATE              48000

//#define Fs             			(float)SAMPLERATE	/* samplerate */
#define Ts						(1.f/SAMPLERATE)  // sample period
#define _2PI                    6.283185307f
#define _PI                    	3.14159265f

//#define BUFF_LEN_DIV4           1000 // number of samples in the audiobuffer for each channel (determines latency)
//#define BUFF_LEN_DIV2           (2*BUFF_LEN_DIV4)
//#define BUFF_LEN                (4*BUFF_LEN_DIV4)  // Audio buffer length : count in 16bits half-words, must be a multiple of 4.
#define VOL                     70 // initial output DAC volume
#define MAXVOL                  100 // maximal output DAC volume

#define PARAM_MAX				21 // maximal parameter index, starting at 0

#define MIDI_MAX				127.f 	// floating max value
#define MIDI_MAXi				127		// integer max value
#define MIDI_MID_i				64		// integer mid value

//#define _CCM_					__attribute__((section(".ccmram"))) // for use of CCM RAM (64kB)
//#define _DTCM_					__attribute__((section(".dtcm_vars")))
//#define _DTCMRAM_					__attribute__((section(".DTCMRAM_section")))
#define _DTCMRAM_
//#define	_ITCMRAM_					__attribute__((section(".itcm_text")))
#define	_ITCMRAM_

#define _CCM_
#define _DTCM_

#if defined   (__GNUC__)        /* GNU Compiler */
    #define __ALIGN    __attribute__ ((aligned (4)))
#endif /* __GNUC__ */

/************************************************************************************/
#endif  /*__CONSTANTS_H__ */
