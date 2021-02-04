#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2711_PERI_BASE	0xfe000000
#define GPIO_BASE		BCM2711_PERI_BASE + 0x200000

#define GPIO_INP		0
#define GPIO_OUT		1

/* GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) */
#define INP_GPIO(g)		*(gpio + (g / 10)) &= ~(7 << (g % 10 * 3))
#define OUT_GPIO(g)		*(gpio + (g / 10)) |=  (1 << (g % 10 * 3))

#define GPIO_SET		*(gpio + 7)	/* sets bits which are 1, ignores bits which are 0 */
#define GPIO_CLR		*(gpio + 10)	/* clears bits which are 1, ignores bits which are 0 */
#define GPIO_READ		*(gpio + 13) & (1 << g)

#define GPIO_PULL		*(gpio + 37)
#define GPIO_PULLCLK0		*(gpio + 38)

void gpio_setup();
void gpio_dir(const uint pin, int dir);
void gpio_set(const uint pin, int mode);
