#ifndef __DRIVER_SERIAL_H__
#define __DRIVER_SERIAL_H__

#include <plt/kernel.h>
#include <plt/object.h>
#include <plt/fifo_buf.h>

struct uart_ops {
    int (*put_char) (uint8_t c);
    int (*get_char) (void);
    int (*dma_send) (uint8_t *buf, size_t size);
};

struct serial {
    char *name;
    struct object parent;
    const struct uart_ops *ops;
    struct fifo_buf xmit;
    size_t xmit_size;
    int flags;
};

int serial_register(struct serial *serial, int flags);
void serial_unregister(struct serial *serial);
int serial_write(struct object *obj, uint8_t *buf, size_t size);
int serial_read(struct object *obj, uint8_t *buf, size_t size);

static inline size_t serial_xmit_size(struct object *obj)
{
    assert(obj != NULL);

    struct serial *serial = container_of(obj, struct serial, parent);
    struct fifo_buf *xmit = &serial->xmit;
    size_t xmit_size = serial->xmit_size;

    return FIFO_CNT(xmit->head, xmit->tail, xmit_size);
}

#endif /* __DRIVER_SERIAL_H__ */
