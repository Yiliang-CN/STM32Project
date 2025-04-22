#ifndef __W25Q64_H
#define __W25Q64_H

// Instruction
#define W25Q64_WRITE_ENABLE 0x06
#define W25Q64_WRITE_DISABLE 0x04
#define W25Q64_READ_STATUS_REGISTER_1 0x05
#define W25Q64_READ_STATUS_REGISTER_2 0x35
#define W25Q64_WRITE_STATUS_REGISTER 0x01
#define W25Q64_PAGE_PROGRAM 0x02
#define W25Q64_QUAD_PAGE_PROGRAM 0x32
#define W25Q64_BLOCK_ERASE_64KB 0xD8
#define W25Q64_BLOCK_ERASE_32KB 0x52
#define W25Q64_SECTOR_ERASE_4KB 0x20
#define W25Q64_CHIP_ERASE 0xC7
#define W25Q64_ERASE_SUSPEND 0x75
#define W25Q64_ERASE_RESUME 0x7A
#define W25Q64_POWER_DOWN 0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE 0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET 0xFF
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID 0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID 0x90
#define W25Q64_READ_UNIQUE_ID 0x4B
#define W25Q64_JEDEC_ID 0x9F
#define W25Q64_READ_DATA 0x03
#define W25Q64_FAST_READ 0x0B
#define W25Q64_FAST_READ_DUAL_OUTPUT 0x3B
#define W25Q64_FAST_READ_DUAL_IO 0xBB
#define W25Q64_FAST_READ_QUAD_OUTPUT 0x6B
#define W25Q64_FAST_READ_QUAD_IO 0xEB
#define W25Q64_OCTAL_WORD_READ_QUAD_IO 0xE3

#define W25Q64_DUMMY_BYTE 0xFF // 接收数据时用作参数的无用数据

// SPI 参数配置
#define W25Q64_RCC_GPIOx RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA
#define W25Q64_RCC_SPIx RCC_APB2Periph_SPI1

#define W25Q64_GPIOx GPIOC
#define W25Q64_Pin_CS GPIO_Pin_0 // PC0

#define W25Q64_GPIOy GPIOA
#define W25Q64_Pin_SCK GPIO_Pin_5  // PA5
#define W25Q64_Pin_MISO GPIO_Pin_6 // PA6
#define W25Q64_Pin_MOSI GPIO_Pin_7 // PA7

// SPI1 模式0
#define W25Q64_SPIx SPI1
#define W25Q64_SPI_BaudRatePrescalerx SPI_BaudRatePrescaler_128
#define W25Q64_SPI_CPOLx SPI_CPOL_Low
#define W25Q64_SPI_CPHAx SPI_CPHA_1Edge

#define W25Q64_Start GPIO_ResetBits(W25Q64_GPIOx, W25Q64_Pin_CS);
#define W25Q64_Stop GPIO_SetBits(W25Q64_GPIOx, W25Q64_Pin_CS);

// static uint8_t W25Q64_WriteRead_Byte(uint8_t WriteByte);
// static void W25Q64_WriteEnable(void);
// static void W25Q64_WaitBusy(void);

void W25Q64_Init(void);
void W25Q64_GetID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *Data, uint16_t Length);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *Data, uint32_t Length);

#endif
