/**
 * Copyright (c) 2015 - 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef SLIP_H__
#define SLIP_H__

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef USE_SLIP_CRC
#define USE_SLIP_CRC 0
#endif


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
typedef enum
{
  SLIP_STATE_DECODING, //!< Ready to receive the next byte.
  SLIP_STATE_ESC_RECEIVED, //!< An ESC byte has been received and the next byte must be decoded differently.
  SLIP_STATE_CLEARING_INVALID_PACKET //!< The received data is invalid and transfer must be restarted.
} slip_read_state_t;


typedef void (*frame_handler)(uint8_t *frame, size_t len);

typedef void (*slip_send_t)(uint8_t c);

typedef struct
{
  slip_read_state_t   state; 
  uint8_t             *buff; 
  uint32_t            idx; 
  uint32_t            frame_len;
  uint32_t            buff_len;
  slip_send_t         send;
} slip_t;

void slip_init(slip_t *slip, uint8_t *buff, uint32_t buff_len, slip_send_t send);
void slip_send(slip_t *slip, uint8_t * data, uint16_t size, bool _continue);
uint32_t slip_parse(slip_t * p_slip, uint8_t c);

#ifdef __cplusplus
}
#endif

#endif // SLIP_H__

/** @} */
