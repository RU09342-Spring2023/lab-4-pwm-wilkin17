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

void main(){
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable low power mode

    GPIOInit();                             // Initialize GPIO
    TimerInit();                            // Initialize Timer

    P2IFG &= ~BIT3;                         // Clear P2.3 Interrupt
    P4IFG &= ~BIT1;                         // Clear P4.1 Interrupt

    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupt
}

void GPIOInit(){
    // Configure P6.0 as the Servo Output
    P6OUT &= ~BIT0;                         // Clear P6.0 output latch for a defined power-on state
    P6DIR |= BIT0;                          // Set P6.0 to output direction
    P6SEL0 |= BIT0;                         // PWM
    P6SEL1 &= ~BIT0;                        // Clear

    // Configure Button on P2.3 as input with pullup resistor
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled

    // Configure Button on P4.1 as input with pullup resistor
    P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low --> High edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled
}

void TimerInit(){
    TB3CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE; // SMCLK, Up mode, clear TBR, enable interrupts
    TB3CCTL0 = OUTMOD_7;                     // CCR1 reset/set
    TB3CCR0 = 20000;                         // Frequency
    TB3CCR1 = 1500;                          // Duty Cycle
}

// Port 2 interrupt
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(TB3CCR1 >= 1000){
        TB3CCR1 -= 100;         // Decrease duty cycle by 10% until it can't turn any more
    }
    else{
        TB3CCR1 = TB3CCR1;      // Hard stop
    }
    P2IFG &= ~BIT3;             // Clear P2.3 Interrupt
}

// Port 4 interrupt
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    if(TB3CCR1 <= 1000){
        TB3CCR1 += 100;         // Increase duty cycle by 10% until it can't turn any more
    }
    else{
        TB3CCR1 = TB3CCR1;      // Hard stop
    }
    P4IFG &= ~BIT1;             // Clear P2.3 Interrupt
}
