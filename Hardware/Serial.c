#include "stm32f10x.h" // Device header
#include "Serial.h"

char Serial_TxTextPacket[100];
char Serial_RxTextPacket[100];
uint8_t Serial_TxHexPacket[4];
uint8_t Serial_RxHexPacket[4];
uint8_t Serial_RxFlag;
uint8_t Serial_Mode_Flag;

/**
 * @brief: 串口GPIO初始化
 * @return {*}
 */
static void Serial_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(Serial_RCC_GPIOx, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = Serial_Pin_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Serial_GPIOx, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = Serial_Pin_RX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Serial_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief: 串口USART初始化
 * @return {*}
 */
static void Serial_USART_Init(void)
{
    RCC_APB2PeriphClockCmd(Serial_RCC_USARTx, ENABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = Serial_BaudRate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(Serial_USARTx, &USART_InitStructure);

    USART_ITConfig(Serial_USARTx, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(Serial_PriorityGroup_x);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = Serial_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Serial_IRQChannelPreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = Serial_IRQChannelSubPriority;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(Serial_USARTx, ENABLE);
}

void Serial_Init(void)
{
    Serial_GPIO_Init();
    Serial_USART_Init();
}

/**
 * @brief: 重定向底层函数printf
 * @param {int} ch
 * @param {FILE} *f
 * @return {*}
 */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

/**
 * @brief: 串口发送一个字节
 * @param {uint8_t} Byte    要发送的字节
 * @return {*}
 */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(Serial_USARTx, Byte);
    while (USART_GetFlagStatus(Serial_USARTx, USART_FLAG_TXE) == RESET)
        ;
}

/**
 * @brief: 串口发送一个数组
 * @param {uint8_t} *Array  数组的首地址
 * @param {uint8_t} Length  数组的长度
 * @return {*}
 */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    for (uint16_t i = 0; i < Length; i++)
        Serial_SendByte(Array[i]);
}

/**
 * @brief: 串口发送一个字符串
 * @param {char} *String    字符串的首地址
 * @return {*}
 */
void Serial_SendString(char *String)
{
    for (uint16_t i = 0; String[i] != '\0'; i++)
        Serial_SendByte(String[i]);
}

/**
 * @brief: 次方函数
 * @param {uint32_t} X  底数
 * @param {uint32_t} Y  次方
 * @return {uint32_t} Result    X的Y次方
 */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
        Result *= X;

    return Result;
}

/**
 * @brief: 串口发送数字
 * @param {uint32_t} Number     数字
 * @param {uint8_t} Length      数字的位数
 * @return {*}
 */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    for (uint8_t i = 0; i < Length; i++)
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
}

/**
 * @brief: 自己封装的prinf函数
 * @param {char} *format
 * @return {*}
 */
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

/**
 * @brief: 获取串口接收标志位
 * @return {uint8_t} 1有 0无
 */
uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

// 中断处理函数
void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t RxPacketNum = 0;

    if (USART_GetITStatus(Serial_USARTx, USART_IT_RXNE) != RESET)
    {
        uint8_t RxData = USART_ReceiveData(Serial_USARTx);

        if (RxState == 0)
        {
            if (RxData == 0xFF) // Hex模式
            {
                Serial_Mode_Flag = 0;
                RxState = 1;
                RxPacketNum = 0;
            }
            else if (RxData == '@') // Text模式
            {
                Serial_Mode_Flag = 1;
                RxState = 1;
                RxPacketNum = 0;
            }
        }
        else if (RxState == 1)
        {
            if (Serial_Mode_Flag == 0) // Hex模式
            {
                Serial_RxHexPacket[RxPacketNum] = RxData;
                RxPacketNum++;
                if (RxPacketNum >= 4)
                    RxState = 2;
            }
            else if (Serial_Mode_Flag == 1) // Text模式
            {
                if (RxData == '\r')
                    RxState = 2;
                else
                {
                    Serial_RxTextPacket[RxPacketNum] = RxData;
                    RxPacketNum++;
                }
            }
        }
        else if (RxState == 2)
        {
            if (Serial_Mode_Flag == 0) // Hex模式
            {
                if (RxData == 0xFE)
                {
                    RxState = 0;
                    Serial_RxFlag = 1;
                }
            }
            else if (Serial_Mode_Flag == 1) // Text模式
            {
                if (RxData == '\n')
                {
                    RxState = 0;
                    Serial_RxTextPacket[RxPacketNum] = '\0';
                    Serial_RxFlag = 1;
                }
            }
        }

        USART_ClearITPendingBit(Serial_USARTx, USART_IT_RXNE);
    }
}
