/*
 * Timers_2_General_Purpose_Timer.c
 *
 *  Created on: Feb 2, 2025
 *      Author: dinesh bobburu
 */

/*
 * Exercise: Input CAPTURE MODE:
 * INPUT CAPTURE MODE: To measure the period of clock signal.
 * */

#include "main.h"
#include"string.h"
#include"stdio.h"

void SystemClockConfig(uint8_t clock_freq);
void GPIO_Init(void);
void Error_handler(void);
void Timer2_Init(void);
void LSE_Configuration(void);


UART_HandleTypeDef huart2;
TIM_HandleTypeDef	htimer2;
TIM_HandleTypeDef	htimer6;//[Not USed in this application]


uint32_t input_capture[2] = {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;



int main(void) {
	uint32_t capture_difference;
	double timer2_cnt_freq = 0;
	double timer2_cnt_resolution = 0;
	double user_signal_time_period = 0;
	double user_signal_freq = 0;
	char user_msg[100];
	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FRQEQ_50_MHZ);
	GPIO_Init();
	Timer2_Init();
	LSE_Configuration();
	HAL_TIM_IC_Start_IT(&htimer2, TIM_CHANNEL_1);
	while(1) {
		if(is_capture_done) {
			if (input_capture[1] >input_capture[0]) {
				capture_difference = input_capture[1] - input_capture[0];
			} else {
				capture_difference = ( 0xFFFFFFFF - input_capture[0]) + input_capture[1];
			}
			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq()*2)/htimer2.Init.Prescaler;
			timer2_cnt_resolution = 1/timer2_cnt_freq;
			user_signal_time_period = capture_difference * timer2_cnt_resolution;
			user_signal_freq = 1/user_signal_time_period;
			sprintf(user_msg,"Frequency of the signal applied  = %f\r\n", user_signal_freq);
			is_capture_done = FALSE;
		}

	}

	return 0;
}


void SystemClockConfig(uint8_t clock_freq) {
	RCC_OscInitTypeDef	osc_init;
	RCC_ClkInitTypeDef	clk_init;
	uint32_t flashLatency = 0;
	char	msg[100];
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI |  RCC_OSCILLATORTYPE_LSE;
	osc_init.LSEState = RCC_LSE_ON;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch (clock_freq) {
		case SYS_CLOCK_FRQEQ_50_MHZ:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			//osc_init.PLL.PLLR = 2;
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider	= RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
			flashLatency = FLASH_LATENCY_1;
			break;
		case SYS_CLOCK_FRQEQ_84_MHZ:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			//osc_init.PLL.PLLR = 2;
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider	= RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
			flashLatency = FLASH_LATENCY_2;
			break;
		case SYS_CLOCK_FRQEQ_120_MHZ:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			//osc_init.PLL.PLLR = 2;
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider	= RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
			flashLatency = FLASH_LATENCY_3;
			break;
		default:
			return;
	}
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_handler();
	}
	HAL_RCC_ClockConfig(&clk_init, flashLatency);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	memset(msg,0,sizeof(msg));
	sprintf(msg, "SYSCLK : %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg, "HCLK : %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg, "PCLK1 : %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg, "PCLK2 : %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);


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

void Timer2_Init(void){
	TIM_IC_InitTypeDef timer2IC_Config;
	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if(HAL_TIM_IC_Init(&htimer2) != HAL_OK) {
		Error_handler();
	}
	timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_Config.ICFilter = 0;

	HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_Config, TIM_CHANNEL_1);

}

void LSE_Configuration(void) {

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if(!is_capture_done) {
		if (count == 1) {
			input_capture[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);

		}
		else if (count == 2) {
			input_capture[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count = 1;
			is_capture_done = TRUE;
		}
	}

}

void Error_handler(void) {
	while(1);
}








