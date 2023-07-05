#ifndef __PLT_ATOMIC_H__
#define __PLT_ATOMIC_H__

#include <arch/atomic.h>

typedef int atomic_t;

/**
 * @brief 原子数自增
 *
 * @param v 需要自增的原子数
 */
static inline void atomic_inc(atomic_t *v)
{
    __arch_atomic_add(v, 1);
}

/**
 * @brief 原子数自减
 *
 * @param v 需要自减的原子数
 */
static inline void atomic_dec(atomic_t *v)
{
    __arch_atomic_sub(v, 1);
}

/**
 * @brief 原子数加法
 *
 * @param v 需要做加法的原子数
 *
 * @param n 原子数需要增加的数
 */
static inline void atomic_add(atomic_t *v, int n)
{
    __arch_atomic_add(v, n);
}

/**
 * @brief 原子数减法
 *
 * @param v 需要做减法的原子数
 *
 * @param n 原子数需要减少的数
 */
static inline void atomic_sub(atomic_t *v, int n)
{
    __arch_atomic_sub(v, n);
}

/**
 * @brief 原子数赋值
 *
 * @param v 需要赋值的原子数
 *
 * @param n 原子数需要赋值的数
 */
static inline void atomic_set(atomic_t *v, int n)
{
    __arch_atomic_set(v, n);
}

#endif /* __PLT_ATOMIC_H__ */