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

#include "slip.h"
#include <string.h>
#include <stdbool.h>

#define SLIP_BYTE_END             0300    /* indicates end of packet */
#define SLIP_BYTE_ESC             0333    /* indicates byte stuffing */
#define SLIP_BYTE_ESC_END         0334    /* ESC ESC_END means END data byte */
#define SLIP_BYTE_ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

static void slip_reset(slip_t * slip){
	slip->idx=0;
	slip->state=SLIP_STATE_DECODING;
}
void slip_init(slip_t *slip, uint8_t *buff, uint32_t buff_len, slip_send_t send){
    slip->buff=buff;
    slip->buff_len=buff_len;
    slip->send=send;
    slip_reset(slip);
}
static void slip_send_buff(slip_t *slip, uint8_t *buff, uint16_t len){
    for (uint16_t i = 0; i < len; i++)
    {
        switch (buff[i])
        {
            case SLIP_BYTE_END:
                slip->send(SLIP_BYTE_ESC);
                slip->send(SLIP_BYTE_ESC_END);
                break;

            case SLIP_BYTE_ESC:
                slip->send(SLIP_BYTE_ESC);
                slip->send(SLIP_BYTE_ESC_ESC);
                break;

            default:
                slip->send(buff[i]);
        }
    }
}
void slip_send(slip_t *slip, uint8_t * data, uint16_t len, bool _continue)
{
    if (slip==NULL)
    {
        return;
    }
    slip_send_buff(slip, data, len);
    if(!_continue)
        slip->send(SLIP_BYTE_END);
    return;
}

uint32_t slip_parse(slip_t * slip, uint8_t c)
{
    if (slip == NULL)
    {
        return 0;
    }

    if (slip->idx == slip->buff_len)
    {
    	slip->state=SLIP_STATE_DECODING;
    	slip->idx =0;
        return 0;
    }

    switch (slip->state)
    {
        case SLIP_STATE_DECODING:
            switch (c)
            {
                case SLIP_BYTE_END:
                    if(slip->idx==0){
                        slip_reset(slip);
                        return 0;
                    }

					          slip->frame_len=slip->idx;
                    slip_reset(slip);             
                    return slip->frame_len;

                case SLIP_BYTE_ESC:
                    // wait for
                    slip->state = SLIP_STATE_ESC_RECEIVED;
                    break;

                default:
                    // add byte to buffer
                    slip->buff[slip->idx++] = c;
                    break;
            }
            break;

        case SLIP_STATE_ESC_RECEIVED:
            switch (c)
            {
                case SLIP_BYTE_ESC_END:
                    slip->buff[slip->idx++] = SLIP_BYTE_END;
                    slip->state = SLIP_STATE_DECODING;
                    break;

                case SLIP_BYTE_ESC_ESC:
                    slip->buff[slip->idx++] = SLIP_BYTE_ESC;
                    slip->state = SLIP_STATE_DECODING;
                    break;

                default:
                    // protocol violation
                    slip->state = SLIP_STATE_CLEARING_INVALID_PACKET;
                    return 0;
            }
            break;

        case SLIP_STATE_CLEARING_INVALID_PACKET:
            if (c == SLIP_BYTE_END)
            {
                slip_reset(slip);
            }
            break;
    }
    return 0;
}
