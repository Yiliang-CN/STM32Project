#include "stm32f10x.h" // Device header
#include "RD03.h"
#include <string.h>
#include <stdlib.h>

uint8_t RD03_RxBuf[RD03_USART_RxBUF];
uint8_t RD03_RxFlag;
uint8_t RD03_RxIndex;
uint8_t RD03_Range;

/**
 * @brief: RD03 GPIO 初始化
 * @return {*}
 */
static void RD03_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RD03_RCC_GPIOx, ENABLE);

    // USART2_TX PA2
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = RD03_Pin_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RD03_GPIOx, &GPIO_InitStructure);

    // USART2_RX PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_InitStructure.GPIO_Pin = RD03_Pin_RX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RD03_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief: RD03 USART 初始化
 * @return {*}
 */
static void RD03_USART_Init(void)
{
    RCC_APB1PeriphClockCmd(RD03_RCC_USARTx, ENABLE);

    // USART2 初始化
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = RD03_BaudRate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(RD03_USARTx, &USART_InitStructure);

    // USART2 NVIC 配置
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RD03_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RD03_USART_PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = RD03_USART_SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(RD03_USARTx, USART_IT_IDLE, ENABLE); // 使能串口空闲中断

    USART_DMACmd(RD03_USARTx, USART_DMAReq_Rx, ENABLE);

    USART_Cmd(RD03_USARTx, ENABLE); // 使能 USART 工作
}

/**
 * @brief: RD03 DMA 初始化
 * @return {*}
 */
static void RD03_DMA_Init(void)
{
    RCC_AHBPeriphClockCmd(RD03_RCC_DMAx, ENABLE);

    // DMA_USART2_RX  USART2->RAM
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&RD03_USARTx->DR); // 外设地址
    DMA_InitStructure.DMA_PeripheralDataSize = RD03_DMA1_PeripheralDataSize; // 外设数据宽度一个字节
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         // 外设地址不自增

    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RD03_RxBuf;     // 存储器地址
    DMA_InitStructure.DMA_MemoryDataSize = RD03_DMA1_MemoryDataSize; // 存储器数据宽度一个字节
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // 存储器地址自增

    DMA_InitStructure.DMA_DIR = RD03_DMA1_DIR;           // 外设到存储器
    DMA_InitStructure.DMA_BufferSize = RD03_USART_RxBUF; // 缓冲区大小 数据传输量
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;        // 正常模式
    DMA_InitStructure.DMA_M2M = RD03_DMA1_M2M;           // 硬件触发
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // 高优先级
    DMA_Init(RD03_DMA1_Channelx, &DMA_InitStructure);

    DMA_Cmd(RD03_DMA1_Channelx, ENABLE); // 使能 USART2_RX DMA
}

/**
 * @brief: RD03初始化
 * @return {*}
 */
void RD03_Init(void)
{
    RD03_GPIO_Init();
    RD03_USART_Init();
    RD03_DMA_Init();
}

/**
 * @brief: 获取接收标志位
 * @return {uint8_t} 1成功 0失败
 */
uint8_t RD03_GetRxFlag(void)
{
    if (RD03_RxFlag == 1)
    {
        RD03_RxFlag = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief: RD03重置DMA
 * @return {*}
 */
static void RD03_ResetDMA(void)
{
    memset(RD03_RxBuf, 0, sizeof(RD03_RxBuf));

    // 重新装填传输计数器
    DMA_Cmd(RD03_DMA1_Channelx, DISABLE); // 使能 USART2_RX DMA
    RD03_DMA1_Channelx->CNDTR = RD03_USART_RxBUF;
    DMA_Cmd(RD03_DMA1_Channelx, ENABLE); // 使能 USART2_RX DMA
}

/**
 * @brief: 获取测距结果
 * @return {int} 成功返回距离 失败-1
 */
int RD03_GetRange(void)
{
    const char result0[] = "ON";
    const char result1[] = "OFF";
    const char result2[] = "Range ";

    if (RD03_RxIndex < sizeof(result0) || strcmp((char *)RD03_RxBuf, result1) == 0)
        goto getFail;

    if (strstr((char *)RD03_RxBuf, result0) != NULL)
    {
        char *temp = strstr((char *)RD03_RxBuf, result2);
        if (temp != NULL)
        {
            RD03_Range = atoi(temp + strlen(result2));

            RD03_ResetDMA();

            return RD03_Range;
        }
    }

getFail:
    RD03_ResetDMA();

    return -1;
}

void USART2_IRQHandler(void)
{
    volatile uint8_t flag;
    if (USART_GetITStatus(RD03_USARTx, USART_IT_IDLE) != RESET) // 串口空闲中断
    {
        RD03_RxIndex = RD03_USART_RxBUF - DMA_GetCurrDataCounter(RD03_DMA1_Channelx);
        RD03_RxFlag = 1; // 接收数据完成标志

        // 软件清除空闲中断标志位 先读SR 再读DR
        flag = RD03_USARTx->SR;
        flag = RD03_USARTx->DR;
    }
}
