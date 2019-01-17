//
// Created by Krzysiek on 2018-07-09.
//

#ifndef WIFIREMOTE_IR_HANDLER_H
#define WIFIREMOTE_IR_HANDLER_H

#define CODE_TV_TOGGLE 0x10
#define PROTOCOL_NEC 0xA1
#define PROTOCOL_PANASONIC 0xA2

#define CODE_VALUE_PANASONIC_ON 0x1007C7D
#define CODE_VALUE_PANASONIC_TOGGLE 0x100BCBD
#define CODE_VALUE_PANASONIC_HDMI2 0x1208DAC
#define CODE_VALUE_PIONEER_ON 0xA55A58A7
#define CODE_VALUE_PIONEER_TOGGLE 0xA55A38C7

void irBegin();

uint8_t addCode(uint8_t id, uint64_t value);

void handleCodes();

void sendCode(uint8_t id, uint64_t value);

#endif //WIFIREMOTE_IR_HANDLER_H
