#include "stm32f10x.h" // Device header
#include "FreeRTOSTask.h"
#include "Delay.h"
#include "Serial.h"
#include "ESP8266.h"
#include "LED.h"
#include "Key.h"

int main(void)
{
	Serial_Init();
	ESP8266_Init();
	LED_Init();

	if (ESP8266_ConnServer("HUAWEI-CR151S", "alt134679", "mqttx_20040903", NULL, NULL, "192.168.3.93", 1883))
	{
		ESP8266_MQTTSUB("test", 0);
	}

	ESP8266_PrintfLog();

	while (1)
	{
		LEDB_Turn();
		ESP8266_PrintfLog();

		if (Key1_GetState() == 1)
		{
			ESP8266_MQTTCLEAN();
		}

		Delay_ms(2000);
	}
}
