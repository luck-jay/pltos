#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <plt/kernel.h>
#include <plt/object.h>
#include <plt/init.h>
#include <driver/serial.h>
#include <plt/pltio.h>

static struct object *pltio_obj = NULL;
static char pltout_buf[1024];
static size_t pltout_cnt = 0;

static void __printk(const char *fmt, va_list vargs)
{
	char buf[256];
	int size;

	size = vsprintf(buf, fmt, vargs);

	if (pltio_obj == NULL) {
		memcpy(pltout_buf + pltout_cnt, buf, size);
		pltout_cnt += size;
	} else {
		if (pltout_cnt != 0) {
			serial_write(pltio_obj, (uint8_t *)pltout_buf, pltout_cnt);
			pltout_cnt = 0;
		}

		serial_write(pltio_obj, (uint8_t *)buf, size);
	}
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

struct pltio __pltin;
/* pltio initial */
void pltio_init(void)
{
	pltio_obj = object_type_find(__pltio_name, OBJECT_TYPE_DRIVER);
	struct serial *serial = container_of(pltio_obj, struct serial, parent);
	__pltin.io = &serial->xmit;
	__pltin.size = serial->xmit_size;
}
INIT_HW_EXPORT(pltio_init);