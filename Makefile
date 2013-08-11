CC=avr-gcc
CFLAGS=-Wall -DF_CPU=1000000L -I/usr/lib/avr/include/ -Os -mmcu=atmega32 -mcall-prologues
LDFLAGS=
SOURCES=BikeComputer.c Clock.c HD44780.c
HEADERS=BikeComputer.h Clock.h HD44780.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bike_computer
OUTPUT_HEX=bike_computer.hex

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	avr-objcopy -O ihex $(EXECUTABLE) $(OUTPUT_HEX)

.c.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE) $(OUTPUT_HEX)

deploy:
	avrdude -e -P usb -c USBasp -p ATmega32 -e -U flash:w:$(OUTPUT_HEX)
