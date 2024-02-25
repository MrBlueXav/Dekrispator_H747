/**
 ******************************************************************************
 * 					DEKRISPATOR H747
 * @file           : main_cm4.c
 * @brief          : Main program body for CM4
 ******************************************************************************
 * @attention
 *
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main_cm4.h"
#include "rtc.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "stdio.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_lcd.h"
#include "stm32_lcd.h"
#include "lcd_trace.h"
#include "interface.h"
#include "qspi.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/*---------------------------------------------------------------------------------------------*/
void BSP_LED_Initialize(void);
void Welcome_message(void);
//static void Display_DemoDescription(void);
/*---------------------------------------------------------------------------------------------*/
/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/*HW semaphore Clock enable*/
	__HAL_RCC_HSEM_CLK_ENABLE();
	/* Activate HSEM notification for Cortex-M4*/
	HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
	/*
	 Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
	 perform system initialization (system clock config, external memory configuration.. )
	 */
	HAL_PWREx_ClearPendingEvent();
	HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
	/* Clear HSEM flag */
	__HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	openamp_init();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	BSP_LED_Initialize();

	/* CM7_SEV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(CM7_SEV_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(CM7_SEV_IRQn);

#if (USE_THE_LCD > 0)

	/* Initialize the LCD */
	BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
	UTIL_LCD_SetFuncDriver(&LCD_Driver);

//	/* Initialize the LCD Log module */
//	UTIL_LCD_TRACE_Init();
//	UTIL_LCD_TRACE_SetHeader((uint8_t*) " DEKRISPATOR H747 ");
	//Display_DemoDescription();

#endif

	MX_USB_HOST_Init();
	QSPI_init();
	HAL_Delay(3000);
	Welcome_message();

	while (1)
	{
		MX_USB_HOST_Process();
		Application_Process(); // in interface.c
	}
}
/*********************************************************************************************/
void BSP_LED_Initialize(void)
{
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_On(LED1);
	BSP_LED_On(LED2);
	BSP_LED_On(LED3);
	BSP_LED_On(LED4);
	HAL_Delay(300);
	BSP_LED_Off(LED1);
	BSP_LED_Off(LED2);
	BSP_LED_Off(LED3);
	BSP_LED_Off(LED4);
}
/*--------------------------------------------------------------------------------------*/
void Welcome_message(void)
{

#if (USE_THE_LCD > 0)

	uint32_t x_size;
	uint32_t y_size;
	BSP_LCD_GetXSize(0, &x_size);
	BSP_LCD_GetYSize(0, &y_size);
	/* Clear the LCD */
	UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

	/* Set LCD Demo description */
	UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
	UTIL_LCD_SetFont(&Font24);
	UTIL_LCD_DisplayStringAt(0, 10, (uint8_t*) "DEKRISPATOR H747", CENTER_MODE);
	UTIL_LCD_SetFont(&Font20);
	UTIL_LCD_DisplayStringAt(0, 45, (uint8_t*) "Copyright (c) Xavier Halgand 2024", CENTER_MODE);

	/* Set the LCD Text Color */
	UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
	UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);

	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

	UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "MIDI controller not connected.", LEFT_MODE);

#endif

	printf("\n");
	printf("-----------------------------------------\n");
	printf("****    This is Dekrispator H747 !   ****\n");
	printf("Waiting for USB MIDI controller device...\n");
	printf("-----------------------------------------\n");
	printf("\n");
}

/**
 * @brief  Display main demo messages
 * @param  None
 * @retval None
 */
//static void Display_DemoDescription(void)
//{
//	char desc[64];
//	uint32_t x_size;
//	uint32_t y_size;
//
//	BSP_LCD_GetXSize(0, &x_size);
//	BSP_LCD_GetYSize(0, &y_size);
//	/* Set LCD Foreground Layer  */
//	UTIL_LCD_SetFont(&UTIL_LCD_DEFAULT_FONT);
//
//	/* Clear the LCD */
//	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
//	UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
//
//	/* Set the LCD Text Color */
//	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
//
//	/* Display LCD messages */
//	UTIL_LCD_DisplayStringAt(0, 10, (uint8_t*) "STM32H747I BSP", CENTER_MODE);
//	UTIL_LCD_DisplayStringAt(0, 35, (uint8_t*) "Drivers examples", CENTER_MODE);
//
//	UTIL_LCD_SetFont(&Font12);
//	UTIL_LCD_DisplayStringAt(0, y_size - 20, (uint8_t*) "Copyright (c) Xavier Halgand 2024", CENTER_MODE);
//
//	UTIL_LCD_SetFont(&Font16);
//	BSP_LCD_FillRect(0, 0, y_size / 2 + 15, x_size, 60, UTIL_LCD_COLOR_BLUE);
//	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
//	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
//	UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30, (uint8_t*) "Press Wakeup button to start :", CENTER_MODE);
//
//	UTIL_LCD_DisplayStringAt(0, y_size / 2 + 45, (uint8_t*) desc, CENTER_MODE);
//}


/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	BSP_LED_On(LED_RED);
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
