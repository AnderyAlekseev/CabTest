/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "font5x7.h"
#include "font7x11.h"
#include "color565.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typeEnv Env;
uint32_t FLAG;
uint32_t count_tic = 0; // счётчик тиков для подсчёта времени выполнения кода
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t Pulse=0, Period=0, N_periods=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	FATFS	FatFs; 	//Fatfs handle
	FRESULT resFS;
	char status[20] = "123456";
//	uint8_t waitCHR[4]= {0x7C, 0x2F, 0x2D, 0x5C}, w_indx=0;//   / - \ |
	Env.Menu.ActiveItem=0;
	Env.Menu.ActivePage=0;
	Env.period=0;
	Env.pulse=0;
	Env.Mode = MODE_MENU;
	uint8_t mode = Env.Mode;
	f_RefreshScreen = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  		  SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT // измерение времени выполнения куска кода в машинных тиках
  		  DWT_CYCCNT = 0;// обнуляем значение
  		  DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик
  		/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
//  LL_SPI_Enable(SPI1);// включить SPI после инициализации ДО иниц. FATFS
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM1);

//	LL_TIM_EnableCounter(TIM3);
//	HAL_Delay(500);
//	LL_TIM_DisableCounter(TIM3);

	LL_SPI_Enable(SPI1);// включить SPI после инициализации ДО иниц. FATFS
	resFS = f_mount(&FatFs, "", 1); //Монтируем файловую систему до первого использования SPI дисплеем

	HAL_ST7735_Init();

	if(resFS != FR_OK)
				{
				ST7735_Clear(DANGER_BGR_COLOR);
				sprintf(status, "SD card" );
				ST7735_DrawString7x11(39,1, status,DANGER_TXT_COLOR,DANGER_BGR_COLOR);
				sprintf(status, "not find" );
				ST7735_DrawString7x11(27,LINE_HEIGTH*1, status,DANGER_TXT_COLOR, DANGER_BGR_COLOR);
				sprintf(status, "Insert SD card" );
				ST7735_DrawString7x11(8,LINE_HEIGTH*2, status, DANGER_TXT_COLOR, DANGER_BGR_COLOR);
				sprintf(status, "and reboot" );
				ST7735_DrawString7x11(29,LINE_HEIGTH*3, status,DANGER_TXT_COLOR, DANGER_BGR_COLOR);
				sprintf(status, "device" );
				ST7735_DrawString7x11(42,LINE_HEIGTH*4, status,DANGER_TXT_COLOR, DANGER_BGR_COLOR);
				while( 1);
				}



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	ST7735_Clear(BGR_COLOR);
	HAL_Delay(500);
	FS_GetFileList( &Env);
	Env.Menu.NmbrAllPages = (uint32_t)(Env.Menu.NmbrAllFiles/ITEM_ON_PAGE_MAX);

	// Init_Output_Input_GPIO();
/* PB4 - выход на зуммер*/
  while (1)
  {
	 DWT_CYCCNT = 0;// обнуляем значение
	f_connect = CheckConnect(Env);
	count_tic = DWT_CYCCNT;//смотрим сколько натикало
	ReadKeyPad();
	mode = Env.Mode;
	switch(mode)	// назначить действие в зависимости от текущего режима
			{
				case MODE_MENU: 	Menu(&Env); 	break;
				case MODE_WAIT: 	Wait(&Env);  	break;
				case MODE_TEST: 	Test(&Env);		break;
				default: break;
			}
	Display(&Env);

	  //HAL_Delay(200);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(72000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
