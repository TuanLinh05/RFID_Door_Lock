#include "keypad.h"

const char keypad_matrix[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char Keypad_Get_Pressed_Key(void) {
    // Kéo tất cả các hàng xuống mức thấp
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);

    // Quét hàng 1
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return keypad_matrix[0][0];
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return keypad_matrix[0][1];
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return keypad_matrix[0][2];
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return keypad_matrix[0][3];
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);

    // Quét hàng 2
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return keypad_matrix[1][0];
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return keypad_matrix[1][1];
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return keypad_matrix[1][2];
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return keypad_matrix[1][3];
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);

    // Quét hàng 3
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return keypad_matrix[2][0];
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return keypad_matrix[2][1];
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return keypad_matrix[2][2];
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return keypad_matrix[2][3];
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);

    // Quét hàng 4
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN)) return keypad_matrix[3][0];
    if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN)) return keypad_matrix[3][1];
    if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN)) return keypad_matrix[3][2];
    if (HAL_GPIO_ReadPin(C4_PORT, C4_PIN)) return keypad_matrix[3][3];
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);

    return '\0'; // Trả về null nếu không có phím nào được nhấn
}
