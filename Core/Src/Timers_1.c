/*
 * Timers_1.c
 *
 *  Created on: Feb 2, 2025
 *      Author: dinesh bobburu
 */

/*
 * ********************EXERCISE*****************
 * Use the basic timer to generate interrupt for every 100ms.
 * Toggle the GPIO or LED inside the timer IRQ handler and verify using logic analyzer.
 *
 * */

// We are going to use Timer 6 and Timer 7


#include"main.h"
#include"string.h"

void SystemClockConfig(void);
void TIMER6_Init(void);
void GPIO_Init(void);
void Error_handler(void);


UART_HandleTypeDef huart2;
TIM_HandleTypeDef	htimer6;


int main(void) {
	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	TIMER6_Init();



	// lets start timer in interrupt CONFIG
	HAL_TIM_Base_Start_IT(&htimer6);
	while(1);

	return 0;
}

void SystemClockConfig(void) {


}

void GPIO_Init(void) {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef ledGpio;
	ledGpio.Pin = GPIO_PIN_12;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	ledGpio.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOD, &ledGpio);
}

void TIMER6_Init(void) {
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24;
	htimer6.Init.Period = 64000-1;
	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK) {
		Error_handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// When update events happens this interrupt callback is called
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}

void Error_handler(void) {
	while(1);
}
