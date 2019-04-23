/*
 * led.c
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 */

#include "stdint.h"
#include "msp.h"
#include "my_msp.h"
#include "led.h"
#include "delay.h"

// Setup LED1
inline void led_init(void){
    P1->SEL0 &= ~BIT0;  // Set sel0 bit low for GPIO
    P1->SEL1 &= ~BIT0;  // Set sel1 bit low for GPIO
    P1->DIR |= GPIO_1_MODE_OUTPUT;  // Set P1.0 to output mode 
    P1->OUT &= ~LED1_STATE_ON;  //  Set LED1 state to off
}

// Toggle LED1
inline void led_toggle(void){
    P1->OUT ^= LED1_STATE_ON;  // XOR LED1 state to toggle
}
inline void led_on(void){
    P1->OUT |= LED1_STATE_ON;
}
inline void led_off(void){
    P1->OUT &= ~LED1_STATE_ON;
}

// Blink LED on and off for given ms
void led_blink_ms(unsigned int ms){
    led_on();
    delay_ms_auto(ms);
    led_off();
}

// Setup RGB LED
void rgb_init(void){
    P1->SEL0 &= ~PIN_RGB_SEL0_GPIO;  // Set sel0 bits low for GPIO
    P1->SEL1 &= ~PIN_RGB_SEL1_GPIO;  // Set sel1 bits low for GPIO
    P2->DIR |= GPIO_RGB_MODE_OUTPUT;  // Set P2.0 - P2.2 to output mode
    P2->OUT &= ~LED_RGB_STATE_ON;  // Set RGB LED state to off
}

// Set 3 bits of RGB LED
void rgb_set(int value){
    P2->OUT &= ~LED_RGB_STATE_ON;  // Clear previous RGB LED state
    P2->OUT |= (value & LED_RGB_STATE_ON);  // Set RGB LED state
}

// Blink all combinations of LEDs
inline void test_leds(){
    int i = 0;
    while(i < 13){
        leds_display_keypad(i);
        delay_ms(50, FREQ_48_MHZ);
        i++;
    }
}

// Display keypad value on LEDs
inline void leds_display_keypad(uint8_t keypad_val){
    switch (keypad_val){
        case 0:
            led_on();
            rgb_set(RGB_WHITE);
            break;
        case 1:
            led_off();
            rgb_set( RGB_RED);
            break;
        case 2:
            led_off();
            rgb_set(RGB_GREEN);
            break;
        case 3:
            led_off();
            rgb_set(RGB_BLUE);
            break;
        case 4:
            led_off();
            rgb_set(RGB_YELLOW);
            break;
        case 5:
            led_off();
            rgb_set(RGB_PURPLE);
            break;
        case 6:
            led_off();
            rgb_set(RGB_TURQUOISE);
            break;
        case 7:
            led_off();
            rgb_set(RGB_WHITE);
            break;
        case 8:
            led_on();
            rgb_set(RGB_RED);
            break;
        case 9:
            led_on();
            rgb_set(RGB_GREEN);
            break;
        case 10:
            led_on();
            rgb_set(RGB_BLUE);
            break;
        case 11:
            led_on();
            rgb_set(RGB_YELLOW);
            break;
        default:
            led_off();
            rgb_set(RGB_OFF);
    }
}
