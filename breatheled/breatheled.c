#include "../include/rpi.h"

#define LED_PIN	18

int main()
{
	int i;
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = 1000000;
	gpio_setup();
	gpio_dir(LED_PIN, PWM_OUT);

	while (1) {
		for (i = 0; i < 1024; ++i) {
			pwm_set(LED_PIN, i);
			nanosleep(&req, NULL);
		}
		for (i = 1024; i > -1; --i) {
			pwm_set(LED_PIN, i);
			nanosleep(&req, NULL);
		}
	}
	return 0;
}
