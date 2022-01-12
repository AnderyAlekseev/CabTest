#include "main.h"

void Display(typeEnv *Env)
{
	uint8_t mode = (*Env).Mode;
	if(f_RefreshScreen == 1)
	{
		f_RefreshScreen = 0;


		switch(mode)	// назначить действие в зависимости от текущего режима
					{
						case MENU: 			DisplayMenu(Env); 		break;
						case CHECK_SCHEME: 	DisplayChekSchem(Env);  	break;
						case TEST: 			DisplayTest(Env);			break;
						case RESULT: 		DisplayResult(Env);		break;
							default: break;
					}

	}

	//sprintf(&EncoderString, "I-%2d P-%2d A-%d", selectedFile, Env->Menu.ActivePage, Env->Menu.NmbrAllFiles);
	//ST7735_DrawString7x11(1,1,EncoderString,COLOR565_ALICE_BLUE,COLOR565_DARK_SLATE_BLUE);
//char CyrStr='А';
//char cyr_str='Б';
//char LatStr='A';
//char lat_str='B';

//	ST7735_DrawChar(110,16, CyrStr,TXT_COLOR,BGR_COLOR);
//	ST7735_DrawChar(110,28, cyr_str,TXT_COLOR,BGR_COLOR);
//	ST7735_DrawChar(110,40, LatStr,TXT_COLOR,BGR_COLOR);
//	ST7735_DrawChar(110,52, lat_str,TXT_COLOR,BGR_COLOR);
//	//ST7735_DrawChar(110,64, 0x7f,TXT_COLOR,BGR_COLOR);
//	ST7735_DrawChar7x11(110,80, CyrStr,TXT_COLOR,BGR_COLOR);
//	ST7735_DrawChar7x11(110,92, cyr_str,TXT_COLOR,BGR_COLOR);
}

void DisplayMenu(typeEnv *Env)
{
	ST7735_ListBox_Item Files_on_SD[ITEM_ON_PAGE_MAX];
	ST7735_ListBox FileList;
	uint32_t selectedFile = Env->Menu.ActiveItem;
	uint32_t end_list = Env->Menu.NmbrOnPageFiles;

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

void DisplayChekSchem(typeEnv *Env)
{
	char EncoderString[12] = "Check sheme";
	ST7735_Clear(BGR_COLOR);
	ST7735_DrawString7x11(1,1,EncoderString,TXT_COLOR,BGR_COLOR);
}

void DisplayTest(typeEnv *Env)
{
	char EncoderString[12] = "Test";
	ST7735_Clear(BGR_COLOR);
	ST7735_DrawString7x11(1,1,EncoderString,TXT_COLOR,BGR_COLOR);
}

void DisplayResult(typeEnv *Env)
{
	char EncoderString[12] = "Result";
	ST7735_Clear(BGR_COLOR);
	ST7735_DrawString7x11(1,1,EncoderString,TXT_COLOR,BGR_COLOR);
}





void debug_LCD_print(uint8_t arg)
{
	char str[1]={0};
	sprintf(str,"%s", arg);
	ST7735_DrawChar(110,16, str,TXT_COLOR,BGR_COLOR);
}
