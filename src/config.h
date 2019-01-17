//
// Created by Krzysiek on 2018-07-09.
//

#ifndef WIFIREMOTE_CONFIG_H
#define WIFIREMOTE_CONFIG_H

#define VERSION_CODE 1
#define VERSION_NAME "0.1"
#define BUILD_DATE (String(__TIME__)+"@" + __DATE__)
#define IR_PIN 4
#define INFO_PIN 5
#define QUE_LENGTH 32

#ifndef AP_NAME
#define AP_NAME "IR Remote"
#endif

#ifndef DEVICE_ID
#define DEVICE_ID "ir_remote"
#endif

#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#define PRINTLN(a) Serial.println(a)
#define PRINT(a) Serial.print(a)
#else
#define PRINTLN(a)
#define PRINT(a)
#endif

#endif //WIFIREMOTE_CONFIG_H
