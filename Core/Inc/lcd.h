#ifndef LCD_H_
#define LCD_H_

#include "stm32f4xx_hal.h"

#define RS_Pin GPIO_PIN_0
#define RS_Port GPIOB
#define EN_Pin GPIO_PIN_1
#define EN_Port GPIOB

#define D4_Pin GPIO_PIN_2
#define D4_Port GPIOB
#define D5_Pin GPIO_PIN_10
#define D5_Port GPIOB
#define D6_Pin GPIO_PIN_12
#define D6_Port GPIOB
#define D7_Pin GPIO_PIN_14
#define D7_Port GPIOB

void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);
void LCD_SendFloat(float num);
void LCD_SendInt(int num);
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif
