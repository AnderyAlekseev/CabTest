/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#include <string.h>
#include "main.h"
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
				strncpy( &(Env->Menu.FileList[indx][0]),  fs_info.fname, strlen(fs_info.fname));
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



uint8_t FS_Read_JSON_File(typeEnv *Env)
{
//	int sz =2000;
//	char *FileName = (*Env).FileNameForTest;
//	char *buf=0; // строка прочитанная из файла
//	char str[200]={0};
//	cJSON * jsonObj=0;
////	cJSON * item = NULL;
//	cJSON * LineNum=0;
//	uint32_t byteRead=0;
//	char *PrintString = 0;
////	buf = (char*)malloc(sz);  // выделяем кусок памяти из кучи
//	char *JsonBuf 		= (*Env).JsonBuf;
//	FIL fs_file;
//	FRESULT fs_result=0;
////открываем файл
//	fs_result = f_open(&fs_file, FileName, FA_READ);
//	if(fs_result != FR_OK)	{ return 1; }
//
//// читаем весь файл в буфер
//	fs_result = f_read(&fs_file, JsonBuf, FILE_SIZE, &byteRead);
//	if(fs_result != FR_OK)	{ return 1; }
//	f_close (&fs_file);
//	(*Env).RealDataSize = byteRead;
//// парсим буфер
////		jsonObj = cJSON_ParseWithLength(JsonBuf, FILE_SIZE);
//		jsonObj = cJSON_Parse(JsonBuf);
//		PrintString = cJSON_Print(jsonObj);
//		sprintf(str, PrintString);
//				 const char *error_ptr = cJSON_GetErrorPtr();
//				 if (error_ptr != NULL)
//				 {
//					 sprintf(str, error_ptr);
////				 fprintf(stderr, "Ошибка до: %s\n", error_ptr);
//				 }
////		cJSON_bool typ 	=  cJSON_IsArray(jsonObj);
////		typ				=  cJSON_IsObject(jsonObj);
////		uint8_t arrSZ = cJSON_GetArraySize(jsonObj);
//		/*cJSON * subitem = cJSON_GetArrayItem(item, i);
//     name = cJSON_GetObjectItem(subitem, "name");
//     index = cJSON_GetObjectItem(subitem, "index");
//     optional = cJSON_GetObjectItem(subitem, "optional");*/
//		cJSON * test = cJSON_GetObjectItem(jsonObj, "test");
//		cJSON_bool typ 	=  cJSON_IsArray(test);
//		if( test != 0)
//		{
//			for(uint8_t i=0; i<52; i++)
//			{
//				cJSON *item 	= cJSON_GetArrayItem(test, i);
//				cJSON *cabel = cJSON_GetObjectItem(item, "cabel");
//				cJSON *adapter = cJSON_GetObjectItem(item, "adapter");
////				(*Env).Lines[i].adapter.begin.name = LineNum->valueint;
//			}
//		}
/*
  cJSON *item = cJSON_GetObjectItem(items,"items");
  for (i = 0 ; i < cJSON_GetArraySize(item) ; i++)
  {
     cJSON * subitem = cJSON_GetArrayItem(item, i);
     name = cJSON_GetObjectItem(subitem, "name");
     index = cJSON_GetObjectItem(subitem, "index");
     optional = cJSON_GetObjectItem(subitem, "optional");
  }*/
//			item = cJSON_GetObjectItem(result, "line");
			//strncpy(str, cJSON_Print(result),sizeof(str));
//			free(buf);

//			return result;
}




/* *********************************************
 *
 * Чтение файла универсальной настройки
 * вызывается в   Menu(typeEnv *Env)
 *
 ************************************ */
uint8_t FS_ReadCSVFile(typeEnv *Env){
	char *FileName = (*Env).FileNameForTest;
	char *p_start=0;
	char *p_end=0;
	FIL fs_file;
	FRESULT fs_result=0;
	uint32_t byteRead=0;

	fs_result = f_open(&fs_file, FileName, FA_READ);
	if(fs_result != FR_OK)
		{
		memset((*Env).Status , 0, sizeof((*Env).Status ));
		strcpy((*Env).Status , 'FS_ERR_OPEN_FILE');// конец файла
		return 1;
		}
	// читаем всесь файл в буфер
	fs_result = f_read(&fs_file, (*Env).CSVBuf, sizeof((*Env).CSVBuf), &byteRead);
	f_close(&fs_file);
	if(fs_result != FR_OK)
		{
		memset((*Env).Status , 0, sizeof((*Env).Status ));
		strcpy((*Env).Status , 'FS_ERR_READ_FILE');
		return 1;
		}

	if(byteRead !=FILE_SIZE)
		{
			(*Env).RealDataSize = byteRead;
		}
	else{
		memset((*Env).Status , 0, sizeof((*Env).Status ));
		strcpy((*Env).Status , 'FS_EOF');// конец файла
	}
	ParseCSVBuf(Env);
}



