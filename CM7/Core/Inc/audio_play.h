/*
 * audio_play.h
 *
 *  Created on: Dec 2, 2023
 *      Author: XavSab
 */

#ifndef APPLICATION_USER_CM7_INC_AUDIO_PLAY_H_
#define APPLICATION_USER_CM7_INC_AUDIO_PLAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"
//#include "stm32h7xx_hal_def.h"
#include "stm32h747i_discovery_audio.h"
#include "soundGen.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,

}AUDIO_ErrorTypeDef;

/* Exported constants --------------------------------------------------------*/
/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
#define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
#define AUDIO_FILE_SIZE              524288

/* Exported functions ------------------------------------------------------- */

void AudioInit (void);
void Application(void);
uint8_t AUDIO_Process(void);
void Error_Handler(void);

#endif /* APPLICATION_USER_CM7_INC_AUDIO_PLAY_H_ */
