/**
 ******************************************************************************
 * File Name		: timers.h
 * Author			: Xavier Halgand
 * Date				:
 * Description		:
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "audio_conf.h"
#include "constants.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_audio.h"
#include "soundGen.h"

/* Exported functions ------------------------------------------------------- */
void 		TimingDelay_Decrement(void);
void 		setSysTick(void);
uint32_t 	getTime(void);
void 		setTime(uint32_t val);
void 		start_LED_On(Led_TypeDef Led, uint32_t duration);
void 		update_temporized_LED(Led_TypeDef Led);
void 		DebounceUserButton(void);

/*************************************************************************************/
#endif /* __TIMERS_H */
