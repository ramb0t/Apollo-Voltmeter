/*
 * Encoder.h
 *
 * Created: 2014/09/27 01:58:54 AM
 *  Author: RobThePyro
 */ 

#ifndef ENCODER_H_
#define ENCODER_H_

	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include "Lib/MyFunctions.h"
	
	#define ENC_A	1
	#define ENC_B	0
	#define ENC_DDR		DDRD
	#define ENC_PORT	PORTD
	#define ENC_PIN		PIND
			
			
	/*	Function prototypes */
	void ENC_Init(void);
	uint8_t ENC_ISR(void);

#endif