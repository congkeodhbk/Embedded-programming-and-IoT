
#include <stdio.h>
#include "debug.h"

static HardwareSerial *serial= nullptr;
static debug_callback_t callback=nullptr;

void debug_init(HardwareSerial *sr, int baud, debug_callback_t cb){
   serial=sr;
   callback=cb;
   serial->begin(baud);
}

int __printf (const char *format, ...)
{
   if(nullptr==serial)
      return 0;
   static char print_buff[IO_BUFF];
   va_list arg;
   int done;
   va_start (arg, format);
   done = vsprintf (print_buff, format, arg);
   va_end (arg);
   serial->print(print_buff);
   return done;
}

void debug_polling(){
   static char buff[IO_BUFF];
   static uint16_t i=0;
   while(serial->available()){
      buff[i]=(char)serial->read();
      if('\r'==buff[i]||'\n'==buff[i]){
         if(i>0){
            if(callback!=nullptr){
               callback(buff);
            }
            i=0;
         }
      }
      else
         i++;
      if(IO_BUFF==i){
         i=0;
      }
   }   
}
