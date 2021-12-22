/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
uint8_t FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path);
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
#include "main.h"
//extern ST7735_ListBox_Item Files_on_SD[ITEM_ON_PAGE_MAX];


uint8_t FS_GetFileList(typeEnv *Env)
{
	DIR dir;
	FRESULT fs_result;
	FILINFO fs_info;
	char* path ="";
	uint32_t cnt_file=0, indx=0;
	uint32_t start_list=0, end_list=0;

	fs_result = f_opendir (&dir, path);
	if(fs_result != FR_OK)
	{ return 1; }

	start_list	= Env->Menu.ActivePage*ITEM_ON_PAGE_MAX ;
	end_list	= Env->Menu.ActivePage*ITEM_ON_PAGE_MAX + (ITEM_ON_PAGE_MAX);
	memset( &(Env->Menu.FileList[0][0]), 0, sizeof(Env->Menu.FileList));

	for (;;)
	{
		fs_result = f_readdir(&dir, &fs_info);  /* Чтение объекта директории */
		if (fs_result != FR_OK || fs_info.fname[0] == 0 ){ break;} /* Останов цикла при ошибке или при достижении конца списка директории */
		if (fs_info.fname[0] == '.') {continue;} /* игнорирование элемента 'точка' */
		if (fs_info.fattrib & AM_DIR){continue;}/* игнорирование элемента 'директория' */
		else
		{   /* Это файл. Заносим название в список файлов */
			if(cnt_file >= start_list && cnt_file < end_list)
			{
				strncpy( &(Env->Menu.FileList[indx][0]),  &fs_info.fname, strlen(fs_info.fname));
				indx++;
				Env->Menu.NmbrOnPageFiles=indx;
			}
			cnt_file++;
		}
		Env->Menu.NmbrAllFiles=cnt_file;
	}

	f_closedir(&dir);
	return 0;
}

uint8_t FS_ReadFile(typeEnv *Env)
{
	char *FileName = (*Env).FileNameForTest;
	//char *DataBuf  = (*Env).DataForTest;
	char *DataBuf  = (*Env).DataForTest;
	char string[DATA_TEST_SIZE]; // строка прочитанная из файла ; 128 байт

	FIL fs_file;
	FRESULT fs_result;
	uint32_t byteRead;
	uint32_t ofs=0;	// смещение от начала файла
	fs_result = f_open(&fs_file, FileName, FA_READ);
		if(fs_result != FR_OK)	{ return 1; }

		/*while(1)
		{
			fs_result = f_lseek (fs_file, ofs++);
			f_gets (&string, DATA_TEST_SIZE, fs_file);
			if(string[0] == '#')
				{
					continue;
				}
			if(string[0] == '\0')
				{
					break;
				}
		}*/

	fs_result = f_read(&fs_file, DataBuf, DATA_TEST_SIZE, &byteRead);
			if(fs_result != FR_OK)	{ return 1; }
	(*Env).RealDataSize = byteRead;

	f_close(&fs_file);
	return 0;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
