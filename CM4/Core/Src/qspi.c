/**
 ******************************************************************************
 * @file    qspi.c
 * @author  Xavier Halgand & MCD Application Team
 * @brief   QSPI initialization
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "qspi.h"

/*----------------------------------------------------------------------------*/
extern QSPI_HandleTypeDef hqspi;

/*  functions ----------------------------------------------------------------*/
void QSPI_init(void)
{
	/* QSPI info structure */
	BSP_QSPI_Info_t pQSPI_Info;
	uint8_t status;

	/*##-1- Configure the QSPI device ########################################*/
	/* QSPI device configuration */
	BSP_QSPI_Init_t init;
	init.InterfaceMode = MT25TL01G_QPI_MODE;
	init.TransferRate = MT25TL01G_DTR_TRANSFER;
	init.DualFlashMode = MT25TL01G_DUALFLASH_ENABLE;
	status = BSP_QSPI_Init(0, &init);

	if (status != BSP_ERROR_NONE)
	{
		//UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "QSPI Initialization : FAILED.", LEFT_MODE);
		printf("QSPI Initialization : FAILED\n");
	}

	else
	{
		//UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*) "QSPI Initialization : OK.", LEFT_MODE);
		printf("QSPI Initialization : OK\n");

		/*##-2- Read & check the QSPI info #######################################*/
		/* Initialize the structure */
		pQSPI_Info.FlashSize = (uint32_t) 0x00;
		pQSPI_Info.EraseSectorSize = (uint32_t) 0x00;
		pQSPI_Info.EraseSectorsNumber = (uint32_t) 0x00;
		pQSPI_Info.ProgPageSize = (uint32_t) 0x00;
		pQSPI_Info.ProgPagesNumber = (uint32_t) 0x00;

		/* Read the QSPI memory info */
		BSP_QSPI_GetInfo(0, &pQSPI_Info);

		/* Test the correctness */
		if ((pQSPI_Info.FlashSize != 0x8000000) || (pQSPI_Info.EraseSectorSize != 0x2000)
				|| (pQSPI_Info.ProgPageSize != 0x100) || (pQSPI_Info.EraseSectorsNumber != 0x4000)
				|| (pQSPI_Info.ProgPagesNumber != 0x80000))
		{
			//UTIL_LCD_DisplayStringAt(20, 140, (uint8_t*) "QSPI GET INFO : FAILED.", LEFT_MODE);
			//UTIL_LCD_DisplayStringAt(20, 130, (uint8_t*) "QSPI Test Aborted.", LEFT_MODE);
			printf("QSPI GET INFO : FAILED\n");
		}
		else
		{
			//UTIL_LCD_DisplayStringAt(20, 140, (uint8_t*) "QSPI GET INFO : OK.   ", LEFT_MODE);
			printf("QSPI GET INFO : OK\n");
		}
	}

}

/*-----------------------------------------------------------------------------------------------------------*/
void QSPI_ReInit(void)
{
	/* Clear busy bit */
	HAL_QSPI_Abort(&hqspi);/* WARNING: Do not make any other memory-mapped access (even using debugger) */

	/* Go back to indirect mode */
	if (BSP_QSPI_DisableMemoryMappedMode(0) != BSP_ERROR_NONE)
	{
		printf("QSPI Disable Memory Mapped Mode : FAILED\n");
	}
	else
	{
		printf("QSPI Disable Memory Mapped Mode : OK\n     ");
	}
	BSP_QSPI_DeInit(0);
	QSPI_init();

}

/*-----------------------------------------------------------------------------------------------------------*/

/**
 * @brief  Display QSPI Demo Hint
 * @param  None
 * @retval None
 */
void QSPI_SetHint(void)
{
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
	UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*) "QSPI test", CENTER_MODE);
	//UTIL_LCD_SetFont(&Font12);

	/* Set the LCD Text Color */
	UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
	UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);

	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
}

