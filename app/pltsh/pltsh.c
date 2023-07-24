#include <string.h>
#include <plt/kernel.h>
#include <readline.h>

void pltsh(void)
{
	uint8_t buf[100];

	while (1) {
		readline(buf, "pltsh# ");
		log_d("%s", buf);
		memset(buf, 0, 100);
	}
}
