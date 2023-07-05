#include <stm32f10x.h>
#include <misc.h>
#include <rtos/FreeRTOS.h>
#include <rtos/task.h>

static void systick_init(void)
{
	SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);
	NVIC_SetPriority(SysTick_IRQn, 0xFF);
}

/*
 * 如果STM32主频为72MHz，最长延时时间不超过 233016us = 233ms = 0.233s
 * 0 <= us <= 116508
* */
void __udelay(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000000 / configTICK_RATE_HZ);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

void borad_init(void)
{
    /* 设置中断分组为4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	systick_init();
}

extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif  /* INCLUDE_xTaskGetSchedulerState */
      xPortSysTickHandler();   //调用systick中断处理函数
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    }
#endif  /* INCLUDE_xTaskGetSchedulerState */
}
