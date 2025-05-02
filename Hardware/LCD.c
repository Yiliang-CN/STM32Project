#include "stm32f10x.h" // Device header
#include "LCD.h"
#include "Delay.h"

uint16_t LCD_X_LENGTH = LCD_MIN_Pixel;
uint16_t LCD_Y_LENGTH = LCD_MAX_Pixel;

uint8_t LCD_SCAN_MODE = 6; // 屏幕扫描模式 0~7

static FONT *LCD_TextFont = &Font8x16;
static uint16_t LCD_TextColor = BLACK; // 默认文本颜色
static uint16_t LCD_BackColor = WHITE; // 默认背景颜色

__inline void LCD_WriteCmd(uint16_t Cmd)
{
    *LCD_CMD_ADDR = Cmd;
}

__inline void LCD_WriteData(uint16_t Data)
{
    *LCD_DATA_ADDR = Data;
}

__inline uint16_t LCD_ReadData(void)
{
    return (*LCD_DATA_ADDR);
}

__inline void LCD_FillColor(uint32_t Pixels, uint16_t Color)
{
    LCD_WriteCmd(LCD_SetPixel);

    for (uint32_t i = 0; i < Pixels; i++)
        LCD_WriteData(Color);
}

/**
 * @brief: LCD GPIO初始化
 * @return {*}
 */
