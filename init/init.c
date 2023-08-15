#include <plt/def.h>
#include <plt/log.h>
#include <plt/init.h>
#include <rtos/FreeRTOS.h>
#include <rtos/task.h>

static void init_start(void) {
}
INIT_EXPORT(init_start, "0");

static void init_end(void) {
}
INIT_EXPORT(init_end, "6.end");

static void system_init(void)
{
	volatile const init_fn_t *fn_ptr;

	for (fn_ptr = &__init_init_start; fn_ptr < &__init_init_end; fn_ptr++)
		(*fn_ptr)();
}

static void entry_app(void *parameters)
{
	extern int main(void);

	main();
}

int entry(void)
{
	extern void borad_init(void);

	/* 板子初始化 */
	borad_init();
	/* 执行所有自动初始化函数 */
	system_init();

	/* 创建第一个任务 */
	xTaskCreate(entry_app, "app", 500, NULL, 1, NULL);

	vTaskStartScheduler();

	return 0;
}
