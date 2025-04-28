#include "stm32f10x.h" // Device header
#include "FreeRTOSTask.h"
#include "Delay.h"
#include "Serial.h"
#include "LED.h"
#include "LCD.h"

int main(void)
{
	LCD_Init();

	Serial_Init();
	LED_Init();

	LCD_DisplayString_EN(10, 10, "STM32F103C8T6");

	while (1)
	{
		LEDB_Turn();

		Delay_ms(2000);
	}
}
