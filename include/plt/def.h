#ifndef __PLT_DEF_H__
#define __PLT_DEF_H__

#if !defined(__CC_ARM) && !defined(__CLANG_ARM)
#include <sys/cdefs.h>
#define __weak __weak_symbol
#endif

#define __printf(m, n)   __attribute__((format(printf, (m), (n))))

#define BIT(n) (1 << (n))

#if !defined(__align) && !defined(__align_down)
#define __align(size, align)       (((size) + (align) - 1) & ~((align) - 1))
#define __align_down(size, align)  ((size) & ~((align) - 1))
#endif

#define align(size, align)       __align(size, align)
#define align_down(size, align)  __align_down(size, align)

#define EIO      1 // IO错误
#define ENXIO    2 // 没有这样的设备或地址
#define E2BIG    3 // 参数太长
#define EAGAIN   4 // Try again
#define ENOMEM   5 // 内存不足
#define EFAULT   6 // 错误的地址
#define EBUSY    7 // 设备或资源忙
#define ENODEV   8 // 没有这样的设备
#define EINVAL   9 // 非法参数
#define ENOSPC   10 // 设备空间不足
#define ESPIPE   11 // Illegal seek
#define EROFS    12 // 只读文件系统
#define EDOM     13 // Math argument out of domain of func
#define ERANGE   14 // 数学结果无法表示
#define ENAMETOOLONG 15 // 文件名字太长

#endif /* __PLT_DEF_H__ */
