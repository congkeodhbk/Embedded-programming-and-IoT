
#include <stdint.h>
#include <string.h>
#include "debug.h"

#define RECEIVER Serial1
#define INPUT_TIMEOUT 20 //milli second
uint8_t rx_buffer[10];
typedef struct {
    uint8_t temp;
    uint8_t humi;
    uint16_t lux;
}env_t;


enum{
    WAITING,
    RECEIVING,
};

int uart_get_input()
{
    static uint8_t state=WAITING;
    static uint8_t input_count=0;
    static uint32_t last_input_millis=0;

    if(state==WAITING)
    {
        if(RECEIVER.available()) //start receiving data
        {
            state=RECEIVING;
            input_count=0;
        }
    }

    if(state==RECEIVING)
    {
        if(RECEIVER.available())
        {
            rx_buffer[input_count]=(uint8_t)RECEIVER.read();
            last_input_millis=millis();
            input_count++;
        }
        else if((millis()-last_input_millis)>INPUT_TIMEOUT)
        {
            state=WAITING;
            return input_count;
        }
    }
    return 0;
}
void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    debug("Hello world\n");
}

void loop() {
    int received=uart_get_input();
    if(received>0){
        debug("received: %d bytes\n", received);
    }
    if(received==sizeof(env_t)){ //received correct array lenght, need crc checking
        env_t *p_env;
        p_env=(env_t *)rx_buffer;
        debug("Temperature: %d\n", p_env->temp);
        debug("Humidity: %d\n", p_env->humi);
        debug("Lux: %d\n\n", p_env->lux);
    }
}
