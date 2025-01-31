/*
 * main_app.c
 *
 *  Created on: Jan 28, 2025
 *      Author: dineshbobburu
 */


#include "main_app.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "string.h"

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef	huart2;

char *user_data = "Hello World\r\n";


int main(void) {
	HAL_Init();
	SystemClockConfig();
	UART2_Init();
	HAL_UART_MspInit(&huart2);
	USART2->BRR = 0x016C;
	HAL_UART_Transmit(&huart2, (uint8_t *)user_data, strlen(user_data), HAL_MAX_DELAY);
	return 0;
}

void SystemClockConfig(void) {

}

void UART2_Init(void){
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1 ;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		// There is a problem
		Error_handler();
	}
}

void Error_handler(void) {

}
