FREQ      = 1000000
DEVICE    = attiny44
SRC       = main.c
TARGET    = usi_spi_master

all: $(SRC)
	avr-gcc -Os -mmcu=$(DEVICE) -DF_CPU=$(FREQ) $(SRC) -o $(TARGET).elf
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).elf

clean:
	rm $(TARGET).elf $(TARGET).hex
