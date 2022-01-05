#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <stdbool.h>

#include <stdio.h>

#include "led_spi.h"
#include "led_buffers.h"

#include "USART_COM.h"

#define FOSC F_CPU // Clock Speed
#define BAUD 38400
#define MYUBRR FOSC / 16 / BAUD - 1

#define abs(x) (x) < 0 ? (-x) : (x)

const int16_t nb_arcs = 256;
uint8_t current_arc = 0;
int16_t arcs_drawn = 0;
uint8_t phase_shift = 64;

// HORLOGE
uint8_t fiftieth_seconds = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;

volatile bool frame_update_requested = false;

ISR(TIMER0_COMPA_vect)
{ // Timer 0 interrupt (for RTC)
    TCNT0 = 0;
    fiftieth_seconds++;
    if (fiftieth_seconds == 50)
    { // One whole second has passed
        fiftieth_seconds = 0;
        seconds++;
        frame_update_requested = true;
    }
    if (seconds == 60)
    {
        minutes++;
        seconds = 0;
    }
    if (minutes == 60)
    {
        hours++;
        minutes = 0;
    }
    if (hours == 24)
    {
        hours = 0;
    }
}

ISR(INT0_vect) // Hall sensor interrupt (new arc cycle)
{
    TCNT1 = 0; // Reset arc timer

    cli(); // Disable interrupts

    int16_t delta = arcs_drawn - nb_arcs;
    OCR1A += delta; // Tweak arc timer "overflow" to get closer to the ideal arc count

    arcs_drawn = 0;
    current_arc = phase_shift;

    sei(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) // 16 bit counter "overflow" (new arc)
{
    TCNT1 = 0;

    uint8_t led_buf_index = current_arc >> 3;
    uint8_t led_buf_value_mask = _BV(current_arc & 0b111);

    uint16_t led_values = 0;
    for (uint8_t led = 0; led < N_LED; led++)
        led_values |= ((led_bufs[led][led_buf_index] & led_buf_value_mask) != 0) << led;

    write_leds(led_values);

    if (arcs_drawn != UINT16_MAX)
        arcs_drawn++;

    current_arc++; // Will overflow, and we want that so the image can "wrap around" during spindown
}

void update_frame_clock_analog(void)
{
    const uint8_t hours_hand_length = 5; // 5 leds
    const uint8_t minutes_hand_length = 10;
    const uint8_t seconds_hand_length = 14;

    // Formula is nb_arcs - (approximate arc value) because the motor runs counter-clockwise

    uint8_t hours_arc = (nb_arcs - (hours << 4) + (hours << 2) + hours + (hours >> 2)) * 2; // 12 hours in a standard clock
    uint8_t minutes_arc = nb_arcs - ((minutes << 2) + (minutes >> 2));
    uint8_t seconds_arc = nb_arcs - ((seconds << 2) + (seconds >> 2));

    reset_buffers(); // Rather costly, could be optimized

    for (uint8_t i = 0; i < N_LED; i++)
    {
        if (i < hours_hand_length)
            set_led_arc_bit(i, hours_arc);
        if (i < minutes_hand_length)
            set_led_arc_bit(i, minutes_arc);
        if (i < seconds_hand_length)
            set_led_arc_bit(i, seconds_arc);
    }

    for (uint8_t i = 0; i < led_buf_len[N_LED - 1]; i++)
        led_bufs[N_LED - 1][i] = 0xFF;
}

void update_frame_clock_digital(void)
{
    return;
}

int main()
{
    init_led_spi();

    // Activate interrupt on PD0 (hall sensor)
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);

    //Setup 16 bit clock for rotationnal period
    TCCR1B |= _BV(CS10);

    OCR1A = INT16_MAX / 2;
    TIMSK1 |= _BV(OCIE1A);

    // //OCR0A = 100; // calcul pour avoir un raise par seconde Doc page 87 - 14.9.2
    TCCR0B |= (1 << CS02) | (1 << CS00); // On active le compteur avec un prescaling de 1024 (A mettre à la fin avant la boucle while)
    OCR0A = 254;                         // calcul pour avoir un raise par seconde Doc page 87 - 14.9.2
    TIMSK0 |= _BV(OCIE0A);

    USART_Init(MYUBRR);
    Interrupt_Init();

    sei(); // Enable interrupts

    reset_buffers();

    while (1)
    {
        if (frame_update_requested == true)
        {
            update_frame_clock_analog();
            frame_update_requested = false;
        }
        //_delay_us(1);
    }
}