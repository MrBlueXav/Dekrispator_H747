/*
 * 	File : perf.c
 * 	Performance measurements using Data Watchpoint and Trace unit (DWT)
 *	Present on all STM32 using the CM3, CM4 and CM7 cores.
 *	The Cortex-M7 DWT registers are located at address range 0xE0001000 to 0xE0001FFC,
 *	on the AHBD.
 *  Created on: Jan 13, 2024
 *      Author: Tesla DeLorean (st.com community) and Xavier Halgand
 */

//****************************************************************************
#include "perf.h"
#include <stdio.h>
#include "stm32h7xx_hal.h"
#include "openamp_interface.h"
#include "constants.h"

//****************************************************************************

typedef struct
{
	float sum;
	uint32_t count;
	float average;

} Performance_t;

//****************************************************************************

static volatile unsigned int *M7_DWT_CYCCNT = (volatile unsigned int*) 0xE0001004;
static volatile unsigned int *M7_DWT_CONTROL = (volatile unsigned int*) 0xE0001000;
static volatile unsigned int *M7_DWT_LAR = (volatile unsigned int*) 0xE0001FB0;
static volatile unsigned int *SCB_DHCSR = (volatile unsigned int*) 0xE000EDF0;
static volatile unsigned int *SCB_DEMCR = (volatile unsigned int*) 0xE000EDFC;
static volatile unsigned int *M7_ITM_TER = (volatile unsigned int*) 0xE0000E00;
//static volatile unsigned int *ITM_TCR      	= (volatile unsigned int *)0xE0000E80;

//****************************************************************************

static int Debug_ITMDebug = 0;
static uint32_t _DTCMRAM_ ticktime, oldtime;
static Performance_t perf _DTCMRAM_;

//****************************************************************************
void EnableTiming(void) // Initialization
{
	if ((*SCB_DHCSR & 1) && (*M7_ITM_TER & 1)) // Enabled?
		Debug_ITMDebug = 1;

	*SCB_DEMCR |= 0x01000000;
	*M7_DWT_LAR = 0xC5ACCE55; // enable access
	*M7_DWT_CYCCNT = 0; // reset the counter
	*M7_DWT_CONTROL |= 1; // enable the counter

	oldtime = HAL_GetTick();
	perf.sum = 0.0f;
	perf.count = 0;
	perf.average = 0.0f;
}

//****************************************************************************
void cyc_count_reset(void) // put this before the function to measure
{
	*M7_DWT_CYCCNT = 0; // reset the counter
}

//****************************************************************************
uint32_t cyc_count_get(void)
{
	return *M7_DWT_CYCCNT;
}

//****************************************************************************
void cyc_count_print(void) // put this just after the function to measure
{
	perf.sum += *M7_DWT_CYCCNT;
	perf.count++;

	ticktime = HAL_GetTick();

	if ((ticktime - oldtime) > 1000) // send report every second
	{
		perf.average = perf.sum / perf.count;
		send_integer_to_CM4((uint32_t)roundf(perf.average));
		//send_string_to_CM4("Ceci est un message !\n");
		oldtime = ticktime;
		perf.sum = 0.0f;
		perf.count = 0;
		perf.average = 0.0f;
	}
}

//****************************************************************************
void Delay(uint32_t cycles)
{
	uint32_t start = *M7_DWT_CYCCNT;

	while ((*M7_DWT_CYCCNT - start) < cycles)
		;
}

//****************************************************************************
