
#ifndef _DEBUG_H
#define _DEBUG_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <stdarg.h>

#define IO_BUFF 64
#define __check_cmd_at_pos(x,p) (strncmp(&result[p], x, strlen(x)) == 0)
#define __check_cmd(x) __check_cmd_at_pos(x,0)
#define __param_pos(x)	((char *)(&result[strlen(x)]))

typedef void (* debug_callback_t)(char *result);
void debug_init(HardwareSerial *sr, int baud, debug_callback_t cb);
void debug_polling();

int __printf (const char *format, ...);

#define debug(...) do{__printf(__VA_ARGS__);} while(0)

#endif
