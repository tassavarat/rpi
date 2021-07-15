#include "../include/rpi.h"

static volatile unsigned int *gpio;

/**
 * gpio_read - read value of pin
 * @pin:	pin to read
 *
 * Return:	value of pin
 */
unsigned int gpio_read(const unsigned int pin)
{
	if (!VALID_PIN(pin))
		return 1;

	return GPIO_READ(pin);
}

/**
 * gpio_pud - pull pin up or down
 * @pin:	pin to pull
 * @mode:	specifies up or down
 *
 * Return:	0 on success, 1 on failure
 */
int gpio_pud(const unsigned int pin, const int mode)
{
	if ((mode != PULL_UP && mode != PULL_DOWN) || !VALID_PIN(pin))
		return 1;

	int pull_reg, pull_shift;
	unsigned int pull_bits;

	pull_reg = 57 + (pin >> 4);
	pull_shift = (pin & 0xf) << 1;
	pull_bits = *(gpio + pull_reg);
	pull_bits &= ~(3 << pull_shift);
	pull_bits |= (mode << pull_shift);
	*(gpio + pull_reg) = pull_bits;
	return 0;
}

/**
 * gpio_set - set or clear gpio bits
 * @pin:	pin to modify
 * @mode:	specifies set or clear
 *
 * Return:	0 on success, 1 on failure
 */
int gpio_set(const unsigned int pin, const int mode)
{
	if (!VALID_PIN(pin))
		return 1;

	if (mode == 1)
		GPIO_SET = 1 << pin;
	else if (mode == 0)
		GPIO_CLR = 1 << pin;
	else
		return 1;
	return 0;
}

/**
 * gpio_dir - set gpio pin to input or output
 * @pin:	pin to set
 * @dir:	specifies input or output
 *
 * Return:	0 on success, 1 on failure
 */
int gpio_dir(const unsigned int pin, const int dir)
{
	if (!VALID_PIN(pin))
		return 1;

	if (dir == GPIO_INP) {
		INP_GPIO(pin);
	} else if (dir == GPIO_OUT) {
		INP_GPIO(pin);
		OUT_GPIO(pin);
	} else {
		return 1;
	}
	return 0;
}

/**
 * gpio_setup - set up memory regions to access GPIO
 * @gpio:	double pointer to mapped area
 *
 * Return:	0 on success, 1 on failure
 */
int gpio_setup()
{
	int fd;
	void *map;

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
		perror("open() error");
		return 1;
	}

	map = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	close(fd);
	if (map == MAP_FAILED) {
		perror("mmap() error");
		return 1;
	}

	gpio = map;
	return 0;
}
