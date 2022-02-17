/*
 * test.c
 *
 *  Created on: 13 дек. 2021 г.
 *      Author: aalexeev
 */
#include "main.h"
void MuxSetIN_Addr(uint8_t addr);
void MuxSetOUT_Addr(uint8_t addr);
void TestProsed(typeEnv *Env);
void DrawTable(typeEnv *Env);

extern uint16_t Pulse, Period, N_periods;

void Test(typeEnv *Env)
{
	static uint8_t addr=0;
	if(f_StepMenu == 1 || f_connect == 0 ) // если вверх или вниз или отключен кабель
		{
			f_StepMenu = 0;
			(*Env).Mode = MODE_WAIT;
			f_RefreshScreen = 1;				// перерисуй экран
		}
	if(f_Action == 1)    // если нужно действие
		{
			f_Action =0;
			f_StartTest	= 1;// запустить тест
		}


	if(f_StartTest	== 1)
	{
		f_StartTest = 0;
		/* сам тест*/
		TestProsed(Env);
		f_RefreshScreen = 1;				// перерисуй экран

	}
}


/************************************
 *
 *
 * **********************************/

void TestProsed(typeEnv *Env)
{
	uint32_t X1[NCheckLine][NCheckLine] = {0};	// заданная матрица
	uint32_t X2[NCheckLine][NCheckLine] = {0};	// результат тестирования
	uint32_t DataAfterTest[2][NCheckLine]={0};	// результат тестирования разреженная матрица
	uint32_t NoLineX1=0, NoLineX2=0;
	uint32_t in_addr=0, out_addr=0, n_line=0;
	uint32_t check=0, index=0;
	uint32_t size=0; // количество соединённых линий, определённое в результате теста

//	Init_Output_Input_Alter();

	LL_TIM_EnableIT_CC1(TIM2); // захват принятого сигнала

	//LL_TIM_EnableCounter(TIM1);// генерация тестового сигнала
	LL_TIM_ClearFlag_UPDATE(TIM4);
	memset(X1, 0, sizeof(X1));
	memset(X2, 0, sizeof(X2));
//	memset((*Env).CheckLine, 0 ,sizeof((*Env).CheckLine));

//	разворачиваем разреженую матрицу из конфиг файла в обычную X1
	for( index=0; index<NCheckLine; index++)
	{
		uint8_t i, j;
		i=(*Env).DataForTest[0][index];
		j=(*Env).DataForTest[1][index];
		if(i!=0 || j!=0)
		{
			X1[i-1][j-1]=1;	// заданная матрица
		}
	}

// процедура теста заполняем матрицу X2
			for( out_addr=0; out_addr<NCheckLine; out_addr++)
			{
				MuxSetOUT_Addr(out_addr);	// установить номер выхода X1
				GPIO_WriteBit(GPIOB, OUT_EN_Pin, RESET);// включить мультиплексор выходной
				for( in_addr=0; in_addr<NCheckLine; in_addr++)
				{
					MuxSetIN_Addr(in_addr);	// установить номер входа X2
					GPIO_WriteBit(GPIOA, IN_EN_Pin, RESET);// включить мультиплексор входной

					LL_TIM_EnableCounter(TIM2);// включить захват принятого сигнала
					N_periods=0;
					LL_TIM_EnableCounter(TIM4);	// триггер для запуска тестового сигнала

					while( !LL_TIM_IsActiveFlag_UPDATE(TIM4)){}
					LL_TIM_DisableCounter(TIM4);// вЫключить триггер для запуска тестового сигнала
					LL_TIM_ClearFlag_UPDATE(TIM4);
					LL_TIM_DisableCounter(TIM2);// вЫключить захват принятого сигнала

					GPIO_WriteBit(GPIOA, IN_EN_Pin, SET);// вЫключить мультиплексор входной
					if( N_periods == 10)
					{
						X2[out_addr][in_addr]=1; // есть контакт
						size++;
					}
				}
				GPIO_WriteBit(GPIOB, OUT_EN_Pin, SET);// вЫключить мультиплексор выходной
			}
			LL_TIM_DisableIT_CC1(TIM2);


			n_line = 0;
			index=0;
			for( out_addr=0; out_addr<NCheckLine; out_addr++)
			{
					for( in_addr=0; in_addr<NCheckLine; in_addr++)
						{
						if(X2[out_addr][in_addr] == 1)
							{
								if(index < NCheckLine)
								{
									DataAfterTest[0][index] = out_addr+1;
									DataAfterTest[1][index] = in_addr+1;
									index++;
								}
								else
								{
									// линий больше чем можно обработать, т.е. есть ошибочные перекрестные соединения
								}
							}
						}


			}
			for(index=0; index<NCheckLine; index++)
			{
				if(index <= size)
				{
					if(			(*Env).DataForTest[0][index] == DataAfterTest[0][index] && 	\
								(*Env).DataForTest[1][index] == DataAfterTest[1][index])
					{
						(*Env).CheckLine[index]=1;
					}
					else
					{
						(*Env).CheckLine[index]=2;
					}
				}
			}
//Init_Output_Input_GPIO();

}





