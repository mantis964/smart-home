#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stm32f4xx_hal.h"

typedef struct {
    float Temperature;
    float Humidity;
} DHT22_Data;

uint8_t DHT22_Read(DHT22_Data *data);

#endif
