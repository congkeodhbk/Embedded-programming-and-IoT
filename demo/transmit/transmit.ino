
#include <stdint.h>
#include <string.h>
#include "debug.h"
uint8_t rx_buffer[10];
typedef struct {
    uint8_t temp;
    uint8_t humi;
    uint16_t lux;
}env_t;

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    debug("Hello world\n");
}

static env_t env;
void read_sensor(void){
	  env.temp+=1;
    env.humi+=2;
    env.lux+=3;
}
void send_data(void){
	Serial1.write((uint8_t *)&env, sizeof(env_t)); //uart_transmit
}
void loop() {
	  read_sensor();
    send_data();
    delay(1000);
}
