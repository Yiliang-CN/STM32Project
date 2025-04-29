#include "stm32f10x.h" // Device header
#include "FreeRTOSTask.h"
#include "Delay.h"
#include "Serial.h"
#include "LED.h"
#include "Key.h"
#include "ESP8266.h"
#include "DHT11.h"

int main(void)
{
	uint8_t ISCONN = 0;
	char Data[512];

	Serial_Init();
	LED_Init();
	Key_Init();
	ESP8266_Init();

	if (ESP8266_ConnServer(ESP8266_SSID, ESP8266_PASSWORD, ESP8266_ClientID, ESP8266_UserName, ESP8266_PassWord, ESP8266_BrokerIP, ESP8266_Port))
	{
		ISCONN = 1;
		ESP8266_PrintfLog();
		Serial_Printf("CONNECT OK\r\n");
	}

	while (1)
	{
		LEDB_Turn();

		DHT11_GET_Data();
		Serial_Printf("Temp: %d.%d  Humi: %d.%d\r\n", DHT11_Data.Temp_Int, DHT11_Data.Temp_Deci, DHT11_Data.Humi_Int, DHT11_Data.Humi_Deci);

		if (ISCONN == 1)
		{
			float temp_value = DHT11_Data.Temp_Int + (DHT11_Data.Temp_Deci / 10.0f);
			float humi_value = DHT11_Data.Humi_Int + (DHT11_Data.Humi_Deci / 10.0f);
			sprintf(Data,
					"{\\\"id\\\":123\\,\\\"dp\\\":{\\\"Temp\\\":[{\\\"v\\\":%.1f}]\\,\\\"Humi\\\":[{\\\"v\\\":%.1f}]}}",
					temp_value, humi_value);

			if (!ESP8266_MQTTPUB(ESP8266_PUBTopic, Data, 0))
				ESP8266_PrintfLog();
		}

		if (Key1_GetState() == 1)
		{
			ESP8266_MQTTCLEAN();
			ESP8266_PrintfLog();
			ISCONN = 0;
		}

		Delay_ms(2000);
	}
}
