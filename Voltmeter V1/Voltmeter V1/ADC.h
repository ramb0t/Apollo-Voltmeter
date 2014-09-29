/*
 * ADC.h
 *
 * Created: 2014/09/28 11:11:22 AM
 *  Author: RobThePyro
 
 * For Atmega32u4
 */ 

#ifndef ADC_H_
#define ADC_H_

	#include <avr/io.h>
	
	#include "globals.h"
	#include "Lib/MyFunctions.h"
	
	/* Function Prototypes */ 
	void ADC_Init(void); 
	uint16_t ADC_Start(uint8_t);
	
	
#endif
