#include <string.h>
#include <plt/kernel.h>
#include <plt/pltio.h>

#define TERMINAL_CUR_UP(n) "\033["#n"A"
#define TERMINAL_CUR_DOWN(n) "\033["#n"B"
#define TERMINAL_CUR_RIGHT(n) "\033["#n"C"
#define TERMINAL_CUR_LEFT(n) "\033["#n"D"
#define TERMINAL_CUR_GOTO(x, y) "\033["#y";"#x"H"
#define TERMINAL_CLEAR "\033[2J"
#define TERMINAL_CLEAR_LINE "\033[K"
#define TERMINAL_CUR_SAVE "\033[s"
#define TERMINAL_CUR_LOAD "\033[u"
#define TERMINAL_CUR_HIDE "\033[?25l"
#define TERMINAL_CUR_SHOW "\033[?25h"

enum input_cmd {
    IN_CMD_DEL = 0,
    IN_CMD_LEFT = 1,
    IN_CMD_RIGHT = 2,
    IN_CMD_UP = 3,
    IN_CMD_DOWN = 4,
    IN_CMD_ENTER = 5,
    IN_CMD_NON = 6,
};

#define INPUT_FLAG_CMD  0
#define INPUT_FLAG_DATA 1

struct in_data {
    int flags;
    uint8_t *data;
    size_t size, cur;
    char *io_data;
    enum input_cmd cmd;
};

static const char input_cmd_table[IN_CMD_NON][4] = {
    [IN_CMD_DEL] = "\b",
    [IN_CMD_LEFT] = "\033[D",
    [IN_CMD_RIGHT] = "\033[C",
    [IN_CMD_UP] = "\033[A",
    [IN_CMD_DOWN] = "\033[B",
    [IN_CMD_ENTER] = "\r",
};

static inline int pltin_read(char *buf, size_t size)
{
    int cnt = 0, tmp = 0;
	struct fifo_buf *io = pltin.io;

	/* 将需要写入的数据放入数据缓冲区 */
    while (1) {
        tmp = pltin_cnt();

        if (tmp > size)
            tmp = size;

        if (tmp <= 0)
            break;

        memcpy(buf, io->buf + io->tail, tmp);
        io->tail = (io->tail + tmp) & (pltin.size - 1);
        buf += tmp;
        size -= tmp;
        cnt += tmp;
    }

    return cnt;
}

static inline void input_cur_data_del(struct in_data *data)
{
    for (int i = data->cur - 1; i < data->size - 1; ++i)
            data->data[i] = data->data[i + 1];
}

static inline void
input_cur_data_add(struct in_data *data, char *buf, size_t size)
{
    for (int i = data->size; i >= (int)data->cur; --i)
        data->data[i + size] = data->data[i];

    memcpy(data->data + data->cur, buf, size);
}

static int input_cmd_process(struct in_data *data, size_t size)
{
    int ret = size;

    switch (data->cmd)
    {
    case IN_CMD_DEL:
        /* 如果光标已经在最前面，就无需删除 */
        if (data->cur == 0)
            break;
        data->io_data[ret++] = '\033';
        data->io_data[ret++] = '[';
        data->io_data[ret++] = 'K';
        printk("%s%s", data->io_data, TERMINAL_CUR_SAVE);
        printk("%s%s", data->data + data->cur, TERMINAL_CUR_LOAD);

        input_cur_data_del(data);

        data->cur -= 1;
        data->size -= 1;
        data->data[data->size] = '\0';
        break;

    case IN_CMD_RIGHT:
        /* 如果光标已经在末尾则不能再移动 */
        if (data->size == data->cur)
            break;
        printk(data->io_data);
        data->cur += 1;
        ret = size;
        break;

    case IN_CMD_LEFT:
        /* 如果光标已经在最前面就不能再移动 */
        if (data->cur == 0)
            break;
        printk(data->io_data);
        data->cur -= 1;
        ret = size;
        break;
    case IN_CMD_ENTER:
        ret = size;
        break;

    default:
        ret = 0;
        break;
    }

    return ret;
}

static int input_parse(struct in_data *data, size_t size)
{
    int ret = 0;

    data->cmd = IN_CMD_NON;
    data->flags = INPUT_FLAG_DATA;
    for (uint8_t i = 0; i < IN_CMD_NON; ++i) {
        if (!strncmp(data->io_data, input_cmd_table[i], size)) {
            data->flags = INPUT_FLAG_CMD;
            data->cmd = i;
            break;
        }
    }

    if (data->flags == INPUT_FLAG_CMD) {
        ret = input_cmd_process(data, size);
    } else {
        input_cur_data_add(data, data->io_data, size);
        data->size += size;
        printk(TERMINAL_CUR_SAVE);
        printk("%s", data->data + data->cur);
        printk("%s%s", TERMINAL_CUR_LOAD, TERMINAL_CUR_RIGHT(1));
        data->cur += 1;
        ret = size;
    }

    return ret;
}

int readline(uint8_t *buf, char *msg)
{
    static char io_buf[256];
    struct in_data data;
    int size;

    printk(msg);
    data.io_data = io_buf;
    data.data = buf;
    data.size = data.cur = 0;

    while (1) {
        size = pltin_cnt();
        if (size != 0) {
            size = pltin_read(io_buf, size);
            size = input_parse(&data, size);
            if (size == 0)
                continue;

            if (data.cmd == IN_CMD_ENTER) {
                printk("\r\n");
                break;
            }

            memset(io_buf, 0, sizeof(io_buf));
        }
    }

    return data.size;
}