/*
 * Low_Power_Mode_1.c
 *
 *  Created on: Feb 7, 2025
 *      Author: dineshbobburu
 */



#include"main.h"


void SystemClockConfig(uint8_t clock_freq);
void UART2_Init(void);
void GPIO_Init(void);
void Timer6_Init(void);
void Error_handler(void);

UART_HandleTypeDef	huart2;
TIM_HandleTypeDef	htimer2;
TIM_HandleTypeDef	htimer6;
CAN_HandleTypeDef	hcan1;


int main(void) {
	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FRQEQ_50_MHZ);
	UART2_Init();
	Timer6_Init();
	GPIO_Init();
	HAL_TIM_Base_Start_IT(&htimer6);

	HAL_PWR_EnableSleepOnExit();

	return 0;
}


void SystemClockConfig(uint8_t clock_freq) {
	RCC_OscInitTypeDef	osc_init;
	RCC_ClkInitTypeDef	clk_init;
	uint32_t flashLatency = 0;
	char	msg[100];
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch (clock_freq) {
		case SYS_CLOCK_FRQEQ_50_MHZ:
			osc_init.PLL.PLLM = 8;
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
			osc_init.PLL.PLLM = 8;
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
			osc_init.PLL.PLLM = 8;
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

void UART2_Init(void) {
	UART_InitTypeDef	uart2_Init;
	huart2.Instance = USART2;
	uart2_Init.BaudRate = 152000;
	uart2_Init.Mode = UART_MODE_TX_RX;
	uart2_Init.StopBits = UART_STOPBITS_1;
	uart2_Init.WordLength = UART_WORDLENGTH_8B;
	uart2_Init.Parity = UART_PARITY_NONE;
	uart2_Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init = uart2_Init;
	if(HAL_UART_Init(&huart2) != HAL_OK) {
		Error_handler();
	}
}

void Timer6_Init(void) {

	htimer6.Instance = TIM6;

	htimer6.Init.Prescaler = 0;
	htimer6.Init.Period = 50000-1;

	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK) {
		Error_handler();
	}
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//	char	msg[100];
//	memset(msg,0,sizeof(msg));
//	sprintf(msg, "Processor Woke Up\r\n");
//	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}

void Error_handler(void) {
	while(1);
}



