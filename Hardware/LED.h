#ifndef __LED_H
#define __LED_H

// LEDPWM调光模式 0关1开
#define LED_PWM_MODE 0

#define LED_RCC_GPIOx RCC_APB2Periph_GPIOB

// G:PB0    B:PB1    R:PB5
#define LED_GPIOx GPIOB
#define LEDG_Pin GPIO_Pin_0 // PB0
#define LEDB_Pin GPIO_Pin_1 // PB1
#define LEDR_Pin GPIO_Pin_5 // PB5

void LED_Init(void);

#if !LED_PWM_MODE

#define LEDG_ON GPIO_ResetBits(LED_GPIOx, LEDG_Pin)
#define LEDB_ON GPIO_ResetBits(LED_GPIOx, LEDB_Pin)
#define LEDR_ON GPIO_ResetBits(LED_GPIOx, LEDR_Pin)

#define LEDG_OFF GPIO_SetBits(LED_GPIOx, LEDG_Pin)
#define LEDB_OFF GPIO_SetBits(LED_GPIOx, LEDB_Pin)
#define LEDR_OFF GPIO_SetBits(LED_GPIOx, LEDR_Pin)

void LEDG_Turn(void); // 电平翻转
void LEDB_Turn(void);
void LEDR_Turn(void);
uint8_t LEDG_GetState(void); // 获取状态
uint8_t LEDB_GetState(void);
uint8_t LEDR_GetState(void);

#else

// PWM频率: Freq = CK_PSC/(PSC+1)/(ARR+1)   CK_PSC = APB2时钟频率
// PWM占空比: Duty = CCR/(ARR+1)
// PWM分辨率: Reso = 1/(ARR+1)

#define LED_RCC_TIMx RCC_APB1Periph_TIM3
#define LED_RCC_AFIO RCC_APB2Periph_AFIO
#define LED_TIMx TIM3
#define LED_Period 100 - 1    // ARR
#define LED_Prescaler 720 - 1 // PSC
#define LED_Pulse 0           // CCR

void LED_SetPWMState(uint8_t R, uint8_t G, uint8_t B);

#endif

#endif
