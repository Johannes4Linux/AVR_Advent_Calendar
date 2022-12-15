#include <avr/io.h>
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

/**
 * @brief Sends a 0 terminated string over UART
 */
void print(char *text) {
	while(*text)
		send_char(*text++);
}

int main() {
	uint8_t value;
	int i;
	char buf[5] ;

	/* Set TX pin to output */
	DDRD = (1<<PD1);

	/* Set the baudrate */
	UBRR0H = (unsigned char) MYUBRR << 8;
	UBRR0L = (unsigned char) MYUBRR;

	/* Enable RX and TX */
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);

	/* Init the SPI interface */
	DDRB = (1<<PB5) | (1<<PB3) | (1<<PB2) | (1<<PB1);
	PORTB |= (1<<PB1);
	SPCR = (1<<MSTR) | 3;
	SPCR |= (1<<SPE);

	while(1) {
		/* Toggle the Load pin */
		PORTB &= ~(1<<PB1);
		_delay_us(100);
		PORTB |= (1<<PB1);

		SPDR = 0xff;
		while(!(SPSR & (1<<SPIF)));
		value = SPDR;

		print("\n\r");
		for(i=0; i<8; i++) {
			print("\n\rDIP");
			buf[0]=i+'0';
			buf[1] = ' ';
			buf[2] = 0;
			print(buf);
			if((value & (1<<i)) > 0)
				print("1");
			else
				print("0");
		}
		_delay_ms(500);
	}
	return 0;
}
