#include <string.h>
#include <plt/kernel.h>
#include <driver/serial.h>

int readline(uint8_t *buf, char *msg)
{
    uint8_t read[20];
    int xmit_size;
    struct object *obj = object_type_find("serial1", OBJECT_TYPE_DRIVER);

    serial_write(obj, (uint8_t *)msg, strlen(msg));

    while (1) {
        xmit_size = serial_xmit_size(obj);
        if (xmit_size != 0) {
            serial_read(obj, read, xmit_size);
            /* \b退格  \033[K 清空光标后的内容 */
            if (read[0] == '\b') {
                read[1] = '\033';
                read[2] = '[';
                read[3] = 'K';
                xmit_size = 4;
            }
            /* \033[A Up \033[B Down  */
            if ((read[0] == '\033' && read[1] == '[') && (read[2] == 'A' || read[2] == 'B'))
                continue;

            serial_write(obj, read, xmit_size);
        }
    }
}