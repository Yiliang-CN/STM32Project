#include "stm32f10x.h" // Device header
#include "ESP8266.h"
#include "Delay.h"
#include "Serial.h"

// ESP8266数据帧结构体
struct ESP8266_RxDataFrame ESP8266_Data = {0};

/**
 * @brief: ESP8266 GPIO初始化
 * @return {*}
 */
static void ESP8266_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(ESP8266_RCC_GPIOx, ENABLE);

    // RST PB9
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = ESP8266_Pin_RST;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ESP8266_GPIOx, &GPIO_InitStructure);

    // TX PB10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = ESP8266_Pin_TX;
    GPIO_Init(ESP8266_GPIOx, &GPIO_InitStructure);

    // RX PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = ESP8266_Pin_RX;
    GPIO_Init(ESP8266_GPIOx, &GPIO_InitStructure);
}

/**
 * @brief: ESP8266 USART初始化
 * @return {*}
 */
static void ESP8266_USART_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = ESP8266_BaudRate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(ESP8266_USARTx, &USART_InitStructure);

    USART_ITConfig(ESP8266_USARTx, USART_IT_RXNE, ENABLE); // 使能串口接收中断
    USART_ITConfig(ESP8266_USARTx, USART_IT_IDLE, ENABLE); // 使能串口总线空闲中断

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ESP8266_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ESP8266_USART_PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ESP8266_USART_SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(ESP8266_USARTx, ENABLE);
}

/**
 * @brief: ESP8266 初始化
 * @return {*}
 */
void ESP8266_Init(void)
{
    ESP8266_GPIO_Init();
    ESP8266_USART_Init();

    ESP8266_RST();

    Delay_ms(2000); // 延时等待模块完全复位
}

/**
 * @brief: ESP8266 复位函数
 * @return {*}
 */
void ESP8266_RST(void)
{
    ESP8266_RST_Low;
    Delay_ms(500);
    ESP8266_RST_High;
}

/**
 * @brief: ESP8266 发送字符串
 * @param {char} *String
 * @return {*}
 */
static void ESP8266_SendString(char *String)
{
    for (uint16_t i = 0; String[i] != '\0'; i++)
    {
        USART_SendData(ESP8266_USARTx, String[i]);
        while (USART_GetFlagStatus(ESP8266_USARTx, USART_FLAG_TXE) == RESET)
            ;
    }
}

/**
 * @brief: ESP8266 发送命令
 * @param {char} *CMD               命令
 * @param {char} *Reply1            回复1
 * @param {char} *Reply2            回复2
 * @param {uint32_t} WaitTime_ms    等待时间
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_SendCmd(char *CMD, char *Reply1, char *Reply2, uint32_t WaitTime_ms)
{
    ESP8266_Data.DataInfo.InfoBit.FrameLength = 0;

    ESP8266_SendString(CMD);

    if ((Reply1 == NULL) && (Reply2 == NULL))
        return true;

    Delay_ms(WaitTime_ms);

    ESP8266_Data.DataBuf[ESP8266_Data.DataInfo.InfoBit.FrameLength] = '\0';

    if ((Reply1 != NULL) && (Reply2 != NULL))
        return ((bool)strstr(ESP8266_Data.DataBuf, Reply1) || (bool)strstr(ESP8266_Data.DataBuf, Reply2));
    else if (Reply1 != NULL)
        return ((bool)strstr(ESP8266_Data.DataBuf, Reply1));
    else
        return ((bool)strstr(ESP8266_Data.DataBuf, Reply2));
}

/**
 * @brief: ESP8266 打印日志
 * @return {*}
 */
void ESP8266_PrintfLog(void)
{
    if (ESP8266_Data.DataInfo.InfoBit.FrameFlag == 1)
    {
        for (int i = 0; i < ESP8266_Data.DataInfo.InfoBit.FrameLength; i++)
        {
            Serial_Printf("%c", ESP8266_Data.DataBuf[i]);
            while (USART_GetFlagStatus(ESP8266_USARTx, USART_FLAG_TC) == RESET)
                ;
        }
        ESP8266_Data.DataInfo.InfoAll = 0;
    }
}

// 以下为基础指令函数

/**
 * @brief: ESP8266 设置工作模式
 * @param {ESP8266_CWMODE} mode 工作模式 1 2 3
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_SetCWMODE(ESP8266_CWMODE mode)
{
    switch (mode)
    {
    case STA:
        return ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK", "no change", 2500);

    case AP:
        return ESP8266_SendCmd("AT+CWMODE=2\r\n", "OK", "no change", 2500);

    case STA_AP:
        return ESP8266_SendCmd("AT+CWMODE=3\r\n", "OK", "no change", 2500);

    default:
        return false;
    }
}

/**
 * @brief: ESP8266 连接WiFi
 * @param {char} *SSID      网络名称
 * @param {char} *PASSWORD  网络密码
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_SetCWJAP(char *SSID, char *PASSWORD)
{
    char CMD[128];
    sprintf(CMD, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWORD);
    return ESP8266_SendCmd(CMD, "OK", NULL, 2500);
}

/**
 * @brief: ESP8266 获取IP信息
 * @return {*}
 */
