TARGET_PLATFORM=atmega32
CC=avr-gcc
CFLAGS=-Wall -DF_CPU=1000000UL -I/usr/lib/avr/include/ -mmcu=$(TARGET_PLATFORM) -Os -mcall-prologues -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields
LDFLAGS=
SOURCES=BikeComputer.c Clock.c HD44780.c
HEADERS=BikeComputer.h Clock.h HD44780.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bike_computer
OUTPUT_HEX=bike_computer.hex

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -mmcu=$(TARGET_PLATFORM) $(LDFLAGS) $(OBJECTS) -o $@
	avr-size --format=avr --mcu=$(TARGET_PLATFORM) $(EXECUTABLE)
	avr-objcopy -R .eeprom -O ihex $(EXECUTABLE) $(OUTPUT_HEX)

.c.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE) $(OUTPUT_HEX)

deploy:
	avrdude -e -P usb -c USBasp -p ATmega32 -e -U flash:w:$(OUTPUT_HEX):a
