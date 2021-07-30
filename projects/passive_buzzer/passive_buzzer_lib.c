#include <math.h>
#include <softTone.h>
#include <wiringPi.h>

#define BUZZER_PIN	0
#define BTN_PIN		1

void buzzer_on(const unsigned int pin)
{
	int i;

	for (i = 0; i < 360; ++i)
		softToneWrite(pin, 2000 + (sin(i * M_PI / 180)) * 500);
}

void buzzer_off(const unsigned int pin)
{
	softToneWrite(pin, 0);
}

int main()
{
	wiringPiSetup();

	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(BTN_PIN, INPUT);
	softToneCreate(BUZZER_PIN);
	pullUpDnControl(BTN_PIN, PUD_UP);
	while (1) {
		if (digitalRead(BTN_PIN) == LOW)
			buzzer_on(BUZZER_PIN);
		else
			buzzer_off(BUZZER_PIN);
	}
	return 0;
}
