#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <misc.h>
#include <stm32f10x_dma.h>
#include <string.h>
#include <driver/serial.h>
#include <plt/init.h>

#define READ_BUF_SIZE 256
static unsigned char uart_tmp_buf[READ_BUF_SIZE];
#define USART1_BUF_SIZE 512
static unsigned char usart1_buf_data[USART1_BUF_SIZE];
#define USART1_SEND_BUF_SIZE 128
static unsigned char usart1_send_buf[USART1_SEND_BUF_SIZE];

static uint8_t send_finsh_flag = 1;
static size_t read_index;

#define uart_fifo_free(fifo) \
    (FIFO_SPACE((fifo)->head, (fifo)->tail, USART1_BUF_SIZE))

#define uart_fifo_size(fifo) \
	(FIFO_CNT((fifo)->head, (fifo)->tail, USART1_BUF_SIZE))

#define DMA_READ_SIZE \
	(READ_BUF_SIZE - get_dmarx_buf_remain_size())

static struct serial serial1 = {
    .name = "serial1",
    .xmit.buf = usart1_buf_data,
    .xmit_size = USART1_BUF_SIZE,
};

static struct fifo_buf *recv = &serial1.xmit;

static inline void fifo_write(struct fifo_buf *fifo, uint16_t size)
{
	int tmp;
	uint8_t *buf = uart_tmp_buf + read_index;

	if (!fifo->buf)
        return;

	/* 将需要写入的数据放入数据缓冲区 */
    while (1) {
        tmp = FIFO_SPACE_TO_END(fifo->head, fifo->tail, serial1.xmit_size);

        if (tmp > size)
            tmp = size;

        if (tmp <= 0)
            break;

        memcpy(fifo->buf + fifo->head, buf, tmp);
        fifo->head = (fifo->head + tmp) & (serial1.xmit_size - 1);
        buf += tmp;
        size -= tmp;
    }
}

static void uart1_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/**
 * @brief  uart1 dma接收通道配置
 * @param
 * @retval
 */
static void uart1_dmarx_config(uint8_t *mem_addr, uint32_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel5);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC; 	/* 传输方向:外设->内存 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);/* 使能DMA半满、溢满、错误中断 */
	DMA_ClearFlag(DMA1_IT_TC5);
	DMA_ClearFlag(DMA1_IT_HT5);
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

/**
 * @brief  uart1初始化配置
 * @param
 * @retval
 */
static void uart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	uart1_gpio_init();

	/* 使能串口和DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode             = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	/* 使能空闲中断 */
	USART_Cmd(USART1, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE); /* 使能DMA收发 */

	/* 串口中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* DMA中断 */
	NVIC_InitStructure.NVIC_IRQChannel    = DMA1_Channel4_IRQn;  /* UART1 DMA1Tx*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel    = DMA1_Channel5_IRQn; /* UART1 DMA1Rx*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

static void uart1_dmatx_config(uint8_t *mem_addr, size_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;

	memcpy(usart1_send_buf, mem_addr, mem_size);

	DMA_DeInit(DMA1_Channel4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)usart1_send_buf;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST; 	/* 传输方向:内存->外设 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC|DMA_IT_TE, ENABLE);
	DMA_ClearFlag(DMA1_IT_TC4);	/* 清除发送完成标识 */
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

static uint16_t get_dmarx_buf_remain_size(void)
{
	return DMA_GetCurrDataCounter(DMA1_Channel5);
}

void usart1_isr_handler(void)
{
    uint16_t len;
	uint16_t recv_size;

	/* USART1空闲中断 */
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		len = DMA_READ_SIZE;
		recv_size = len - read_index;

		/* 将数据放入环形缓冲区 */
		fifo_write(recv, recv_size);

		read_index = len;

		/* 为了清除RXN标志位 */
		USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}

void usart1_txdma_isr_handler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TE4))
	{
		DMA_ClearFlag(DMA1_IT_TE4);
	}

	/* DMA接收完成中断 */
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		send_finsh_flag = 1;
		DMA_ClearFlag(DMA1_IT_TC4);
	}
}

void usart1_rxdma_isr_handler(void)
{
    uint16_t len;
	uint16_t recv_size;

	if(DMA_GetITStatus(DMA1_IT_TE5))
	{
		DMA_ClearFlag(DMA1_IT_TE5);
	}

	/* DMA接收半完成中断 */
    if(DMA_GetITStatus(DMA1_IT_HT5))
	{
		len = DMA_READ_SIZE;
		recv_size = len - read_index;

		/* 将数据放入环形缓冲区 */
		fifo_write(recv, recv_size);

		read_index = len;

		DMA_ClearFlag(DMA1_FLAG_HT5);
	}

	/* DMA接收完成中断 */
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{
		recv_size = READ_BUF_SIZE - read_index;

		/* 将数据放入环形缓冲区 */
		fifo_write(recv, recv_size);

		read_index = 0;

		DMA_ClearFlag(DMA1_FLAG_TC5);
	}
}

static int uart1_dma_send(uint8_t *buf, size_t size)
{
	while (send_finsh_flag == 0);
	send_finsh_flag = 0;
	uart1_dmatx_config(buf, size);

	return size;
}

static const struct uart_ops uart1_ops = {
	.dma_send = uart1_dma_send,
};

void serial1_init(void)
{
	uart1_init();

	uart1_dmarx_config(uart_tmp_buf, READ_BUF_SIZE);

	serial1.ops = &uart1_ops;

	serial_register(&serial1, 0);
}
INIT_BOARD_EXPORT(serial1_init);
