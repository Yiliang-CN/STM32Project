#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define Serial_RCC_USARTx RCC_APB2Periph_USART1
#define Serial_RCC_GPIOx RCC_APB2Periph_GPIOA

#define Serial_GPIOx GPIOA
#define Serial_Pin_TX GPIO_Pin_9  // PA9
#define Serial_Pin_RX GPIO_Pin_10 // PA10

#define Serial_BaudRate 9600 // 串口波特率

#define Serial_USARTx USART1

#define Serial_PriorityGroup_x NVIC_PriorityGroup_1
#define Serial_IRQChannel USART1_IRQn
#define Serial_IRQChannelPreemptionPriority 1
#define Serial_IRQChannelSubPriority 1

extern char Serial_TxTextPacket[];
extern char Serial_RxTextPacket[];
extern uint8_t Serial_TxHexPacket[];
extern uint8_t Serial_RxHexPacket[];

void Serial_Init(void);                                  // 串口初始化
void Serial_SendByte(uint8_t Byte);                      // 发送一个字节
void Serial_SendArray(uint8_t *Array, uint16_t Length);  // 发送一个数组
void Serial_SendString(char *String);                    // 发送一个字符串
void Serial_SendNumber(uint32_t Number, uint8_t Length); // 发送一个数字
void Serial_Printf(char *format, ...);                   // 发送一个格式化字符串
uint8_t Serial_GetRxFlag(void);                          // 获取接收标志

#endif
