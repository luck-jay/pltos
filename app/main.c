#include <stdio.h>
#include <string.h>
#include <driver/serial.h>
#include <readline.h>

int main(void)
{
    uint8_t buf[100];

	while (1) {
        readline(buf, "pltsh# ");
        log_d("%s", buf);
        memset(buf, 0, 100);
    }
}
