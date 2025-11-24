#include "lcd.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>


static void LCD_EnablePulse() {
    HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(EN_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

static void LCD_Send4Bits(uint8_t data) {
    HAL_GPIO_WritePin(D4_Port, D4_Pin, (data >> 0) & 0x01);
    HAL_GPIO_WritePin(D5_Port, D5_Pin, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(D6_Port, D6_Pin, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(D7_Port, D7_Pin, (data >> 3) & 0x01);
}

void LCD_SendCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(RS_Port, RS_Pin, GPIO_PIN_RESET);

    LCD_Send4Bits(cmd >> 4);
    LCD_EnablePulse();

    LCD_Send4Bits(cmd & 0x0F);
    LCD_EnablePulse();

    HAL_Delay(2);
}

void LCD_SendData(uint8_t data) {
    HAL_GPIO_WritePin(RS_Port, RS_Pin, GPIO_PIN_SET);

    LCD_Send4Bits(data >> 4);
    LCD_EnablePulse();

    LCD_Send4Bits(data & 0x0F);
    LCD_EnablePulse();
}

void LCD_SendString(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}

void LCD_Init() {
    HAL_Delay(50);

    LCD_Send4Bits(0x03);
    LCD_EnablePulse();
    HAL_Delay(5);

    LCD_Send4Bits(0x03);
    LCD_EnablePulse();
    HAL_Delay(1);

    LCD_Send4Bits(0x02); // 4-bit mode
    LCD_EnablePulse();

    LCD_SendCommand(0x28); // 4-bit, 2-line, 5x8 dots
    LCD_SendCommand(0x0C); // Display on, cursor off
    LCD_SendCommand(0x06); // Shift cursor right
    LCD_SendCommand(0x01); // Clear
    HAL_Delay(2);
}
void LCD_SendInt(int num) {
    char buffer[12];
    sprintf(buffer, "%d", num);
    LCD_SendString(buffer);
}

void LCD_SendFloat(float num) {
    int int_part = (int)num;
    int dec_part = (int)((num - int_part) * 10);  // 1 decimal place

    if (dec_part < 0) dec_part = -dec_part; // handle negative numbers

    LCD_SendInt(int_part);
    LCD_SendString(".");
    LCD_SendInt(dec_part);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};

    if (row > 3) row = 3;     // limit for 20x4
    if (col > 19) col = 19;   // limit for 20 characters

    LCD_SendCommand(0x80 | (col + row_offsets[row]));
}

