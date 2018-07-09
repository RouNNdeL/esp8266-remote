//
// Created by Krzysiek on 2018-07-09.
//

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <string.h>
#include <Arduino.h>
#include "ir_handler.h"
#include "config.h"

uint8_t id_que[QUE_LENGTH];
uint64_t code_que[QUE_LENGTH];
uint8_t que_length = 0;

IRsend send(IR_PIN);

void irBegin()
{
    send.begin();
}

uint8_t addCode(uint8_t id, uint64_t value)
{
    if(que_length < QUE_LENGTH)
    {
        code_que[que_length] = value;
        id_que[que_length] = id;
        que_length++;
        return 1;
    }
    return 0;
}

void handleCodes()
{
    if(que_length)
    {
        que_length--;
        uint8_t code_id = id_que[0];
        uint64_t code_value = code_que[0];

        memmove(id_que, id_que + 1, que_length * sizeof(uint8_t));
        memmove(code_que, code_que + 1, que_length * sizeof(uint64_t));

        sendCode(code_id, code_value);
    }
}

void sendCode(uint8_t id, uint64_t value)
{
    digitalWrite(INFO_PIN, 1);
    switch(id)
    {
        case CODE_NEC:
        {
            send.sendNEC(value);
        }
        case CODE_PANASONIC:
        {
            /* Panasonic TVs require for holding the button in order to turn  on the tv */
            if(value == CODE_VALUE_PANASONIC_ON)
            {
                for(int i = 0; i < 4; ++i)
                {
                    send.sendPanasonic(0x4004, (uint32_t) value);
                }
            }
            send.sendPanasonic(0x4004, (uint32_t) value);
        }
        default:
            break;
    }
    digitalWrite(INFO_PIN, 0);
}