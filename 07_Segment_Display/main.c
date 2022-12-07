#include <avr/io.h>
#include <util/delay.h>

#define BUTTON PD7

int main() {
	int state=0, counter = 0;
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRB = 0x7f;
	/* Set PD7 to input */
	DDRD &= ~(1<<PD7);

	/* Set internal Pull Uü */
	PORTD |= (1<<BUTTON);

	while(1) {
		if((PIND & (1<<BUTTON)) == 0) {
			_delay_ms(50); /* Debouncing */
			if(state == 0) {
				counter++;
				state = 1;
			}
		}
		else {
			state = 0;
		}
		PORTB = segment_values[counter & 0xf];
	}
	return 0;
}
