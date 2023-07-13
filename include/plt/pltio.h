#ifndef __PLT_PLTIO_H__
#define __PLT_PLTIO_H__

#include "fifo_buf.h"
#include "printk.h"

struct pltio {
    struct fifo_buf *io;
    size_t size;
};

extern struct pltio __pltin;
#define pltin __pltin

#define pltin_cnt() \
    (FIFO_CNT(__pltin.io->head, __pltin.io->tail, __pltin.size))

#endif /* __PLT_PLTIO_H__ */
