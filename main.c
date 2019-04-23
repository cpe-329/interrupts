#include "msp.h"
#include <stdint.h>

int main(void)
{
    // Hold the watchdog
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Configuring P1.0 as output for red LED
    // and P1.1 (left button) as input with pull-up resistor.

    P1->SEL0 &= ~(BIT0 + BIT1); // set P1.0 and P1.1 as GPIO
    P1->SEL1 &= ~(BIT0 + BIT1);

    P1->DIR |= BIT0;        // set P1.0 output
    P1->OUT &= ~BIT0;

    P1->DIR &= ~BIT1;       // set P1.1 as input
    P1->REN |= BIT1;        // enable pull up resistor
    P1->OUT |= BIT1;

    P1->IES |= BIT1;        // Interrupt on high-to-low transition
    P1->IFG &= ~BIT1;       // Clear all P1 interrupt flags
    P1->IE  |= BIT1;        // Enable interrupt for P1.1

    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    // Enable global interrupt
    __enable_irq();

    while (1);  // loop that does nothing
}

/* Port1 ISR */
void PORT1_IRQHandler(void)
{
    // Toggling the output on the LED
    if(P1->IFG & BIT1)
        P1->OUT ^= BIT0;

    P1->IFG &= ~BIT1;

}
