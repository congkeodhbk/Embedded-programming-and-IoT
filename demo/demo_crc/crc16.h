#ifndef __crc16_h__
#define __crc16_h__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool crc16_frame_check(uint8_t *frame, uint16_t frame_size);
uint16_t crc16_append(uint8_t *buf, uint16_t data_size);
uint16_t crc16_compute(uint8_t *data, uint16_t len, uint16_t *init);

#ifdef __cplusplus
}
#endif

#endif
