#include <plt/def.h>
#include <plt/log.h>
#include <plt/init.h>

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

int entry(void)
{
	int ret = 0;

	extern int main(void);
	extern void borad_init(void);

	// /* 板子初始化 */
	borad_init();
	/* 执行所有自动初始化函数 */
	system_init();
	/* 执行主函数 */
	ret = main();

	return ret;
}
