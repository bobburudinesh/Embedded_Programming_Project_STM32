/*
 * msp.c
 *
 *  Created on: Jan 28, 2025
 *      Author: dineshbobburu
 */
#include "main.h"

void HAL_MspInit(void) {
	// Here we will do low level processor specific init's.
	// 1. Setup the priority grouping of arm cortex mx procesor.
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the requires system exceptions of the arm cortex mx processor.
	SCB->SHCSR |= 0x7<<16; // Enabled usage fault, bus fault and memory fault system exceptions

	//3. configure the priority for the system exceptions.

	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
 // here we are going to do the low level initializations of USART2 peripheral.
	//1. Enable the clock for USART2 Peri.
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//2. DO the pin muxing configurations.
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2; // UART2_TX
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	gpio_uart.Pin = GPIO_PIN_3; // UART2_RX
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	//3. Enable the IRQ and set the priority(NVIC Settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer) {
	// 1. Enable the clock for the timer 6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2. Enable IRQ of TIM6;

	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. Setup the priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);


}
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
	//1. ENable the clock for Timer2

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. COnfigure GPIO as Tim 2 Channel 1 alternate function
	GPIO_InitTypeDef tim2Channel;
	tim2Channel.Mode = GPIO_MODE_AF_PP;
	tim2Channel.Alternate = GPIO_AF1_TIM2;
	tim2Channel.Pull = GPIO_NOPULL;
	tim2Channel.Speed = GPIO_SPEED_FREQ_LOW;
	tim2Channel.Pin = GPIO_PIN_5; //PA0 and PA1
	HAL_GPIO_Init(GPIOA, &tim2Channel);




	//3. NVIC Settings
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim) {
	//1. ENable the clock for Timer2

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	//2. COnfigure GPIO as Tim 2 Channel 1,2,3,4 alternate function
	GPIO_InitTypeDef tim2Channel;
	/*
	 * PA0  -  TIM2_CH1
	 * PA1  -  TIM2_CH2
	 * PB10  -  TIM2_CH3
	 * PB2  -  TIM2_CH4
	 * */

	tim2Channel.Mode = GPIO_MODE_AF_PP;
	tim2Channel.Alternate = GPIO_AF1_TIM2;
	tim2Channel.Pull = GPIO_NOPULL;
	tim2Channel.Speed = GPIO_SPEED_FREQ_LOW;
	tim2Channel.Pin = GPIO_PIN_0 | GPIO_PIN_1; //PA0 and PA1
	HAL_GPIO_Init(GPIOA, &tim2Channel);
	tim2Channel.Mode = GPIO_MODE_AF_PP;
		tim2Channel.Alternate = GPIO_AF1_TIM2;
		tim2Channel.Pull = GPIO_NOPULL;
		tim2Channel.Speed = GPIO_SPEED_FREQ_LOW;
	tim2Channel.Pin = GPIO_PIN_2 | GPIO_PIN_10; //PB2 and PB10
	HAL_GPIO_Init(GPIOB, &tim2Channel);



	//3. NVIC Settings
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
	//1. ENable the clock for Timer2

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	//2. COnfigure GPIO as Tim 2 Channel 1,2,3,4 alternate function
	GPIO_InitTypeDef tim2Channel;
	/*
	 * PA0  -  TIM2_CH1
	 * PA1  -  TIM2_CH2
	 * PB10  -  TIM2_CH3
	 * PB2  -  TIM2_CH4
	 * */

	tim2Channel.Mode = GPIO_MODE_AF_PP;
	tim2Channel.Alternate = GPIO_AF1_TIM2;
	tim2Channel.Pull = GPIO_NOPULL;
	tim2Channel.Speed = GPIO_SPEED_FREQ_LOW;
	tim2Channel.Pin = GPIO_PIN_0 | GPIO_PIN_1; //PA0 and PA1
	HAL_GPIO_Init(GPIOA, &tim2Channel);
	tim2Channel.Mode = GPIO_MODE_AF_PP;
		tim2Channel.Alternate = GPIO_AF1_TIM2;
		tim2Channel.Pull = GPIO_NOPULL;
		tim2Channel.Speed = GPIO_SPEED_FREQ_LOW;
	tim2Channel.Pin = GPIO_PIN_2 | GPIO_PIN_10; //PB2 and PB10
	HAL_GPIO_Init(GPIOB, &tim2Channel);



	//3. NVIC Settings
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef	*hcan) {
	GPIO_InitTypeDef canGPIOInit;
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	canGPIOInit.Mode = GPIO_MODE_AF_PP;
	canGPIOInit.Pull = GPIO_NOPULL;
	canGPIOInit.Alternate = GPIO_AF9_CAN1;
	canGPIOInit.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	canGPIOInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &canGPIOInit);


	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);

	HAL_NVIC_SetPriority(CAN2_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 15, 0);
}
