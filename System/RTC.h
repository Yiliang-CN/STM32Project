#ifndef __RTC_H
#define __RTC_H

#define RTC_RCC_PWR RCC_APB1Periph_PWR
#define RTC_RCC_BKP RCC_APB1Periph_BKP

#define RTC_Prescaler_Value 32768 - 1

// 年 月 日 时 分 秒
extern uint16_t RTC_Time[];

void RTC_Init(void);
void RTC_SetTime(void);
void RTC_GetTime(void);

#endif
