#ifndef __KEY_H
#define __KEY_H

// 按键中断模式 1开，0关
#define KEY_INTERRUPTS_MODE 0

#define Key_RCC_GPIOx RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC

#define Key1_GPIOx GPIOA // 按键一PA0
#define Key1_Pin GPIO_Pin_0

#define Key2_GPIOx GPIOC // 按键二PC13
#define Key2_Pin GPIO_Pin_13

void Key_Init(void); // 按键初始化

#if !KEY_INTERRUPTS_MODE

uint8_t Key1_GetState(void); // 按下1，松开0
uint8_t Key2_GetState(void);

#else

// 以下为中断配置
#define Key_RCC_AFIO RCC_APB2Periph_AFIO

#define key_EXTI_Linex EXTI_Line0 | EXTI_Line13  // EXTI中断线
#define key_PriorityGroup_x NVIC_PriorityGroup_2 // NVIC优先级分组

#define Key1_PortSrcGPIOx GPIO_PortSourceGPIOA // 按键一中断源
#define Key1_PinSrc GPIO_PinSource0            // 按键一中断引脚
#define Key1_EXTI_Line EXTI_Line0              // 按键一中断线
#define Key1_EXTI_IRQn EXTI0_IRQn              // 按键一中断号
#define Key1_PreemptionPriority 2              // 按键一抢占优先级
#define Key1_SubPriority 2                     // 按键一响应优先级

#define Key2_PortSrcGPIOx GPIO_PortSourceGPIOC // 按键二中断源
#define Key2_PinSrc GPIO_PinSource13           // 按键二中断引脚
#define Key2_EXTI_Line EXTI_Line13             // 按键二中断线
#define Key2_EXTI_IRQn EXTI15_10_IRQn          // 按键二中断号
#define Key2_PreemptionPriority 2              // 按键二抢占优先级
#define Key2_SubPriority 2                     // 按键二响应优先级

#endif

#endif
