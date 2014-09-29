/*
 * Timer3_Capt.c
 *
 * Created: 2014/09/25 11:05:17 PM
 *  Author: RobThePyro
 */ 

#include "Timer3_Capt.h"


// sets up timer 3 for capture
void init_timer3(void){
	
	TCCR3A = 0; // default
	TCCR3B = (1<<ICNC3); // Input Capture Noise Filter on
	TCCR3C = 0; // default	
}

// sets timer to trigger on rising edge ICP3
void trig_rising(void){
	TCCR3B |= (1<<ICES3);
}

// sets timer to trigger on falling edge ICP3
void trig_falling(void){
	TCCR3B &= !(1<<ICES3);
}

// starts the input capture timer system
void start_input_capture(void){
	// make sure we start from zero
	TCNT3 = 0; 
	TIFR3 |= (1<<ICF3)|(1<<TOV3); // clear flags by writing '1'
	
	// set the prescaler to start the timer
	// set to clk/64. For 16MHz we have 4uS period or ticks
	// for 16bit timer max time is 0.262144 S
	// and for 160mS dint time we have 40 000 counts :) 
	TCCR3B &= !(1<<CS32);
	TCCR3B |= (1<<CS31)|(1<<CS30);
}

// stops the timer and clears any outstanding flags
void stop_and_clear_input_capture(void){
	TCCR3B &= !((1<<CS32)|(1<<CS31)|(1<<CS30)); // turn off by dissconnecting the clock
	TIFR3 |= (1<<ICF3)|(1<<TOV3); // clear flags by writing '1'
}

// returns the input capture register value
uint16_t get_capture_time(void){
	uint16_t cap = ICR3;
	return cap;
}