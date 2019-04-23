/*
 * main.c
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 *  
 *  MCLK = = DCO ~24MHz
 *  SMCLK = MCLK / 8   
 *  TIMER_A0 utilize SMCLK 3MHz
 *  TIMER_A0 CCR0 and CCR1 to create 2 timing events
 */

#include <stdint.h>
#include "msp.h"
#include "my_msp.h"
#include "delay.h"
#include "led.h"

volatile uint8_t counter_2bit = 0;

void main(void)
{
    init(FREQ_1_5_MHZ);

	P4->SEL0 |= P4_3;
	P4->SEL1 &= ~P4_3;
	P4->DIR |= P4_3;

	// setup TIMER_A0
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt
    // TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    // TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enabled

    TIMER_A0->CCR[0] = 657;   // set CCR0 count
    // TIMER_A0->CCR[1] = 8;   // set CCR1 count

    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | // SMCLK,
                    TIMER_A_CTL_MC_1;  // UP mode, count up to CCR[0]

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);   // set NVIC interrupt
    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);   // TA0_0 and TA0_N

    __enable_irq();     // Enable global interrupt

    while(1){
			switch(counter_2bit){
				case 1:
					led_on();
					rgb_set(RGB_OFF);
					break;
				case 2:
					led_off();
					rgb_set(RGB_RED);
					break;
				case 3:
					led_on();
					rgb_set(RGB_RED);
				default:
					led_off();
					rgb_set(RGB_OFF);
					break;
			}
    }
}

// Timer A0_0 interrupt service routine
void TA0_0_IRQHandler(void) {
	TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
	
	switch(counter_2bit){
		case 0:
			counter_2bit = 1;
			break;
		case 1:
			counter_2bit = 2;
			break;
		case 2:
			counter_2bit = 3;
			break;
		default:
			counter_2bit = 0;
			break;
	}
}

