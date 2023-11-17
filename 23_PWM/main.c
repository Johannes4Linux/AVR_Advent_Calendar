#include <avr/io.h>
#include <util/delay.h>

int main() {
	uint8_t i;

	/* Init the IO for the LED */
	DDRD = (1<<PD6);

	/* Setup the Timer for PWM */
	TCCR0A = (1<<COM0A1) | (1<<WGM00);
	TCCR0B = (1<<CS00) | (1<<CS02);

	while(1) {
		OCR0A = 0;
		_delay_ms(100);
		OCR0A = 64;
		_delay_ms(100);
		OCR0A = 128;
		_delay_ms(100);
		OCR0A = 192;
		_delay_ms(100);
		OCR0A = 255;
		_delay_ms(100);
	}
	return 0;
}
