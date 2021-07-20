#include <wiringPi.h>

#define LED_PIN	0
#define BTN_PIN	1

int main()
{
	wiringPiSetup();

	pinMode(LED_PIN, OUTPUT);
	pinMode(BTN_PIN, INPUT);

	pullUpDnControl(BTN_PIN, PUD_UP);
	while(1) {
		if (digitalRead(BTN_PIN) == LOW)
			digitalWrite(LED_PIN, HIGH);
		else
			digitalWrite(LED_PIN, LOW);
	}
	return 0;
}