void DrawTable(typeEnv *Env)
{
	uint8_t h=17, w=16;
	uint8_t col=8, row=5;
	uint8_t sx=Mx, ex=col*w+sx;
	uint8_t sy=20, ey=row*h+sy;
	uint8_t indx_col=0, indx_row=0;
	uint8_t CHR[3]={0};
	uint8_t X1[3]="X1";
	uint8_t X2[3]="X2";
	uint16_t color=0;
	memset(CHR,0,sizeof(CHR));
	for(indx_row=0;indx_row<=row;indx_row++)
	{
		ST7735_DrawHLine(sx,ex,sy+indx_row*h,TXT_COLOR);
	}
	ST7735_DrawVLine(sx,sy,sy+row*h,TXT_COLOR);
	ST7735_DrawVLine(sx+col*w,sy,sy+row*h,TXT_COLOR);
	for(indx_col=0; indx_col<=col; indx_col++)
	{
		ST7735_DrawVLine(sx+indx_col*w,sy+h,sy+4*h,TXT_COLOR);
	}

	ST7735_DrawString7x11(WIDTH/2-w/2,sy+4,X1,TXT_COLOR,BGR_COLOR);
	ST7735_DrawString7x11(WIDTH/2-w/2,sy+4+4*h,X2,TXT_COLOR,BGR_COLOR);

	for(indx_col=0; indx_col<col; indx_col++)
	{
		memset(CHR,0,sizeof(CHR));
		sprintf(CHR, "%d ", (*Env).DataForTest[0][indx_col]);
		ST7735_DrawChar7x11(sx+5+indx_col*w,sy+4+h,CHR[0],TXT_COLOR,BGR_COLOR);
	}

	for(indx_col=0; indx_col<col; indx_col++)
	{
		memset(CHR,0,sizeof(CHR));
		sprintf(CHR, "%d ", (*Env).DataForTest[1][indx_col]);
//		ST7735_DrawString7x11(sx+5+indx_col*w,sy+4+3*h,CHR,TXT_COLOR,BGR_COLOR);
		ST7735_DrawChar7x11(sx+5+indx_col*w,sy+4+3*h,CHR[0],TXT_COLOR,BGR_COLOR);
	}
	for(indx_col=0; indx_col<NCheckLine; indx_col++)
	{
		if( (*Env).CheckLine[indx_col] == 1)
		{
			color = COLOR565_LAWN_GREEN;
		}
		else if((*Env).CheckLine[indx_col] == 2)
		{
			color = COLOR565_ORANGE_RED;
		}
		else
		{
			color = BGR_COLOR;
		}
		ST7735_DrawString7x11(sx+5+indx_col*w,sy+4+2*h,"*",color,BGR_COLOR);
	}
	memset((*Env).CheckLine, 0 ,sizeof((*Env).CheckLine));  // очистить CheckLine до следующего теста

}


uint8_t CheckConnect(typeEnv *Env)
{
	uint8_t in_addr=0, out_addr=0;
	uint32_t connect=0;

	Init_Output_Input_GPIO();

	GPIO_WriteBit(GPIOA, LL_GPIO_PIN_11, SET);// включить out
	GPIO_WriteBit(GPIOB, OUT_EN_Pin, RESET);// включить мультиплексор выходной
	GPIO_WriteBit(GPIOA, IN_EN_Pin, RESET);// включить мультиплексор входной
	for( out_addr=0; out_addr<NCheckLine; out_addr++)
	{
		MuxSetOUT_Addr(out_addr);	// установить номер выхода X1
//		GPIO_WriteBit(GPIOB, OUT_EN_Pin, RESET);// включить мультиплексор выходной
		for( in_addr=0; in_addr<NCheckLine; in_addr++)
		{

			MuxSetIN_Addr(in_addr);	// установить номер входа X2
//			GPIO_WriteBit(GPIOA, IN_EN_Pin, RESET);// включить мультиплексор входной


			HAL_Delay(1);
			connect += LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0); //считать in

//			GPIO_WriteBit(GPIOA, IN_EN_Pin, SET);// вЫключить мультиплексор входной
		}
//		GPIO_WriteBit(GPIOB, OUT_EN_Pin, SET);// вЫключить мультиплексор выходной
	}
	GPIO_WriteBit(GPIOA, IN_EN_Pin, SET);// вЫключить мультиплексор входной
	GPIO_WriteBit(GPIOB, OUT_EN_Pin, SET);// вЫключить мультиплексор выходной
	GPIO_WriteBit(GPIOA, LL_GPIO_PIN_11, RESET);// вЫключить out
	if( connect != 0)
	{
		Init_Output_Input_Alter();
		return 1;
	}
	return 0;
}



void MuxEN_OUT(uint8_t addr)
{
	LL_GPIO_ResetOutputPin(GPIOB, OUT_EN_Pin);
}

void MuxEN_IN(uint8_t addr)
{
	LL_GPIO_ResetOutputPin(GPIOA, IN_EN_Pin);
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
