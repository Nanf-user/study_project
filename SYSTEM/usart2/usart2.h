#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"

// 接收缓冲区大小
#define USART2_RX_BUF_SIZE  256

// 接收状态标志
// bit15: 接收完成标志
// bit14: 收到 0x0D (回车)
// bit13~0: 接收到的数据长度
extern volatile uint16_t USART2_RX_STA;
extern uint8_t USART2_RX_BUF[USART2_RX_BUF_SIZE];

// 函数声明
void USART2_Init(uint32_t bound);
uint8_t USART2_RxComplete(void);
void USART2_RxReset(void);
void USART2_SendByte(uint8_t data);
void USART2_SendString(const char *str);

#endif
