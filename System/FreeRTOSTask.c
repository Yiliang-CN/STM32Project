#include "stm32f10x.h" // Device header
#include "FreeRTOSTask.h"
#include "Serial.h"
#include "Key.h"
#include "LED.h"
#include "Buzzer.h"
#include "MPU6050.h"
#include "RD03.h"

#if isUseTask

/**
 * @brief: LED灯任务
 * @param {void} *pvParameters
 * @return {*}
 */
void LED_Task(void *pvParameters)
{
    while (1)
    {
        if (Key1_State() == 1)
        {
            LEDG_Turn();
            if (LEDG_State() == 1)
                Serial_Printf("LEDG is ON\r\n");
            else
                Serial_Printf("LEDG is OFF\r\n");
        }
        vTaskDelay(100);
    }
}

/**
 * @brief: 蜂鸣器任务
 * @param {void} *pvParameters
 * @return {*}
 */
void Buzzer_Task(void *pvParameters)
{
    while (1)
    {
        if (Key2_State() == 1)
        {
            Buzzer_Turn();
            if (Buzzer_State() == 1)
                Serial_Printf("LEDG is ON\r\n");
            else
                Serial_Printf("LEDG is OFF\r\n");
        }
        vTaskDelay(100);
    }
}

/**
 * @brief: RD03任务
 * @param {void} *pvParameters
 * @return {*}
 */
void RD03_Task(void *pvParameters)
{
    while (1)
    {
        if (RD03_GetRxFlag() != 0)
            Serial_Printf("Range: %d\n", RD03_GetRange());
    }
}

/**
 * @brief: MPU6050任务
 * @param {void} *pvParameters
 * @return {*}
 */
void MPU6050_Task(void *pvParameters)
{
    // uint8_t MPU6050_ID;
    int16_t AX, AY, AZ, GX, GY, GZ;

    while (1)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        Serial_Printf("\n%d, %d\n %d, %d\n %d, %d\n", AX, GX, AY, GY, AZ, GZ);

        vTaskDelay(1000); // 延时1s
    }
}

#endif
