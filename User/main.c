#include "stm32f10x.h" // Device header
#include "FreeRTOSTask.h"
#include "Delay.h"
#include "Serial.h"
#include "LCD.h"

int main(void)
{
	LCD_Init();
	Serial_Init();

	LCD_ShowString_EN(0, 0, "STM32F103C8T6");

	while (1)
	{
		Delay_ms(2000);
	}
}
