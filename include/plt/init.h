#ifndef __PLT_INIT_H__
#define __PLT_INIT_H__

typedef void (*init_fn_t)(void);

#define INIT_EXPORT(fn,level) \
	__attribute__((used)) const init_fn_t \
    __init_##fn __attribute__((section(".init_fn." level))) = fn

#define INIT_BOARD_EXPORT(fn)   INIT_EXPORT(fn, "1")
#define INIT_BUS_EXPORT(fn)     INIT_EXPORT(fn, "2")
#define INIT_DRIVER_EXPORT(fn)  INIT_EXPORT(fn, "3")
#define INIT_DEVICE_EXPORT(fn)  INIT_EXPORT(fn, "4")
#define INIT_HW_EXPORT(fn)      INIT_EXPORT(fn, "5")
#define INIT_APP_EXPORT(fn)     INIT_EXPORT(fn, "6")

#endif /* __PLT_INIT_H__ */
