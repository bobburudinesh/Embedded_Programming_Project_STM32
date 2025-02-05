/*
 * Timers2_GeneralPurpose_OC_Mode_PWM.c
 *
 *  Created on: Feb 4, 2025
 *      Author: dinesh bobburu
 */


/*
 * Exercise: PWM:
 * 25%,45%, 75%, 90% on Channel 1, Channel 2, Channel 3, Channel 4.
 * */

#include "main.h"
#include"string.h"
#include"stdio.h"

void SystemClockConfig(uint8_t clock_freq);
void GPIO_Init(void);
void Error_handler(void);
void Timer2_Init(void);



UART_HandleTypeDef huart2;
TIM_HandleTypeDef	htimer2;
TIM_HandleTypeDef	htimer6;//[Not USed in this application]




uint32_t	pulse1_value = 25000;	// To produce 500Hz
uint32_t	pulse2_value = 12500;	// To produce 1000Hz
uint32_t	pulse3_value = 6250;	// To produce 2000Hz
uint32_t	pulse4_value = 3125;	// To produce 4000Hz
uint32_t ccr_Content;

int main(void) {

	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FRQEQ_50_MHZ);
	GPIO_Init();
	Timer2_Init();
	if (HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_2) != HAL_OK){
			Error_handler();
		}
	if (HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_3) != HAL_OK){
			Error_handler();
		}
	if (HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_4) != HAL_OK){
			Error_handler();
		}
	while(1);
	return 0;
}


void SystemClockConfig(uint8_t clock_freq) {
	RCC_OscInitTypeDef	osc_init;
	RCC_ClkInitTypeDef	clk_init;
	uint32_t flashLatency = 0;
	char	msg[100];
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI |  RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	osc_init.LSEState = RCC_LSE_ON;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSEState = RCC_HSE_ON;
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
	if (HAL_RCC_ClockConfig(&clk_init, flashLatency) != HAL_OK){
		Error_handler();
	}

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
	TIM_OC_InitTypeDef tim2PWM_init;


	htimer2.Instance = TIM2;
	htimer2.Init.Period = 10000-1;
	htimer2.Init.Prescaler = 4999;
	if(HAL_TIM_PWM_Init(&htimer2) != HAL_OK){
		Error_handler();
	}
	memset(&tim2PWM_init,0,sizeof(tim2PWM_init));
	tim2PWM_init.OCMode = TIM_OCMODE_PWM1;
	tim2PWM_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2PWM_init.Pulse = (htimer2.Init.Period * 25)/100;

	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_init, TIM_CHANNEL_1) !=  HAL_OK){
		Error_handler();
	}
	tim2PWM_init.Pulse = (htimer2.Init.Period * 45)/100;

	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_init, TIM_CHANNEL_2) !=  HAL_OK){
		Error_handler();
	}
	tim2PWM_init.Pulse = (htimer2.Init.Period * 75)/100;

	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_init, TIM_CHANNEL_3) !=  HAL_OK){
		Error_handler();
	}
	tim2PWM_init.Pulse = (htimer2.Init.Period * 90)/100;

	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_init, TIM_CHANNEL_4) !=  HAL_OK){
		Error_handler();
	}

}



void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
	// Timer2 CHANNEL 1 - 500Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_Content+pulse1_value);
	}
	// Timer2  CHANNEL 2 - 1000Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_Content+pulse2_value);

	}
	// Timer2 CHANNEL 3 - 2000Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_Content+pulse3_value);

		}
	// Timer2 CHANNEL 4 - 4000Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_Content+pulse4_value);

		}
}



void Error_handler(void) {
	while(1);
}










