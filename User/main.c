#include "stm32f10x.h" // Device header
#include "FreeRTOSTask.h"
#include "Delay.h"
#include "Serial.h"
#include "LED.h"
#include "ESP8266.h"
#include "string.h"

int main(void)
{
	Serial_Init();
	ESP8266_Init();
	LED_Init();

	if (ESP8266_SetCWMODE(STA))
	{
		if (ESP8266_SetCWJAP("HUAWEI-CR151S", "alt134679"))
		{
			ESP8266_GetCIFSR();
			if (ESP8266_MQTTUSERCFG("mqttx_20040903", "", ""))
			{
				if (ESP8266_MQTTCONN("192.168.3.90", 1883))
				{
					Serial_Printf("MQTTCONN OK\r\n");
					if (ESP8266_MQTTSUB("test", 0))
					{
						Serial_Printf("MQTTSUB OK\r\n");
					}
					else
					{
						Serial_Printf("MQTTSUB ERROR\r\n");
					}
				}
				else
				{
					Serial_Printf("MQTTCONN ERROR\r\n");
				}
			}
		}
	}
	else
	{
		Serial_Printf("SetCWMODE ERROR\r\n");
	}

	while (1)
	{
		LEDB_Turn();
		ESP8266_PrintfLog();

		Delay_ms(2000);
	}
}
