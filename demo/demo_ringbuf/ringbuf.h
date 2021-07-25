/**
	Header file for ringbuffer implementation base on contiki-os core and many
	resources
	
	HieuNT
	17/7/2015
*/

#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>
#ifndef __IO
#define __IO volatile
#endif
 typedef struct
{
    __IO uint32_t head;                
    __IO uint32_t tail;                
    __IO uint32_t size;                
    __IO uint8_t *pt;  					
} RINGBUF;

void RINGBUF_Init(RINGBUF *r, uint8_t* buf, uint32_t size);
int32_t RINGBUF_Put(RINGBUF *r, uint8_t c);																										
int32_t RINGBUF_Get(RINGBUF *r, uint8_t* c);
int32_t RINGBUF_Count(RINGBUF *r);
int32_t RINGBUF_Free(RINGBUF *r);
int32_t RINGBUF_Available(RINGBUF *r);
void RINGBUF_Flush(RINGBUF *r);
int32_t RINGBUF_Size(RINGBUF *r);
#ifdef __cplusplus
 }
#endif
	 
#endif
