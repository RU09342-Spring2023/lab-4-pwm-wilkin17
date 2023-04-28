# Hardware PWM
**Designed by Luke Wilkins**

## What is it?
This program cycles through colors on an external 4-prong
RBG LED. 6.0 corresponds to the Red Port, 6.1 corresponds to
the Green Port, and 6.2 corresponds to the Blue Port.

## How does it work?
The program has four timers, one for each color, and one to
control the cycling speed. TB3CCR0 is the cycle speed, TB3CCR1
is the Duty Cycle of the Red, TB3CCR1 is the Duty Cycle of the
Green, and TB3CCR2 is the Duty Cycle of the Blue. The timer cycles
through three different states:

1. The RedtoGreen state
2. The GreentoBlue state
3. The BluetoRed state

All states work the same, so I will only explain the RedtoGreen state.
The duty cycle of the Red is decremented to 0 while the duty cycle of
Green is incremeneted. Once the duty cycle for Red becomes 0, the program
swaps states to the GreentoBlue state, where Green is decremented and Blue
is incremeneted.

There is also overflow protection on the fade timer so that the program can
run for as long as you want it to.