static void LCD_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(LCD_RCC_GPIOx, ENABLE);

    // 推挽输出模式引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_BK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_GPIOx_BK, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_RST;
    GPIO_Init(LCD_GPIOx_RST, &GPIO_InitStructure);

    // 推挽复用模式引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
    GPIO_Init(LCD_GPIOx_CS, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
    GPIO_Init(LCD_GPIOx_RD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
    GPIO_Init(LCD_GPIOx_WR, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_DC;
    GPIO_Init(LCD_GPIOx_DC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D0;
    GPIO_Init(LCD_GPIOx_D0, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D1;
    GPIO_Init(LCD_GPIOx_D1, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D2;
    GPIO_Init(LCD_GPIOx_D2, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D3;
    GPIO_Init(LCD_GPIOx_D3, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D4;
    GPIO_Init(LCD_GPIOx_D4, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D5;
    GPIO_Init(LCD_GPIOx_D5, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D6;
    GPIO_Init(LCD_GPIOx_D6, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D7;
    GPIO_Init(LCD_GPIOx_D7, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D8;
    GPIO_Init(LCD_GPIOx_D8, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D9;
    GPIO_Init(LCD_GPIOx_D9, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D10;
    GPIO_Init(LCD_GPIOx_D10, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D11;
    GPIO_Init(LCD_GPIOx_D11, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D12;
    GPIO_Init(LCD_GPIOx_D12, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D13;
    GPIO_Init(LCD_GPIOx_D13, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D14;
    GPIO_Init(LCD_GPIOx_D14, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_Pin_D15;
    GPIO_Init(LCD_GPIOx_D15, &GPIO_InitStructure);
}

/**
 * @brief: LCD FSMC初始化
 * @return {*}
 */
static void LCD_FSMC_Init(void)
{
    RCC_AHBPeriphClockCmd(LCD_RCC_FSMC, ENABLE);

    FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
    // 地址建立时间（ADDSET）为1个HCLK 2/72M=28ns
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0x01; // 地址建立时间
    // 数据保持时间（DATAST）+ 1个HCLK = 5/72M=70ns
    FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 0x04; // 数据建立时间
    // 选择控制的模式
    // 模式B,异步NOR FLASH模式，与ILI9341的8080时序匹配
    FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;
    /*以下配置与模式B无关*/
    // 地址保持时间（ADDHLD）模式B未用到
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0x00; // 地址保持时间
    // 设置总线转换周期，仅用于复用模式的NOR操作
    FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
    // 设置时钟分频，仅用于同步类型的存储器
    FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
    // 数据保持时间，仅用于同步型的NOR
    FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;

    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank_NORSRAMx;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /* 使能 FSMC_Bank1_NORSRAM4 */
    FSMC_NORSRAMCmd(FSMC_Bank_NORSRAMx, ENABLE);
}

/**
 * @brief: LCD 背光设置
 * @param {FunctionalState} enumState
 * @return {*}
 */
static void LCD_Backlight_Config(FunctionalState enumState)
{
    if (enumState)
        GPIO_ResetBits(LCD_GPIOx_BK, LCD_Pin_BK);
    else
        GPIO_SetBits(LCD_GPIOx_BK, LCD_Pin_BK);
}

/**
 * @brief: LCD 复位
 * @return {*}
 */
static void LCD_Rst(void)
{
    GPIO_ResetBits(LCD_GPIOx_RST, LCD_Pin_RST); // 低电平复位

    Delay_ms(50);

    GPIO_SetBits(LCD_GPIOx_RST, LCD_Pin_RST);

    Delay_ms(50);
}

/**
 * @brief: LCD 寄存器设置
 * @return {*}
 */
static void LCD_REG_Config(void)
{
    /*  Power control B (CFh)  */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0x81);
    LCD_WriteData(0x30);

    /*  Power on sequence control (EDh) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xED);
    LCD_WriteData(0x64);
    LCD_WriteData(0x03);
    LCD_WriteData(0x12);
    LCD_WriteData(0x81);

    /*  Driver timing control A (E8h) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xE8);
    LCD_WriteData(0x85);
    LCD_WriteData(0x10);
    LCD_WriteData(0x78);

    /*  Power control A (CBh) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xCB);
    LCD_WriteData(0x39);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x02);

    /* Pump ratio control (F7h) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xF7);
    LCD_WriteData(0x20);

    /* Driver timing control B */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xEA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xB1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1B);

    /*  Display Function Control (B6h) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xB6);
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);

    /* Power Control 1 (C0h) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x35);

    /* Power Control 2 (C1h) */
    // DEBUG_DELAY();
    LCD_WriteCmd(0xC1);
    LCD_WriteData(0x11);

    /* VCOM Control 1 (C5h) */
    LCD_WriteCmd(0xC5);
    LCD_WriteData(0x45);
    LCD_WriteData(0x45);

    /*  VCOM Control 2 (C7h)  */
    LCD_WriteCmd(0xC7);
    LCD_WriteData(0xA2);

    /* Enable 3G (F2h) */
    LCD_WriteCmd(0xF2);
    LCD_WriteData(0x00);

    /* Gamma Set (26h) */
    LCD_WriteCmd(0x26);
    LCD_WriteData(0x01);
    // DEBUG_DELAY();

    /* Positive Gamma Correction */
    LCD_WriteCmd(0xE0); // Set Gamma
    LCD_WriteData(0x0F);
    LCD_WriteData(0x26);
    LCD_WriteData(0x24);
    LCD_WriteData(0x0B);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x09);
    LCD_WriteData(0x54);
    LCD_WriteData(0xA8);
    LCD_WriteData(0x46);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x17);
    LCD_WriteData(0x09);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x07);
    LCD_WriteData(0x00);

    /* Negative Gamma Correction (E1h) */
    LCD_WriteCmd(0XE1); // Set Gamma
    LCD_WriteData(0x00);
    LCD_WriteData(0x19);
    LCD_WriteData(0x1B);
    LCD_WriteData(0x04);
    LCD_WriteData(0x10);
    LCD_WriteData(0x07);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x47);
    LCD_WriteData(0x39);
    LCD_WriteData(0x03);
    LCD_WriteData(0x06);
    LCD_WriteData(0x06);
    LCD_WriteData(0x30);
    LCD_WriteData(0x38);
    LCD_WriteData(0x0F);

    /* memory access control set */
    // DEBUG_DELAY();
    LCD_WriteCmd(0x36);
    LCD_WriteData(0xC8); /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
    // DEBUG_DELAY();

    /* column address control set */
    LCD_WriteCmd(LCD_SetCoordinateX);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xEF);

    /* page address control set */
    // DEBUG_DELAY();
    LCD_WriteCmd(LCD_SetCoordinateY);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3F);

    /*  Pixel Format Set (3Ah)  */
    // DEBUG_DELAY();
    LCD_WriteCmd(0x3a);
    LCD_WriteData(0x55);

    /* Sleep Out (11h)  */
    LCD_WriteCmd(0x11);
    Delay_ms(50);
    // DEBUG_DELAY();

    /* Display ON (29h) */
    LCD_WriteCmd(0x29);
}

/*
---------------------------------------------------------
模式0       模式1           模式2           模式3
o X0---->   |   		    |    		    |   <----3Y o
Y           |	    	 	|       		|	        X
0           |   		 	|               |	        3
|           |	    	 	|    		    |	        |
V           |           	|               |           V
---------------------------------------------------------
模式4           模式5           模式6           模式7
            |   A           |	        A   |
            |	|		    |	        |   |
            |	5		    |	        6   |
            |	X		    |	        Y   |
            |	o Y5---->   |	<----6X 0   |
---------------------------------------------------------
    LCD屏示例
|---------------|
|	  Logo   	|
|				|
|				|
|				|
|			    |
|				|
|				|
|---------------|
屏幕正面（宽240，高320）

竖向推荐    模式0(logo居上) 或 模式6(logo居下)
横向推荐    模式3(logo居左) 或 模式5(logo居右)
*/
/**
 * @brief: LCD 设置扫描模式
 * @param {uint8_t} Mode
 * @return {*}
 */
static void LCD_SetScanMode(uint8_t Mode)
{
    if (Mode > 7)
        return;

    LCD_SCAN_MODE = Mode;

    if (Mode % 2 == 0)
    {
        // 在0 2 4 6模式下 X方向像素宽度为240 Y方向像素宽度为320
        LCD_X_LENGTH = LCD_MIN_Pixel;
        LCD_Y_LENGTH = LCD_MAX_Pixel;
    }
    else
    {
        // 在1 3 5 7模式下 X方向像素宽度为320 Y方向像素宽度为240
        LCD_X_LENGTH = LCD_MAX_Pixel;
        LCD_Y_LENGTH = LCD_MIN_Pixel;
    }

    // 使用0x36命令参数的高3位设置GRAM扫描方向
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x08 | (Mode << 5));

    // 开窗
    LCD_WriteCmd(LCD_SetCoordinateX);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(((LCD_X_LENGTH - 1) >> 8) & 0xFF);
    LCD_WriteData((LCD_X_LENGTH - 1) & 0xFF);

    LCD_WriteCmd(LCD_SetCoordinateY);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(((LCD_Y_LENGTH - 1) >> 8) & 0xFF);
    LCD_WriteData((LCD_Y_LENGTH - 1) & 0xFF);

    LCD_WriteCmd(LCD_SetPixel);
}

/**
 * @brief: LCD 初始化
 * @return {*}
 */
void LCD_Init(void)
{
    LCD_GPIO_Init();
    LCD_FSMC_Init();

    LCD_Backlight_Config(ENABLE);
    LCD_Rst();
    LCD_REG_Config();
    LCD_SetScanMode(5);

    LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
}

/**
 * @brief: 在LCD屏幕上开辟一个窗口
 * @param {uint16_t} X  X坐标
 * @param {uint16_t} Y  Y坐标
 * @param {uint16_t} Width  宽度
 * @param {uint16_t} Height 高度
 * @return {*}
 */
static void LCD_OpenWindow(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{
    LCD_WriteCmd(LCD_SetCoordinateX);
    LCD_WriteData(X >> 8);
    LCD_WriteData(X & 0xFF);
    LCD_WriteData((X + Width - 1) >> 8);
    LCD_WriteData((X + Width - 1) & 0xFF);

    LCD_WriteCmd(LCD_SetCoordinateY);
    LCD_WriteData(Y >> 8);
    LCD_WriteData(Y & 0xFF);
    LCD_WriteData((Y + Height - 1) >> 8);
    LCD_WriteData((Y + Height - 1) & 0xFF);
}

/**
 * @brief: LCD 指定位置清屏
 * @param {uint16_t} X          X坐标
 * @param {uint16_t} Y          Y坐标
 * @param {uint16_t} Width      宽度
 * @param {uint16_t} Height     高度
 * @return {*}
 */
void LCD_Clear(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{
    LCD_OpenWindow(X, Y, Width, Height);
    LCD_FillColor(Width * Height, LCD_BackColor);
}

// 显示文本

/**
 * @brief: LCD 显示英文字符
 * @param {uint16_t} X      X坐标
 * @param {uint16_t} Y      Y坐标
 * @param {char} Ch         字符
 * @return {*}
 */
void LCD_ShowChar_EN(uint16_t X, uint16_t Y, const char Ch)
{
    uint8_t ByteCount, BitCount, FontLength;
    uint16_t RelativePositon;
    uint8_t *Font;

    RelativePositon = Ch - ' ';

    FontLength = (LCD_TextFont->Width * LCD_TextFont->Height) / 8;

    Font = (uint8_t *)&LCD_TextFont->table[RelativePositon * FontLength];

    LCD_OpenWindow(X, Y, LCD_TextFont->Width, LCD_TextFont->Height);

    LCD_WriteCmd(LCD_SetPixel);

    for (ByteCount = 0; ByteCount < FontLength; ByteCount++)
    {
        for (BitCount = 0; BitCount < 8; BitCount++)
        {
            if (Font[ByteCount] & (0x80 >> BitCount))
                LCD_WriteData(LCD_TextColor);
            else
                LCD_WriteData(LCD_BackColor);
        }
    }
}

/**
 * @brief: LCD 显示英文字符串
 * @param {uint16_t} X      X坐标
 * @param {uint16_t} Y      Y坐标
 * @param {char} *Str       字符串
 * @return {*}
 */
void LCD_ShowString_EN(uint16_t X, uint16_t Y, const char *Str)
{
    while (*Str != '\0')
    {
        if ((X - LCD_X_Start + LCD_TextFont->Width) > LCD_X_LENGTH)
        {
            X = LCD_X_Start;
            Y += LCD_TextFont->Height;
        }

        if ((Y - LCD_Y_Start + LCD_TextFont->Height) > LCD_Y_LENGTH)
        {
            X = LCD_X_Start;
            Y = LCD_Y_Start;
        }

        LCD_ShowChar_EN(X, Y, *Str++);
        X += LCD_TextFont->Width;
    }
}

/**
 * @brief: LCD 显示图片
 * @param {uint16_t} X          X坐标
 * @param {uint16_t} Y          Y坐标
 * @param {uint16_t} Width      宽度
 * @param {uint16_t} Height     高度
 * @param {uint16_t} *Image     图片
 * @return {*}
 */
void LCD_ShowImage(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, const uint8_t *Image)
{
    LCD_OpenWindow(X, Y, Width, Height);
    LCD_WriteCmd(LCD_SetPixel);

    for (uint16_t h = 0; h < Height; h++)
    {
        const uint16_t *row = (const uint16_t *)(Image + h * Width * 2);
        for (uint16_t w = 0; w < Width; w++)
        {
            LCD_WriteData(row[w]);
        }
    }
}

// LCD常用设置

/**
 * @brief: 设置英文字体类型
 * @param {FONT} *fonts     字体规格
 *      @arg  Font8x16
 * @return {*}
 */
void LCD_SetFont(FONT *fonts)
{
    LCD_TextFont = fonts;
}

/**
 * @brief: 获取当前字体类型
 * @return {FONT}   当前字体类型
 */
FONT *LCD_GetFont(void)
{
    return LCD_TextFont;
}

/**
 * @brief: 设置LCD文本颜色
 * @param {uint16_t} Color  文本颜色
 * @return {*}
 */
void LCD_SetTextColor(uint16_t Color)
{
    LCD_TextColor = Color;
}

/**
 * @brief: 获取LCD文本颜色
 * @param {uint16_t} *Color
 * @return {*}
 */
void LCD_GetTextColor(uint16_t *Color)
{
    *Color = LCD_TextColor;
}

/**
 * @brief: 设置LCD背景颜色
 * @param {uint16_t} Color  背景颜色
 * @return {*}
 */
void LCD_SetBackColor(uint16_t Color)
{
    LCD_BackColor = Color;
}

/**
 * @brief: 获取LCD背景颜色
 * @param {uint16_t} *Color
 * @return {*}
 */
void LCD_GetBackColor(uint16_t *Color)
{
    *Color = LCD_BackColor;
}
