FREQ      = 1000000
DEVICE    = attiny44
SRC       = twi/i2c-master.c main.c
TARGET    = usi_i2c_master
INC       = -Itwi/

all: $(SRC)
	avr-gcc -Os -mmcu=$(DEVICE) -DF_CPU=$(FREQ) $(INC) $(SRC) -o $(TARGET).elf
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).elf

clean:
	rm $(TARGET).elf $(TARGET).hex
