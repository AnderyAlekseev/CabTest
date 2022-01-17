/*
 * test.c
 *
 *  Created on: 13 дек. 2021 г.
 *      Author: aalexeev
 */
#include "main.h"
void MuxSetIN_Addr(uint8_t addr);
void MuxSetOUT_Addr(uint8_t addr);

void Test(typeEnv *Env)
{
	static uint8_t addr=0;
	if(f_StepMenu != 0) // если вверх или вниз
		{
			f_StepMenu = 0;
			(*Env).Mode = CHECK_SCHEME;

			f_RefreshScreen = 1;				// перерисуй экран
		}
	if(f_Action !=0)    // если нужно действие
		{
			f_Action =0;
			f_StartTest	= 1;// запустить тест
		}


	if(f_StartTest	== 1)
	{
		f_StartTest = 0;
		/* сам тест*/
		for(uint8_t out_addr=0; out_addr<8; out_addr++)
		{
			MuxSetOUT_Addr(out_addr);	// установить номер выхода
			GPIO_WriteBit(GPIOB, OUT_EN_Pin, RESET);// включить мультиплексор выходной
			LL_TIM_EnableCounter(TIM1);	// включить генерацию ШИМ таймер 1
			LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);

			for(uint8_t in_addr=0; in_addr<8; in_addr++)
			{
				MuxSetIN_Addr(in_addr);	// установить номер входа
				GPIO_WriteBit(GPIOA, IN_EN_Pin, RESET);// включить мультиплексор входной
				HAL_Delay(100);
			}

		}
		LL_TIM_DisableCounter(TIM1);	// вЫключить генерацию ШИМ таймер 1
		GPIO_WriteBit(GPIOB, OUT_EN_Pin, SET);// вЫключить мультиплексор выходной
		GPIO_WriteBit(GPIOA, IN_EN_Pin, SET);// вЫключить мультиплексор входной
	}
}

void TestProsed()
{

}






void MuxEN_OUT(uint8_t addr)
{
	LL_GPIO_ResetOutputPin(GPIOB, OUT_EN_Pin);
}

void MuxEN_IN(uint8_t addr)
{
	LL_GPIO_ReetOutputPin(GPIOA, IN_EN_Pin);
}

void MuxSetIN_Addr(uint8_t addr)
{
uint8_t mask = 0;
mask= addr & 0x01;
	GPIO_WriteBit(GPIOA, IN_A_Pin, mask);
mask= addr & 0x02;
	GPIO_WriteBit(GPIOA, IN_B_Pin, mask);
mask= addr & 0x04;
	GPIO_WriteBit(GPIOA, IN_C_Pin, mask);
}


void MuxSetOUT_Addr(uint8_t addr)
{
uint8_t mask = 0;
mask= addr & 0x01;
	GPIO_WriteBit(GPIOB, OUT_A_Pin, mask);
mask= addr & 0x02;
	GPIO_WriteBit(GPIOB, OUT_B_Pin, mask);
mask= addr & 0x04;
	GPIO_WriteBit(GPIOB, OUT_C_Pin, mask);

}
