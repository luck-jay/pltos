#ifndef __PLT_KCONFIG_H__
#define __PLT_KCONFIG_H__

/* 名字的最大长度 */
#define SYS_NAME_MAX_SIZE CONFIG_KERNEL_NAME_MAX_SIZE

/* 定义日志输出等级 */
#ifdef CONFIG_LOG
#define LOG_LEVEL  CONFIG_LOG_LEVEL
#else
#define LOG_LEVEL  -1
#endif

/* 系统工作频率 */
#ifdef CONFIG_SYSTEM_CLOCK_1000HZ
#define SYSTEM_FREQ 1000
#endif
#ifdef CONFIG_SYSTEM_CLOCK_250HZ
#define SYSTEM_FREQ 250
#endif
#ifdef CONFIG_SYSTEM_CLOCK_100HZ
#define SYSTEM_FREQ 100
#endif
#ifdef CONFIG_SYSTEM_CLOCK_10HZ
#define SYSTEM_FREQ 10
#endif

/* 开启debug选项将开启assert功能 */
#ifdef CONFIG_DEBUG
#define USE_ASSERT
#endif

/* plt输入缓冲区 */
#define __pltio_name  "serial1"

#endif  /* __PLT_KCONFIG_H__ */