#include <wiringPi.h>

#define LEDCOUNT	10
int PIN_AR[LEDCOUNT] =	{0, 1, 2, 3, 4, 5, 6, 21, 22, 26};

/**
 * light_barled_lib - light LED for specified duration
 * @i:	LED to light
 * @ns:	time in milliseconds to stay lit
 */
void light_barled_lib(int i, int ms)
{
	digitalWrite(PIN_AR[i], LOW);
	delay(ms);
	digitalWrite(PIN_AR[i], HIGH);
}

int main()
{
	int i;

	wiringPiSetup();
	for (i = 0; i < LEDCOUNT; ++i)
		pinMode(PIN_AR[i], OUTPUT);

	while (1) {
		for (i = 0; i < LEDCOUNT; ++i)
			light_barled_lib(i, 100);
		for (i = LEDCOUNT - 1; i > -1; --i)
			light_barled_lib(i, 100);
	}
	return 0;
}
