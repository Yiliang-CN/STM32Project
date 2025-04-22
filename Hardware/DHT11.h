#ifndef __DHT11_H
#define __DHT11_H

// DHT11使用时不需要调用初始化函数，直接调用获取数据函数即可

#define DHT11_RCC_GPIOx RCC_APB2Periph_GPIOE

#define DHT11_GPIOx GPIOE
#define DHT11_Pin GPIO_Pin_6 // PE6

#define DHT11_High GPIO_SetBits(DHT11_GPIOx, DHT11_Pin)
#define DHT11_Low GPIO_ResetBits(DHT11_GPIOx, DHT11_Pin)
#define DHT11_Read GPIO_ReadInputDataBit(DHT11_GPIOx, DHT11_Pin)

typedef struct
{
    uint8_t Humi_Int;  // 湿度整数
    uint8_t Humi_Deci; // 湿度小数
    uint8_t Temp_Int;  // 温度整数
    uint8_t Temp_Deci; // 温度小数
    uint8_t Check;     // 校验
} DHT11_DATA;          // DHT11数据结构

extern DHT11_DATA DHT11_Data;

// static void DHT11_Init_OUT(void);    // 初始化引脚为输出
// static void DHT11_Init_IN(void);     // 初始化引脚为输入
// static void DHT11_Start(void);       // 主机发送启动信号
// static uint8_t DHT11_GET_Byte(void); // 获取一个字节

uint8_t DHT11_GET_Data(void); // 获取数据

#endif
