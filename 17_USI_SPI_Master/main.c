#include <avr/io.h>
#include <util/delay.h>

#define PORT_SPI PORTA
#define DDR_SPI DDRA
#define USI_SCK PA4
#define USI_DO PA5
#define USI_DI PA6
#define USI_CS PA7

#define UNSET_CS PORT_SPI|=(1<<USI_CS)
#define SET_CS PORT_SPI&=~(1<<USI_CS)

uint8_t spi_xfer(uint8_t data) {
	SET_CS;
	USIDR = data;
	USISR = (1<<USIOIF);

	while((USISR & (1<<USIOIF)) == 0) {
		USICR = (1<<USIWM0) | (1<<USICS1) | (1<<USICLK) | (1<<USITC);
	}

	UNSET_CS;
	return USIDR;
}


int main() {
	int display;
	DDR_SPI &= ~(1<<USI_DI);
	DDR_SPI |= (1<<USI_DO)|(1<<USI_SCK)|(1<<USI_CS);

	while(1) {
		/* Read current displayed value */
		spi_xfer(0x11);
		display = spi_xfer(0xff) + 1;

		/* Set the new value to display */
		spi_xfer(0x22);
		spi_xfer(display);
		_delay_ms(500);
	}
	return 0;
}
