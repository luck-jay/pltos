#ifndef __PLT_KCONFIG_H__
#define __PLT_KCONFIG_H__

/* 名字的最大长度 */
#define SYS_NAME_MAX_SIZE 20

/* 定义日志输出等级 */
#define LOG_LEVEL  LOG_LEVEL_ERROR

#define CONFIG_SYSTEM_FREQ_1000HZ 1

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

/***************************************************************************************************************/
/*                                        FreeRTOS基础配置配置选项                                              */
/***************************************************************************************************************/
#define configUSE_PREEMPTION                    1                       //1使用抢占式内核，0使用协程
#define configUSE_TIME_SLICING                  1                       //1使能时间片调度(默认式使能的)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1                       //1启用特殊方法来选择下一个要运行的任务
                                                                        //一般是硬件计算前导零指令，如果所使用的
                                                                        //MCU没有这些硬件指令的话此宏应该设置为0！
#define configUSE_TICKLESS_IDLE                 0                       //1启用低功耗tickless模式
#define configUSE_QUEUE_SETS                    1                       //为1时启用队列
#define configCPU_CLOCK_HZ                      (72000000)       //CPU频率
#define configTICK_RATE_HZ                      (SYSTEM_FREQ)                  //时钟节拍频率，这里设置为1000，周期就是1ms
#define configMAX_PRIORITIES                    (32)                    //可使用的最大优先级
#define configMINIMAL_STACK_SIZE                ((unsigned short)130)   //空闲任务使用的堆栈大小
#define configMAX_TASK_NAME_LEN                 (SYS_NAME_MAX_SIZE)                    //任务名字字符串长度

#define configUSE_16_BIT_TICKS                  0                       //系统节拍计数器变量数据类型，
                                                                        //1表示为16位无符号整形，0表示为32位无符号整形
#define configIDLE_SHOULD_YIELD                 1                       //为1时空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configUSE_TASK_NOTIFICATIONS            1                       //为1时开启任务通知功能，默认开启
#define configUSE_MUTEXES                       1                       //为1时使用互斥信号量
#define configQUEUE_REGISTRY_SIZE               8                       //不为0时表示启用队列记录，具体的值是可以
                                                                        //记录的队列和信号量最大数目。
#define configCHECK_FOR_STACK_OVERFLOW          0                       //大于0时启用堆栈溢出检测功能，如果使用此功能
                                                                        //用户必须提供一个栈溢出钩子函数，如果使用的话
                                                                        //此值可以为1或者2，因为有两种栈溢出检测方法。
#define configUSE_RECURSIVE_MUTEXES             1                       //为1时使用递归互斥信号量
#define configUSE_MALLOC_FAILED_HOOK            0                       //1使用内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1                       //为1时使用计数信号量

/***************************************************************************************************************/
/*                                FreeRTOS与内存申请有关配置选项                                                */
/***************************************************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //支持动态内存申请
#define configTOTAL_HEAP_SIZE                   ((size_t)(20*1024))     //系统所有总的堆大小

/***************************************************************************************************************/
/*                                FreeRTOS与钩子函数有关的配置选项                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK                     0                       //1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK                     0                       //1，使用时间片钩子；0，不使用
/***************************************************************************************************************/
/*                                FreeRTOS与中断有关的配置选项                                                  */
/***************************************************************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS             __NVIC_PRIO_BITS
#else
	#define configPRIO_BITS             4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        15                      //中断最低优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   5                       //系统可管理的最高中断优先级
#define configKERNEL_INTERRUPT_PRIORITY          ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*******************************************************************************/
/*              FreeRTOS与中断服务函数有关的配置选项                           */
/******************************************************************************/
#define xPortPendSVHandler                       PendSV_Handler
#define vPortSVCHandler                          SVC_Handler

#endif  /* __PLT_KCONFIG_H__ */