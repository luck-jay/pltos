#include <plt/clock.h>

volatile ubase_t ticks = 0;

extern void __udelay(uint32_t us);

/**
 * @brief 微妙级延时函数
 *
 * @param us 需要延时的微妙数
 */
void udelay(uint32_t us)
{
	__udelay(us);
}

/**
 * @brief 毫秒级延时函数
 *
 * @param us 需要延时的毫秒数
 */
void mdelay(uint32_t ms)
{
	ubase_t timeout = ticks + ms_to_ticks(ms);

	while (!time_after(ticks, timeout));
}

/**
 * @brief 系统tick数自增操作
 */
void tick_increase(void)
{
	++ticks;
}

