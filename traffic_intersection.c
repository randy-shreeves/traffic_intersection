// 0. Documentation Section
// This system uses a finite state machine model to program a TM4C123G microcontroller, which controls buttons and LEDs to simulate a traffic intersection.
// The circuit represents a 4-way intersection, with one road going north-to-south, and the other going east-to-west. There's also a pedestrian crossing light.
// The southbound sensor is represented by the push of a button. When pushed, if the westbound light is green, then it will turn yellow, and then red, and then the southbound light turns green.
// The westbound sensor is also represented by a button. When pushed, if the southbound light is green, then it will turn yellow, and then red, and then the westbound light will turn green.
// The pedestrian crossing is also controlled by a button. When pushed, any green traffic lights will go to yellow and then to red, and then the green walk light will energize.
// If multiple buttons are pressed, then the relevant lights will cycle appropriately.
// If no buttons are pressed, then the lights remain in whatever state they're currently in.
// Author: Randy Shreeves
// Date: 11/08/2023

// 1. Pre-processor Directives Section
// Define Port B adresses
#define GPIO_PORTB_DATA_R (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R (*((volatile unsigned long *)0x4000552C))

// Define Port E addresses
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long *)0x4002452C))
	
// Define Port F addresses
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R (*((volatile unsigned long *)0x4002552C))
	
// Define Clock Gating Control address
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))

// 2. Global Declarations Section

// 3. Subroutines Section