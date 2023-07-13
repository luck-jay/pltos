#include <string.h>
#include <driver/serial.h>

int serial_register(struct serial *serial, int flags)
{
    serial->parent.obj_t = OBJECT_TYPE_DRIVER;
    object_init_and_add(&serial->parent, serial->name);
    serial->flags = flags;
    return 0;
}

void serial_unregister(struct serial *serial)
{
    object_deinit(&serial->parent);
}

int serial_write(struct object *obj, uint8_t *buf, size_t size)
{
    struct serial *serial = container_of(obj, struct serial, parent);

    serial->ops->dma_send(buf, size);
    return 0;
}

int serial_read(struct object *obj, uint8_t *buf, size_t size)
{
    struct serial *serial = container_of(obj, struct serial, parent);
    int tmp;
	struct fifo_buf *xmit = &serial->xmit;

	if (!xmit->buf)
        return 0;

	/* 将需要写入的数据放入数据缓冲区 */
    while (1) {
        tmp = serial_xmit_size(obj);

        if (tmp > size)
            tmp = size;

        if (tmp <= 0)
            break;

        memcpy(buf, xmit->buf + xmit->tail, tmp);
        xmit->tail = (xmit->tail + tmp) & (serial->xmit_size - 1);
        buf += tmp;
        size -= tmp;
    }

    return tmp;
}