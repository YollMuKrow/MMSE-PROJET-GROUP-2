#ifndef LED_BUFFERS_H
#define LED_BUFFERS_H

#include <avr/common.h>

#define N_LED 16

#ifndef LED_00_WIDTH
#define LED_00_WIDTH 32 // In bytes
#define LED_01_WIDTH 32
#define LED_02_WIDTH 32
#define LED_03_WIDTH 32
#define LED_04_WIDTH 32
#define LED_05_WIDTH 32
#define LED_06_WIDTH 32
#define LED_07_WIDTH 32
#define LED_08_WIDTH 32
#define LED_09_WIDTH 32
#define LED_10_WIDTH 32
#define LED_11_WIDTH 32
#define LED_12_WIDTH 32
#define LED_13_WIDTH 32
#define LED_14_WIDTH 32
#define LED_15_WIDTH 32
#endif //LED_WIDTH

uint8_t led00_buf[LED_00_WIDTH] = {0};
uint8_t led01_buf[LED_01_WIDTH] = {0};
uint8_t led02_buf[LED_02_WIDTH] = {0};
uint8_t led03_buf[LED_03_WIDTH] = {0};
uint8_t led04_buf[LED_04_WIDTH] = {0};
uint8_t led05_buf[LED_05_WIDTH] = {0};
uint8_t led06_buf[LED_06_WIDTH] = {0};
uint8_t led07_buf[LED_07_WIDTH] = {0};
uint8_t led08_buf[LED_08_WIDTH] = {0};
uint8_t led09_buf[LED_09_WIDTH] = {0};
uint8_t led10_buf[LED_10_WIDTH] = {0};
uint8_t led11_buf[LED_11_WIDTH] = {0};
uint8_t led12_buf[LED_12_WIDTH] = {0};
uint8_t led13_buf[LED_13_WIDTH] = {0};
uint8_t led14_buf[LED_14_WIDTH] = {0};
uint8_t led15_buf[LED_15_WIDTH] = {0};

uint8_t led_buf_len[N_LED] = {LED_00_WIDTH,
                              LED_01_WIDTH,
                              LED_02_WIDTH,
                              LED_03_WIDTH,
                              LED_04_WIDTH,
                              LED_05_WIDTH,
                              LED_06_WIDTH,
                              LED_07_WIDTH,
                              LED_08_WIDTH,
                              LED_09_WIDTH,
                              LED_10_WIDTH,
                              LED_11_WIDTH,
                              LED_12_WIDTH,
                              LED_13_WIDTH,
                              LED_14_WIDTH,
                              LED_15_WIDTH};

uint8_t *led_bufs[N_LED] = {led00_buf,
                            led01_buf,
                            led02_buf,
                            led03_buf,
                            led04_buf,
                            led05_buf,
                            led06_buf,
                            led07_buf,
                            led08_buf,
                            led09_buf,
                            led10_buf,
                            led11_buf,
                            led12_buf,
                            led13_buf,
                            led14_buf,
                            led15_buf};

void reset_buffers(void)
{
    for (uint8_t led = 0; led < N_LED; led++)
        for (uint8_t i = 0; i < led_buf_len[led]; i++)
            led_bufs[led][i] = 0x00;
}

void invert_buffers(void)
{
    for (uint8_t led = 0; led < N_LED; led++)
        for (uint8_t i = 0; i < led_buf_len[led]; i++)
            led_bufs[led][i] ^= 0xFF;
}

bool get_led_arc_bit(uint8_t led, uint8_t arc)
{
    return (led_bufs[led][arc >> 3] & _BV(arc & 0b111)) >> (arc & 0b111);
}

void set_led_arc_bit(uint8_t led, uint8_t arc)
{
    led_bufs[led][arc >> 3] |= _BV(arc & 0b111);
}

void clear_led_arc_bit(uint8_t led, uint8_t arc)
{
    led_bufs[led][arc >> 3] &= ~_BV(arc & 0b111);
}

void toggle_led_arc_bit(uint8_t led, uint8_t arc)
{
    led_bufs[led][arc >> 3] ^= _BV(arc & 0b111);
}

#endif //LED_BUFFERS_H