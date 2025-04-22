#ifndef __BUZZER_H
#define __BUZZER_H

// 蜂鸣器PWM模式 0关1开
#define BUZZER_PWM_MODE 0

#define Buzzer_RCC_GPIOx RCC_APB2Periph_GPIOA

#define Buzzer_GPIOx GPIOA
#define Buzzer_Pin GPIO_Pin_8 // PA8

void Buzzer_Init(void);

#if !BUZZER_PWM_MODE

#define Buzzer_ON GPIO_SetBits(Buzzer_GPIOx, Buzzer_Pin)
#define Buzzer_OFF GPIO_ResetBits(Buzzer_GPIOx, Buzzer_Pin)

void Buzzer_Turn(void);        // 电平翻转
uint8_t Buzzer_GetState(void); // 获取当前状态

#else

// PWM频率: Freq = CK_PSC/(PSC+1)/(ARR+1)   CK_PSC = APB2时钟频率
// PWM占空比: Duty = CCR/(ARR+1)
// PWM分辨率: Reso = 1/(ARR+1)

#define Buzzer_RCC_TIMx RCC_APB2Periph_TIM1
#define Buzzer_TIMx TIM1
#define Buzzer_Period 100 - 1    // ARR
#define Buzzer_Prescaler 720 - 1 // PSC
#define Buzzer_Pulse 0           // CCR

void Buzzer_SetPWMState(uint8_t Compare); // 设置占空比 Compare范围0~100

#endif

#endif
