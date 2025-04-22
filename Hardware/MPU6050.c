#include "stm32f10x.h" // Device header
#include "MPU6050.h"

static uint8_t MPU6050_ReadReg(uint8_t RegAddress);
static void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
static void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_Event);

/**
 * @brief: MPU6050 GPIO初始化
 * @return {*}
 */
static void MPU6050_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(MPU6050_RCC_GPIOx, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = MPU6050_Pin_SCL | MPU6050_Pin_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU6050_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief: MPU6050 I2C初始化
 * @return {*}
 */
static void MPU6050_I2C_Init(void)
{
    RCC_APB1PeriphClockCmd(MPU6050_RCC_I2Cx, ENABLE);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_ClockSpeed = MPU6050_I2C_ClockSpeed;
    I2C_InitStructure.I2C_DutyCycle = MPU6050_I2C_DutyCycle;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = MPU6050_I2C_AcknowledgedAddress;
    I2C_InitStructure.I2C_OwnAddress1 = MPU6050_I2C_OwnAddress1;
    I2C_Init(MPU6050_I2Cx, &I2C_InitStructure);

    I2C_Cmd(MPU6050_I2Cx, ENABLE);
}

/**
 * @brief: MPU6050初始化
 * @return: {*}
 */
void MPU6050_Init(void)
{
    MPU6050_GPIO_Init();
    MPU6050_I2C_Init();

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

/**
 * @brief: MPU6050获取ID号
 * @return: {uint8_t} MPU6050的ID号
 */
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
 * @brief: MPU6050获取数据
 * @param {int16_t} *AccX   加速度计X轴数据
 * @param {int16_t} *AccY   加速度计Y轴数据
 * @param {int16_t} *AccZ   加速度计Z轴数据
 * @param {int16_t} *GyroX  陀螺仪X轴数据
 * @param {int16_t} *GyroY  陀螺仪Y轴数据
 * @param {int16_t} *GyroZ  陀螺仪Z轴数据
 * @return: {*}
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;
}

/**
 * @brief: MPU6050读寄存器
 * @param {uint8_t} RegAddress  寄存器的地址
 * @return {uint8_t} Data       寄存器的数据
 */
static uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;

    I2C_GenerateSTART(MPU6050_I2Cx, ENABLE);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(MPU6050_I2Cx, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(MPU6050_I2Cx, RegAddress);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTART(MPU6050_I2Cx, ENABLE);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(MPU6050_I2Cx, MPU6050_ADDRESS, I2C_Direction_Receiver);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

    I2C_AcknowledgeConfig(MPU6050_I2Cx, DISABLE);
    I2C_GenerateSTOP(MPU6050_I2Cx, ENABLE);

    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);

    Data = I2C_ReceiveData(MPU6050_I2Cx);

    I2C_AcknowledgeConfig(MPU6050_I2Cx, ENABLE);

    return Data;
}

/**
 * @brief: MPU6050写入寄存器
 * @param {uint8_t} RegAddress  寄存器地址
 * @param {uint8_t} Data    要写入寄存器的数据
 * @return {*}
 */
static void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    I2C_GenerateSTART(MPU6050_I2Cx, ENABLE);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(MPU6050_I2Cx, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(MPU6050_I2Cx, RegAddress);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING);

    I2C_SendData(MPU6050_I2Cx, Data);
    MPU6050_WaitEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTOP(MPU6050_I2Cx, ENABLE);
}

/**
 * @brief: MPU6050等待事件
 * @param {I2C_TypeDef} *I2Cx
 * @param {uint32_t} I2C_Event
 * @return {*}
 */
static void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_Event)
{
    uint32_t Timeout = 0;
    Timeout = 10000;

    while (I2C_CheckEvent(I2Cx, I2C_Event) != SUCCESS)
    {
        Timeout--;
        if (Timeout == 0)
        {
            // 此处可以进行超时处理
            break;
        }
    }
}
