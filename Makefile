CC=avr-gcc
CFLAGS=-c -Wall -DF_CPU=1000000UL -I/usr/lib/avr/include/ -D__AVR_ATmega32__ -Os
LDFLAGS=
SOURCES=BikeComputer.c Clock.c HD44780.c
HEADERS=BikeComputer.h Clock.h HD44780.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bike_computer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)

deploy:
	avrdude -e -P usb -c USBasp -p ATmega32 -e -U flash:w:$(EXECUTABLE):a
