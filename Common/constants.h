/**
 ******************************************************************************
 * File Name          : constants.h
 * Author			  : Xavier Halgand
 * Date               :
 * Description        : Global defines for Dekrispator
 ******************************************************************************
 */
#ifndef __DEKR_CONSTANTS_H__
#define __DEKR_CONSTANTS_H__

/*--------------------------------------------------------------------------------------*/

#define AUDIO_BUFFER_SIZE       1024 /* AUDIO_BUFFER_SIZE (in bytes) must be a multiple of 8   */

#define FREQ_CM7				400000000

#define SAMPLERATE              48000 // Don't modify !

#define LAST_PATCH				63	//Number - 1  of patches that can be stored in QSPI Flash

#define Ts						(1.f/SAMPLERATE)  // sample period
#define _2PI                    6.283185307f
#define _PI                    	3.14159265f

#define VOL                     70 // initial output DAC volume
#define MAXVOL                  100 // maximal output DAC volume

#define MIDI_MAX				127.f 	// floating max value
#define MIDI_MAXi				127		// integer max value
#define MIDI_MID_i				64		// integer mid value

#define USE_THE_LCD				1

/*****************************************************************************************************************/
//#define _CCM_					__attribute__((section(".ccmram"))) // for use of CCM RAM (64kB)
//#define _DTCM_				__attribute__((section(".dtcm_vars")))
#define _DTCMRAM_				__attribute__((section(".DTCMRAM_section_bss")))
//#define _ITCMRAM_				__attribute__((section(".itcm_text")))
#define	_ITCMRAM_
//#define _DTCMRAM_

#define _CCM_
#define _DTCM_
/******************************************************************************************************************/
#define MAX_PATCH_SIZE						1024
/* Align X to 4 bytes */
#define MEM_ALIGN(x)                        (((x) + 0x00000003) & ~(0x00000003))

/* Shared RAM between 2 cores is SRAM4 in D3 domain */
#define SHD_RAM_START_ADDR                  0x3800C000 /* last 16K of SRAM4 */
#define SHD_RAM_LEN                         0x00003FFF /* 16K */

/* Buffer from CM4 to CM7 */
#define BUFF_CM4_TO_CM7_ADDR                MEM_ALIGN(SHD_RAM_START_ADDR)
#define BUFF_CM4_TO_CM7_LEN                 MEM_ALIGN(MAX_PATCH_SIZE)
//#define BUFFDATA_CM4_TO_CM7_ADDR            MEM_ALIGN(BUFF_CM4_TO_CM7_ADDR + BUFF_CM4_TO_CM7_LEN)
//#define BUFFDATA_CM4_TO_CM7_LEN             MEM_ALIGN(0x00000400)

/* Buffer from CM7 to CM4 */
//#define BUFF_CM7_TO_CM4_ADDR                MEM_ALIGN(BUFFDATA_CM4_TO_CM7_ADDR + BUFFDATA_CM4_TO_CM7_LEN)
#define BUFF_CM7_TO_CM4_ADDR                MEM_ALIGN(BUFF_CM4_TO_CM7_ADDR + BUFF_CM4_TO_CM7_LEN)
#define BUFF_CM7_TO_CM4_LEN                 MEM_ALIGN(MAX_PATCH_SIZE)
//#define BUFFDATA_CM7_TO_CM4_ADDR            MEM_ALIGN(BUFF_CM7_TO_CM4_ADDR + BUFF_CM7_TO_CM4_LEN)
//#define BUFFDATA_CM7_TO_CM4_LEN             MEM_ALIGN(0x00000400)

/******************************************************************************************************************/
#if defined   (__GNUC__)        /* GNU Compiler */
    #define __ALIGN    __attribute__ ((aligned (4)))
#endif /* __GNUC__ */

/************************************************************************************/
#endif  /*__DEKR_CONSTANTS_H__ */
