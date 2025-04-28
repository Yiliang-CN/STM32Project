#include "stm32f10x.h" // Device header
#include "Key.h"

#if KEY_INTERRUPTS_MODE
// 这里写中断需要的头文件
#include "LED.h"

#endif

/**
 * @brief: 按键GPIO初始化
 * @return: {*}
 */
static void Key_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(Key_RCC_GPIOx, ENABLE);

	// 按键一初始化
	GPIO_InitTypeDef GPIO_InitStructure_Key1;
	GPIO_InitStructure_Key1.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 已外部下拉 可设置浮空输入
	GPIO_InitStructure_Key1.GPIO_Pin = Key1_Pin;
	GPIO_Init(Key1_GPIOx, &GPIO_InitStructure_Key1);

	// 按键二初始化
	GPIO_InitTypeDef GPIO_InitStructure_Key2;
	GPIO_InitStructure_Key2.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure_Key2.GPIO_Pin = Key2_Pin;
	GPIO_Init(Key2_GPIOx, &GPIO_InitStructure_Key2);
}

#if KEY_INTERRUPTS_MODE

/**
 * @brief: 按键中断初始化
 * @return {*}
 */
static void Key_NVIC_Init(void)
{

	RCC_APB2PeriphClockCmd(Key_RCC_AFIO, ENABLE);

	// 按键中断引脚映射
	GPIO_EXTILineConfig(Key1_PortSrcGPIOx, Key1_PinSrc);
	GPIO_EXTILineConfig(Key2_PortSrcGPIOx, Key2_PinSrc);

	// 按键中断初始化
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = key_EXTI_Linex;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(key_PriorityGroup_x);

	// 按键中断优先级和子优先级
	NVIC_InitTypeDef NVIC_InitStructure_Key1;
	NVIC_InitStructure_Key1.NVIC_IRQChannel = Key1_EXTI_IRQn;
	NVIC_InitStructure_Key1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure_Key1.NVIC_IRQChannelPreemptionPriority = Key1_PreemptionPriority;
	NVIC_InitStructure_Key1.NVIC_IRQChannelSubPriority = Key1_SubPriority;
	NVIC_Init(&NVIC_InitStructure_Key1);

	NVIC_InitTypeDef NVIC_InitStructure_Key2;
	NVIC_InitStructure_Key2.NVIC_IRQChannel = Key2_EXTI_IRQn;
	NVIC_InitStructure_Key2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure_Key2.NVIC_IRQChannelPreemptionPriority = Key2_PreemptionPriority;
	NVIC_InitStructure_Key2.NVIC_IRQChannelSubPriority = Key2_SubPriority;
	NVIC_Init(&NVIC_InitStructure_Key2);
}

#endif

/**
 * @brief: 按键初始化 及中断配置
 * @return {*}
 */
void Key_Init(void)
{
	Key_GPIO_Init();

#if KEY_INTERRUPTS_MODE // 是否配置中断模式

	Key_NVIC_Init();

#endif
}

#if !KEY_INTERRUPTS_MODE

/**
 * @brief: 获取按键1的状态
 * @return: {uint8_t} 1按下 0松开
 */
uint8_t Key1_GetState(void)
{
	uint8_t Key_State = 0;

	if (GPIO_ReadInputDataBit(Key1_GPIOx, Key1_Pin) == 1)
	{
		// 有硬件消抖 可免去软件消抖
		while (GPIO_ReadInputDataBit(Key1_GPIOx, Key1_Pin) == 1)
			;

		Key_State = 1;
	}

	return Key_State;
}

/**
 * @brief: 获取按键2的状态
 * @return: {uint8_t} 1按下 0松开
 */
uint8_t Key2_GetState(void)
{
	uint8_t Key_State = 0;

	if (GPIO_ReadInputDataBit(Key2_GPIOx, Key2_Pin) == 1)
	{
		// 有硬件消抖 可免去软件消抖
		while (GPIO_ReadInputDataBit(Key2_GPIOx, Key2_Pin) == 1)
			;

		Key_State = 1;
	}

	return Key_State;
}

#else

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(Key1_EXTI_Line) == SET)
	{
		// 这里写中断要执行的内容
		LEDB_Turn();
		EXTI_ClearITPendingBit(Key1_EXTI_Line);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(Key2_EXTI_Line) == SET)
	{
		// 这里写中断要执行的内容
		LEDG_Turn();
		EXTI_ClearITPendingBit(Key2_EXTI_Line);
	}
}

#endif
