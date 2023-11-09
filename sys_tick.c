// 0. Documentation Section
// This file is intended to initialize the SysTick timer and
// use it to create delays in 1ms intervals while running on an 80MHz clock.
// Author: Randy Shreeves
// Date: 11/08/2023

// 1. Pre-procesor Directives Section
#define NVIC_ST_CTRL_R (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile unsigned long *)0xE000E018))

// 2. Global Declarations Section
// No global declarations needed.

// 3. Subroutines Section
void initialize_systick(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 79999;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x00000005;
}// end inititalize_systick()

void delay_1ms(unsigned long msec){
	unsigned long n = msec;
	while (n > 0){
		initialize_systick();
		while((NVIC_ST_CTRL_R&0x00010000) == 0) {
			// wait for count flag to be set
		}// end inner while loop
		NVIC_ST_CTRL_R = 0;
		n--;
	}// end outer while loop
}// end delay_1ms()
