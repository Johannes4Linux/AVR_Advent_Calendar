#include <avr/io.h>
#include <avr/wdt.h>
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

void print(char *text) {
	while(*text)
		send_char(*text++);
}

ISR(USART_RX_vect) {
	uint8_t cmd;
	cli();
	cmd = UDR0;
	send_char(cmd);
	print(" pressed\n\r");
	switch(cmd) {
		case 's':
		case 'S':
			PORTB |= 1;
			print("LED on\n\r");
			break;
		case 'r':
		case 'R':
			PORTB &= 0xfe;
			print("LED off\n\r");
			break;
		case 'A':
			while(1);
			break;
		default:
			break;
	}
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

	/* Enable the watchdog */
	wdt_enable(WDTO_2S); /* 2 seconds until reset */

	print("Start program\n\r");

	while(1) {
		wdt_reset();
		_delay_ms(250);
	}
	return 0;
}
