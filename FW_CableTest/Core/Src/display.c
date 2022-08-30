#include "main.h"
#include "st7735.h"
void DisplayMenu(typeEnv *Env);
void DisplayWait(typeEnv *Env);
void DisplayTest(typeEnv *Env);
void DrawTable(typeEnv *Env);

void Display(typeEnv *Env)
{
	uint8_t mode = (*Env).Mode;
	switch(mode)	// назначить действие в зависимости от текущего режима
		{
			case MODE_MENU: 	DisplayMenu(Env); 			break;
			case MODE_WAIT: 	DisplayWait(Env);  			break;
			case MODE_TEST: 	DisplayTest(Env);			break;
			default: break;
		}
}

void DisplayMenu(typeEnv *Env)
{
	ST7735_ListBox_Item Files_on_SD[ITEM_ON_PAGE_MAX];
	ST7735_ListBox FileList;
	uint32_t selectedFile = Env->Menu.ActiveItem;
	uint32_t end_list = Env->Menu.NmbrOnPageFiles;

	if(f_RefreshScreen == 1)
	{
		f_RefreshScreen = 0;
		for(uint32_t index =0; index < end_list; index++)
		{
			Files_on_SD[index].text  = &(Env->Menu.FileList[index]);
			Files_on_SD[index].visible=1;
			Files_on_SD[index].selected=0;
		}
		ST7735_ListBox_StructInit(&FileList,0,0,&Files_on_SD,end_list);
		Files_on_SD[selectedFile].selected=1;
		FileList.BackgroundColor = BGR_COLOR;
		FileList.BorderColor	=	BRD_COLOR;
		FileList.ItemTextColor	=	TXT_COLOR;
		FileList.SelectedItemColor	=SEL_COLOR;
		ST7735_ListBox_Draw(&FileList);
	}
}


void DisplayWait(typeEnv *Env)
{
	static uint8_t countTick =0;
	uint8_t WaitMessage[7]={"Wait..."};
	uint8_t Message[20]={"Check scheme"};
	uint8_t waitCHR[4]= {0x5c,0x7c,0x2f,0x2d};//   / - \ |
	static uint8_t indx_WaitChar=0;

	if(f_waitTick == 1)
	{
		f_waitTick=0;
		if(countTick++ ==1)
		{
			countTick = 0 ;
			ST7735_DrawString(Mx,6,&WaitMessage[0],TXT_COLOR,BGR_COLOR);
			ST7735_DrawChar(57,6, waitCHR[indx_WaitChar++],TXT_COLOR,BGR_COLOR);
			if(indx_WaitChar == 4)
			{ indx_WaitChar = 0; }
		}
	}

	if(f_RefreshScreen == 1)
		{
			f_RefreshScreen = 0;
			ST7735_Clear(BGR_COLOR);
			DrawTable(Env);
			ST7735_DrawString(Mx,My,Message,TXT_COLOR,BGR_COLOR);
		}
}

void DisplayTest(typeEnv *Env)
{
	if(f_RefreshScreen == 1)
	{
		f_RefreshScreen = 0;
		ST7735_Clear(BGR_COLOR);
		DrawTable(Env);
		uint8_t Message[22]={"Press OK for run test"};
		ST7735_DrawString(Mx,My,Message,TXT_COLOR,BGR_COLOR);

		ST7735_DrawString(Mx,6,(*Env).Status,TXT_COLOR,BGR_COLOR);
	}
}


void debug_LCD_print(uint8_t arg)
{
	char str[1]={0};
	sprintf(str,"%s", arg);
	ST7735_DrawChar(110,16, &str,TXT_COLOR,BGR_COLOR);
}
