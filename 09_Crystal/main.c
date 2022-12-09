#include <avr/io.h>
#include <util/delay.h>

#define GREEN PB0

int main() {
	volatile long int i;
	/* Init the IOs for the LEDs */
	DDRB = (1<<PB0);
	PORTB = 1;

	while(1) {
		PORTB ^= 0x1;
		for(i=0; i<20000; i++)
			asm("NOP");
	}
	return 0;
}
