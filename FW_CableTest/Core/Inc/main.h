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

#define NLin 32	// максимально количество тестируемых линий

#define FILENAME_SIZE 		(uint8_t)13  								// максимальная длина имени файла для списка
#define DATA_TEST_SIZE  	(uint8_t)255								// длина строки с данными из файла
#define ITEM_ON_PAGE_MAX 	(uint8_t)9	//11									// максимальное количество строк на странице меню
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

/*	 режимы работы (состояния прибора)*/
#define MENU			1	// отображение и навигация по меню
#define CHECK_SCHEME	2	// отображение схемы проверки из файла перед запуском теста
#define TEST			3	// тест - НЕ ПРЕДУСМОТРЕН ОТКЛ�?К НА КЛАВ�?АТУРУ
#define RESULT			4	//
/* коды нажатия кнопок*/
#define UP		0x0B
#define DOWN	0x0E
#define OK		0x0D
#define ENC_BUT	0x07

/* ПАРАМЕТРЫ ЧТЕН�?Я Д�?РЕКТОР�?�? SD карты
 * используется в  FS_GetFileList()*/
//#define FS_ALL_DIR		1		// читать все файлы, чтобы узнать количество всех файлов
//#define FS_ONE_PAGE		2		// читать только для одной страницы меню, чтобы заполнить список названиями файлов

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct typeMenu{
	char		FileList[ITEM_ON_PAGE_MAX][FILENAME_SIZE];
	char	 	ActiveFileName;
	short int	ActivePage;
	short int 	ActiveItem;
	uint32_t 	NmbrOnPageFiles;
	uint32_t 	NmbrAllFiles;
	uint32_t 	NmbrAllPages;
	uint16_t	BGR_Color;	// фон
	uint16_t	TXT_Color;	// текст
	uint16_t	BRD_Color;	// бордер
	uint16_t	SEL_Color;	// выделение
	uint16_t	DANGER_TXT_Color;	//
	uint16_t	DANGER_BGR_Color;	//

}typeMenu;


typedef struct {
	typeMenu Menu;
	char Status;
	char *FileNameForTest;
	uint8_t Mode;
	uint32_t DataForTest[2][NLin];
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
#define SD_CS_Pin LL_GPIO_PIN_1
#define SD_CS_GPIO_Port GPIOB
#define BUT_OK_Pin LL_GPIO_PIN_10
#define BUT_OK_GPIO_Port GPIOB
#define BUT_UP_Pin LL_GPIO_PIN_11
#define BUT_UP_GPIO_Port GPIOB
#define ADR_A_Pin LL_GPIO_PIN_12
#define ADR_A_GPIO_Port GPIOB
#define ADR_B_Pin LL_GPIO_PIN_13
#define ADR_B_GPIO_Port GPIOB
#define ADR_C_Pin LL_GPIO_PIN_14
#define ADR_C_GPIO_Port GPIOB
#define EN_OUT_Pin LL_GPIO_PIN_15
#define EN_OUT_GPIO_Port GPIOB
#define EN_INP_Pin LL_GPIO_PIN_8
#define EN_INP_GPIO_Port GPIOA
#define BUTTON_Pin LL_GPIO_PIN_15
#define BUTTON_GPIO_Port GPIOA
#define BUT_DW_Pin LL_GPIO_PIN_6
#define BUT_DW_GPIO_Port GPIOB
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
#define f_ReadKeyPad        BBAdr(FLAG,6)
//#define f_ButtRead			BBAdr(FLAG,7)
#define f_StepMenu			BBAdr(FLAG,8)
#define f_DirStepMenu		BBAdr(FLAG,9)
#define f_Action			BBAdr(FLAG,10)
#define f_StartTest			BBAdr(FLAG,11)


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
