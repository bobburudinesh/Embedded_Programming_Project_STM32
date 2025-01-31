/*
 * UART_Interrupt_Based.c
 *
 *  Created on: Jan 31, 2025
 *      Author: bobbu
 */


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

#define TRUE	1
#define	 FALSE 	0

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef	huart2;

char *initial_data = "Application is running \r\n Please Enter data and press enter\r\n";
uint8_t data_buffer[100];
uint8_t	recvd_data;
uint32_t count;
uint8_t reception_complete;


int main(void) {
	HAL_Init();
	SystemClockConfig();
	UART2_Init();
	HAL_UART_MspInit(&huart2);
	USART2->BRR = 0x016C;
	HAL_UART_Transmit(&huart2, (uint8_t *)initial_data, strlen(initial_data), HAL_MAX_DELAY);
	while(!reception_complete) {
		HAL_UART_Receive_IT(&huart2, &recvd_data, 1);
	}

	while(1);
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (recvd_data == '\r') {
		reception_complete = TRUE;
		data_buffer[count++] = '\r';
		HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
	} else {
		data_buffer[count++] = recvd_data;
	}

}

void Error_handler(void) {

}
