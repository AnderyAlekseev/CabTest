#ifndef ST7335_H
#define ST7335_H

#include <string.h>
//#include "hrddef.h"
//#include "stm32f10x.h"
#include "stm32f1xx.h"
#include "font5x7.h"
#include "font7x11.h"
#include "color565.h"

#include "main.h"

//extern SPI_HandleTypeDef hspi1;
//extern HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);

//Defines
#define LCD_SPI SPI1
// orientation
#define HORIZ //HORIZ // или VERTICAL
#ifdef VERTICAL
#define WIDTH  127
#define HEIGHT 159
#else
#define WIDTH  159
#define HEIGHT 127
#endif

#define LINE_HEIGTH 		(uint32_t)13			// высота строки в пикселях для вывода на дисплей

//ST7735 COMMANDS
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

#define LCD_COMMAND 0
#define LCD_DATA 1


/*HAL_GPIO_DEF*/
#define  CS_HIGH_ST7735         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define  CS_LOW_ST7735          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define  RESET_HIGH_ST7735      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define  RESET_LOW_ST7735       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define  A0_HIGH_ST7735         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)
#define  A0_LOW_ST7735          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)

// Variables
static __IO uint32_t TimingDelay;
//Enums
typedef enum 
{
	ST7735_TextAlign_Left = 0,
	ST7735_TextAlign_Center = 1,
	ST7735_TextAlign_Right = 2
} ST7735_TextAlign;
//Structs
//ST7735_Label
typedef struct 
{
	uint8_t x,y;//X,Y Coords of btn
	uint8_t h,w;//H,W of btn
	
	uint16_t BorderColor,TextColor,BackgroundColor;//BorderColor
	
	uint8_t* txt;//Text on Btn
	uint8_t txt_length;//Length of string
	ST7735_TextAlign txt_Align;//Align of text
	
} ST7735_Label;
//ST7735_Button
typedef struct 
{
	uint8_t x,y;//X,Y Coords of btn
	uint8_t h,w;//H,W of btn
	
	uint16_t BorderColor,TextColor,BackgroundColor;//BorderColor
	
	uint8_t* txt;//Text on Btn
	uint8_t txt_length;//Length of string
	ST7735_TextAlign txt_Align;//Align of text
	
} ST7735_Button;

void ST7735_Button_StructInit(ST7735_Button* btn,uint8_t x,uint8_t y,char* txt,uint8_t length);
void ST7735_Button_Draw(ST7735_Button* btn);
void ST7735_Button_Destroy(ST7735_Button* btn);

//ST7735_ProgressBar

typedef struct 
{
	uint8_t x,y;//X,Y Coords o
	uint8_t h,w;//Height,width
	
	uint16_t BorderColor,BackgroundColor,ProgressColor;//BorderColor
	uint8_t progress;
} ST7735_ProgressBar;

void ST7735_ProgressBar_StructInit(ST7735_ProgressBar* pb,uint8_t x,uint8_t y);
void ST7735_ProgressBar_SetProgress(ST7735_ProgressBar* pb,uint8_t progress);
void ST7735_ProgressBar_Draw(ST7735_ProgressBar* pb);
void ST7735_ProgressBar_Destroy(ST7735_ProgressBar* pb);

//ST7735_ListBox

typedef struct ST7735_ListBox_Item
{
	char * text;
	//char  text[FILENAME_SIZE];
	uint8_t visible;
	uint8_t selected;
}ST7735_ListBox_Item;

typedef struct 
{
	uint8_t x,y;//X,Y Coords o
	uint8_t h,w;//Height,width
	
	uint16_t BorderColor,BackgroundColor,ItemTextColor,SelectedItemColor;//BorderColor
	ST7735_ListBox_Item* items;
	uint8_t items_count,SelectedItemIndex;
} ST7735_ListBox;

void ST7735_ListBox_Draw(ST7735_ListBox* lb);
void ST7735_ListBox_SetSelected(ST7735_ListBox* lb,uint8_t num);
void ST7735_ListBox_SetVisibleRange(ST7735_ListBox* lb,uint8_t min,uint8_t max);
ST7735_ListBox_Item ST7735_ListBox_GetSelectedItem(ST7735_ListBox* lb);
void ST7735_ListBox_StructInit(ST7735_ListBox* lb,uint8_t x,uint8_t y,ST7735_ListBox_Item* items,uint8_t item_count);
void ST7735_ListBox_Destroy(ST7735_ListBox* lb);
//Function defines
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

//Functions with DMA
#define ST7735_DMA_Channel DMA1_Channel3
#ifdef ST7735_DMA_Channel
void ST7735_DMA_Init(void);
void ST7735_DMA_FillScreen(uint8_t *array,uint8_t increment_memory);
void ST7735_DMA_DrawBMP(uint16_t *array,uint8_t x,uint8_t y,uint8_t width,uint8_t height);
#endif
//Functions without DMA

void HAL_ST7735_Init(void);
void ST7735_Clear(uint16_t Color);
void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
void ST7735_DrawPixel(uint8_t x,uint8_t y, uint16_t Color);
void ST7735_DrawHLine(uint8_t x,uint8_t x1,uint8_t y,uint16_t Color);
void ST7735_DrawVLine(uint8_t x,uint8_t y,uint8_t y1,uint16_t Color);
void ST7735_DrawBMP(const uint8_t *array,uint32_t length);
void ST7735_DrawChar(uint8_t x,uint8_t y,uint8_t chr,uint16_t color,uint16_t background_color);
void ST7735_DrawChar7x11(uint8_t x,uint8_t y,uint8_t chr,uint16_t color,uint16_t background_color);
void ST7735_DrawString7x11(uint8_t x,uint8_t y,uint8_t* chr,uint16_t color,uint16_t background_color);
void ST7735_DrawRectangle (uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint16_t Color);
#endif
