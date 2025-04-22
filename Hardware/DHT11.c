#include "stm32f10x.h" // Device header
#include "DHT11.h"
#include "Delay.h"

DHT11_DATA DHT11_Data;

/**
 * @brief: 初始化为输出模式
 * @return: {*}
 */
static void DHT11_Init_OUT(void)
{
    RCC_APB2PeriphClockCmd(DHT11_RCC_GPIOx, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief: 初始化为输入模式
 * @return: {*}
 */
static void DHT11_Init_IN(void)
{
    RCC_APB2PeriphClockCmd(DHT11_RCC_GPIOx, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief: 发送开始信号
 * @return: {*}
 */
static void DHT11_Start(void)
{
    DHT11_Init_OUT(); // 输出模式，发送信号

    DHT11_High; // 主机初始先拉高
    Delay_us(30);

    DHT11_Low; // 主机拉低20ms
    Delay_ms(20);

    DHT11_High; // 主机拉高30us
    Delay_us(30);

    DHT11_Init_IN(); // 输入模式，接收数据
}

/**
 * @brief: 获取一个字节
 * @return {uint8_t} data   获取到的字节
 */
static uint8_t DHT11_GET_Byte(void)
{
    uint8_t data;

    for (uint8_t i = 0; i < 8; i++)
    {
        while (DHT11_Read == 0)
            ;
        Delay_us(30); // 延迟30us用于区别数据0和1，0只有26~28us，1有70us

        data <<= 1; // 左移一位

        if (DHT11_Read == 1)
            data |= 1; // 30us过后，如果还能读取到1，则说明数据为1

        while (DHT11_Read == 1)
            ; // 确保信号结束后再进行下一轮
    }

    return data;
}

/**
 * @brief: 获取测量数据
 * @return: {uint8_t} 1成功 0失败
 */
uint8_t DHT11_GET_Data(void)
{
    uint8_t Temp_Int, Temp_Deci, Humi_Int, Humi_Deci, Check;

    DHT11_Start(); // 主机发送开始信号

    if (DHT11_Read == 0)
    {
        while (DHT11_Read == 0) // 等待从机拉低
            ;
        while (DHT11_Read == 1) // 等待从机拉高
            ;

        Humi_Int = DHT11_GET_Byte();  // 湿度整数
        Humi_Deci = DHT11_GET_Byte(); // 湿度小数
        Temp_Int = DHT11_GET_Byte();  // 温度整数
        Temp_Deci = DHT11_GET_Byte(); // 温度小数
        Check = DHT11_GET_Byte();     // 校验值

        DHT11_Low; // 当最后一bit数据传送完毕后，DHT11拉低总线至少50us
        Delay_us(55);
        DHT11_High; // 随后总线由上拉电阻拉高进入空闲状态

        if (Humi_Int + Humi_Deci + Temp_Int + Temp_Deci == Check)
        {
            DHT11_Data.Humi_Int = Humi_Int;
            DHT11_Data.Humi_Deci = Humi_Deci;
            DHT11_Data.Temp_Int = Temp_Int;
            DHT11_Data.Temp_Deci = Temp_Deci;
            return 1;
        }
    }

    return 0;
}
