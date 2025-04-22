#ifndef __RD03_H
#define __RD03_H

#define RD03_RCC_GPIOx RCC_APB2Periph_GPIOA
#define RD03_RCC_USARTx RCC_APB1Periph_USART2
#define RD03_RCC_DMAx RCC_AHBPeriph_DMA1

#define RD03_GPIOx GPIOA
#define RD03_Pin_TX GPIO_Pin_2 // PA2
#define RD03_Pin_RX GPIO_Pin_3 // PA3

#define RD03_BaudRate 115200 // 波特率

#define RD03_USARTx USART2

// USART2 NVIC 配置
#define RD03_USART_IRQn USART2_IRQn
#define RD03_USART_PreemptionPriority 8
#define RD03_USART_SubPriority 1

// DMA1_Channel6 USART2_RX 配置
#define RD03_DMA1_Channelx DMA1_Channel6
#define RD03_DMA1_PeripheralDataSize DMA_PeripheralDataSize_Byte
#define RD03_DMA1_MemoryDataSize DMA_MemoryDataSize_Byte
#define RD03_DMA1_DIR DMA_DIR_PeripheralSRC  // 外设到存储器
#define RD03_DMA1_M2M DMA_M2M_Disable        // 硬件触发
#define RD03_DMA1_Priority DMA_Priority_High // 高优先级

#define RD03_USART_RxBUF 16

extern uint8_t RD03_RxBuf[];

/*
    获取距离前先调用RD03_GetRxFlag()判断是否有数据
    然后再调用RD03_GetRange()获取距离
 */

// static void RD03_USART_Init(void);
// static void RD03_DMA1_Init(void);
// static void RD03_ResetDMA(void);

void RD03_Init(void);
uint8_t RD03_GetRxFlag(void);
int RD03_GetRange(void);

#endif
