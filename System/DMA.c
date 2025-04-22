#include "stm32f10x.h" // Device header
#include "DMA.h"

uint16_t DMA1_Size;

void DMA1_Init_Normal(uint32_t SRC_Addr, uint32_t DST_Addr, uint32_t Size)
{
    DMA1_Size = Size;

    RCC_AHBPeriphClockCmd(DMA_RCC_DMAx, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = SRC_Addr;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA1_PeripheralDataSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryBaseAddr = DST_Addr;
    DMA_InitStructure.DMA_MemoryDataSize = DMA1_MemoryDataSize;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_DIR = DMA1_DIR;
    DMA_InitStructure.DMA_BufferSize = Size;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA1_M2M;
    DMA_InitStructure.DMA_Priority = DMA1_Priority;
    DMA_Init(DMA1_Channelx, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channelx, DISABLE);
}

void DMA1_Start(void)
{
    // 传输计数器重新赋值时必须要先关闭DMA 否则会出错
    DMA_Cmd(DMA1_Channelx, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channelx, DMA1_Size);
    DMA_Cmd(DMA1_Channelx, ENABLE);

    // 等待传输完成 并清除标志位
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET)
        ;
    DMA_ClearFlag(DMA1_FLAG_TC1);
}
