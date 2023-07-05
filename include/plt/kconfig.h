#ifndef __PLT_KCONFIG_H__
#define __PLT_KCONFIG_H__

/* 名字的最大长度 */
#define SYS_NAME_MAX_SIZE 20

/* 定义日志输出等级 */
// #define LOG_LEVEL  LOG_LEVEL_ERROR

/* 系统工作频率 */
#if CONFIG_SYSTEM_FREQ_1000HZ
#define SYSTEM_FREQ 1000
#elif CONFIG_SYSTEM_FREQ_100HZ
#define SYSTEM_FREQ 100
#elif CONFIG_SYSTEM_FREQ_10HZ
#define SYSTEM_FREQ 10
#endif

/* 开启debug选项将开启assert功能 */
#ifdef CONFIG_DEBUG_INFO
#define USE_ASSERT
#endif

#endif  /* __PLT_KCONFIG_H__ */