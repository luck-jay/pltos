#include <driver/serial.h>

int serial_register(struct serial *serial, int flags)
{
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
    serial->ops->get_char();
    return 0;
}