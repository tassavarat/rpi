#include "rpi.h"

/**
 * gpio_setup - set up memory regions to access GPIO
 * @gpio: double pointer to mapped area
 */
void gpio_setup(volatile unsigned int **gpio)
{
	int fd;
	void *map;

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
		perror("open() error");
		exit(1);
	}

	map = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	close(fd);
	if (map == MAP_FAILED) {
		perror("mmap() error");
		exit(1);
	}

	*gpio = map;
}
