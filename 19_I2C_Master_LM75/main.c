#include <avr/io.h>
#include <util/delay.h>
#include "twi_master.h"

#define BAUDRATE 4800
#define MYUBRR F_CPU / 16 / BAUDRATE - 1

#define LM75_ADDR 0x48

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
	uint8_t reg_addr = 0;
	float temperature;

	/* Set TX pin to output */
	DDRD = (1<<PD1);

	/* Set the baudrate */
	UBRR0H = (unsigned char) MYUBRR << 8;
	UBRR0L = (unsigned char) MYUBRR;

	/* Enable RX and TX */
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);

	/* Init TWI */
	tw_init(TW_FREQ_100K, 1);

	while(1) {
		tw_master_transmit(LM75_ADDR, &reg_addr, 1, 0);

		tw_master_receive(LM75_ADDR, (uint8_t *) &raw, 2);

		raw = ((raw & 0xff) << 3) | (raw >> 13);
		temperature = raw * 0.125;
		float2str(temperature, buf);
		print("Temperature: ");
		print(buf);
		print("degree\n\r");
		_delay_ms(1000);
	}
	return 0;
}
