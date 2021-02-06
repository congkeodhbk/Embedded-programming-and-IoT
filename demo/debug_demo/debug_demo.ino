
#include <stdint.h>
#include <string.h>
#include "debug.h"

volatile int led_cycle=0;

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
  else if(__check_cmd("led frequency ")){
    int freq;
    if(1==sscanf(__param_pos("led frequency "), "%d", &freq)){
      debug("set led frequency to %d Hz\n", freq);
      if(0==freq){
        led_cycle=0;
      }
      else{
        led_cycle=1000/freq/2;
      }
    }
  }
  else{
    debug("What's up?\n");
  }
}

void setup() {
    debug_init(&Serial, 9600, console_handler);
    pinMode(LED_BUILTIN, OUTPUT);
    debug("Hello world\n");
}
void led_process(){
  static uint32_t tick=0;
  if(led_cycle>0){
    if(millis()-tick>led_cycle){
      tick=millis();
      //toggle led
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}

void loop() {
  led_process();
  debug_polling();
}
