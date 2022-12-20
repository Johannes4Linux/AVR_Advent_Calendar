FREQ      = 20000000
DEVICE    = atmega88pa
SRC       = twi/I2CSlave.c main.c
TARGET    = i2c_slave
INC       = -Itwi/

all: $(SRC)
	avr-gcc -Os -mmcu=$(DEVICE) -DF_CPU=$(FREQ) $(INC) $(SRC) -o $(TARGET).elf
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).elf

clean:
	rm $(TARGET).elf $(TARGET).hex
