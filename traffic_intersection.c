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

// 2. Global Declarations Section

// 3. Subroutines Section