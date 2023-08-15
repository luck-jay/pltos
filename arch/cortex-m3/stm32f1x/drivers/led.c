#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <plt/kernel.h>
#include <plt/init.h>
#include <rtos/FreeRTOS.h>
#include <rtos/task.h>

void led_init(void)
{
	GPIO_InitTypeDef gpio_init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_5;
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpio_init);

}
INIT_BOARD_EXPORT(led_init);

void led_toggle(void *p)
{
	while (1) {
		GPIOB->ODR ^= BIT(5);

		vTaskDelay(1000);
	}
}

void led_task_init(void)
{
	xTaskCreate(led_toggle, "led", 128, NULL, 5, NULL);
}
INIT_APP_EXPORT(led_task_init);

