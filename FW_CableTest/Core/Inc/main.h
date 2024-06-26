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
//#include "../../lv_conf.h"
//#include "../../lvgl/lvgl.h"

#define pinMAX	(uint32_t)26		// максимальное количество выводов на одном разъеме девайса
#define lineMAX 	pinMAX*2		// максимально возможное количество тестируемых линий. Возьмем с запасом в два раза. Если обнаружатся перекрестные соединения и будет более 52 линий, это уже обнаруженная ошибка. Всего выводов будет 2х26=52, если считать все перекрёстные соединения получится 52х52 = 2704 линии!!!! нет смысла их все обнаруживать и анализировать!

//////////////////////////////
#define NLin 32				// максимальное количество тестируемых линий
#define NCheckLine 8		// количество тестируемых линий
//////////////////////////////

#define FILE_SIZE 			(uint32_t)1000								// длина строки с данными из файла (размер файла )
#define FILENAME_SIZE 		(uint32_t)13 								// максимальная длина имени файла для списка
#define DATA_TEST_SIZE  	(uint32_t)255								// длина строки с данными из файла
#define ITEM_ON_PAGE_MAX 	(uint32_t)9	//11							// максимальное количество строк на странице меню
#define PAGE_MAX			(uint32_t)(FILELIST_MAX/ITEM_ON_PAGE_MAX)	// максимальное количество страниц меню
#define KEY_LENGHT			(uint32_t)16									// МАКСИМАЛЬНАЯ ДЛИНА СТРОКИ С НАЗВАНИЕМ КЛЮЧА В JSON ОБЪЕКТЕ

//#define FS_EOF		1	// конец файла
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
#define MODE_MENU		1	// отображение и навигация по меню
#define MODE_WAIT		2	// ОЖ�?ДАН�?Е ПОДКЛЮЧЕН�?Я КАБЕЛЯ, отображение схемы проверки из файла перед запуском теста
#define MODE_TEST		3	// тест - при отключении кабеля переходит в MODE_WAIT

/* коды нажатия кнопок*/
#define UP		0x0B
#define DOWN	0x0E
#define OK		0x0D
#define ENC_BUT	0x07

/*Координаты для строки сообщений*/
#define Mx 15
#define My 112

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


}typeMenu;



/*********************************************
 * 	точка подключения описывается именем разъёма
 * 	и номером вывода на этом разъёме
 * 	пример: 12 вывод на разъёме ХР23 -
 *
 * 	typePoint BeginPoint.name = "XP23"
 * 	typePoint BeginPoint.num = "12"
 *
 * ************************/
typedef struct {
		char *name;
		char *num;
}t_Point;


/****************************************************************
 * 	Проводник описывается двуми точками: начало и конец
 * 	Пример: XP23.12 -> XS12.9
 * 		typeWire FirstWire.begin.name = "X1";
 * 		typeWire FirstWire.begin.num  = 11;
 * 		typeWire FirstWire.end.name   = "X1";
 * 		typeWire FirstWire.end.num    = 2;
 *
 * *******************/
typedef struct {
	t_Point begin;
	t_Point end;
}t_Wire;


/****************************************************************
 * 	Линия состоит из проводника адаптера и проводника кабеля
 * 	Пример: Есть массив из 8 линий typeLine Lines[8] = {0};
 * 	первая линия: (запись в последовательности прохождения сигнала)
 *
 * 				 Lines[0].adapter.begin.name = "X1"; - поступает на X1.1 адаптера (выходит с одноимённого вывода на девайсе)
 * 				 Lines[0].adapter.begin.num  = 1;
 * 				 Lines[0].cabel.begin.name   = "X4"; - поступает на X4.1 кабеля (выходит с адаптера)проходит через кабель
 * 				 Lines[0].cabel.begin.num    = 1;
 * 				 Lines[0].cabel.end.name     = "X3"; - и возвращается с разъема кабеля X3.4, проходит через адаптер
 * 				 Lines[0].cabel.end.num      = 4;
 * 				 Lines[0].adapter.end.name   = "X1"; - и выходит с адаптера на X1.5 (одноимённый вывод девайса)
 * 				 Lines[0].adapter.end.num    = 5;
 *________  ____________      __________
 *device  | | adapter 	|	 | cabel    |
*  X1.1-->|-|X1.1---->--|->--|-X4.1		|--->---
*		  | |X1.2		|	 |__________|		|
*		  | |X1.3       |     __________		|
*		  | |X1.4    	|	 |cabel     |		|
*  X1.5-<-|-|X1.5----<--|-<--|-X3.4 	|---<---
*_________| |___________|	 |__________|
 * *********************/
typedef struct {
	uint32_t numLine;
	t_Wire adapter;
	t_Wire cabel;
}t_Line;




typedef struct {
	typeMenu 	Menu;
	t_Line 	    Lines[lineMAX] ;
	char 		Status[24];
	char *		FileNameForTest;
	char 		CSVBuf[FILE_SIZE];   // строка прочитанная из файла ;

	uint8_t 	Mode;
	uint32_t	DataForTest[2][NLin];
	uint8_t 	CheckLine[NCheckLine];
	uint8_t 	RealDataSize;
	uint16_t	period;
	uint16_t	pulse;

}typeEnv;


//--------------------------------------------





/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
//#define str(x) #x
//#define xstr(x) str(x)

// измерение времени выполнения куска кода в машинных тиках
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC
extern uint32_t count_tic;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void LineStructInit(typeEnv *Env);
//void ParseJsonBuf(typeEnv *Env);
void ReadKeyPad(void);
void Result(typeEnv *Env);
void Test(typeEnv *Env);
void Wait(typeEnv *Env);
void Menu(typeEnv *Env);
void Display(typeEnv *Env);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, uint8_t BitVal);
void Init_Output_Input_GPIO();
void Init_Output_Input_Alter();

extern uint32_t Pulse, Period, N_periods;


//extern lv_disp_draw_buf_t  lvgl_disp_buf;



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
#define OUT_A_Pin LL_GPIO_PIN_12
#define OUT_A_GPIO_Port GPIOB
#define OUT_B_Pin LL_GPIO_PIN_13
#define OUT_B_GPIO_Port GPIOB
#define OUT_C_Pin LL_GPIO_PIN_14
#define OUT_C_GPIO_Port GPIOB
#define OUT_EN_Pin LL_GPIO_PIN_15
#define OUT_EN_GPIO_Port GPIOB
#define IN_A_Pin LL_GPIO_PIN_8
#define IN_A_GPIO_Port GPIOA
#define IN_B_Pin LL_GPIO_PIN_9
#define IN_B_GPIO_Port GPIOA
#define IN_C_Pin LL_GPIO_PIN_10
#define IN_C_GPIO_Port GPIOA
#define IN_EN_Pin LL_GPIO_PIN_12
#define IN_EN_GPIO_Port GPIOA
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
#define f_waitTick			BBAdr(FLAG,7)
#define f_StepMenu			BBAdr(FLAG,8)
#define f_DirStepMenu		BBAdr(FLAG,9)
#define f_Action			BBAdr(FLAG,10)
#define f_StartTest			BBAdr(FLAG,11)
#define f_connect			BBAdr(FLAG,12)
#define f_failtest			BBAdr(FLAG,13)
#define f_sound				BBAdr(FLAG,14)

/**
   * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval None
  */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
