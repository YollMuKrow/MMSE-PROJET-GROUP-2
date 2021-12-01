//
// Created by alheritier on 01/12/2021.
//

#include <avr/io.h>
#include <util/delay.h>

int main() {
    // Pin is output(low independence)
    DDRD |= _BV(PD6);

    // Pin is high level (5v)
    while(1){
        PORTD |= _BV(PD6);
        _delay_ms(500);
        PORTD &= ~_BV(PD6);
        _delay_ms(500);

    }
}
