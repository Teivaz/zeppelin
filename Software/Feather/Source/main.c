#include "main.h"
#include "CMSIS/STM32F10x.h"
#include "SoftSpiPhy.h"

int main()
{
	Init();

	for(;;)
	{
		GPIOA->BSRR = GPIO_BSRR_BS2;
		GPIOA->BSRR = GPIO_BSRR_BR5;
		GPIOA->BSRR = GPIO_BSRR_BR2;
		GPIOA->BSRR = GPIO_BSRR_BS5;
	}
}

void Init()
{
	stm32_Init();
}

bool ReadSpiDataBit()
{
	return GPIOA->CRH;
}

void WriteSpiDataBit(bool bit)
{
	GPIOA->BSRR = bit ? GPIO_BSRR_BS2 : GPIO_BSRR_BR2;
}

void EXTI0_IRQHandler()
{
	SpiClkIrq();
}
