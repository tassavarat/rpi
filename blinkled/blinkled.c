#include "rpi.h"
#include <sys/stat.h>

#define LED_PIN	17

int main()
{
	volatile unsigned int *gpio;

	gpio_setup(&gpio);
	INP_GPIO(LED_PIN), OUT_GPIO(LED_PIN);
	while (1) {
		GPIO_SET = 1 << LED_PIN;
		sleep(1);
		GPIO_CLR = 1 << LED_PIN;
		sleep(1);
	}
	return 0;
}
