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

float samplerate ;

/* Private define ------------------------------------------------------------*/

/*Since SysTick is set to 1ms (unless to set it quicker) */
/* to run up to 48khz, a buffer around 1000 (or more) is requested*/
/* to run up to 96khz, a buffer around 2000 (or more) is requested*/
#define AUDIO_DEFAULT_VOLUME    70

/* Audio file size and start address are defined here since the audio file is
 stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_START_OFFSET_ADDRESS    0            /* Offset relative to audio file header size */
#define AUDIO_BUFFER_SIZE            2048

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
	uint8_t buff[AUDIO_BUFFER_SIZE];
	uint32_t fptr;
	BUFFER_StateTypeDef state;
	uint32_t AudioFileSize;
	uint32_t *SrcAddress;

} AUDIO_BufferTypeDef;

/* Private variables ---------------------------------------------------------*/
ALIGN_32BYTES(static AUDIO_BufferTypeDef buffer_ctl);
static AUDIO_PLAYBACK_StateTypeDef audio_state;

__IO uint32_t uwVolume = 20;
static uint32_t *AudioFreq_ptr;
static uint32_t AudioFreq[9] =
{ 8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000, 192000 };

BSP_AUDIO_Init_t *AudioPlayInit;

/* Private function prototypes -----------------------------------------------*/

AUDIO_ErrorTypeDef AUDIO_Start(uint32_t *psrc_address, uint32_t file_size);
AUDIO_ErrorTypeDef AUDIO_Stop(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Audio Play demo
 * @param  None
 * @retval None
 */
void AudioInit(void)
{

	AudioFreq_ptr = AudioFreq + 6; /*AF_48K*/
	uwVolume = 60;

	//sound_generator_init0();

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

		BSP_LED_Off(LED_ORANGE);
		/* 1st half buffer played; so fill it and continue playing from bottom*/
		if (buffer_ctl.state == BUFFER_OFFSET_HALF)
		{
			//make_test_sound0((uint16_t*) &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE / 8);
			make_sound((uint16_t*) &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE / 8);
			buffer_ctl.state = BUFFER_OFFSET_NONE;
			/* Clean Data Cache to update the content of the SRAM */
			SCB_CleanDCache_by_Addr((uint32_t*) &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE / 2);
		}

		/* 2nd half buffer played; so fill it and continue playing from top */
		if (buffer_ctl.state == BUFFER_OFFSET_FULL)
		{
			//make_test_sound0((uint16_t*) &buffer_ctl.buff[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 8);
			make_sound((uint16_t*) &buffer_ctl.buff[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 8);
			buffer_ctl.state = BUFFER_OFFSET_NONE;

			/* Clean Data Cache to update the content of the SRAM */
			SCB_CleanDCache_by_Addr((uint32_t*) &buffer_ctl.buff[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2);
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
