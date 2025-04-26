#ifndef __ESP8266_H
#define __ESP8266_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define ESP8266_RCC_USARTx RCC_APB1Periph_USART3
#define ESP8266_RCC_GPIOx RCC_APB2Periph_GPIOB

#define ESP8266_GPIOx GPIOB
#define ESP8266_Pin_RST GPIO_Pin_9 // PB9
#define ESP8266_Pin_TX GPIO_Pin_10 // PB10
#define ESP8266_Pin_RX GPIO_Pin_11 // PB11

#define ESP8266_BaudRate 115200 // 串口波特率

#define ESP8266_USARTx USART3

#define ESP8266_PriorityGroup_x NVIC_PriorityGroup_1
#define ESP8266_USART_IRQn USART3_IRQn
#define ESP8266_USART_PreemptionPriority 2
#define ESP8266_USART_SubPriority 2

#define ESP8266_RST_High GPIO_SetBits(ESP8266_GPIOx, ESP8266_Pin_RST)
#define ESP8266_RST_Low GPIO_ResetBits(ESP8266_GPIOx, ESP8266_Pin_RST)

#define ESP8266_RxDataBufSize 1024

// SSID "HUAWEI-CR151S"
// PASSWORD "alt134679"
// IP "192.168.3.87"
// PORT 1883

typedef enum
{
    STA,
    AP,
    STA_AP
} ESP8266_CWMODE;

extern struct ESP8266_RxDataFrame
{
    char DataBuf[ESP8266_RxDataBufSize];

    union DataInfo
    {
        volatile uint16_t InfoAll;
        struct InfoBit
        {
            volatile uint16_t FrameLength : 15;
            volatile uint16_t FrameFlag : 1;
        } InfoBit;

    } DataInfo;

} ESP8266_Data;

void ESP8266_Init(void);
void ESP8266_SendString(char *String);
bool ESP8266_SendCmd(char *cmd, char *reply1, char *reply2, uint32_t timeout_ms);
void ESP8266_PrintfLog(void);

void ESP8266_RST(void);
bool ESP8266_SetCWMODE(ESP8266_CWMODE mode);
bool ESP8266_SetCWJAP(char *SSID, char *PASSWORD);
void ESP8266_GetCIFSR(void);

bool ESP8266_MQTTUSERCFG(char *ClientID, char *UserName, char *PassWord);
bool ESP8266_MQTTCONN(char *BrokerIP, uint16_t Port);
bool ESP8266_MQTTSUB(char *Topic, uint8_t Qos);
bool ESP8266_MQTTPUB(char *Topic, char *Data, uint8_t Qos);
bool ESP8266_MQTTCLEAN(void);

bool ESP8266_ConnServer(char *SSID, char *PASSWORD, char *ClientID, char *UserName, char *PassWord, char *BrokerIP, uint16_t Port);
bool ESP8266_SendData(void);
bool ESP8266_RecvData(void);

#endif
