#include <avr/io.h>
#include <util/delay.h>

#define BUTTON PD7

int main() {
	int counter = 0;
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRB = (1<<PB5) | (1<<PB3) | (1<<PB2) | (1<<PB1);
	PORTB &= ~(1<<PB1); /* PB1 -> RCLK */

	/* Init SPI interface */
	SPCR = (1<<MSTR) | 3;
	SPCR |= (1<<SPE);

	while(1) {
		SPDR = segment_values[counter & 0xf];
		while(!(SPSR & (1<<SPIF)));
		/* Toggle the RCLK */
		PORTB |= (1<<PB1); /* Set RCLK */
		_delay_ms(5);
		PORTB &= ~(1<<PB1); /* Reset RCLK */
		
		counter++;
		_delay_ms(500);

	}
	return 0;
}
