#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "usart.h"
void debug_init(UART_HandleTypeDef *uart);
void debug();

#endif