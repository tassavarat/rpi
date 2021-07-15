#include <softPwm.h>
#include <wiringPi.h>

#define LED_PIN	1

int main()
{
	int i;

	wiringPiSetup();
	softPwmCreate(LED_PIN, 0, 100);
	while (1) {
		for (i = 0; i < 100; ++i) {
			softPwmWrite(LED_PIN, i);
			delay(20);
		}
		delay(300);
		for (i = 100; i > -1; --i) {
			softPwmWrite(LED_PIN, i);
			delay(20);
		}
		delay(300);
	}
	return 0;
}
