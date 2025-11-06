/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rc522.h"
#include "string.h"
#include "lcd.h"
#include "DFPlayer_Mini.h"
#include "keypad.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    STATE_IDLE,                 // Trạng thái chờ, quét thẻ VÀ bàn phím
    STATE_ENTERING_PASSWORD,    // Trạng thái đang nhập mật khẩu
    STATE_CARD_ACCESS_GRANTED,  // Trạng thái thẻ đúng
    STATE_CARD_ACCESS_DENIED,   // Trạng thái thẻ sai
    STATE_PASS_ACCESS_GRANTED,  // Trạng thái mật khẩu đúng
    STATE_PASS_ACCESS_DENIED    // Trạng thái mật khẩu sai
} SystemState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CARD_ACCESS_DURATION    5000 // Thời gian mở cửa bằng thẻ: 5s
#define CARD_DENIED_DURATION    3000 // Thời gian báo lỗi thẻ: 3s
#define PASS_ACCESS_DURATION    4000 // Thời gian mở cửa bằng pass: 4s
#define PASS_DENIED_DURATION    3000 // Thời gian báo lỗi pass: 3s
#define PASSWORD_LENGTH         5    // Độ dài mật khẩu
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t status;
uint8_t str[MAX_LEN]; // Max_LEN = 16
uint8_t sNum[4];
uint8_t masterCardUID[4] = {4, 123, 253, 113};

volatile SystemState currentState = STATE_IDLE;
uint32_t actionTimer = 0;

