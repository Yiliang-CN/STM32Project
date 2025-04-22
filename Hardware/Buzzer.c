#include "stm32f10x.h" // Device header
#include "Buzzer.h"

// 蜂鸣器PWM模式 0关1开
#if !BUZZER_PWM_MODE

/**
 * @brief: 蜂鸣器GPIO初始化
 * @return {*}
 */
static void Buzzer_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(Buzzer_RCC_GPIOx, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Buzzer_GPIOx, &GPIO_InitStructure);

	GPIO_ResetBits(Buzzer_GPIOx, Buzzer_Pin);
}

#else

/**
 * @brief: 蜂鸣器PWM初始化
 * @return: {*}
 */
static void Buzzer_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(Buzzer_RCC_GPIOx | Buzzer_RCC_TIMx, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Buzzer_GPIOx, &GPIO_InitStructure);

	GPIO_ResetBits(Buzzer_GPIOx, Buzzer_Pin);

	// TIM_InternalClockConfig(Buzzer_TIMx);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数0~65535
	TIM_TimeBaseInitStructure.TIM_Period = Buzzer_Period;			// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = Buzzer_Prescaler;		// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 高级定时器才有，用不上就置0
	TIM_TimeBaseInit(Buzzer_TIMx, &TIM_TimeBaseInitStructure);

	TIM_CtrlPWMOutputs(Buzzer_TIMx, ENABLE); // 高级定时器专属——MOE主输出使能

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // 先默认初始化一遍结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Buzzer_Pulse; // CCR

	// 根据需求设置对应通道
	TIM_OC1Init(Buzzer_TIMx, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(Buzzer_TIMx, TIM_OCPreload_Enable);

	TIM_Cmd(Buzzer_TIMx, ENABLE);
}

#endif

/**
 * @brief: Buzzer初始化
 * @return {*}
 */
void Buzzer_Init(void)
{
#if !BUZZER_PWM_MODE

	Buzzer_GPIO_Init();

#else

	Buzzer_PWM_Init();

#endif
}

#if !BUZZER_PWM_MODE

/**
 * @brief: 蜂鸣器电平翻转
 * @return: {*}
 */
void Buzzer_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Buzzer_GPIOx, Buzzer_Pin) == 1)
		GPIO_ResetBits(Buzzer_GPIOx, Buzzer_Pin);
	else
		GPIO_SetBits(Buzzer_GPIOx, Buzzer_Pin);
}

/**
 * @brief: 获取蜂鸣器状态
 * @return: {uint8_t} 1开 0关
 */
uint8_t Buzzer_GetState(void)
{
	if (GPIO_ReadOutputDataBit(Buzzer_GPIOx, Buzzer_Pin) == 1)
		return 1;
	else
		return 0;
}

#else

/**
 * @brief: 设置PWM占空比
 * @param {uint8_t} Compare 范围0-100
 * @return: {*}
 */
void Buzzer_SetPWMState(uint8_t Compare)
{
	if (Compare > (Buzzer_Period + 1))
		Compare = (Buzzer_Period + 1);

	// 根据需求设置对应通道
	TIM_SetCompare1(Buzzer_TIMx, Compare);
}

#endif
