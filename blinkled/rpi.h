#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2711_PERI_BASE	0x7e000000
#define GPIO_BASE		BCM2711_PERI_BASE + 0x200000

#define BLOCK_SIZE		4 * 1024

#define INP_GPIO(g) *(gpio.addr+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio.addr+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET *(gpio.addr+7)
#define GPIO_CLR *(gpio.addr+10)

struct bcm2711_peri {
	int mem_fd;
	void *map;
	volatile unsigned int *addr;
};
