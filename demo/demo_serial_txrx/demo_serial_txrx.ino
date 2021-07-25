
#include <stdint.h>
#include <string.h>
#include "debug.h"

typedef struct {
    int8_t temp;
    uint8_t humi;
    uint16_t lux;
}env_t;


void setup() {
    Serial.begin(9600);
    env_t env;
    env.temp=20;
    env.humi=80;
    env.lux=10000;
    debug("Transmit temperature: %d, humidity: %d, lux: %d\n\n",\
          env.temp, env.humi, env.lux);
    uint8_t *p_env=(uint8_t *)&env;
    uint8_t rx_buffer[10];
    for(uint8_t i=0; i<sizeof(env_t); i++){
        rx_buffer[i]=*(p_env+i);
    }
    env_t *rx_env=(env_t *)rx_buffer;
    debug("Received temperature: %d\n",rx_env->temp);
    debug("Received humidity: %d\n",rx_env->humi);
    debug("Received lux: %d\n",rx_env->lux);
}

void loop(){
  
}
