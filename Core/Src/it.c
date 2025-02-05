/*
 * it.c
 *
 *  Created on: Jan 28, 2025
 *      Author: dineshbobburu
 */

#include "main.h"

extern UART_HandleTypeDef	huart2;
extern TIM_HandleTypeDef	htimer6;
extern TIM_HandleTypeDef	htimer2;

void SysTick_Handler (void) {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart2);
}

void TIM6_DAC_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htimer6);
}

void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htimer2);
}
