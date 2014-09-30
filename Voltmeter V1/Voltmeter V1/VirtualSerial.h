/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Header file for VirtualSerial.c.
 */

#ifndef _VIRTUALSERIAL_H_
#define _VIRTUALSERIAL_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>
		#include <stdio.h>
		#include <stdlib.h> // math? itoa? 
		#include <math.h>
		#include <inttypes.h> // float stuff? 

		#include "Descriptors.h"

		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		
		#include "globals.h"
		#include "Lib/MyFunctions.h"
		//#include "Lib/HD44780/hd44780.h"
		#include "Lib/UART/uart.h"
		#include "Timer3_Capt.h"
		#include "Encoder.h"
		#include "ADC.h"
		
	
	

	/* Macros: */
		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)

	/* Function Prototypes: */
		void SetupHardware(void);
		void SetupUSART1(void);
		int16_t Read_DualSlope(void);
		void SendInt16LCD(int16_t, uint8_t, uint8_t);
		void SendUInt8LCD(uint8_t, uint8_t, uint8_t);
		void CheckBatt(void);
		void SelectRange(uint8_t);
		void SelectHigherRange(void);
		void SelectLowerRange(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);
 

	
	//** Sets up the pins and hardware for the dual slope front end
	static inline void Dual_Slope_Init(void)
		{
			// set PORTB I/O
			SETBITMASK(DDRB, PORTB_OUTS);
			//CLEARBITMASK(DDRB, PORTB_INS);
			
			// set PORTC I/O
			SETBITMASK(DDRC, PORTC_OUTS);
			CLEARBITMASK(DDRC, PORTC_INS);
			
			// set PORTD I/O
			SETBITMASK(DDRD, PORTD_OUTS);
			CLEARBITMASK(DDRD, PORTD_INS);
			
			// set PORTE I/O
			//SETBITMASK(DDRE, PORTE_OUTS);
			CLEARBITMASK(DDRE, PORTE_INS);
			
			// set PORTF I/O
			SETBITMASK(DDRF, PORTF_OUTS);
			CLEARBITMASK(DDRF, PORTF_INS);
			
			C_SETBIT(BUZZ);		//Buzzer off
			
			C_SETBIT(ARInh);		//Inh input
			// dont need worry about A/B/C then!
			C_SETBIT(IntInhSwt);	//Disable Input
			
			C_SETBIT(ZeroSwt);		//Zero Cap
			
			// Default state, all off, cap shorted
			
			
			
		}
		
		
		/* makes a quick beep */ 
		static inline void Beep(void){
			C_CLEARBIT(BUZZ);
			_delay_ms(5);
			C_SETBIT(BUZZ);
			
		}
		

	

#endif

