#include <avr/io.h>
#include <util/delay.h>

#define BUTTON PB0
#define EEPROM_ADDR 0x2;

int main() {
	int state=0, counter = 0;
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRC = 0x3f;
	DDRD = 1;
	/* Set PB0 to input */
	DDRB &= ~(1<<PB0);

	/* Set internal Pull Uü */
	PORTB |= (1<<BUTTON);

	/* Read from EEPROM to get counter value */
	while(EECR & (1<<EEPE));
	EEAR = EEPROM_ADDR;
	EECR |= (1<<EERE);
	counter = EEDR;

	while(1) {
		if((PINB & (1<<BUTTON)) == 0) {
			_delay_ms(50); /* Debouncing */
			if(state == 0) {
				counter++;
				/* Write value to eeprom */
				while(EECR & (1<<EEPE));
				EEAR = EEPROM_ADDR;
				EEDR = counter;
				EECR |= (1<<EEMPE);
				EECR |= (1<<EEPE);
				state = 1;
			}
		}
		else {
			state = 0;
		}
		PORTC = segment_values[counter & 0xf] & 0x3f;
		PORTD &= ~(1<<0);
		if((segment_values[counter & 0xf] & 0x40) > 0) 
			PORTD |= (1<<0);

	}
	return 0;
}
