//
// Created by Krzysiek on 2018-07-09.
//

#ifndef WIFIREMOTE_IR_HANDLER_H
#define WIFIREMOTE_IR_HANDLER_H

#define CODE_TV_ON 0x10
#define CODE_TV_OFF 0x11
#define CODE_NEC 0xA1
#define CODE_PANASONIC 0xA2

#define CODE_VALUE_PANASONIC_ON 0x1007C7D

void irBegin();

uint8_t addCode(uint8_t id, uint64_t value);

void handleCodes();

void sendCode(uint8_t id, uint64_t value);

#endif //WIFIREMOTE_IR_HANDLER_H
