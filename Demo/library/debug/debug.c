#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "debug.h"
static UART_HandleTypeDef *debug_uart;

void debug_init(UART_HandleTypeDef *uart){
    debug_uart=uart;
}

void debug(){
    
}