#include <softPwm.h>
#include <wiringPi.h>

#define LEDPIN	1

int main()
{
	int i;

	wiringPiSetup();
	softPwmCreate(LEDPIN, 0, 100);

	while (1) {
		for (i = 0; i < 100; ++i) {
			softPwmWrite(LEDPIN, i);
			delay(20);
		}
		delay(300);
		for (i = 100; i > -1; --i) {
			softPwmWrite(LEDPIN, i);
			delay(20);
		}
		delay(300);
	}
	return 0;
}
