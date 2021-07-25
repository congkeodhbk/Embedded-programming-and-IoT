
#include "slip.h"
#include "ringbuf.h"

typedef enum{
  DATA_FRAME,
  CONTROL_FRAME,
}frame_type_t;

typedef struct{
  frame_type_t type;
  int temperature;
  int humidity;
}data_frame_t;

typedef struct{
  frame_type_t type;
  bool on_off;
}control_frame_t;

slip_t A_slip;
uint8_t A_slip_buff[32];
RINGBUF A_ringbuf;
uint8_t A_ringbuf_buff[128];

slip_t B_slip;
uint8_t B_slip_buff[32];
RINGBUF B_ringbuf;
uint8_t B_ringbuf_buff[128];

void A_send_byte(uint8_t b){
  RINGBUF_Put(&B_ringbuf, b);
}

void B_send_byte(uint8_t b){
  RINGBUF_Put(&A_ringbuf, b);
}

void slip_parse_handler(uint8_t *data, uint16_t len){
  frame_type_t *type =(frame_type_t *)data;
  if(*type == CONTROL_FRAME){
    Serial.println("Received control frame");
    control_frame_t *control= (control_frame_t *)data;
    Serial.print("Control: "); Serial.println(control->on_off?"ON":"OFF");
  }
  else if(*type==DATA_FRAME){
    Serial.println("Received data frame");
    data_frame_t *frame=(data_frame_t *)data;
    Serial.print("Temperature: "); Serial.print(frame->temperature); Serial.println("*C");
    Serial.print("Humidity: "); Serial.print(frame->humidity); Serial.println("%");
  }
}

void setup()
{
  delay(100);
  Serial.begin(115200);
  while (!Serial){
    
  }
  slip_init(&A_slip, A_slip_buff, sizeof(A_slip_buff), A_send_byte);
  slip_init(&B_slip, B_slip_buff, sizeof(B_slip_buff), B_send_byte);
  RINGBUF_Init(&A_ringbuf, A_ringbuf_buff, sizeof(A_ringbuf_buff));
  RINGBUF_Init(&B_ringbuf, B_ringbuf_buff, sizeof(B_ringbuf_buff));

  data_frame_t data_frame;
  data_frame.type=DATA_FRAME;
  data_frame.temperature=20;
  data_frame.humidity=80;

  control_frame_t control_frame;
  control_frame.type=CONTROL_FRAME;
  control_frame.on_off=true;

  Serial.println("A send data to B");
  slip_send(&A_slip, (uint8_t *)&data_frame, sizeof(data_frame_t), false);
  slip_send(&A_slip, (uint8_t *)&control_frame, sizeof(control_frame_t), false);
  
  while(RINGBUF_Available(&B_ringbuf)){
    static uint8_t b;
    static uint16_t len;
    RINGBUF_Get(&B_ringbuf, &b);
    len=slip_parse(&B_slip, b);
    if(len>0){
        slip_parse_handler(B_slip.buff, len);
    }
  }
  Serial.println("");
  Serial.println("B send data to A");
  slip_send(&B_slip, (uint8_t *)&data_frame, sizeof(data_frame_t), false);
  slip_send(&B_slip, (uint8_t *)&control_frame, sizeof(control_frame_t), false);
  
  while(RINGBUF_Available(&A_ringbuf)){
    static uint8_t b;
    static uint16_t len;
    RINGBUF_Get(&A_ringbuf, &b);
    len=slip_parse(&A_slip, b);
    if(len>0){
        slip_parse_handler(A_slip.buff, len);
    }
  }
}
void loop()
{
  
}
