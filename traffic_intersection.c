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
	
// Define outputs and inputs
#define TRAFFIC_LIGHTS (*((volatile unsigned long *)0x400050FC))
#define WALK_LIGHTS (*((volatile unsigned long *)0x40025028))
#define SENSORS (*((volatile unsigned long *)0x4002401C))
	
// Define states
#define GO_WEST 0
#define WAIT_WEST 1
#define GO_SOUTH 2
#define WAIT_SOUTH 3
#define GO_WALK 4
#define DONT_WALK_LIGHT1 5
#define WALK_LIGHT_OFF1 6
#define DONT_WALK_LIGHT2 7
#define WALK_LIGHT_OFF2 8

// 2. Global Declarations Section
void initialize_port_b(void);
void initialize_port_f(void);
void initialize_port_e(void);
struct state{
	unsigned long traffic_light_output;
	unsigned long walk_light_output;
	unsigned long time;
	unsigned long next[8];
};
typedef const struct state state;
state fsm[9] = {
  {0x0C, 0x02, 1000, {GO_WEST, GO_WEST, WAIT_WEST, WAIT_WEST, WAIT_WEST, WAIT_WEST, WAIT_WEST, WAIT_WEST}}, // 0.go_west
	{0x14, 0x02,  500, {GO_SOUTH, GO_SOUTH, GO_SOUTH, GO_SOUTH, GO_WALK, GO_WALK, GO_SOUTH, GO_SOUTH}}, // 1.wait_west
	{0x21, 0x02,  1000, {GO_SOUTH, WAIT_SOUTH, GO_SOUTH, WAIT_SOUTH, WAIT_SOUTH, WAIT_SOUTH, WAIT_SOUTH, WAIT_SOUTH}}, // 2.go_south
	{0x22, 0x02,  500, {GO_WEST, GO_WEST, GO_WEST, GO_WEST, GO_WALK, GO_WALK, GO_WALK, GO_WALK}}, // 3.wait_south
	{0x24, 0x08, 1000, {GO_WALK, DONT_WALK_LIGHT1, DONT_WALK_LIGHT1, DONT_WALK_LIGHT1, GO_WALK, DONT_WALK_LIGHT1, DONT_WALK_LIGHT1, DONT_WALK_LIGHT1}}, // 4.go_walk
	{0x24, 0x02,  500, {WALK_LIGHT_OFF1, WALK_LIGHT_OFF1, WALK_LIGHT_OFF1, WALK_LIGHT_OFF1, WALK_LIGHT_OFF1, WALK_LIGHT_OFF1, WALK_LIGHT_OFF1, WALK_LIGHT_OFF1}}, // 5.dont_walk_light1
	{0x24, 0x00, 500, {DONT_WALK_LIGHT2, DONT_WALK_LIGHT2, DONT_WALK_LIGHT2, DONT_WALK_LIGHT2, DONT_WALK_LIGHT2, DONT_WALK_LIGHT2, DONT_WALK_LIGHT2, DONT_WALK_LIGHT2}}, // 6.walk_light_off1
	{0x24, 0x02, 500, {WALK_LIGHT_OFF2, WALK_LIGHT_OFF2, WALK_LIGHT_OFF2, WALK_LIGHT_OFF2, WALK_LIGHT_OFF2, WALK_LIGHT_OFF2, WALK_LIGHT_OFF2, WALK_LIGHT_OFF2}}, // 7.dont_walk_light2
	{0x24, 0x00, 500, {GO_WEST, GO_WEST, GO_SOUTH, GO_WEST, GO_WALK, GO_WEST, GO_SOUTH, GO_WEST}} // 8.walk_light_off2
};
unsigned long current_state;
unsigned long input;

// 3. Subroutines Section
void initialize_port_b(void){
	GPIO_PORTB_AMSEL_R &= ~0x3F;
	GPIO_PORTB_PCTL_R &= ~0x00FFFFFF;
	GPIO_PORTB_DIR_R |= 0x3F;
	GPIO_PORTB_AFSEL_R &= ~0x3F;
	GPIO_PORTB_DEN_R |= 0x3F;
}// end initialize_port_b()

void initialize_port_f(void){
	GPIO_PORTF_AMSEL_R &= ~0xFF;
	GPIO_PORTF_PCTL_R &= ~0x000000FF;
	GPIO_PORTF_DIR_R |= 0xFF;
	GPIO_PORTF_AFSEL_R &= ~0xFF;
	GPIO_PORTF_DEN_R |= 0xFF;
}// end initialize_port_f()

void initialize_port_e(void){
	GPIO_PORTE_AMSEL_R &= ~0xFF;
	GPIO_PORTE_PCTL_R &= ~0x000000FF;
	GPIO_PORTE_DIR_R |= 0x00;
	GPIO_PORTE_AFSEL_R &= ~0xFF;
	GPIO_PORTE_DEN_R |= 0xFF;
}// end initialize_port_e()
