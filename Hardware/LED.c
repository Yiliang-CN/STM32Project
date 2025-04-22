#include "stm32f10x.h" // Device header
#include "LED.h"

// LEDPWM调光模式 0关1开
#if !LED_PWM_MODE

/**
 * @brief: LED RGB灯GPIO初始化
 * @return: {*}
 */
static void LED_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(LED_RCC_GPIOx, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LEDG_Pin | LEDB_Pin | LEDR_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIOx, &GPIO_InitStructure);

	// 设置初始状态
	GPIO_SetBits(LED_GPIOx, LEDG_Pin | LEDB_Pin | LEDR_Pin);
}

#else

/**
 * @brief: LED RGB灯PWM调光初始化
 * @return: {*}
 */
static void LED_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(LED_RCC_GPIOx | LED_RCC_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(LED_RCC_TIMx, ENABLE);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = LEDR_Pin | LEDG_Pin | LEDB_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIOx, &GPIO_InitStructure);

	TIM_InternalClockConfig(LED_TIMx);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = LED_Period;
	TIM_TimeBaseInitStructure.TIM_Prescaler = LED_Prescaler;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(LED_TIMx, &TIM_TimeBaseInitStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = LED_Pulse;

	TIM_OC2Init(LED_TIMx, &TIM_OCInitStructure);
	TIM_OC3Init(LED_TIMx, &TIM_OCInitStructure);
	TIM_OC4Init(LED_TIMx, &TIM_OCInitStructure);

	TIM_Cmd(LED_TIMx, ENABLE);

	// 设置初始状态
	TIM_SetCompare2(LED_TIMx, LED_Period + 1);
	TIM_SetCompare3(LED_TIMx, LED_Period + 1);
	TIM_SetCompare4(LED_TIMx, LED_Period + 1);
}

#endif

/**
 * @brief: LED初始化 及PWM调光
 * @return {*}
 */
void LED_Init(void)
{
#if !LED_PWM_MODE

	LED_GPIO_Init();

#else

	LED_PWM_Init();

#endif
}

#if !LED_PWM_MODE

/**
 * @brief: 绿灯电平翻转
 * @return: {*}
 */
void LEDG_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIOx, LEDG_Pin) == 1)
		GPIO_ResetBits(LED_GPIOx, LEDG_Pin);
	else
		GPIO_SetBits(LED_GPIOx, LEDG_Pin);
}

/**
 * @brief: 蓝灯电平翻转
 * @return: {*}
 */
void LEDB_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIOx, LEDB_Pin) == 1)
		GPIO_ResetBits(LED_GPIOx, LEDB_Pin);
	else
		GPIO_SetBits(LED_GPIOx, LEDB_Pin);
}

/**
 * @brief: 红灯电平翻转
 * @return: {*}
 */
void LEDR_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIOx, LEDR_Pin) == 1)
		GPIO_ResetBits(LED_GPIOx, LEDR_Pin);
	else
		GPIO_SetBits(LED_GPIOx, LEDR_Pin);
}

/**
 * @brief: 获取绿灯状态
 * @return: {uint8_t} 0灭 1亮
 */
uint8_t LEDG_GetState(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIOx, LEDG_Pin) == 1)
		return 0;
	else
		return 1;
}

/**
 * @brief: 获取蓝灯状态
 * @return: {uint8_t} 0灭 1亮
 */
uint8_t LEDB_GetState(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIOx, LEDB_Pin) == 1)
		return 0;
	else
		return 1;
}

/**
 * @brief: 获取红灯状态
 * @return: {uint8_t} 0灭 1亮
 */
uint8_t LEDR_GetState(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIOx, LEDR_Pin) == 1)
		return 0;
	else
		return 1;
}

#else

/**
 * @brief: 设置PWM占空比
 * @param {uint8_t} R 范围0-255
 * @param {uint8_t} G 范围0-255
 * @param {uint8_t} B 范围0-255
 * @return: {*}
 */
void LED_SetPWMState(uint8_t R, uint8_t G, uint8_t B)
{
	float sR, sG, sB;

	sR = 100 - (R / 255) * 100;
	sG = 100 - (G / 255) * 100;
	sB = 100 - (B / 255) * 100;

	// 根据需求设置对应通道 2R 3G 4B
	TIM_SetCompare2(LED_TIMx, sR);
	TIM_SetCompare3(LED_TIMx, sG);
	TIM_SetCompare4(LED_TIMx, sB);
}

#endif
