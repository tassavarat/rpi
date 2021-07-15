#include "../include/rpi.h"

static volatile unsigned int *gpio;
static volatile unsigned int start_val_arr[MAX_PINS];
static volatile unsigned int range_arr[MAX_PINS];
static volatile unsigned int new_pin;

/**
 * soft_pwm_set - set PWM value to specified pin
 * @pin:	pin to set PWM value
 * @val:	PWM value to set
 *
 * Return:	0 on success, 1 on failure
 */
int soft_pwm_set(const unsigned int pin, unsigned int val)
{
	if (pin >= MAX_PINS)
		return 1;

	if (val > range_arr[pin])
		val = range_arr[pin];
	start_val_arr[pin] = val;
	return 0;
}

/**
 * soft_pwm_thread - handles PWM output
 * @arg:	pointer to pin to apply PWM
 */
void *soft_pwm_thread(void *arg)
{
	struct timespec req;
	struct sched_param param;
	int pin, start_val, range;

	req.tv_sec = 0;
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	if (pthread_setschedparam(pthread_self(), SCHED_RR, &param) != 0) {
		perror("pthread_setschedparam() error");
		return NULL;
	}

	pin = *(int *)arg;
	free(arg);

	pin = new_pin;
	new_pin = 0;
	while(1) {
		start_val = start_val_arr[pin];
		range = range_arr[pin] - start_val;
		if (start_val != 0)
			gpio_set(pin, 1);
		req.tv_nsec = 100000 * start_val;
		nanosleep(&req, NULL);
		if (range != 0)
			gpio_set(pin, 0);
		req.tv_nsec = 100000 * range;
		nanosleep(&req, NULL);
	}
	pthread_exit(NULL);
}

/**
 * soft_pwm_setup - set up software PWM
 * @pin:	pin to apply PWM
 * @start_val:	PWM frequency start
 * @range:	PWM frequency range
 *
 * Return:	0 on success, 1 on failure
 */
int soft_pwm_setup(const unsigned int pin, const unsigned int start_val, const unsigned int range)
{
	int *tinfo;
	struct timespec req;
	pthread_t thread;

	req.tv_sec = 0;
	req.tv_nsec = 1000000;
	if (VALID_PIN(pin) == 0)
		return 1;

	if (range == 0)
		return 1;

	if (range_arr[pin] != 0)
		return 1;

	tinfo = malloc(sizeof(*tinfo));
	if (tinfo == NULL)
		return 1;

	*tinfo = pin;
	gpio_set(pin, 0);
	gpio_dir(pin, GPIO_OUT);

	start_val_arr[pin] = start_val;
	range_arr[pin] = range;

	new_pin = pin;
	if (pthread_create(&thread, NULL, soft_pwm_thread, tinfo) != 0)
		return 1;

	while (new_pin != 0)
		nanosleep(&req, NULL);

	return 0;
}

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
