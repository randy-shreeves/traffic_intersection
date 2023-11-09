// 0. Documentation Section
// This file is intended to activate the phase lock loop in order to
// take advantage of the boards external crystal by setting the system 
// to run at 80MHz.
// Author: Randy Shreeves
// Date: 11/08/2023

// 1. Pre-processor Directives Section
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))

// 2. Global Declarations Section
// No global declarations needed

// 3. Subroutines Section
void initialize_pll(void){
  SYSCTL_RCC2_R |=  0x80000000;	// using RCC2 because it provides more options
  SYSCTL_RCC2_R |=  0x00000800;  // bypassing PLL while initializing
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clearing crystal field
                 + 0x00000540;   // configuring for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configuring for main oscillator source
  SYSCTL_RCC2_R &= ~0x00002000; // clearing PWRDN to activate the PLL
  SYSCTL_RCC2_R |= 0x40000000;   // using 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clearing system clock divider
                  + (4<<22);      // configuring for 80 MHz clock
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit to set
  SYSCTL_RCC2_R &= ~0x00000800;	// enable the PLL by clearing BYPASS2 bit
}
