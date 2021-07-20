#include <wiringPi.h>

#define LED_PIN	0
#define BTN_PIN	1

int main()
{
	int reading, btn_prev_state = HIGH, btn_state = HIGH, led_state = LOW;
	unsigned long state_change_time, capture_time = 50;

	wiringPiSetup();
	pinMode(LED_PIN, OUTPUT);
	pinMode(BTN_PIN, INPUT);

	pullUpDnControl(BTN_PIN, PUD_UP);
	while (1) {
		if ((reading = digitalRead(BTN_PIN)) != btn_prev_state)
			state_change_time = millis();
		if (millis() - state_change_time >= capture_time && reading != btn_state &&
				(btn_state = reading) == LOW)
			led_state =! led_state;
		digitalWrite(LED_PIN, led_state);
		btn_prev_state = reading;
	}
	return 0;
}
