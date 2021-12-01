CC = /bin/avr-gcc
F_CPU = 13000000
MCU = atmega328p
PORT = /dev/ttyS0
SOURCE = projet

$(SOURCE).out: $(SOURCE).c
	$(CC) $^ -o $@ -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Os

$(SOURCE).bin: $(SOURCE).out
	avr-objcopy -O binary $^ $@

flash: $(SOURCE).bin
	avrdude -c usbasp -p m328p -P $(PORT) -U flash:w:$^

all:
	$(SOURCE).bin

read:
	stty -F $(PORT) 9600
	while true; do cat $(PORT); done