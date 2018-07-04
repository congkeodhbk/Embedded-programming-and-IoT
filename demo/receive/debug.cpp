
#include <stdio.h>
#include "debug.h"

char print_buff[IO_BUFF];
int __printf (const char *format, ...)
{
   va_list arg;
   int done;
   va_start (arg, format);
   done = vsprintf (print_buff, format, arg);
   va_end (arg);
   DEBUGGER.print(print_buff);
   return done;
}


