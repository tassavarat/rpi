#include "../include/rpi.h"

#define LED_PIN	17
#define BTN_PIN	18

int main()
{
	unsigned int read, btn_prev_state = 1, btn_state = 1, led_state = 0;
	struct timespec state_change, current;
	const long unsigned capture = 50000000;

	gpio_setup();
	gpio_dir(LED_PIN, GPIO_OUT);
	gpio_dir(BTN_PIN, GPIO_INP);

	gpio_pud(BTN_PIN, PULL_UP);
	while (1) {
		if ((read = gpio_read(BTN_PIN)) != btn_prev_state)
			clock_gettime(CLOCK_MONOTONIC, &state_change);
		clock_gettime(CLOCK_MONOTONIC, &current);
		if ((long unsigned) (current.tv_nsec - state_change.tv_nsec) >= capture &&
				read != btn_state && (btn_state = read) == 0)
			led_state =! led_state;
		gpio_set(LED_PIN, led_state);
		btn_prev_state = read;
	}
	return 0;
}
