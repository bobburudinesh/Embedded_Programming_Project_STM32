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
extern CAN_HandleTypeDef	hcan1;

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

void CAN1_TX_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX0_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}
void CAN1_RX1_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}
void CAN1_SCE_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


