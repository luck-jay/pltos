#define configUSE_PREEMPTION                     1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1
#define configUSE_TICKLESS_IDLE                  0
#define configUSE_IDLE_HOOK                      0
#define configUSE_MALLOC_FAILED_HOOK             0

#define configUSE_16_BIT_TICKS 1
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 4
#define configKERNEL_INTERRUPT_PRIORITY 1
#define configMAX_PRIORITIES 5
#define configMINIMAL_STACK_SIZE         128
#define configMAX_TASK_NAME_LEN   16
#define configUSE_TICK_HOOK 0
#define configTOTAL_HEAP_SIZE       2048
#define configCPU_CLOCK_HZ     72000000
#define configTICK_RATE_HZ     1000
