/*
 * ADC.c
 *
 * Created: 2014/09/28 11:11:39 AM
 *  Author: RobThePyro
 */ 

#include "ADC.h"

// Sets up the ADC unit
void ADC_Init(void){
	//ADMUX |= (1<<REFS0)|(1<<ADLAR); // Ref: AVcc, ADC Left adjust, 
}

// Starts the ADC unit with the provided channel
uint16_t ADC_Start(uint8_t channel){
	CLEARBITMASK(ADMUX, (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0)); // clear ADMUX first!!
	SETBITMASK(ADMUX, (1<<REFS0)|channel);  // Ref: AVcc, channel as the selected pin
	SETBITMASK(ADCSRA, (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); // Turn ADC on, Start Conversion,  clk/128 scale = 125khz ADC clock
	
	// ok ADC started, now wait for result
	while(!CHECKBIT(ADCSRA, ADIF)){
		// wait for bit to go 1
	}
	SETBIT(ADCSRA, ADIF); // clear flag by writing 1
	// turn ADC off
	//ADCSRA = 0;
	CLEARBIT(ADCSRA,ADEN);
	
	return ADC; // send back the adc result!
	
}