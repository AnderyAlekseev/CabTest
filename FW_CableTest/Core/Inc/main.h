/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_crc.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "font5x7.h"
#include "font7x11.h"
#include "color565.h"
#include "ff.h"
#define FILENAME_SIZE 		(uint8_t)13  								// максимальная длина имени файла для списка
#define DATA_TEST_SIZE  	(uint8_t)256								// длина строки с данными из файла
#define ITEM_ON_PAGE_MAX 	(uint8_t)11									// максимальное количество строк на странице меню
#define PAGE_MAX			(uint8_t)(FILELIST_MAX/ITEM_ON_PAGE_MAX)	// максимальное количество страниц меню

/* для парсинга данных из файла
 * обозначение для начала и окончания фрагмента с данными.
 * */
#define PARS_START_DATA '['	// после этого символа начинаются данные
#define PARS_END_DATA ']'	// этим символом заканчиваются данные
#define PARS_DELIVER ','	// символ разделяющий данные

/*	 цвет оформления на дисплее	*/
#define    BGR_COLOR	COLOR565_DARK_BLUE// фон
#define    TXT_COLOR	COLOR565_ALICE_BLUE// текст
#define    BRD_COLOR	COLOR565_ALICE_BLUE// бордер
#define    SEL_COLOR	COLOR565_BLUE// выделение
#define    DANGER_TXT_COLOR	COLOR565_WHITE//
#define    DANGER_BGR_COLOR	COLOR565_RED//

/* ПАРАМЕТРЫ ЧТЕНИЯ ДИРЕКТОРИИ SD карты
 * используется в  FS_GetFileList()*/
#define FS_ALL_DIR		1		// читать все файлы, чтобы узнать количество всех файлов
#define FS_ONE_PAGE		2		// читать только для одной страницы меню, чтобы заполнить список названиями файлов

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct typeMenu{
	char		FileList[ITEM_ON_PAGE_MAX][FILENAME_SIZE];
	uint32_t 	NmbrOnPageFiles;
	uint32_t 	NmbrAllFiles;
	uint32_t 	NmbrAllPages;
	short int	ActivePage;
	short int 	ActiveItem;
	char	 	ActiveFileNane;
	uint16_t	BGR_Color;	// фон
	uint16_t	TXT_Color;	// текст
	uint16_t	BRD_Color;	// бордер
	uint16_t	SEL_Color;	// выделение
	uint16_t	DANGER_TXT_Color;	//
	uint16_t	DANGER_BGR_Color;	//
		//uint32_t 	Step;		// 1 - перемещаемся на один шаг по меню
	//uint32_t 	DirStep;	// 1 - вниз; 0 - вверх

}typeMenu;


typedef struct {
	typeMenu Menu;
	char Status;
	char *FileNameForTest;
	char DataForTest[DATA_TEST_SIZE];
	uint8_t RealDataSize;
}typeEnv;



/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define str(x) #x
#define xstr(x) str(x)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void Test(typeEnv *Env);
void Menu( typeEnv *Env);
void Display(typeEnv *Env);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin LL_GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LCD_RESET_Pin LL_GPIO_PIN_1
#define LCD_RESET_GPIO_Port GPIOA
#define LCD_A0_Pin LL_GPIO_PIN_3
#define LCD_A0_GPIO_Port GPIOA
#define LCD_CS_Pin LL_GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOA
#define GREEN_LED_Pin LL_GPIO_PIN_0
#define GREEN_LED_GPIO_Port GPIOB
#define SD_CS_Pin LL_GPIO_PIN_10
#define SD_CS_GPIO_Port GPIOB
#define BUTTON_Pin LL_GPIO_PIN_15
#define BUTTON_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */


//******************************************************************************
//      BitBanding
//******************************************************************************
#define RAM_BASE       (uint32_t)0x20000000
#define RAM_BB_BASE    (uint32_t)0x22000000
#define BBAdr(BitAddr, BitNumber)  *(uint32_t *)  ((RAM_BB_BASE | ((uint32_t)&BitAddr - RAM_BASE) << 5) | (BitNumber << 2))

extern  uint32_t FLAG;

#define f_1ms 		        BBAdr(FLAG,0)
#define f_10ms		        BBAdr(FLAG,1)
#define f_100ms 			BBAdr(FLAG,2)
#define f_1sec		        BBAdr(FLAG,3)
#define f_1min              BBAdr(FLAG,4)
#define f_RefreshScreen     BBAdr(FLAG,5)
#define f_EncoderRead       BBAdr(FLAG,6)
#define f_ButtRead			BBAdr(FLAG,7)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
