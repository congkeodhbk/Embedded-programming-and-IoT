
#ifndef _DEBUG_H
#define _DEBUG_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <stdarg.h>


#define IO_BUFF 256

#define DEBUGGER Serial
int __printf (const char *format, ...);

#define debug(...) do{__printf(__VA_ARGS__);} while(0)


#endif

