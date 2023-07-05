#ifndef __PLT_THREAD_H__
#define __PLT_THREAD_H__

uint8_t *stack_init(void *entry,
				void *parameter,
				void *stack_addr,
				void *exit);

/**
 * @brief 线程切换函数，从上一个线程from切换到线程to
 *
 * @param thread_from 当前正在运行的线程的sp指针地址
 *
 * @param thread_to 需要切换到的线程的sp指针地址
 */
extern void context_switch(ubase_t thread_from, ubase_t thread_to);

/**
 * @brief 线程切换函数，立即切换到线程to
 *
 * @param thread_to 需要切换到的线程的sp指针地址
 */
extern void context_switch_to(ubase_t thread_to);

#endif /* __PLT_THREAD_H__ */