#include <wiringPi.h>

#define LED_PIN	1

int main()
{
	int i;

	wiringPiSetup();
	pinMode (1, PWM_OUTPUT) ;

	while (1) {
		for (i = 0; i < 1024; ++i) {
			pwmWrite(LED_PIN, i);
			delay(1);
		}
		for (i = 1024; i > -1; --i) {
			pwmWrite(LED_PIN, i);
			delay(1);
		}
	}
	return 0;
}
