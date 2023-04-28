# Software PWM
**Designed by Luke Wilkins**

## What is it?
This code changes the brightness of the on board LEDs every time you
press their respective buttons. (Button 2.3 is tied to the RED LED and
Button 4.1 is tied to the GREEN LED). The brightness cycles after reaching
the max.

## How does it work?
The code works by changing the Duty Cycle of the LEDs every time the button is
pressed. Using two different timers in up-mode (one for each LED), the CCR0 is
incremented each time the button is pressed, and when the timer reaches its max,
it loops back around to the beginning.