void ESP8266_GetCIFSR(void)
{
    if (ESP8266_SendCmd("AT+CIFSR\r\n", "OK", NULL, 2500))
        ESP8266_PrintfLog();
    else
        Serial_Printf("AT+CIFSR ERROR\r\n");
}

// 以下为MQTT指令函数

/**
 * @brief: ESP8266 MQTT用户配置
 * @param {char} *ClientID  客户端ID
 * @param {char} *UserName  用户名
 * @param {char} *PassWord  密码
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_MQTTUSERCFG(char *ClientID, char *UserName, char *PassWord)
{
    char CMD[128];
    sprintf(CMD, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", ClientID, UserName, PassWord);
    return ESP8266_SendCmd(CMD, "OK", NULL, 1000);
}

/**
 * @brief: ESP8266 MQTT连接Broker
 * @param {char} *BrokerIP  Broker的IP地址
 * @param {uint16_t} Port   Broker的端口号
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_MQTTCONN(char *BrokerIP, uint16_t Port)
{
    char CMD[128];
    sprintf(CMD, "AT+MQTTCONN=0,\"%s\",%d,0\r\n", BrokerIP, Port);
    return ESP8266_SendCmd(CMD, "OK", NULL, 1000);
}

/**
 * @brief: ESP8266 MQTT订阅主题
 * @param {char} *Topic     主题
 * @param {uint8_t} Qos     Qos等级
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_MQTTSUB(char *Topic, uint8_t Qos)
{
    char CMD[128];
    sprintf(CMD, "AT+MQTTSUB=0,\"%s\",%d\r\n", Topic, Qos);
    return ESP8266_SendCmd(CMD, "OK", NULL, 1000);
}

/**
 * @brief: ESP8266 MQTT发布主题
 * @param {char} *Topic     主题
 * @param {char} *Data      发布的数据
 * @param {uint8_t} Qos     Qos等级
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_MQTTPUB(char *Topic, char *Data, uint8_t Qos)
{
    char CMD[128];
    sprintf(CMD, "AT+MQTTPUB=0,\"%s\",\"%s\",%d,0\r\n", Topic, Data, Qos);
    return ESP8266_SendCmd(CMD, "OK", NULL, 1000);
}

/**
 * @brief: ESP8266 MQTT清除连接
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_MQTTCLEAN(void)
{
    char CMD[128];
    sprintf(CMD, "AT+MQTTCLEAN=0\r\n");
    return ESP8266_SendCmd(CMD, "OK", NULL, 1000);
}

// 以下为业务函数

/**
 * @brief: ESP8266 连接服务
 * @param {char} *SSID          网络名称
 * @param {char} *PASSWORD      网络密码
 * @param {char} *ClientID      客户端ID
 * @param {char} *UserName      用户名
 * @param {char} *PassWord      密码
 * @param {char} *BrokerIP      Broker的IP地址
 * @param {uint16_t} Port       Broker的端口号
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_ConnServer(char *SSID, char *PASSWORD, char *ClientID, char *UserName, char *PassWord, char *BrokerIP, uint16_t Port)
{
    if (!ESP8266_SetCWMODE(STA))
    {
        Serial_Printf("SetCWMODE ERROR\r\n");
        return false;
    }

    if (!ESP8266_SetCWJAP(SSID, PASSWORD))
    {
        Serial_Printf("SetCWJAP ERROR\r\n");
        return false;
    }

    ESP8266_GetCIFSR();

    if (!ESP8266_MQTTUSERCFG(ClientID, UserName, PassWord))
    {
        Serial_Printf("MQTTUSERCFG ERROR\r\n");
        return false;
    }

    if (!ESP8266_MQTTCONN(BrokerIP, Port))
    {
        Serial_Printf("MQTTCONN ERROR\r\n");
        return false;
    }

    Serial_Printf("MQTTCONN OK\r\n");

    return true;
}

/**
 * @brief: ESP8266 发送数据
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_SendData(void)
{
    return true;
}

/**
 * @brief: ESP8266 接收数据
 * @return {bool} 成功 true 失败 false
 */
bool ESP8266_RecvData(void)
{
    return true;
}

void USART3_IRQHandler(void)
{
    uint8_t ch;
    if (USART_GetITStatus(ESP8266_USARTx, USART_IT_RXNE) != RESET)
    {
        ch = USART_ReceiveData(ESP8266_USARTx);

        if (ESP8266_Data.DataInfo.InfoBit.FrameLength < (ESP8266_RxDataBufSize - 1))
            ESP8266_Data.DataBuf[ESP8266_Data.DataInfo.InfoBit.FrameLength++] = ch;
    }

    if (USART_GetITStatus(ESP8266_USARTx, USART_IT_IDLE) == SET)
    {
        ESP8266_Data.DataInfo.InfoBit.FrameFlag = 1;
        ch = USART_ReceiveData(ESP8266_USARTx);
    }
}
