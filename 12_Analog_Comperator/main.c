#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define GREEN PB0

ISR(ANALOG_COMP_vect) {
	if(ACSR & (1<<ACO))
			PORTB |= 1;
	else
			PORTB &= ~(1);
}

int main() {
	volatile long int i;
	/* Init the IOs for the LEDs */
	DDRB = (1<<PB0);
	PORTB = 1;

	/* Init the Analog Comperator */
	ACSR = (1<<ACIE);
	ADCSRB = 0;
	sei();

	while(1) {
		;
	}
	return 0;
}
