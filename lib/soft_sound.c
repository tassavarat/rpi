#include "rpi.h"

static volatile pthread_t threads[MAX_PINS];
static volatile int freq_arr[MAX_PINS];
static volatile unsigned int new_pin;

/**
 * soft_sound_set - set PWM value to specified pin
 * @pin:	pin to set PWM value
 * @val:	PWM value to set
 */
void soft_sound_set(unsigned int pin, unsigned int val)
{
	pin &= MAX_PINS - 1;
	if (val > 5000)
		val = 5000;
	freq_arr[pin] = val;
}

/**
 * soft_sound_thread - handles sound output
 */
void *soft_sound_thread()
{
	struct sched_param param;
	int pin, freq;
	struct timespec req;

	req.tv_sec = 0;

	param.sched_priority = sched_get_priority_max(SCHED_RR);
	if (pthread_setschedparam(pthread_self(), SCHED_RR, &param) != 0) {
		perror("pthread_setschedparam() error");
		return NULL;
	}

	pin = new_pin;
	new_pin = 0;
	while (1)
		if ((freq = freq_arr[pin]) == 0) {
			req.tv_nsec = 1000000;
			nanosleep(&req, NULL);
		} else {
			gpio_set(pin, 1);
			req.tv_nsec = 500000000 / freq;
			nanosleep(&req, NULL);
			gpio_set(pin, 0);
			nanosleep(&req, NULL);
		}
	pthread_exit(NULL);
}

/**
 * soft_sound_setup - set up software sound
 * @pin:	pin to apply sound
 *
 * Return:	0 on success, 1 on failure
 */
int soft_sound_setup(const unsigned int pin)
{
	pthread_t thread;
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = 1000000;

	gpio_dir(pin, GPIO_OUT);
	gpio_set(pin, 0);
	if (threads[pin] != 0)
		return 1;

	freq_arr[pin] = 0;
	new_pin = pin;
	if (pthread_create(&thread, NULL, soft_sound_thread, NULL) != 0)
		return 1;

	while (new_pin != 0)
		nanosleep(&req, NULL);
	return 0;
}
