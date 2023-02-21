/*
 * Part1.c
 *
 *  Created on: Feb 21, 2023
 *      Author: Luke Wilkins
 *
 *
 */

#include <msp430.h>

void LEDSetup();
void ButtonSetup();
void TimerB0Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P2IFG &= ~BIT3; // P2.3 IFG cleared
    P4IFG &= ~BIT1; // P4.1 IFG cleared
    LEDSetup();     // Initialize LEDS
    ButtonSetup();  // Initialize buttons
    TimerB0Setup(); // Initialize Timer0
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

/*
 * INITIALIZATION
 */

void LEDSetup(){
    // Configure RED LED on P1.0 as Output
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    // Configure GREEN LED on P6.6 a Output
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction
}

void ButtonSetup(){
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

void TimerB0Setup(){
    TB0CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE;      // SMCLK, up mode, clear TBR, enable interrupt
    TB0CCTL1 |= CCIE;                             // Enable TB0 CCR1 Interrupt
    TB0CCTL2 |= CCIE;                             // Enable TB0 CCR2 Interrupt
    TB0CCR1 = 1000;                       // Set CCR1 to set the Red LEDs duty cycle
    TB0CCR2 = 1000;                       // Set CCR2 to set the Green LEDs duty cycle
    TB0CCR0 = 1000;
}

/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine - For RED LED
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
}

// Port 4 interrupt service routine - For GREEN LED
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
}

// Timer0 B1 Interrupt
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt
        case TB0IV_TBCCR1:
            P1OUT |= BIT0;
            break;                               // CCR1 Set the pin to a 1 (for Red LED)
        case TB0IV_TBCCR2:
            P6OUT |= BIT6;
            break;                               // CCR2 Set the pin to a 1 (for Green LED)
        case TB0IV_TBIFG:
            P6OUT &= ~BIT6;
            P1OUT &= ~BIT0;                       // overflow Set the pin to a 0
            break;
        default:
            break;
    }
}
