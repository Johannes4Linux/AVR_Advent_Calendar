#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t  counter = 0;

ISR(TIMER1_COMPA_vect) {
	counter++;
	TCNT1 = 0;
}

int main() {
	int state=0;
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRC = 0x3f;
	DDRD = 1;

	TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
	OCR1A = 9766; 
	TIMSK1 = (1<<OCIE1A);
	sei();

	while(1) {
		PORTC = segment_values[counter & 0xf] & 0x3f;
		PORTD &= ~(1<<0);
		if((segment_values[counter & 0xf] & 0x40) > 0) 
			PORTD |= (1<<0);
	}
	return 0;
}
