#include "../include/rpi.h"

static volatile unsigned int *gpio;

/**
 * gpio_set - set or clear gpio bits
 * @pin:	pin to modify
 * @mode:	specifies set or clear
 */
void gpio_set(const uint pin, int mode)
{
	if (mode == 1)
		GPIO_SET = 1 << pin;
	else
		GPIO_CLR = 1 << pin;
}

/**
 * gpio_dir - set gpio pin to input or output
 * @pin:	pin to set
 * @dir:	specifies input or output
 */
void gpio_dir(const uint pin, int dir)
{
	INP_GPIO(pin);
	if (dir == GPIO_OUT)
		OUT_GPIO(pin);
}

/**
 * gpio_setup - set up memory regions to access GPIO
 * @gpio:	double pointer to mapped area
 */
void gpio_setup()
{
	int fd;
	void *map;

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
		perror("open() error");
		exit(1);
	}

	map = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	close(fd);
	if (map == MAP_FAILED) {
		perror("mmap() error");
		exit(1);
	}

	gpio = map;
}
