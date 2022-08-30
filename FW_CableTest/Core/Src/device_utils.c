/*
 * device_utils.c
 *
 *  Created on: 12 апр. 2022 г.
 *      Author: aalexeev
 */
#include "main.h"

/*********************************************
 * 	Инициализация структуры для (проверки)
 *
 * */
void LineStructInit(typeEnv *Env){
 char *str=0;
//	for(uint8_t i=0; i<lineMAX; i++){
//		str = (*Env).Lines[i].adapter.begin.name;
//		strncpy(str, "XP1", 4);
//		(*Env).Lines[i].adapter.begin.num = 21;
//
//		str = (*Env).Lines[i].adapter.end.name;
//		strncpy(str, "XP2", 4);
//		(*Env).Lines[i].adapter.end.num = 12;
//
//		str = (*Env).Lines[i].cabel.begin.name;
//		strncpy(str, "XS1", 4);
//		(*Env).Lines[i].cabel.begin.num = 22;
//
//		str = (*Env).Lines[i].cabel.end.name;
//		strncpy(str, "XS1", 4);
//		(*Env).Lines[i].cabel.end.num = 24;
//	}
}

/**********************************************************************************
 * Парсим csv данные прочитанные из файла
 * Данные идут в следующем порядке:
 *
номер  |		 кабель	        |		адаптер
строки |  начало	|  конец	|	 начало	  |  конец
	   |имя	 номер	| имя  номер|	имя	номер |	имя	номер
_______|________________________|____________________________________________________
	1  |l_026_010				|							--------- имя файла/кабеля
	2  |XP1	  1		 XS1	 4	|	X1   1		X1	 2
	3  |XP1	  2		 XS1	 2	|	X1	 3		X1	 4
	4  |XP1	  3		 XS1	 5	|	X1	 5		X1	 6

 * */
void ParseCSVBuf(typeEnv *Env)
{
#define tmpSZ 50
	char *buf = (*Env).CSVBuf;
	char *p_cabName=0;
	char *p_line=0;
	char *p_token=0;
	int   cnt_line=0;
	char sepLine[2] = "\n";
	char sepToken[1] = ",";
	p_line = strtok(buf,sepToken);		// первая строка до запятой - название файла/кабеля
	p_cabName = p_line;
	p_token = strtok(0,sepLine);				// находит следущую строку с которой начинается запись конфигурации
	do
	{
			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].cabel.begin.name = p_token;	// кабель.начало.имя

			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].cabel.begin.num = p_token;	//кабель.начало.номер

			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].cabel.end.name = p_token;  //кабель.конец.имя

			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].cabel.end.num = p_token; //кабель.конец.номер

			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].adapter.begin.name = p_token; //адаптер.начало.имя

			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].adapter.begin.num = p_token; //адаптер.начало.номер

			p_token = strtok (NULL,sepToken);
			(*Env).Lines[cnt_line].adapter.end.name = p_token; //адаптер.конец.имя

//			p_token = strtok (NULL,sepLine);					// "\r\n"   символы "конец линии" (строки) оставляем в строке
			p_token = strtok (NULL,sepToken);					// ","
			(*Env).Lines[cnt_line].adapter.end.num = p_token; //адаптер.конец.номер

		(*Env).Lines[cnt_line].numLine = cnt_line+1;
		cnt_line++;
	} while(p_token !=NULL);

}
