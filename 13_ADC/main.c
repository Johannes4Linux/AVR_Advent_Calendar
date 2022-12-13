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

/**
 * @brief Converts a float to a string
 */
void float2str(float in, char *out) {
	int post = (int) in;
	int pre = (int) ((in - post) * 100);
	char buf[25];
	int len = 0, i;

	while(post) {
		buf[len++] = post % 10 + '0';
		post = post / 10;
	}

	for(i=0; i<len; i++)
		out[i] = buf[len - i - 1];
	if(len == 0)
		out[i++] = '0';
	
	out[i++] = '.';
	out[i++] = pre / 10 + '0';
	out[i++] = pre % 10 + '0';
	out[i] = 0;
}

int main() {
	char buf[32];
	uint16_t raw;
	float voltage;

	/* Set TX pin to output */
	DDRD = (1<<PD1);

	/* Set the baudrate */
	UBRR0H = (unsigned char) MYUBRR << 8;
	UBRR0L = (unsigned char) MYUBRR;

	/* Enable RX and TX */
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);

	/* Init ADC */
	ADMUX = 5;
	ADCSRA = (1<<ADEN) | 5; /* Set prescaler to 32 */

	while(1) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC)); /* Waiting for conversion to be done */
		raw = ADC;
		voltage = 5.0 * raw / (1<<10);
		float2str(voltage, buf);
		print("Analog Input: ");
		print(buf);
		print("V\n\r");
		_delay_ms(500);
	}
	return 0;
}
