#include <softPwm.h>
#include <stdlib.h>
#include <wiringPi.h>

#define LED_PIN_R	0
#define LED_PIN_G	1
#define LED_PIN_B	2

int main()
{
	wiringPiSetup();

	softPwmCreate(LED_PIN_R, 0, 100);
	softPwmCreate(LED_PIN_G, 0, 100);
	softPwmCreate(LED_PIN_B, 0, 100);
	while (1) {
		softPwmWrite(LED_PIN_R, random() % 100);
		softPwmWrite(LED_PIN_G, random() % 100);
		softPwmWrite(LED_PIN_B, random() % 100);
		delay(1000);
	}
	return 0;
}
