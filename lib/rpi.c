#include "../include/rpi.h"

static volatile unsigned int *gpio;
static volatile unsigned int *pwm;

static unsigned int gpioToGPFSEL[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

static unsigned int gpioToShift[] = {
	0, 4, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27
};

static unsigned int gpioToPwmALT[] = {
	0,         0,         0,         0,         0,         0,         0,         0,	//  0 ->  7
	0,         0,         0,         0, FSEL_ALT0, FSEL_ALT0,         0,         0,	//  8 -> 15
	0,         0, FSEL_ALT5, FSEL_ALT5,         0,         0,         0,         0, // 16 -> 23
	0,         0,         0,         0,         0,         0,         0,         0,	// 24 -> 31
	0,         0,         0,         0,         0,         0,         0,         0,	// 32 -> 39
	FSEL_ALT0, FSEL_ALT0, 0,         0,         0, FSEL_ALT0,         0,         0,	// 40 -> 47
	0,         0,         0,         0,         0,         0,         0,         0,	// 48 -> 55
	0,         0,         0,         0,         0,         0,         0,         0	// 56 -> 63
};

static unsigned int gpioToPwmPort [] =
{
	0,         0,         0,         0,         0,         0,         0,         0,	//  0 ->  7
	0,         0,         0,         0, PWM0_DATA, PWM1_DATA,         0,         0, //  8 -> 15
	0,         0, PWM0_DATA, PWM1_DATA,         0,         0,         0,         0, // 16 -> 23
	0,         0,         0,         0,         0,         0,         0,         0,	// 24 -> 31
	0,         0,         0,         0,         0,         0,         0,         0,	// 32 -> 39
	PWM0_DATA, PWM1_DATA, 0,         0,         0, PWM1_DATA,         0,         0,	// 40 -> 47
	0,         0,         0,         0,         0,         0,         0,         0,	// 48 -> 55
	0,         0,         0,         0,         0,         0,         0,         0	// 56 -> 63

};

/**
 * gpio_read - read value of pin
 * @pin:	pin to read
 *
 * Return:	value of pin
 */
unsigned int gpio_read(const unsigned int pin)
{
	return GPIO_READ(pin);
}

/**
 * gpio_pud - pull pin up or down
 * @pin:	pin to pull
 * @mode:	specifies up or down
 */
void gpio_pud(const unsigned int pin, const int mode)
{
	if (mode != PULL_UP && mode != PULL_DOWN)
		return;

	int pull_reg, pull_shift;
	unsigned int pull_bits;

	pull_reg = 57 + (pin >> 4);
	pull_shift = (pin & 0xf) << 1;
	pull_bits = *(gpio + pull_reg);
	pull_bits &= ~(3 << pull_shift);
	pull_bits |= (mode << pull_shift);
	*(gpio + pull_reg) = pull_bits;
}

void pwm_set(const unsigned int pin, const unsigned int val)
{
	*(pwm + gpioToPwmPort[pin]) = val;
}

/**
 * gpio_set - set or clear gpio bits
 * @pin:	pin to modify
 * @mode:	specifies set or clear
 */
void gpio_set(const unsigned int pin, const int mode)
{
	if (mode == 1)
		GPIO_SET = 1 << pin;
	else if (mode == 0)
		GPIO_CLR = 1 << pin;
	else
		return;
}

/**
 * gpio_dir - set gpio pin to input or output
 * @pin:	pin to set
 * @dir:	specifies input or output
 */
void gpio_dir(const unsigned int pin, const int dir)
{
	int fSel = gpioToGPFSEL[pin], shift = gpioToShift[pin], alt;

	if (dir == GPIO_INP) {
		INP_GPIO(pin);
	} else if (dir == GPIO_OUT) {
		INP_GPIO(pin);
		OUT_GPIO(pin);
	} else if (dir == PWM_OUT && (alt = gpioToPwmALT[pin]) != 0) {
		*(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | (alt << shift) ;
		*(pwm + PWM_CONTROL) = PWM0_ENABLE | PWM1_ENABLE;
		*(pwm + PWM0_RANGE) = *(pwm + PWM1_RANGE) = 1024;
	} else {
		return;
	}
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
	if (map == MAP_FAILED) {
		perror("mmap() error");
		exit(1);
	}

	gpio = map;
	map = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_PWM);
	close(fd);
	if (map == MAP_FAILED) {
		perror("map() error");
		exit(1);
	}

	pwm = map;
}
