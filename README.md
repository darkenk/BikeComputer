Project BikeComputer

Basic idea is to create bike computer using atmega32a.

SETUP:
install: gcc-avr, avrdude, avr-libc
put followin line into /etc/udev/rules.d/52-avr.rules
SUBSYSTEMS=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05dc", GROUP="users", MODE="0666"

INSTALLATION:
To build:
make all

To deploy:
make deploy

To clean:
make clean
