/*
 * Part2.c
 *
 *  Created on: April 28, 2023
 *      Author: Luke Wilkins
 *
 *
 */

#include <msp430.h>

void GPIOInit();
void TimerInit();

char color = 0; // Starts Red --> Green

void main(){
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable low power mode

    GPIOInit();                             // Initialize GPIO
    TimerInit();                            // Initialize Timer

    P6IFG &= ~BIT0;                         // P6.0 Interrupt Cleared
    P6IFG &= ~BIT1;                         // P6.1 Interrupt Cleared
    P6IFG &= ~BIT2;                         // P6.2 Interrupt Cleared

    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupt
}

void GPIOInit(){
    // Configure P6.0 as an LED Output
      P6OUT &= ~BIT0;                         // Clear P6.0 output latch for a defined power-on state
      P6DIR |= BIT0;                          // Set P6.0 to output direction
      P6SEL0 |= BIT0;                         // PWM
      P6SEL1 &= ~BIT0;                        // Clear
      P6IE |= BIT0;                           // Enable interrupt

    // Configure P6.1 as an LED Output
      P6OUT &= ~BIT1;                         // Clear P6.1 output latch for a defined power-on state
      P6DIR |= BIT1;                          // Set P6.1 to output direction
      P6SEL0 |= BIT1;                         // PWM
      P6SEL1 &= ~BIT1;                        // Clear
      P6IE |= BIT1;                           // Enable interrupt

    // Configure P6.2 as an LED Output
      P6OUT &= ~BIT2;                         // Clear P6.2 output latch for a defined power-on state
      P6DIR |= BIT2;                          // Set P6.2 to output direction
      P6SEL0 |= BIT2;                         // PWM
      P6SEL1 &= ~BIT2;                        // Clear
      P6IE |= BIT2;                           // Enable interrupt
}

void TimerInit(){
    TB3CCR0 = 1000 - 1;                                 // PWM period
    TB3CCR1 = 1000 - 1;                                 // Initial Duty Cycle of Red 999
    TB3CCR2 = 0;                                        // Initial Duty Cycle of Green 0
    TB3CCR3 = 0;                                        // Initial Duty Cycle of Blue 0

    TB3CTL = TBSSEL_2 | MC_1 | TBCLR;                   // SMCLK, Up mode, clear TBR
    TB3CCTL1 = OUTMOD_7;                                // Reset/set CCR1
    TB3CCTL2 = OUTMOD_7;                                // Reset/set CCR2
    TB3CCTL3 = OUTMOD_7;                                // Reset/set CCR3

    TB1CTL = TBSSEL_1 | MC_2 | ID_3 | TBCLR | TBIE;     // ACLK, cont. mode, 8 divider, clear TBR, enable interrupts
    TB1CCTL0 |= CCIE;
    TB1CCR0 = 1;                                        // Initial cycle
}

// Timer0 B1 Interrupt
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
    switch (color){
    case 0:                         // Red --> Green
        TB3CCR1--;                  // Decrease Red
        TB3CCR2++;                  // Increase Green
        if (TB3CCR1 == 0){
            color = 1;     // Once LED is completely green, swap to blue transition
        }
        break;
    case 1:                         // Green --> Blue
        TB3CCR2--;                  // Decrease Green
        TB3CCR3++;                  // Increase Blue
        if (TB3CCR2 == 0){
            color = 2;       // Once LED is completely blue, swap to red transition
        }
        break;
    case 2:                         // Blue --> Red
        TB3CCR3--;                  // Decrease Red
        TB3CCR1++;                  // Increase Blue
        if (TB3CCR3 == 0){
            color = 0;      // Once LED is completely red, swap to green transition
        }
        break;
    }
    if (TB3R >= 60000){             // Overflow protection
        TB3R = 1;
    }
    TB1CCR0 += 20;                  // Fade speed
}
