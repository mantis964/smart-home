/*
 * dht22.c
 *
 *  Created on: Nov 26, 2025
 *      Author: Tasnim
 */


#include "dht22.h"

extern TIM_HandleTypeDef htim2;

#define DHT_PORT GPIOB
#define DHT_PIN  GPIO_PIN_5

void delay_us(uint16_t time)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (__HAL_TIM_GET_COUNTER(&htim2) < time);
}

uint8_t DHT22_Read(DHT22_Data *data)
{
    uint8_t bits[5] = {0};
    uint32_t i, j = 0;

    // Set pin as output
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);

    // Start signal
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    HAL_Delay(2);  // > 1ms
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);

    delay_us(30);

    // Switch to input
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);

    delay_us(40);
    if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) return 1;
    delay_us(80);
    if (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) return 1;

    while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));

    // Read 40 bits
    for (i = 0; i < 40; i++)
    {
        while (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
        delay_us(40);

        if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
            bits[j/8] |= (1 << (7 - (j % 8)));

        while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
        j++;
    }

    data->Humidity = (bits[0] << 8 | bits[1]) / 10.0;
    data->Temperature = (bits[2] << 8 | bits[3]) / 10.0;

    return 0; // OK
}
