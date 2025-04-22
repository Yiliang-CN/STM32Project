#ifndef __DMA_H
#define __DMA_H

#define DMA_RCC_DMAx RCC_AHBPeriph_DMA1

// DMA1_Channel1配置
#define DMA1_Channelx DMA1_Channel1
#define DMA1_PeripheralDataSize DMA_PeripheralDataSize_Byte
#define DMA1_MemoryDataSize DMA_MemoryDataSize_Byte
#define DMA1_DIR DMA_DIR_PeripheralSRC    // 外设到存储器
#define DMA1_M2M DMA_M2M_Enable           // 软件触发
#define DMA1_Priority DMA_Priority_Medium // 中优先级

void DMA1_Init_Normal(uint32_t SRC_Addr, uint32_t DST_Addr, uint32_t Size);
void DMA1_Start(void);

#endif
