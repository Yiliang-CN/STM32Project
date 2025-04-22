#ifndef __W25Q64_H
#define __W25Q64_H

// Instruction
#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_PWR_MGMT_2 0x6C
#define MPU6050_WHO_AM_I 0x75

// I2C 参数配置
#define MPU6050_RCC_GPIOx RCC_APB2Periph_GPIOB
#define MPU6050_RCC_I2Cx RCC_APB1Periph_I2C1

#define MPU6050_GPIOx GPIOB
#define MPU6050_Pin_SCL GPIO_Pin_6 // PB6
#define MPU6050_Pin_SDA GPIO_Pin_7 // PB7

// I2C1 配置
#define MPU6050_I2Cx I2C1
#define MPU6050_I2C_ClockSpeed 100000         // 100KHz~400KHz
#define MPU6050_I2C_DutyCycle I2C_DutyCycle_2 // 速度在100KHz以下使用2:1 大于100KHz建议使用16:9
#define MPU6050_I2C_AcknowledgedAddress I2C_AcknowledgedAddress_7bit
#define MPU6050_I2C_OwnAddress1 0x00 // 自身地址 0x00~0x7F

#define MPU6050_ADDRESS 0xD0

// static uint8_t MPU6050_ReadReg(uint8_t RegAddress);
// static void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
// static void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_Event);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif
