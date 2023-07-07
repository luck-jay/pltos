#include <string.h>
#include <plt/kernel.h>
#include <plt/pltio.h>

static inline int pltin_read(char *buf, size_t size)
{
    int cnt;
	struct fifo_buf *io = pltin.io;

	/* 将需要写入的数据放入数据缓冲区 */
    while (1) {
        cnt = pltin_cnt();

        if (cnt > size)
            cnt = size;

        if (cnt <= 0)
            break;

        memcpy(buf, io->buf + io->tail, cnt);
        io->tail = (io->tail + cnt) & (pltin.size - 1);
        buf += cnt;
        size -= cnt;
    }

    return cnt;
}

int readline(uint8_t *buf, char *msg)
{
    char read[20];
    int size;

    printk(msg);

    while (1) {
        size = pltin_cnt();
        if (size != 0) {
            pltin_read(read, size);
            /* \b退格  \033[K 清空光标后的内容 */
            if (read[0] == '\b') {
                read[1] = '\033';
                read[2] = '[';
                read[3] = 'K';
                size = 4;
            }
            /* \033[A Up \033[B Down  */
            if ((read[0] == '\033' && read[1] == '[') && (read[2] == 'A' || read[2] == 'B'))
                continue;

            printk(read);
            memset(read, 0, sizeof(read));
        }
    }
}