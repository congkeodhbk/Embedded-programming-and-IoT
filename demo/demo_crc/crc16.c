#include "crc16.h"
#include <string.h>

uint16_t crc16_compute(uint8_t *data, uint16_t len, uint16_t *init)
{
    const uint16_t generator = 0x1021; /* divisor is 16bit */
    uint16_t crc; /* CRC value is 16bit */
    if(init!=NULL){
        crc=*init;
    }
    else{
        crc=0xffff;
    }

    for(uint16_t i=0; i<len; i++)
    {
        crc ^= (uint16_t)(data[i] << 8); /* move byte into MSB of 16bit CRC */

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x8000) != 0) /* test for MSB = bit 15 */
            {
                crc = (uint16_t)((crc << 1) ^ generator);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint16_t crc16_append(uint8_t *buf, uint16_t data_size)
{
  uint16_t crc=crc16_compute(buf, data_size, NULL);
  buf[data_size]=crc &0x0ff;
  buf[data_size+1]= (crc >> 8) & 0x0ff;
}
bool crc16_frame_check(uint8_t *data, uint16_t len){
    if(len <=2){
        return false;
    }
    uint16_t cal_crc=crc16_compute(data, len-2, NULL);
    uint16_t recv_crc=data[len-2] | (((uint16_t)data[len-1])<<8);
    if(cal_crc!=recv_crc){
        return false;
    }
    return true;
}
