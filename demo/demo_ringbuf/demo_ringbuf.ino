#include "ringbuf.h"

RINGBUF demo_ringbuf;
uint8_t ringbuf_buffer[10];

void ringbuf_demo(int putCount){
    Serial.print("Put item: "); Serial.println(putCount);
    for(uint8_t i=0; i<putCount; i++){
        RINGBUF_Put(&demo_ringbuf, i);
    }
    //Receive data
    uint8_t count =0;
    while(RINGBUF_Available(&demo_ringbuf)){
        uint8_t item;
        count++;
        RINGBUF_Get(&demo_ringbuf, &item);
        Serial.print("Ringbuf item: "); Serial.println(item);
    }
    Serial.print("Ring buff item count: "); Serial.println(count); Serial.println("");
}

void setup(){
    Serial.begin(115200);
    RINGBUF_Init(&demo_ringbuf, ringbuf_buffer, sizeof(ringbuf_buffer));
    //Simulate serial uart put

    Serial.println("Ring buff normal operation demo");
    ringbuf_demo(5);
    
    Serial.println("Ring buff over flow demo");
    ringbuf_demo(12);
}

void loop(){

}
