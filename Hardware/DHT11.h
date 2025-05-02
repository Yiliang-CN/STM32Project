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
    float Humidity;    //  湿度

    uint8_t Temp_Int;  // 温度整数
    uint8_t Temp_Deci; // 温度小数
    float Temperature; // 温度

    uint8_t Check; // 校验

} DHT11_DATA; // DHT11数据结构

extern DHT11_DATA DHT11_Data;

uint8_t DHT11_GET_Data(void); // 获取数据

#endif
