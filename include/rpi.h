#ifndef RPI_H
#define RPI_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#define BCM2711_PERI_BASE	0xfe000000
#define GPIO_BASE		BCM2711_PERI_BASE + 0x200000
#define GPIO_PWM		BCM2711_PERI_BASE + 0x20C000

/* GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) */
#define INP_GPIO(g)		*(gpio + (g / 10)) &= ~(7 << (g % 10 * 3))
#define OUT_GPIO(g)		*(gpio + (g / 10)) |=  (1 << (g % 10 * 3))

#define GPIO_SET		*(gpio + 7)	/* sets bits which are 1, ignores bits which are 0 */
#define GPIO_CLR		*(gpio + 10)	/* clears bits which are 1, ignores bits which are 0 */
#define GPIO_READ(g)		*(gpio + 13) & (1 << g)

#define	FSEL_INPT		0b000
#define	FSEL_OUTP		0b001
#define	FSEL_ALT0		0b100
#define	FSEL_ALT1		0b101
#define	FSEL_ALT2		0b110
#define	FSEL_ALT3		0b111
#define	FSEL_ALT4		0b011
#define	FSEL_ALT5		0b010

#define	PWM_CONTROL		0
#define	PWM0_RANGE		4
#define	PWM0_DATA		5
#define	PWM1_RANGE		8
#define	PWM1_DATA		9

#define	PWM0_ENABLE		0x0001
#define	PWM1_ENABLE		0x0100

enum {
	GPIO_INP,
	GPIO_OUT,
	PWM_OUT,
	PULL_UP,
	PULL_DOWN
};

void gpio_setup();
void gpio_dir(const unsigned int pin, const int dir);
void gpio_set(const unsigned int pin, const int mode);
void pwm_set(const unsigned int pin, const unsigned int val);
void gpio_pud(const unsigned int pin, const int mode);
unsigned int gpio_read(const unsigned int pin);

#endif /* RPI_H */
