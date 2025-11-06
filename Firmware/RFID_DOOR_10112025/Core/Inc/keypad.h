// File: keypad.h

#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f1xx_hal.h"

// Cấu hình các chân kết nối với bàn phím (ĐÃ CẬP NHẬT)
// Hàng (ROWS) - Cấu hình là OUTPUT
#define R1_PORT GPIOB
#define R1_PIN  GPIO_PIN_12
#define R2_PORT GPIOB
#define R2_PIN  GPIO_PIN_13
#define R3_PORT GPIOB
#define R3_PIN  GPIO_PIN_14
#define R4_PORT GPIOB
#define R4_PIN  GPIO_PIN_15

// Cột (COLS) - Cấu hình là INPUT_PULLUP
#define C1_PORT GPIOA
#define C1_PIN  GPIO_PIN_8
#define C2_PORT GPIOA
#define C2_PIN  GPIO_PIN_9
#define C3_PORT GPIOA
#define C3_PIN  GPIO_PIN_10
#define C4_PORT GPIOA
#define C4_PIN  GPIO_PIN_11

char Keypad_Get_Pressed_Key(void);

#endif // KEYPAD_H
