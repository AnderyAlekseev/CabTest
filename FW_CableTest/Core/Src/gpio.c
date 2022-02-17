/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */





/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LCD_RESET_Pin|LCD_A0_Pin|LCD_CS_Pin|IN_A_Pin
                          |IN_B_Pin|IN_C_Pin|IN_EN_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, SD_CS_Pin|OUT_A_Pin|OUT_B_Pin|OUT_C_Pin
                          |OUT_EN_Pin);

  /**/
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_RESET_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_A0_Pin|IN_A_Pin|IN_B_Pin|IN_C_Pin
                          |IN_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = BUT_OK_Pin|BUT_UP_Pin|BUT_DW_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = OUT_A_Pin|OUT_B_Pin|OUT_C_Pin|OUT_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);


  //проверка PA00
//   GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
//   GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
//   GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//   GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//   LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//   for(uint8_t indx=0; indx<20; indx++)
//   {
//	   LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_0);
//   }

}



/* USER CODE BEGIN 2 */

void Init_Output_Input_Alter()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	// in
		GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	//	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   // out
		GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void Init_Output_Input_GPIO()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
	// in
		GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		//GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   // out
		GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
/*
 *  GPIO_InitStruct.Pin = OUT_A_Pin|OUT_B_Pin|OUT_C_Pin|OUT_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);*/

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, uint8_t BitVal)
{
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	//DWT_CYCCNT = 0;// обнуляем значение
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	//BitVal ?  LL_GPIO_SetOutputPin(GPIOx, GPIO_Pin) : LL_GPIO_ResetOutputPin(GPIOx, GPIO_Pin) ;
  if (BitVal != RESET)
  {
    LL_GPIO_SetOutputPin(GPIOx, GPIO_Pin);
  }
  else
  {
	  LL_GPIO_ResetOutputPin(GPIOx, GPIO_Pin);
  }
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	//	   count_tic = DWT_CYCCNT;//смотрим сколько натикало
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
