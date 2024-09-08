#include "main.h"

#include "hw.h"
#include "hw_if.h"

void NMI_Handler(void) {
}

void HardFault_Handler(void) {
	while (1) {
	}
}

void MemManager_Handler(void) {
	while (1) {
	}
}

void BusFault_Handler(void) {
	while (1) {
	}
}

void UsageFault_Handler(void) {
	while (1) {
	}
}

void SVCall_Handler(void) {
}

void Debug_Handler(void) {
}

void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
	HAL_IncTick();
}

void EXTI4_Handler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void DMA1_CH1_Handler(void) {
	HAL_DMA_IRQHandler(getUsartDma());
}

void USART1_Handler(void) {
	HAL_UART_IRQHandler(geUart1());
}

void RTC_WKUP_Handler(void) {
	HW_TS_RTC_Wakeup_Handler();
}

void IPCC_C1_RX_IT_Handler(void) {
	HW_IPCC_Rx_Handler();
}

void IPCC_C1_TX_IT_Handler(void) {
	HW_IPCC_Tx_Handler();
}
