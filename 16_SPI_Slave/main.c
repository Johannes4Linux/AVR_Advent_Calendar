#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int state=0, display = 0;

#define GET 0x11
#define SET 0x22
#define IDLE 0x0


ISR(SPI_STC_vect) {
	switch(state) {
		case IDLE:
			state = SPDR;
			switch(state) {
				case GET:
					SPDR = display;
					break;
				case SET:
					break;
				default:
					state = IDLE;
					break;
			}
			break;
		case GET:
			state = IDLE;
			break;
		case SET:
			display = SPDR & 0xf;
			state = IDLE;
			break;
		default:
			state = IDLE;
			break;
	}
}

int main() {
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRC = 0x3f;
	DDRB = (1<<PB1);

	/* Init the pins for SPI */
	DDRB |= (1<<PB4);

	/* Init the SPI IP */
	SPCR = (1<<SPIE);
	SPCR |= (1<<SPE);
	sei();

	while(1) {
		PORTC = segment_values[display & 0xf] & 0x3f;
		PORTB &= ~(1<<PB1);
		if((segment_values[display & 0xf] & (1<<6)) > 0)
			PORTB |= (1<<PB1);
	}
	return 0;
}
