#include "rpi.h"

void unmap_peripheral(struct bcm2711_peri *gpio)
{
	munmap(gpio->map, BLOCK_SIZE);
}

int map_peripheral(struct bcm2711_peri *gpio)
{
	if ((gpio->mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
		perror("open() error");
		return -1;
	}

	gpio->map = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, gpio->mem_fd, GPIO_BASE);
	close(gpio->mem_fd);
	if (gpio->map == MAP_FAILED) {
		perror("mmap() error");
		return -1;
	}

	gpio->addr = (volatile unsigned int *)gpio->map;
	return 0;
}

int main()
{
	struct bcm2711_peri gpio;

	if (map_peripheral(&gpio) == -1) {
		fprintf(stderr, "map_peripheral() failed\n");
		return 1;
	}

	INP_GPIO(10);
	OUT_GPIO(10);
	while (1) {
		GPIO_SET = 1 << 4;
		sleep(1);
		GPIO_CLR = 1 << 4;
		sleep(1);
	}
}
