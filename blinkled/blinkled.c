#include "rpi.h"

#define p1	17

int main()
{
	volatile unsigned int *gpio;

	gpio_setup(&gpio);
	INP_GPIO(p1), OUT_GPIO(p1);

	while (1) {
		GPIO_SET = 1 << p1;
		sleep(1);
		GPIO_CLR = 1 << p1;
		sleep(1);
	}
	return 0;
}
