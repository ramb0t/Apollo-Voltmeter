/*
 * Encoder.c
 *
 * Created: 2014/09/27 01:59:33 AM
 *  Author: RobThePyro
 */ 
#include "Encoder.h"

/* Init rotary encoder hardware */
void ENC_Init(void){
	CLEARBITMASK(ENC_DDR, (1<<ENC_A)|(1<<ENC_B)); // set encoder pins as inputs
	SETBITMASK(ENC_PORT, (1<<ENC_A)|(1<<ENC_B));  // enable pull ups on encoder
	EICRA |= (1<<ISC10)|(1<<ISC00);				  // enable interrupts on any edge for INT0 and INT1
	EIMSK |= (1<<INT1)|(1<<INT0);				  // set interrupt flag mask for INT1:0
}

/* deal with an encoder interupt */ 
uint8_t ENC_ISR(void){
	static uint8_t old_AB = 3;  //lookup table index
	static int8_t encval = 0;   //encoder value
	static const int8_t enc_states[ ]=
	{0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};  //encoder lookup table
	/**/
	old_AB <<=2;  //remember previous state
	old_AB |= ( ENC_PIN & 0x03 );
	encval += enc_states[( old_AB & 0x0f )];
	/* post "Navigation forward/reverse" event */
	if( encval > 3 ) {  //four steps forward
		encval = 0;
		return 1;
	}
	else if( encval < -3 ) {  //four steps backwards
		encval = 0;
		return -1;
	}
}