#include <avr/io.h>
#include <util/delay.h>
#include "I2CSlave.h"

#define I2C_ADDR 0x11

uint8_t display = 4;

/**
 * @brief Function is called on I2C Write
 */
void callback_recv(uint8_t data) {
	display = data & 0xf;
}

/**
 * @brief Function is called on I2C Read
 */
void callback_req() {
	I2C_transmitByte(display);
}

int main() {
	uint8_t segment_values[16] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113 };

	/* Init the IOs for the display */
	DDRB = 0x3f;
	DDRD = (1<<PD7);

	/* Init I2C Slave */
	I2C_init(I2C_ADDR);
	I2C_setCallbacks(callback_recv, callback_req);


	while(1) {
		PORTB = segment_values[display & 0xf] & 0x3f;
		PORTD &= ~(1<<PD7);
		if((segment_values[display & 0xf] & (1<<6)) > 0)
			PORTD |= (1<<PD7);
	}
	return 0;
}
