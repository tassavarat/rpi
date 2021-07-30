#ifndef RPI_H
#define RPI_H

#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#define BCM2711_PERI_BASE	0xfe000000
#define GPIO_BASE		BCM2711_PERI_BASE + 0x200000
#define GPIO_PWM		BCM2711_PERI_BASE + 0x20C000

/* 28 */
#define MAX_PINS		64
#define VALID_PIN(g)		(g < MAX_PINS)

/* GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) */
#define INP_GPIO(g)		*(gpio + (g / 10)) &= ~(7 << (g % 10 * 3))
#define OUT_GPIO(g)		*(gpio + (g / 10)) |=  (1 << (g % 10 * 3))

#define GPIO_SET		*(gpio + 7)	/* sets bits which are 1, ignores bits which are 0 */
#define GPIO_CLR		*(gpio + 10)	/* clears bits which are 1, ignores bits which are 0 */
#define GPIO_READ(g)		*(gpio + 13) & (1 << g)

enum {
	GPIO_INP,
	GPIO_OUT,
	PULL_UP,
	PULL_DOWN
};

/* gpio.c */
int gpio_setup();
int gpio_dir(const unsigned int pin, const int dir);
int gpio_set(const unsigned int pin, const int mode);
int gpio_pud(const unsigned int pin, const int mode);
unsigned int gpio_read(const unsigned int pin);

/* soft_pwm.c */
int soft_pwm_setup(const unsigned int pin, const unsigned int start_val, const unsigned int range);
int soft_pwm_set(const unsigned int pin, unsigned int val);

/* soft_sound.c */
int soft_sound_setup(const unsigned int pin);
void soft_sound_set(unsigned int pin, unsigned int val);
#endif /* RPI_H */
