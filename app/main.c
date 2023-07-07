#include <stdio.h>
#include <string.h>
#include <driver/serial.h>
#include <readline.h>

int main(void)
{
    uint8_t buf[20];

	readline(buf, "pltsh# ");
}
