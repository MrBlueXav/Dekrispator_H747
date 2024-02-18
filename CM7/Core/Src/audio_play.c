/**
 ******************************************************************************
 * @file    audio_play.c
 * @author  MCD Application Team
 * @brief   This example code shows how to use the audio feature in the
 *          stm32747i_discovery driver
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_play.h"
#include "stm32h747i_discovery.h"

float samplerate ;

/* Private define ------------------------------------------------------------*/

#define AUDIO_DEFAULT_VOLUME    70

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	AUDIO_STATE_IDLE = 0, AUDIO_STATE_INIT, AUDIO_STATE_PLAYING, AUDIO_STATE_PAUSE

} AUDIO_PLAYBACK_StateTypeDef;

typedef enum
{
	BUFFER_OFFSET_NONE = 0, BUFFER_OFFSET_HALF, BUFFER_OFFSET_FULL,

} BUFFER_StateTypeDef;

typedef struct
{
	uint8_t buff[AUDIO_BUFFER_SIZE]; // AUDIO_BUFFER_SIZE is defined in constants.h
	BUFFER_StateTypeDef state;

} AUDIO_BufferTypeDef;

/* Private variables ---------------------------------------------------------*/
ALIGN_32BYTES(static AUDIO_BufferTypeDef buffer_ctl);
static AUDIO_PLAYBACK_StateTypeDef audio_state;
__IO uint32_t uwVolume = 20;
BSP_AUDIO_Init_t *AudioPlayInit;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Audio Play demo
 * @param  None
 * @retval None
 */
void AudioInit(void)
{
	uwVolume = 60;

	AudioPlayInit->Device = AUDIO_OUT_DEVICE_HEADPHONE;
	AudioPlayInit->ChannelsNbr = 2;
	AudioPlayInit->SampleRate = SAMPLERATE;
	AudioPlayInit->BitsPerSample = AUDIO_RESOLUTION_16B;
	AudioPlayInit->Volume = uwVolume;

	if (BSP_AUDIO_OUT_Init(0, AudioPlayInit) != BSP_ERROR_NONE)
	{
		Error_Handler();
	}
	HAL_Delay(500);

	/*
	 Start playing the file from a circular buffer, once the DMA is enabled, it is
	 always in running state. Application has to fill the buffer with the audio data
	 using Transfer complete and/or half transfer complete interrupts callbacks
	 (AUDIO_TransferComplete_CallBack() or AUDIO_HalfTransfer_CallBack()...
	 */
	buffer_ctl.state = BUFFER_OFFSET_NONE;
	BSP_AUDIO_OUT_Play(0, (uint8_t*) &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
	audio_state = AUDIO_STATE_PLAYING;
}

/*----------------------------------------------------------------------------------------------------*/
uint8_t AUDIO_Process(void)
{
	AUDIO_ErrorTypeDef error_state = AUDIO_ERROR_NONE;

	switch (audio_state)
	{
	case AUDIO_STATE_PLAYING:

		BSP_LED_Off(LED_ORANGE); // CPU load indicator

		/* 1st half buffer played; so fill it and continue playing from bottom*/
		if (buffer_ctl.state == BUFFER_OFFSET_HALF)
		{
			cyc_count_reset();

			make_sound((uint16_t*) &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE / 8);
			buffer_ctl.state = BUFFER_OFFSET_NONE;

			/* Clean Data Cache to update the content of the SRAM */
			SCB_CleanDCache_by_Addr((uint32_t*) &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE / 2);
			cyc_count_print();
		}

		/* 2nd half buffer played; so fill it and continue playing from top */
		if (buffer_ctl.state == BUFFER_OFFSET_FULL)
		{
			cyc_count_reset();

			make_sound((uint16_t*) &buffer_ctl.buff[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 8);
			buffer_ctl.state = BUFFER_OFFSET_NONE;

			/* Clean Data Cache to update the content of the SRAM */
			SCB_CleanDCache_by_Addr((uint32_t*) &buffer_ctl.buff[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2);
			cyc_count_print();
		}

		BSP_LED_On(LED_ORANGE);

		break;

	default:
		error_state = AUDIO_ERROR_NOTREADY;
		break;
	}

	return (uint8_t) error_state;
}

/*------------------------------------------------------------------------------
 Callbacks implementation:
 the callbacks API are defined __weak in the stm32h747i_discovery_audio.c file
 and their implementation should be done the user code if they are needed.
 Below some examples of callback implementations.
 ----------------------------------------------------------------------------*/
/**
 * @brief  Manages the full Transfer complete event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
	if (audio_state == AUDIO_STATE_PLAYING)
	{
		/* allows AUDIO_Process() to refill 2nd part of the buffer  */
		buffer_ctl.state = BUFFER_OFFSET_FULL;
	}
}
/*----------------------------------------------------------------------------------------------------*/
/**
 * @brief  Manages the DMA Half Transfer complete event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
	if (audio_state == AUDIO_STATE_PLAYING)
	{
		/* allows AUDIO_Process() to refill 1st part of the buffer  */
		buffer_ctl.state = BUFFER_OFFSET_HALF;
	}
}
/*----------------------------------------------------------------------------------------------------*/
/**
 * @brief  Manages the DMA FIFO error event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
	BSP_LED_On(LED_RED);
}
/*-------------------------------- END OF FILE ---------------------------------------*/
