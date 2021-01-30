#include <unistd.h>
#include <wiringPi.h>

#define p1	0

int main()
{
	wiringPiSetup();
	pinMode(p1, OUTPUT);
	
	while (1) {
		digitalWrite(p1, HIGH);
		sleep(1);
		digitalWrite(p1, LOW);
		sleep(1);
	}
}
