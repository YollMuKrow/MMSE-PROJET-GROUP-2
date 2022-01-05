#ifndef LED_SPI_H
#define LED_SPI_H

#include <avr/interrupt.h>

uint8_t led_state_l; // Leds 0-7 (0 being LSB)
uint8_t led_state_h; // Leds 8-15

bool led_transmit_status;

void init_led_spi()
{
    DDRB |= _BV(PB2) | _BV(PB3) | _BV(PB5); // We need MOSI, SCK and SS (PB2) as outputs
    // If PB2 is not set to output, SPI might start in slave mode (we dont want that)

    SPCR = _BV(MSTR) | _BV(SPE); // Set SPI to master mode and enable it

    SPCR |= _BV(SPIE); // Enable SPI Transfer Interrupt
    DDRC |= _BV(PC2) | _BV(PC1); // Set PC1 and PC2 to output mode

    PORTC &= ~_BV(PC2); // Latch low
    PORTC &= ~_BV(PC1); // Set PC1 output to LOW (enable LED driver output)
}

void set_driver_output(bool enabled)
{
}

void write_leds(uint16_t led_state)
{
    led_state_l = led_state & 0xFF;
    led_state_h = led_state >> 8;

    SPDR = led_state_h;
    led_transmit_status = true;
}

ISR(SPI_STC_vect)
{
    if (!led_transmit_status)
    {
        PORTC |= _BV(PC2); // Toggle the latch pin to write both bytes to the LED driver memory
        PORTC &= ~_BV(PC2);
        return;
    }

    SPDR = led_state_l;
    led_transmit_status = false;
}

#endif //LED_SPI_H