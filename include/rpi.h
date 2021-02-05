#ifndef RPI_H
#define RPI_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2711_PERI_BASE	0xfe000000
#define GPIO_BASE		BCM2711_PERI_BASE + 0x200000

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

void gpio_setup();
void gpio_dir(const unsigned int pin, const int dir);
void gpio_set(const unsigned int pin, const int mode);
void gpio_pud(const unsigned int pin, const int mode);
unsigned int gpio_read(const unsigned int pin);

#endif /* RPI_H */
