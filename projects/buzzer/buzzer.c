#include "../../include/rpi.h"

#define BUZZER_PIN	17
#define BTN_PIN		18

int main()
{
	if (gpio_setup() == 1)
		return 1;

	gpio_dir(BUZZER_PIN, GPIO_OUT);
	gpio_dir(BTN_PIN, GPIO_INP);

	gpio_pud(BTN_PIN, PULL_UP);
	while (1) {
		if (gpio_read(BTN_PIN) == 0)
			gpio_set(BUZZER_PIN, 1);
		else
			gpio_set(BUZZER_PIN, 0);
	}
	return 0;
}
