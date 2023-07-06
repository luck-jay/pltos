#include "uart.h"

void USART1_IRQHandler(void)
{
	usart1_isr_handler();
}

void DMA1_Channel4_IRQHandler(void)
{
	usart1_txdma_isr_handler();
}

void DMA1_Channel5_IRQHandler(void)
{
	usart1_rxdma_isr_handler();
}
