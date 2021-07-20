#include "../../include/rpi.h"

#define LED_PIN_R	17
#define LED_PIN_G	18
#define LED_PIN_B	27

int init_pwm()
{
	if (soft_pwm_setup(LED_PIN_R, 0, 100) == 1)
		return 1;
	if (soft_pwm_setup(LED_PIN_G, 0, 100) == 1)
		return 1;
	if (soft_pwm_setup(LED_PIN_B, 0, 100) == 1)
		return 1;
	return 0;
}

int main()
{
	if (gpio_setup() == 1)
		return 1;

	if (init_pwm() == 1)
		return 1;

	while (1) {
		soft_pwm_set(LED_PIN_R, random() % 100);
		soft_pwm_set(LED_PIN_G, random() % 100);
		soft_pwm_set(LED_PIN_B, random() % 100);
		sleep(1);
	}
	return 0;
}
