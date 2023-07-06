#include <stdarg.h>
#include <plt/def.h>

static void __printk(const char *fmt, va_list vargs)
{

}

/**
 * @brief 通过串口打印输出
 *
 * @param fmt 需要打印的内容
 */
__printf(1, 2) void printk(const char *fmt, ...)
{
    va_list arg_list;

	va_start(arg_list, fmt);
	__printk(fmt, arg_list);
	va_end(arg_list);
}

/**
 * @brief 通过串口打印输出不同等级日志
 *
 * @param level 日志等级，暂时未使用
 *
 * @param fmt 打印的日志内容
 */
__printf(2, 3) void print_log(int level, const char *fmt, ...)
{
	va_list arg_list;

	va_start(arg_list, fmt);
	__printk(fmt, arg_list);
	va_end(arg_list);
}

/**
 * @brief 校验出错时将调用该函数，输出校验失败的文件和所在行
 */
__weak void assert_failed(char *file_name, int line)
{
	printk("%s %d assert failed !\r\n", file_name, line);
}