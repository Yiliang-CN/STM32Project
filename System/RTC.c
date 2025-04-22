#include "stm32f10x.h" // Device header
#include "RTC.h"
#include <time.h>

uint16_t RTC_Time[] = {2025, 1, 1, 8, 0, 0};

/**
 * @brief: RTC时钟初始化
 * @return {*}
 */
void RTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RTC_RCC_PWR | RTC_RCC_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);

    if (BKP_ReadBackupRegister(BKP_DR1) != 0x2004)
    {
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET)
            ;

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);

        RTC_WaitForSynchro();
        RTC_WaitForLastTask();

        RTC_SetPrescaler(RTC_Prescaler_Value);
        RTC_WaitForLastTask();

        RTC_SetTime();

        BKP_WriteBackupRegister(BKP_DR1, 0x2004);
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
    }
}

/**
 * @brief: RTC时钟根据RTC_Time中的数据设置实时时钟的时间
 * @return {*}
 */
void RTC_SetTime(void)
{
    time_t time_cnt;
    struct tm time_data;

    time_data.tm_year = RTC_Time[0] - 1900;
    time_data.tm_mon = RTC_Time[1] - 1;
    time_data.tm_mday = RTC_Time[2];
    time_data.tm_hour = RTC_Time[3];
    time_data.tm_min = RTC_Time[4];
    time_data.tm_sec = RTC_Time[5];

    time_cnt = mktime(&time_data) - 8 * 60 * 60;

    RTC_SetCounter(time_cnt);
    RTC_WaitForLastTask();
}

/**
 * @brief: RTC时钟获取当前时间并转化为本地时间并存储到RTC_Time中
 * @return {*}
 */
void RTC_GetTime(void)
{
    time_t time_cnt;
    struct tm time_data;

    time_cnt = RTC_GetCounter() + 8 * 60 * 60;

    time_data = *localtime(&time_cnt);

    RTC_Time[0] = time_data.tm_year + 1900;
    RTC_Time[1] = time_data.tm_mon + 1;
    RTC_Time[2] = time_data.tm_mday;
    RTC_Time[3] = time_data.tm_hour;
    RTC_Time[4] = time_data.tm_min;
    RTC_Time[5] = time_data.tm_sec;
}
