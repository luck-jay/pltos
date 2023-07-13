#ifndef __PLT_ATOMIC_H__
#define __PLT_ATOMIC_H__

#include <rtos/FreeRTOS.h>
#include <rtos/atomic.h>

typedef uint32_t atomic_t;

/**
 * @brief 原子数赋值
 *
 * @param v 需要赋值的原子数
 *
 * @param n 原子数需要赋值的数
 */
static inline void atomic_set(atomic_t volatile *v, int n)
{
    *v = n;
}

#endif /* __PLT_ATOMIC_H__ */