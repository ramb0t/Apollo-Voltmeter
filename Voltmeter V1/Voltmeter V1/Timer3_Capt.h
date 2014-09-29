/*
 * Timer3_Capt.h
 *
 * Created: 2014/09/25 11:03:34 PM
 *  Author: RobThePyro
 */ 

#ifndef TIMER3_CAPT_H_
#define TIMER3_CAPT_H_
	#include <avr/io.h>
	#include <avr/interrupt.h>

	/* Function Prototypes */
	void init_timer3(void);
	void trig_rising(void);
	void trig_falling(void);
	void start_input_capture(void);
	void stop_and_clear_input_capture(void);
	uint16_t get_capture_time(void);

#endif

