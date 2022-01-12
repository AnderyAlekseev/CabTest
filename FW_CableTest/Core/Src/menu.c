/*
 * menu.c
 *
 *  Created on: 8 дек. 2021 г.
 *      Author: aalexeev
 */
#include "main.h"
#include "ff.h"

uint8_t FS_GetFileList(typeEnv *Env);
uint8_t FS_ReadFile(typeEnv *Env);
void ParseString(char  *str_in, char *data_out);

const uint16_t  COLORS565[140] = {0xF7DF, 0xFF5A, 0x07FF, 0x7FFA, 0xF7FF, 0xF7BB, 0xFF38, 0x0, 0xFF59, 0x001F, 0x895C, 0xA145, 0xDDD0, 0x5CF4, 0x7FE0, 0xD343, 0xFBEA, 0x64BD, 0xFFDB, 0xD8A7, 0x07FF, 0x11, 0x451, 0xBC21, 0xAD55, 0x320, 0xBDAD, 0x8811, 0x5345, 0xFC60, 0x9999, 0x8800, 0xECAF, 0x8DF1, 0x49F1, 0x2A69, 0x067A, 0x901A, 0xF8B2, 0x05FF, 0x6B4D, 0x1C9F, 0xB104, 0xFFDE, 0x2444, 0xF81F, 0xDEFB, 0xFFDF, 0xFEA0, 0xDD24, 0x8410, 0x400, 0xAFE5, 0xF7FE, 0xFB56, 0xCAEB, 0x4810, 0xFFFE, 0xF731, 0xE73F, 0xFF9E, 0x7FE0, 0xFFD9, 0xAEDC, 0xF410, 0xE7FF, 0xFFDA, 0xD69A, 0x9772, 0xFDB8, 0xFD0F, 0x2595, 0x867F, 0x7453, 0xB63B, 0xFFFC, 0x7, 0x3666, 0xFF9C, 0xF81F, 0x8000, 0x6675, 0x19, 0xBABA, 0x939B, 0x3D8E, 0x7B5D, 0x07D3, 0x4E99, 0xC0B0, 0x18CE, 0xF7FF, 0xFF3C, 0xFF36, 0xFEF5, 0x10, 0xFFBC, 0x8400, 0x6C64, 0xFD20, 0xFA20, 0xDB9A, 0xEF55, 0x9FD3, 0xAF7D, 0xDB92, 0xFF7A, 0xFED7, 0xCC27, 0xFE19, 0xDD1B, 0xB71C, 0x8010, 0xF800, 0xBC71, 0x435C, 0x8A22, 0xFC0E, 0xF52C, 0x2C4A, 0xFFBD, 0xA285, 0xC618, 0x867D, 0x6AD9, 0x7412, 0xFFDF, 0x07EF, 0x4416, 0xD5B1, 0x410, 0xDDFB, 0xFB08, 0x471A, 0xEC1D, 0xF6F6, 0xFFFF, 0xF7BE, 0xFFE0, 0x9E66 };
void Menu(typeEnv *Env)
{
	static uint32_t EncoderCount=0x7fff;
	uint32_t currentCount = LL_TIM_GetCounter(TIM3);
	uint32_t end_list = Env->Menu.NmbrOnPageFiles;
	short int ActiveItem = (*Env).Menu.ActiveItem;
	short int ActivePage = (*Env).Menu.ActivePage;
	char *fileName = (*Env).FileNameForTest;
	static uint8_t but_latch=0, but_press=0, indx=0, key=0;
	uint8_t but_OK=0, but_DWN=0, but_UP=0, but_ENC=0;

	char String[FILENAME_SIZE];


	if(f_EncoderRead == 1)
	{
		f_EncoderRead = 0;
		if(EncoderCount != currentCount  	)
			{
				if(EncoderCount > currentCount  	)
				{
					ActiveItem++;
					if(ActiveItem >= end_list)
						{
						ActiveItem = 0;
						ActivePage++;
						if(ActivePage > Env->Menu.NmbrAllPages) {ActivePage = Env->Menu.NmbrAllPages;}
						}
				}
				else if (EncoderCount < currentCount )
				{
					ActiveItem--;
					if(ActiveItem < 0)
						{
							ActiveItem = 0;
							ActivePage--;
							if(ActivePage < 0) {ActivePage = 0;}
						}
				}

				if(ActivePage != (*Env).Menu.ActivePage)
				{
					(*Env).Menu.ActivePage = ActivePage;
					FS_GetFileList(Env);
					ST7735_Clear((*Env).Menu.BGR_Color);
				}
				EncoderCount = currentCount;
				(*Env).Menu.ActiveItem = ActiveItem;
				f_RefreshScreen = 1;
			}

					/* обработка кнопки */
		but_ENC = LL_GPIO_IsInputPinSet(BUTTON_GPIO_Port, BUTTON_Pin)<<3;
		but_UP  = LL_GPIO_IsInputPinSet(BUT_UP_GPIO_Port, BUT_UP_Pin)<<2;
		but_OK  = LL_GPIO_IsInputPinSet(BUT_OK_GPIO_Port, BUT_OK_Pin)<<1;
		but_DWN = LL_GPIO_IsInputPinSet(BUT_DWN_GPIO_Port,BUT_DWN_Pin);
		but_latch = but_ENC | but_UP | but_OK | but_DWN;
		if(but_latch !=0xF)
		{
			but_press=1;
			key = but_latch;
		}
		if( but_latch==0xF &&  but_press ==1)
		{
			but_press=0;
			Keyboard_handler(Env, key);
			key=0;
		}
				/*	if( but_latch == 0 )
					{
						but_press=1;
					}
					if( but_latch==1 &&  but_press ==1)
					{
						but_press=0;
						(*Env).FileNameForTest = &(*Env).Menu.FileList[ActiveItem][0];// указателю отдаем адрес
						FS_ReadFile(Env);
					}*/
//
//		ST7735_DrawChar7x11(110,92, String,TXT_COLOR,BGR_COLOR);
	}

}

void Keyboard_handler(typeEnv *Env, uint8_t key)
{
	char String[8]={0};
	sprintf(&String, "%X", key);
	ST7735_DrawString7x11(100,92,String,TXT_COLOR,BGR_COLOR);
}

void ParseString(char  *str_in, char *data_out)
{
	uint32_t len = sizeof(str_in);
	for(;;)
	{


	}
}
