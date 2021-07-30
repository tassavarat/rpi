#include "rpi.h"

#define BUZZER_PIN	17
#define BTN_PIN		18

int main()
{
	int i;

	if (gpio_setup() == 1)
		return 1;

	gpio_dir(BUZZER_PIN, GPIO_OUT);
	gpio_dir(BTN_PIN, GPIO_INP);
	soft_sound_setup(BUZZER_PIN);
	gpio_pud(BTN_PIN, PULL_UP);
	while (1) {
		if (gpio_read(BTN_PIN) == 0)
			for (i = 0; i < 360; ++i)
				soft_sound_set(BUZZER_PIN, 2000 + (sin(i * M_PI / 180)) * 500);
		else
			soft_sound_set(BUZZER_PIN, 0);
	}
	return 0;
}
