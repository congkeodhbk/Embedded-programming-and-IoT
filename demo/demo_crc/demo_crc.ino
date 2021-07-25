#include "crc16.h"

void setup()
{
  Serial.begin(115200);
  //data length is 5 bytes
  uint8_t frame[7]={0xf0, 0x01, 0x01, 0xe0, 0x02, 0x0, 0x0};
  Serial.println("Append crc");
  crc16_append(frame, 5);
  Serial.print("Frame check: "); Serial.println( crc16_frame_check(frame, 7)?"OK":"ERROR");
  Serial.println("Modify frame"); 
  frame[0]=1;
  Serial.print("Frame check: "); Serial.println( crc16_frame_check(frame, 7)?"OK":"ERROR");
}

void loop()
{
  
}
