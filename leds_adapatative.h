#ifndef LEDS_ADAPTATIVE_H
#define LEDS_ADAPTATIVE_H

#include <stdlib.h>
#include <avr/common.h>

// We want the LEDS that are the closest to the center
// To have less angular accuracy to conserve memory
// For example, led00 has 8*8  =  64 "pixels"
//              led15 has 8*68 = 544 (8.5 times as much!)
//Doing so uses about 608 bytes out of the 2KB SRAM

#define LED_00_WIDTH 8 // In bytes
#define LED_01_WIDTH 12
#define LED_02_WIDTH 16
#define LED_03_WIDTH 20
#define LED_04_WIDTH 24
#define LED_05_WIDTH 28
#define LED_06_WIDTH 32
#define LED_07_WIDTH 36
#define LED_08_WIDTH 40
#define LED_09_WIDTH 44
#define LED_10_WIDTH 48
#define LED_11_WIDTH 52
#define LED_12_WIDTH 56
#define LED_13_WIDTH 60
#define LED_14_WIDTH 64
#define LED_15_WIDTH 68

#endif // LEDS_PROPORTIONNAL_H