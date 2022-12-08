#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON PD2

int counter = 0;

ISR(INT0_vect) {
	cli(); /* Disable global IRQs */
	_delay_ms(50); /* Debouncing */
	counter ++;
	sei(); /* globally enable IRQs */
}

int main() {
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRB = 0x7f;
	/* Set PD2 to input */
	DDRD &= ~(1<<PD2);

	/* Set internatl Pull Up */
	PORTD |= (1<<BUTTON);

	EICRA = 0x2; /* IRQ trigger falling edge */
	EIMSK = 1; /* Enable INT0 */
	sei(); /* globally enable IRQs */

	while(1) {
		/* Display ther counter variable */
		PORTB = segment_values[counter & 0xf];
	}
	return 0;
}
