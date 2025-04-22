#include "stm32f10x.h" // Device header
#include "W25Q64.h"

static uint8_t W25Q64_Swap_Byte(uint8_t WriteByte);
static void W25Q64_WriteEnable(void);
static void W25Q64_WaitBusy(void);

/**
 * @brief: W25Q64 GPIO 初始化
 * @return {*}
 */
static void W25Q64_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(W25Q64_RCC_GPIOx, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = W25Q64_Pin_CS;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25Q64_GPIOx, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = W25Q64_Pin_SCK | W25Q64_Pin_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25Q64_GPIOy, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = W25Q64_Pin_MISO;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25Q64_GPIOy, &GPIO_InitStructure);
}

/**
 * @brief: W25Q64 SPI 初始化
 * @return {*}
 */
static void W25Q64_SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(W25Q64_RCC_SPIx, ENABLE);

    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = W25Q64_SPI_BaudRatePrescalerx;
    SPI_InitStructure.SPI_CPOL = W25Q64_SPI_CPOLx;
    SPI_InitStructure.SPI_CPHA = W25Q64_SPI_CPHAx;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(W25Q64_SPIx, &SPI_InitStructure);

    SPI_Cmd(W25Q64_SPIx, ENABLE);
}

/**
 * @brief: W25Q64 初始化
 * @return {*}
 */
void W25Q64_Init(void)
{
    W25Q64_GPIO_Init();
    W25Q64_SPI_Init();

    W25Q64_Stop; // 初始输出高电平
}

/**
 * @brief: MPU6050获取ID号
 * @param {uint8_t} *MID    厂商ID号
 * @param {uint16_t} *DID   设备ID号
 * @return {*}
 */
void W25Q64_GetID(uint8_t *MID, uint16_t *DID)
{
    W25Q64_Start;
    W25Q64_Swap_Byte(W25Q64_JEDEC_ID); // 获取ID指令码
    *MID = W25Q64_Swap_Byte(W25Q64_DUMMY_BYTE);
    *DID = W25Q64_Swap_Byte(W25Q64_DUMMY_BYTE);
    *DID <<= 8;
    *DID |= W25Q64_Swap_Byte(W25Q64_DUMMY_BYTE);
    W25Q64_Stop;
}

/**
 * @brief: W25Q64页编程
 * @return {*}
 */
void W25Q64_PageProgram(uint32_t Address, uint8_t *Data, uint16_t Length)
{
    W25Q64_WriteEnable();

    W25Q64_Start;
    W25Q64_Swap_Byte(W25Q64_PAGE_PROGRAM);
    W25Q64_Swap_Byte(Address >> 16);
    W25Q64_Swap_Byte(Address >> 8);
    W25Q64_Swap_Byte(Address);
    for (uint16_t i = 0; i < Length; i++)
    {
        W25Q64_Swap_Byte(Data[i]);
    }
    W25Q64_Stop;

    W25Q64_WaitBusy();
}

/**
 * @brief: W25Q64扇区擦除 4KB
 * @return {*}
 */
void W25Q64_SectorErase(uint32_t Address)
{
    W25Q64_WriteEnable();

    W25Q64_Start;
    W25Q64_Swap_Byte(W25Q64_SECTOR_ERASE_4KB);
    W25Q64_Swap_Byte(Address >> 16);
    W25Q64_Swap_Byte(Address >> 8);
    W25Q64_Swap_Byte(Address);
    W25Q64_Stop;

    W25Q64_WaitBusy();
}

/**
 * @brief: W25Q64读取数据
 * @param {uint32_t} Address    数据的起始地址
 * @param {uint8_t} *Data       接收数据的数组
 * @param {uint32_t} Length     读取数据的长度
 * @return {*}
 */
void W25Q64_ReadData(uint32_t Address, uint8_t *Data, uint32_t Length)
{
    W25Q64_Start;
    W25Q64_Swap_Byte(W25Q64_READ_DATA);
    W25Q64_Swap_Byte(Address >> 16);
    W25Q64_Swap_Byte(Address >> 8);
    W25Q64_Swap_Byte(Address);
    for (uint32_t i = 0; i < Length; i++)
    {
        Data[i] = W25Q64_Swap_Byte(W25Q64_DUMMY_BYTE);
    }
    W25Q64_Stop;
}

/**
 * @brief: W25Q64交换传输一个字节
 * @param {uint8_t} WriteByte   要写入的一个字节
 * @return {uint8_t} 接收的一个字节
 */
static uint8_t W25Q64_Swap_Byte(uint8_t WriteByte)
{
    while (SPI_I2S_GetFlagStatus(W25Q64_SPIx, SPI_I2S_FLAG_TXE) != SET)
        ;

    SPI_I2S_SendData(W25Q64_SPIx, WriteByte);

    while (SPI_I2S_GetFlagStatus(W25Q64_SPIx, SPI_I2S_FLAG_RXNE) != SET)
        ;

    return SPI_I2S_ReceiveData(W25Q64_SPIx);
}

/**
 * @brief: W25Q64写使能
 * @return {*}
 */
static void W25Q64_WriteEnable(void)
{
    W25Q64_Start;
    W25Q64_Swap_Byte(W25Q64_WRITE_ENABLE);
    W25Q64_Stop;
}

/**
 * @brief: W25Q64等待忙
 * @return {*}
 */
static void W25Q64_WaitBusy(void)
{
    uint32_t Timeout = 100000;
    W25Q64_Start;
    W25Q64_Swap_Byte(W25Q64_READ_STATUS_REGISTER_1);
    while ((W25Q64_Swap_Byte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
    {
        Timeout--;
        if (Timeout == 0)
            break;
    }
    W25Q64_Stop;
}
