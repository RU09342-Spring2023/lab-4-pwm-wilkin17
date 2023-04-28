# Servo Control
**Designed by Luke Wilkins**

## What is it?
This program moves a motor clockwise and
counter-clockwise based off of the user's button 
inputs. P2.3 turns the servo counter-clockwise,
and P4.1 turns the servo clockwise.

## How does it work?
This program works by manipulating the duty cycle of
the servo. Timer B3 is used to generate a duty cycle,
and TB3CCR1 is incremented or decremented based off of
user input (up until a certain point). The value of CCR1
directly influences where the servo turns. Once it can't turn
anymore, that button won't do anything extra. P2.3 decrements
TB3CCR1, turning the servo counter-clockwise, and P4.1 increments
TB3CCR1, turning the servo clockwise.