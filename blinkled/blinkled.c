#include "../include/rpi.h"

#define LED_PIN	17

int main()
{
	gpio_setup();
	gpio_dir(LED_PIN, GPIO_OUT);
	while (1) {
		gpio_set(LED_PIN, 1);
		sleep(1);
		gpio_set(LED_PIN, 0);
		sleep(1);
	}
	return 0;
}
