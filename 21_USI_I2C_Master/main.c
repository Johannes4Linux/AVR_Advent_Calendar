#include <avr/io.h>
#include <util/delay.h>
#include "i2c-master.h"

#define I2C_ADDR 0x11

int main() {
	uint8_t display = 0;

	i2c_master_init();

	while(1) {
		i2c_master_read(I2C_ADDR, display, &display, 1);
		display++;
		i2c_master_write(I2C_ADDR, display, 0, 0);
		_delay_ms(250);
	}
	return 0;
}
