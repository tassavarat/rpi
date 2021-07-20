#include <wiringPi.h>

#define BUZZER_PIN	0
#define BTN_PIN		1

int main()
{
	wiringPiSetup();

	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(BTN_PIN, INPUT);

	pullUpDnControl(BTN_PIN, PUD_UP);
	while (1) {
		if (digitalRead(BTN_PIN) == LOW)
			digitalWrite(BUZZER_PIN, HIGH);
		else
			digitalWrite(BUZZER_PIN, LOW);
	}
	return 0;
}
