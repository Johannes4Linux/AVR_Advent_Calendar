#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUDRATE 4800
#define MYUBRR F_CPU / 16 / BAUDRATE - 1

/**
 * @brief function to receive a char
 *
 * return received char
 */
char recv_char() {
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

/**
 * @brief function to send a char
 *
 * @param data char to send
 */
void send_char(char data) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

ISR(USART_RX_vect) {
	cli();
	UDR0 = UDR0 +1;
	sei();
}

int main() {
	/* Set TX pin to output */
	DDRD = (1<<PD1);

	/* Init LED Pin */
	DDRB = (1<<PB0);

	/* Set the baudrate */
	UBRR0H = (unsigned char) MYUBRR << 8;
	UBRR0L = (unsigned char) MYUBRR;

	/* Enable RX and TX */
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0)| (1<<RXCIE0);

	/* Globally enable IRQs */
	sei();

	while(1) {
		PORTB ^= (1<<PB0);
		_delay_ms(250);
	}
	return 0;
}
