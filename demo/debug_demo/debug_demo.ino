
#include <stdint.h>
#include <string.h>
#include "debug.h"

void console_handler(char *result){
  if(__check_cmd("hello")){
    debug("hi there\n");
  }
  else if(__check_cmd("hi")){
    debug("good morning\n");
  }
  else if(__check_cmd("one number ")){
    int x;
    if(1==sscanf(__param_pos("one number "), "%d", &x)){
      debug("You just enter %d\n", x);
    }
  }
  else if(__check_cmd("two numbers")){
    int x, y;
    if(2==sscanf(__param_pos("one number "), "%d %d", &x, &y)){
      debug("You just enter %d and %d\n", x, y);
    }
  }
  else{
    debug("What's up?\n");
  }
}

void setup() {
    debug_init(&Serial, 9600, console_handler);
    debug("Hello world\n");
}

void loop() {
  debug_polling();
}
