#include "main.h"

void Display(typeEnv *Env)
{
	ST7735_ListBox_Item Files_on_SD[ITEM_ON_PAGE_MAX];
	ST7735_ListBox FileList;
	uint32_t selectedFile = Env->Menu.ActiveItem;
	uint32_t end_list = Env->Menu.NmbrOnPageFiles;
	char EncoderString[12] = "Привет";
	//static uint8_t step=0;
	if(f_RefreshScreen == 1)
	{
		f_RefreshScreen = 0;

		for(uint32_t index =0; index < end_list; index++)
			{
				//memset( &(Files_on_SD[index].text), 0, sizeof(Files_on_SD[index].text));
				//strncpy( &(Files_on_SD[index].text),  &(Env->Menu.FileList[index]), FILENAME_SIZE);
				Files_on_SD[index].text  = &(Env->Menu.FileList[index]);
				Files_on_SD[index].visible=1;
				Files_on_SD[index].selected=0;
			}
		ST7735_ListBox_StructInit(&FileList,0,9,&Files_on_SD,end_list);
		Files_on_SD[selectedFile].selected=1;
		FileList.BackgroundColor = (*Env).Menu.BGR_Color;
		FileList.BorderColor	=	(*Env).Menu.BRD_Color;
		FileList.ItemTextColor	=	(*Env).Menu.TXT_Color;
		FileList.SelectedItemColor	=(*Env).Menu.SEL_Color;
		ST7735_ListBox_Draw(&FileList);
	}

	//sprintf(&EncoderString, "I-%2d P-%2d A-%d", selectedFile, Env->Menu.ActivePage, Env->Menu.NmbrAllFiles);
	//ST7735_DrawString7x11(1,1,EncoderString,COLOR565_ALICE_BLUE,COLOR565_DARK_SLATE_BLUE);




}
