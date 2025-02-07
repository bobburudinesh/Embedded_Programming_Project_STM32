/*
 * CAN_Tx_Rx_Functionality_LB_Mode.c
 *
 *  Created on: Feb 5, 2025
 *      Author: dinesh bobburu
 */

/*
 * Test CAN Tx and Rx functionality using STM32 CAn(bxCAn) loop-back Mode.
 *
 * */


#include "main.h"
#include"string.h"
#include"stdio.h"

void SystemClockConfig(uint8_t clock_freq);
void UART2_Init(void);
void GPIO_Init(void);
void Error_handler(void);
void Timer2_Init(void);
void CAN1_Init(void);
void CAN1_TX(void);
void CAN1_RX(void);
void CAN_Filter_Config(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef	htimer2;
TIM_HandleTypeDef	htimer6;//[Not USed in this application]
CAN_HandleTypeDef	hcan1;




uint32_t	pulse1_value = 25000;	// To produce 500Hz
uint32_t	pulse2_value = 12500;	// To produce 1000Hz
uint32_t	pulse3_value = 6250;	// To produce 2000Hz
uint32_t	pulse4_value = 3125;	// To produce 4000Hz
uint32_t ccr_Content;

int main(void) {

	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FRQEQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	CAN1_Init(); // Moves can from Sleep to Initialization mode
	// We have to do filter configuration before can start as we have to do all configurations in CAN Initialization phase.
	// We cannot do then after can start state.
	CAN_Filter_Config();

	if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK) {
		Error_handler();
	}


	if(HAL_CAN_Start(&hcan1) != HAL_OK){ // Moves CAN from Initialization mode to STart Mode(REady for OPerations)
		Error_handler();
	}

	CAN1_TX();

	//CAN1_RX(); This can be enabled if polling receiving function has to be used. Commented it as we are using interrupt based receiving

	while(1);
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

void GPIO_Init(void) {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef ledGpio;
	ledGpio.Pin = GPIO_PIN_12;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	ledGpio.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOD, &ledGpio);
}

void CAN1_Init(void) {
	hcan1.Instance = CAN1;
	hcan1.Init.Mode = CAN_MODE_LOOPBACK;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	// Settings related to CAN bit timings
	hcan1.Init.Prescaler = 5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

	if( HAL_CAN_Init(&hcan1) != HAL_OK){
		Error_handler();
	}
}

void CAN1_TX(void) {
	CAN_TxHeaderTypeDef	TxHeader;
	char msg[50];
	uint8_t message[5] = {'H', 'E', 'L', 'L', 'O'};
	uint32_t TxMailBox;
	TxHeader.DLC = 5;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;

	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, message, &TxMailBox) != HAL_OK) {
		Error_handler();
	}
	while(HAL_CAN_IsTxMessagePending(&hcan1, TxMailBox));

	memset(msg,0,sizeof(msg));
	sprintf(msg, "Message Transmitted\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void CAN1_RX(void) {
	char msg[50];
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t recvd_data[5];
	// We are waiting for atleast one message in to the RX FIFO
	while(!HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0));

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, recvd_data) != HAL_OK) {
		Error_handler();
	}
	memset(msg,0,sizeof(msg));
	sprintf(msg, "Message Received: %s\r\n", recvd_data);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void CAN_Filter_Config(void){
	CAN_FilterTypeDef	can1_Filter_Init;

	can1_Filter_Init.FilterIdHigh = 0x0000;
	can1_Filter_Init.FilterIdLow = 0x0000;
	can1_Filter_Init.FilterMaskIdHigh = 0x0000;
	can1_Filter_Init.FilterMaskIdLow = 0x0000;
	can1_Filter_Init.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	can1_Filter_Init.FilterActivation = ENABLE;
	can1_Filter_Init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_Filter_Init.FilterBank = 0;
	can1_Filter_Init.FilterScale = CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(&hcan1, &can1_Filter_Init);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
	char msg[50];
	memset(msg,0,sizeof(msg));
	sprintf(msg, "Message Transmitted: M0\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
	char msg[50];
	memset(msg,0,sizeof(msg));
	sprintf(msg, "Message Transmitted: M1\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
	char msg[50];
	memset(msg,0,sizeof(msg));
	sprintf(msg, "Message Transmitted: M2\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	char msg[50];
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t recvd_data[5];

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, recvd_data) != HAL_OK) {
		Error_handler();
	}
	memset(msg,0,sizeof(msg));
	sprintf(msg, "Message Received: %s\r\n", recvd_data);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
	char msg[50];
	memset(msg,0,sizeof(msg));
	sprintf(msg, "CAN Error Detected... \r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
void Error_handler(void) {
	while(1);
}












