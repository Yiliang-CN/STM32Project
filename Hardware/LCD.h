#ifndef __LCD_H
#define __LCD_H

#include "Fonts.h"

// LCD总线时钟

#define LCD_RCC_GPIOx RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
#define LCD_RCC_FSMC RCC_AHBPeriph_FSMC

#define FSMC_Bank_NORSRAMx FSMC_Bank1_NORSRAM1

#define LCD_CMD_ADDR (__IO uint16_t *)(0x60000000)
#define LCD_DATA_ADDR (__IO uint16_t *)(0x60020000)

// LCD常用命令
#define LCD_SetCoordinateX 0x2A // 设置X坐标
#define LCD_SetCoordinateY 0x2B // 设置Y坐标
#define LCD_SetPixel 0x2C

// LCD屏幕参数
#define LCD_X_Start 0 // X坐标起点
#define LCD_Y_Start 0 // Y坐标起点

#define LCD_MAX_Pixel 320 // LCD最长像素点
#define LCD_MIN_Pixel 240 // LCD最短像素点

// LCD标准颜色设置
#define WHITE 0xFFFF   // 白色
#define BLACK 0x0000   // 黑色
#define GREY 0xF7DE    // 灰色
#define BLUE 0x001F    // 蓝色
#define BLUE2 0x051F   // 浅蓝色
#define RED 0xF800     // 红色
#define MAGENTA 0xF81F // 红紫色，洋红色
#define GREEN 0x07E0   // 绿色
#define CYAN 0x7FFF    // 蓝绿色，青色
#define YELLOW 0xFFE0  // 黄色
#define BRED 0xF81F
#define GRED 0xFFE0
#define GBLUE 0x07FF

// LCD控制引脚
#define LCD_GPIOx_CS GPIOD
#define LCD_Pin_CS GPIO_Pin_7 // PD7

#define LCD_GPIOx_RST GPIOE
#define LCD_Pin_RST GPIO_Pin_1 // PE1

#define LCD_GPIOx_BK GPIOD
#define LCD_Pin_BK GPIO_Pin_12 // PD12

#define LCD_GPIOx_RD GPIOD
#define LCD_Pin_RD GPIO_Pin_4 // PD4

#define LCD_GPIOx_WR GPIOD
#define LCD_Pin_WR GPIO_Pin_5 // PD5

#define LCD_GPIOx_DC GPIOD
#define LCD_Pin_DC GPIO_Pin_11 // PD11

// LCD数据引脚
#define LCD_GPIOx_D0 GPIOD
#define LCD_Pin_D0 GPIO_Pin_14 // PD14

#define LCD_GPIOx_D1 GPIOD
#define LCD_Pin_D1 GPIO_Pin_15 // PD15

#define LCD_GPIOx_D2 GPIOD
#define LCD_Pin_D2 GPIO_Pin_0 // PD0

#define LCD_GPIOx_D3 GPIOD
#define LCD_Pin_D3 GPIO_Pin_1 // PD1

#define LCD_GPIOx_D4 GPIOE
#define LCD_Pin_D4 GPIO_Pin_7 // PE7

#define LCD_GPIOx_D5 GPIOE
#define LCD_Pin_D5 GPIO_Pin_8 // PE8

#define LCD_GPIOx_D6 GPIOE
#define LCD_Pin_D6 GPIO_Pin_9 // PE9

#define LCD_GPIOx_D7 GPIOE
#define LCD_Pin_D7 GPIO_Pin_10 // PE10

#define LCD_GPIOx_D8 GPIOE
#define LCD_Pin_D8 GPIO_Pin_11 // PE11

#define LCD_GPIOx_D9 GPIOE
#define LCD_Pin_D9 GPIO_Pin_12 // PE12

#define LCD_GPIOx_D10 GPIOE
#define LCD_Pin_D10 GPIO_Pin_13 // PE13

#define LCD_GPIOx_D11 GPIOE
#define LCD_Pin_D11 GPIO_Pin_14 // PE14

#define LCD_GPIOx_D12 GPIOE
#define LCD_Pin_D12 GPIO_Pin_15 // PE15

#define LCD_GPIOx_D13 GPIOD
#define LCD_Pin_D13 GPIO_Pin_8 // PD8

#define LCD_GPIOx_D14 GPIOD
#define LCD_Pin_D14 GPIO_Pin_9 // PD9

#define LCD_GPIOx_D15 GPIOD
#define LCD_Pin_D15 GPIO_Pin_10 // PD10

void LCD_Init(void);
void LCD_SetScanMode(uint8_t Mode);
void LCD_Clear(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height);

// LCD显示函数

void LCD_ShowChar_EN(uint16_t X, uint16_t Y, const char Ch);
void LCD_ShowString_EN(uint16_t X, uint16_t Y, const char *Str);

void LCD_ShowImage(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, const uint8_t *Image);

// LCD常用设置

void LCD_SetFont(FONT *fonts);
FONT *LCD_GetFont(void);
void LCD_SetTextColor(uint16_t Color);
void LCD_GetTextColor(uint16_t *Color);
void LCD_SetBackColor(uint16_t Color);
void LCD_GetBackColor(uint16_t *Color);

#endif
