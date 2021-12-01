//
// Created by alheritier on 01/12/2021.
//

#include <avr/io.h>
#include <util/delay.h>

int main() {
    // Pin is output(low independence)
    DDRB |= _BV(PB5);

    // Pin is high level (5v)
    while(1){
        PORTB |= _BV(PB5);
        _delay_ms(500);
        PORTB &= ~_BV(PB5);
        _delay_ms(500);

    }
}
