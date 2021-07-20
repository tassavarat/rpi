#include "rpi.h"

#define LEDCOUNT	10
int PIN_AR[LEDCOUNT] =	{17, 18, 27, 22, 23, 24, 25, 5, 6, 12};

/**
 * light_barled - light LED for specified duration
 * @i:	LED to light
 * @ns:	time in nanoseconds to stay lit
 */
void light_barled(int i, int ns)
{
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = ns;

	gpio_set(PIN_AR[i], 0);
	nanosleep(&req, NULL);
	gpio_set(PIN_AR[i], 1);
}

int main()
{
	int i;

	gpio_setup();
	for (i = 0; i < LEDCOUNT; ++i)
		gpio_dir(PIN_AR[i], GPIO_OUT);

	while (1) {
		for (i = 0; i < LEDCOUNT; ++i)
			light_barled(i, 100000000);
		for (i = LEDCOUNT - 1; i > -1; --i)
			light_barled(i, 100000000);
	}
	return 0;
}
