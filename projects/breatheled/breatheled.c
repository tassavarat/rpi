#include "rpi.h"

#define LED_PIN	18

int main()
{
	int i;
	struct timespec req;

	req.tv_sec = 0;
	if (gpio_setup() == 1)
		return 1;

	if (soft_pwm_setup(LED_PIN, 0, 100) == 1)
		return 1;

	while (1) {
		for (i = 0; i < 100; ++i) {
			soft_pwm_set(LED_PIN, i);
			req.tv_nsec = 20000000;
			nanosleep(&req, NULL);
		}
		req.tv_nsec = 300000000;
		nanosleep(&req, NULL);
		for (i = 100; i > -1; --i) {
			soft_pwm_set(LED_PIN, i);
			req.tv_nsec = 20000000;
			nanosleep(&req, NULL);
		}
		req.tv_nsec = 300000000;
		nanosleep(&req, NULL);
	}
	return 0;
}
