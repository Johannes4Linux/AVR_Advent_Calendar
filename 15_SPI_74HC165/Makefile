FREQ      = 20000000
DEVICE    = atmega88pa
SRC       = main.c
TARGET    = spi_in

all: $(SRC)
	avr-gcc -Os -mmcu=$(DEVICE) -DF_CPU=$(FREQ) $(SRC) -o $(TARGET).elf
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).elf

clean:
	rm $(TARGET).elf $(TARGET).hex