const char correctPassword[PASSWORD_LENGTH + 1] = "12345"; // Mật khẩu đúng + ký tự null
char enteredPassword[PASSWORD_LENGTH + 1];
uint8_t passwordIndex = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void display_welcome_screen(void);
void reset_password_entry(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display_welcome_screen(void) {
    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string("Quet the hoac");
    lcd_put_cur(1, 0);
    lcd_send_string("nhap mat khau");
}

void reset_password_entry(void) {
    passwordIndex = 0;
    memset(enteredPassword, 0, sizeof(enteredPassword));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  MFRC522_Init();
  lcd_init();
  DF_Init(&huart1);


  HAL_Delay(200); // Chờ một chút để module sẵn sàng
  DF_SetVolume(25); // Đặt âm lượng (từ 0 đến 30)


  display_welcome_screen();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	switch(currentState)
//	{
//		case STATE_IDLE_SCANNING:
//		{
//			// Chỉ quét thẻ khi hệ thống đang ở trạng thái chờ
//			if (MFRC522_Request(PICC_REQIDL, str) == MI_OK)
//			{
//				if (MFRC522_Anticoll(str) == MI_OK)
//				{
//					memcpy(sNum, str, 4);
//					if (memcmp(sNum, masterCardUID, 4) == 0)
//					{
//						// === THẺ ĐÚNG ===
//						// 1. Chuyển sang trạng thái Mở cửa
//						currentState = STATE_ACCESS_GRANTED;
//						// 2. Ghi lại mốc thời gian bắt đầu
//						actionTimer = HAL_GetTick();
//
//						// 3. Thực hiện hành động MỞ CỬA ngay lập tức
//						lcd_clear();
//						lcd_put_cur(0, 0);
//						lcd_send_string("The dung,");
//						lcd_put_cur(1, 2);
//						lcd_send_string("xin moi vao");
//						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); // Bật Relay
//					}
//					else
//					{
//						// === THẺ SAI ===
//						// 1. Chuyển sang trạng thái Báo lỗi
//						currentState = STATE_ACCESS_DENIED;
//						// 2. Ghi lại mốc thời gian bắt đầu
//						actionTimer = HAL_GetTick();
//
//						// 3. Hiển thị thông báo lỗi
//						lcd_clear();
//						lcd_put_cur(0, 4);
//						lcd_send_string("The sai!");
//					}
//				}
//			}
//			break;
//		}
//
//		case STATE_ACCESS_GRANTED:
//		{
//			// Liên tục kiểm tra xem đã đủ 3 giây chưa
//			if (HAL_GetTick() - actionTimer >= ACCESS_GRANTED_DURATION)
//			{
//				// Đã đủ 3 giây, thực hiện hành động ĐÓNG CỬA
//				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); // Tắt Relay
//				display_welcome_screen(); // Hiển thị lại màn hình chờ
//				currentState = STATE_IDLE_SCANNING; // Quay về trạng thái chờ
//			}
//			break;
//		}
//
//		case STATE_ACCESS_DENIED:
//		{
//			// Liên tục kiểm tra xem đã đủ 2 giây hiển thị lỗi chưa
//			if (HAL_GetTick() - actionTimer >= ACCESS_DENIED_DURATION)
//			{
//				// Đã đủ thời gian, quay về trạng thái chờ
//				display_welcome_screen();
//				currentState = STATE_IDLE_SCANNING;
//			}
//			break;
//		}
//	} // end switch

//		switch(currentState)
//		{
//			case STATE_IDLE_SCANNING:
//			{
//				if (MFRC522_Request(PICC_REQIDL, str) == MI_OK)
//				{
//					if (MFRC522_Anticoll(str) == MI_OK)
//					{
//						memcpy(sNum, str, 4);
//						if (memcmp(sNum, masterCardUID, 4) == 0)
//						{
//							// === THẺ ĐÚNG ===
//							currentState = STATE_ACCESS_GRANTED;
//							actionTimer = HAL_GetTick();
//
//							lcd_clear();
//							lcd_put_cur(0, 0);
//							lcd_send_string("The dung,");
//							lcd_put_cur(1, 2);
//							lcd_send_string("xin moi vao");
//							HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
//							DF_Play(1); // <-- PHÁT FILE 0001.mp3
//						}
//						else
//						{
//							// === THẺ SAI ===
//							currentState = STATE_ACCESS_DENIED;
//							actionTimer = HAL_GetTick();
//
//							lcd_clear();
//							lcd_put_cur(0, 4);
//							lcd_send_string("The sai!");
//							DF_Play(2); // <-- PHÁT FILE 0002.mp3
//						}
//					}
//				}
//				break;
//			}
//
//			case STATE_ACCESS_GRANTED:
//			{
//				if (HAL_GetTick() - actionTimer >= ACCESS_GRANTED_DURATION)
//				{
//					HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
//					display_welcome_screen();
//					currentState = STATE_IDLE_SCANNING;
//				}
//				break;
//			}
//
//			case STATE_ACCESS_DENIED:
//			{
//				if (HAL_GetTick() - actionTimer >= ACCESS_DENIED_DURATION)
//				{
//					display_welcome_screen();
//					currentState = STATE_IDLE_SCANNING;
//				}
//				break;
//			}
//		} // end switch
//
//  }

	  switch(currentState)
	  	{
	  		case STATE_IDLE:
	  		{
	  			// ---- Ưu tiên 1: Quét thẻ ----
	  			if (MFRC522_Request(PICC_REQIDL, str) == MI_OK)
	  			{
	  				if (MFRC522_Anticoll(str) == MI_OK)
	  				{
	  					// Kêu bíp báo hiệu nhận thẻ
	  					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET); HAL_Delay(50);
	  					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

	  					memcpy(sNum, str, 4);
	  					if (memcmp(sNum, masterCardUID, 4) == 0)
	  					{
	  						currentState = STATE_CARD_ACCESS_GRANTED;
	  						actionTimer = HAL_GetTick();
	  						lcd_clear();
	  						lcd_put_cur(0, 0); lcd_send_string("The dung,");
	  						lcd_put_cur(1, 2); lcd_send_string("xin moi vao");
	  						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
	  						DF_Play(1); // Phát âm thanh thẻ đúng
	  					}
	  					else
	  					{
	  						currentState = STATE_CARD_ACCESS_DENIED;
	  						actionTimer = HAL_GetTick();
	  						lcd_clear();
	  						lcd_put_cur(0, 4); lcd_send_string("The sai!");
	  						DF_Play(2); // Phát âm thanh thẻ sai
	  					}
	  				}
	  			}

	  			// ---- Ưu tiên 2: Quét bàn phím ----
	  			char key = Keypad_Get_Pressed_Key();
	  			if (key != '\0') // Nếu có phím được nhấn
	  			{
	  				// Kêu bíp báo hiệu nhấn phím
	  				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET); HAL_Delay(50);
	  				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

	  				// Nếu là một chữ số thì bắt đầu nhập mật khẩu
	  				if (key >= '0' && key <= '9')
	  				{
	  					currentState = STATE_ENTERING_PASSWORD;
	  					reset_password_entry();
	  					enteredPassword[passwordIndex++] = key;

	  					lcd_clear();
	  					lcd_put_cur(0, 0); lcd_send_string("Mat khau la:");
	  					lcd_put_cur(1, 0); lcd_send_string("*");
	  				}
	  			}
	  			break;
	  		}

	  		case STATE_ENTERING_PASSWORD:
	  		{
	  			// Vô hiệu hóa quét thẻ khi đang nhập pass
	  			char key = Keypad_Get_Pressed_Key();

	  			// Chỉ chấp nhận các phím số
	  			if (key != '\0' && (key >= '0' && key <= '9'))
	  			{
	  				// Kêu bíp báo hiệu nhấn phím
	  				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET); HAL_Delay(50);
	  				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

	  				if (passwordIndex < PASSWORD_LENGTH)
	  				{
	  					enteredPassword[passwordIndex++] = key;
	  					// Hiển thị '*'
	  					lcd_put_cur(1, passwordIndex - 1);
	  					lcd_send_data('*');
	  				}

	  				// Khi đã đủ 5 ký tự
	  				if (passwordIndex == PASSWORD_LENGTH)
	  				{
	  					if (strcmp(enteredPassword, correctPassword) == 0)
	  					{
	  						currentState = STATE_PASS_ACCESS_GRANTED;
	  						actionTimer = HAL_GetTick();
	  						lcd_clear();
	  						lcd_put_cur(0, 0); lcd_send_string("Mat khau chinh xac");
	  						lcd_put_cur(1, 2); lcd_send_string("Xin moi vao");
	  						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
	  						DF_Play(3); // Phát âm thanh pass đúng
	  					}
	  					else
	  					{
	  						currentState = STATE_PASS_ACCESS_DENIED;
	  						actionTimer = HAL_GetTick();
	  						lcd_clear();
	  						lcd_put_cur(0, 0); lcd_send_string("Mat khau sai,");
	  						lcd_put_cur(1, 0); lcd_send_string("xin hay nhap lai");
	  						DF_Play(4); // Phát âm thanh pass sai
	  					}
	  				}
	  			}
	  			break;
	  		}

	  		case STATE_CARD_ACCESS_GRANTED:
	  		{
	  			if (HAL_GetTick() - actionTimer >= CARD_ACCESS_DURATION) {
	  				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
	  				display_welcome_screen();
	  				currentState = STATE_IDLE;
	  			}
	  			break;
	  		}
	  		case STATE_CARD_ACCESS_DENIED:
	  		{
	  			if (HAL_GetTick() - actionTimer >= CARD_DENIED_DURATION) {
	  				display_welcome_screen();
	  				currentState = STATE_IDLE;
	  			}
	  			break;
	  		}
	  		case STATE_PASS_ACCESS_GRANTED:
	  		{
	  			if (HAL_GetTick() - actionTimer >= PASS_ACCESS_DURATION) {
	  				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
	  				display_welcome_screen();
	  				currentState = STATE_IDLE;
	  			}
	  			break;
	  		}
	  		case STATE_PASS_ACCESS_DENIED:
	  		{
	  			if (HAL_GetTick() - actionTimer >= PASS_DENIED_DURATION) {
	  				display_welcome_screen();
	  				currentState = STATE_IDLE;
	  			}
	  			break;
	  		}
	  	} // end switch
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RST_Pin|ROW_1_Pin|ROW_2_Pin|ROW_3_Pin
                          |ROW_4_Pin|RELAY_Pin|BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RST_Pin ROW_1_Pin ROW_2_Pin ROW_3_Pin
                           ROW_4_Pin RELAY_Pin BUZZER_Pin */
  GPIO_InitStruct.Pin = RST_Pin|ROW_1_Pin|ROW_2_Pin|ROW_3_Pin
                          |ROW_4_Pin|RELAY_Pin|BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : COL_1_Pin COL_2_Pin COL_3_Pin COL_4_Pin */
  GPIO_InitStruct.Pin = COL_1_Pin|COL_2_Pin|COL_3_Pin|COL_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
