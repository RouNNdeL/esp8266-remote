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
uint8_t raw_que[QUE_LENGTH][MAX_RAW_SIZE];
uint8_t que_length = 0;

IRsend send(IR_PIN);

uint8_t char2int(char input) {
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input + 10 - 'A';
    if(input >= 'a' && input <= 'f')
        return input + 10 - 'a';
    return 0;
}

void irBegin() {
    send.begin();
}

uint8_t addCode(uint8_t id, uint64_t value) {
    if(que_length < QUE_LENGTH) {
        code_que[que_length] = value;
        id_que[que_length] = id;
        que_length++;
        return 1;
    }
    return 0;
}

uint8_t addRaw(String raw) {
    if(que_length < QUE_LENGTH) {
        code_que[que_length] = 0;
        id_que[que_length] = PROTOCOL_RAW36K;
        for(int i = 0; i < MAX_RAW_SIZE; ++i) {
            uint8_t c1 = (uint8_t) raw.charAt(i * 2);
            uint8_t c2 = (uint8_t) raw.charAt(i * 2 + 1);

            raw_que[que_length][i] = char2int(c1) << 4 | char2int(c2);
        }
        que_length++;
        return 1;
    }
    return 0;
}

void handleCodes() {
    while(que_length) {
        sendCode();
        que_length--;

        memmove(id_que, id_que + 1, que_length * sizeof(uint8_t));
        memmove(code_que, code_que + 1, que_length * sizeof(uint64_t));
        memmove(raw_que, raw_que + MAX_RAW_SIZE, que_length * sizeof(uint8_t) * MAX_RAW_SIZE);
    }
}

void sendCode() {
    digitalWrite(INFO_PIN, 1);
    uint8_t id = id_que[0];
    uint64_t value = code_que[0];
    switch(id) {
        case PROTOCOL_NEC: {
            if(value == CODE_VALUE_PIONEER_ON || value == CODE_VALUE_PIONEER_TOGGLE) {
                send.sendNEC(value);
                PRINTLN("Sending NEC: " + String((unsigned long) value, 16));
                delay(10);
            }
            send.sendNEC(value);
            PRINTLN("Sending NEC: " + String((unsigned long) value, 16));
            break;
        }
        case PROTOCOL_PANASONIC: {
            /* Panasonic TVs require for holding the button in order to turn  on the tv */
            if(value == CODE_VALUE_PANASONIC_ON || value == CODE_VALUE_PANASONIC_TOGGLE) {
                for(int i = 0; i < 6; ++i) {
                    send.sendPanasonic(0x4004, (uint32_t) value);
                }
            }
            send.sendPanasonic(0x4004, (uint32_t) value);
            PRINTLN("Sending Panasonic: " + String((unsigned long) value, 16));
            break;
        }
        case PROTOCOL_RAW36K: {
            uint16_t raw_bytes[MAX_RAW_SIZE] = {0};
            uint16 length = 0;
            for(int i = 0; i < MAX_RAW_SIZE; ++i) {
                raw_bytes[i] = raw_que[0][i] * 50;
                if(raw_que[0][i]) {
                    length++;
                }
            }
            send.sendRaw(raw_bytes, length, 36);
        }
        default: {
            break;
        }
    }
    delay(10);
    digitalWrite(INFO_PIN, 0);
}