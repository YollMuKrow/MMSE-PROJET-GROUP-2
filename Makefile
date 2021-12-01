CC = /bin/avr-gcc
F_CPU = 13000000
MCU = atmega328p
PORT = /dev/ttyS0

main.out: main.c
	$(CC) $^ -o $@ -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Os

main.bin: main.out
	avr-objcopy -O binary $^ $@

flash: main.bin
	avrdude -c usbasp -p m328p -P $(PORT) -U flash:w:$^

all:
	main.bin

read:
	stty -F $(PORT) 9600
	while true; do cat $(PORT); done