uint8_t FS_ReadFile(typeEnv *Env)
{
	char *FileName = (*Env).FileNameForTest;

	char X1[NLin*2]={0}, X2[NLin*2]={0};
	char string[DATA_TEST_SIZE]={0}; // строка прочитанная из файла ; 255 байт
	char FormatStr[5*NLin]={0};// 5 символов в строке форматирования "%d%*c" для одного значения
	char *p_start=0;
	char *p_end=0;
	int LenData=0, MaxLen=0;
	uint32_t DataBuf[2][NLin]={0};

	FIL fs_file;
	FRESULT fs_result=0;
	uint32_t byteRead=0;

	fs_result = f_open(&fs_file, FileName, FA_READ);
		if(fs_result != FR_OK)	{ return 1; }
// читаем всесь файл в буфер
		fs_result = f_read(&fs_file, string, DATA_TEST_SIZE, &byteRead);
		if(fs_result != FR_OK)	{ return 1; }
		(*Env).RealDataSize = byteRead;

		// выделяем данные заключенные в скобки [] для разъёма X1
		p_start = strchr(&string, (char)PARS_START_DATA);
		p_end = strchr(&string, (char)PARS_END_DATA);
		LenData = p_end - p_start;
		MaxLen = LenData;
		memmove(&X1, (p_start+1),  LenData-1);
		for(uint8_t i=0; i<LenData/2;i++)
		{
			strcat( &FormatStr,  (const char)"%d%*c");

		}
		sscanf(&X1, &FormatStr,			&DataBuf[0][0], \
										&DataBuf[0][1],  &DataBuf[0][2], \
										&DataBuf[0][3],  &DataBuf[0][4],\
										&DataBuf[0][5],  &DataBuf[0][6],\
										&DataBuf[0][7],  &DataBuf[0][8],\
										&DataBuf[0][9],  &DataBuf[0][10],\
										&DataBuf[0][11], &DataBuf[0][12],\
										&DataBuf[0][13], &DataBuf[0][14],\
										&DataBuf[0][15], &DataBuf[0][16],\
										&DataBuf[0][17], &DataBuf[0][18],\
										&DataBuf[0][19], &DataBuf[0][20],\
										&DataBuf[0][21], &DataBuf[0][22],\
										&DataBuf[0][23], &DataBuf[0][24],\
										&DataBuf[0][25], &DataBuf[0][26], \
										&DataBuf[0][27], &DataBuf[0][28],\
										&DataBuf[0][29], &DataBuf[0][30], &DataBuf[0][31]);
		// выделяем данные заключенные в скобки [] для разъёма X2
		p_start = strchr(p_end, PARS_START_DATA);
		p_end = strchr(p_start, PARS_END_DATA);
		LenData = p_end - p_start;
		if(LenData > MaxLen) {MaxLen = LenData; }

		memmove(&X2, (p_start+1),  LenData-1);

		memset(FormatStr,0, sizeof(FormatStr));
		for(uint8_t i=0; i<LenData/2;i++)
		{
			strcat( &FormatStr,  (const char)"%d%*c");
		}

		sscanf(&X2, &FormatStr, 	&DataBuf[1][0],  &DataBuf[1][1], \
									&DataBuf[1][2],  &DataBuf[1][3],\
									&DataBuf[1][4],  &DataBuf[1][5],\
									&DataBuf[1][6],  &DataBuf[1][7],\
									&DataBuf[1][8],  &DataBuf[1][9],\
									&DataBuf[1][10], &DataBuf[1][11],\
									&DataBuf[1][12], &DataBuf[1][13],\
									&DataBuf[1][14], &DataBuf[1][15],\
									&DataBuf[1][16], &DataBuf[1][17],\
									&DataBuf[1][18], &DataBuf[1][19],\
									&DataBuf[1][20], &DataBuf[1][21],\
									&DataBuf[1][22], &DataBuf[1][23],\
									&DataBuf[1][24], &DataBuf[1][25],\
									&DataBuf[1][26], &DataBuf[1][27],\
									&DataBuf[1][28], &DataBuf[1][29],\
									&DataBuf[1][30], &DataBuf[1][31]);
	f_close(&fs_file);
	Env->RealDataSize=MaxLen/2;
	memmove( (*Env).DataForTest, &DataBuf, sizeof(DataBuf)	);
	return 0;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
