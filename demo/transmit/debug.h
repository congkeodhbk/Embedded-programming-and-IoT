
#ifndef DEBUG_H
#define DEBUG_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <stdarg.h>


#define IO_BUFF 256

#define DEBUGGER Serial
int __printf (const char *format, ...);
int __scanf(const char *format, ...);

#define debug(...) do{__printf(__VA_ARGS__);} while(0)
#define debug_scan(...) do{__scanf(__VA_ARGS__);} while(0)


#endif

