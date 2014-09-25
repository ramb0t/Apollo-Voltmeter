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
		#include <inttypes.h> // float stuff? 

		#include "Descriptors.h"

		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Board/Joystick.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		
		#include "Lib/MyFunctions.h"
		#include "Lib/HD44780/hd44780.h"
		#include "Lib/UART/uart.h"
		
	/* Intergration Defines */
		#define ZERO_TIME  30
		#define INT_TIME  80
		#define DINT_TIME  160
		
	/* Pin Defines: */
		#define IntSwt	PORTB, 4
		#define VrefSwt	PORTB, 5
		#define ZeroSwt	PORTB, 6
		
		#define InhSwt	PORTC, 6
		
		#define CompIn	PORTC, 7
		
		#define PORTB_OUTS	(1<<4)|(1<<5)|(1<<6)
		#define PORTB_INS	
		#define PORTC_OUTS	(1<<6)
		#define PORTC_INS	(1<<7)
			
		/* 9600 baud */
		#define UART_BAUD_RATE      9600
		
	/* LCD Command defines */
	#define	CMD_RESULT0	0xA0
	#define CMD_RESULT1 0xA2

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
		void CheckJoystickMovement(void);
		int8_t Read_DualSlope(void);
		void SendValLCD(int8_t);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);
 
	/* Inline Functions: */
	#define	PINS_In_Switch PF0
	#define PINS_Ref_Switch PF1
	#define PINS_Comp_Input PF4
	#define PINS_PortF	(PINS_In_Switch|PINS_Ref_Switch)
	
	//** Sets up the pins and hardware for the dual slope front end
	static inline void Dual_Slope_Init(void)
		{
			// set PORTB I/O
			SETBITMASK(DDRB, PORTB_OUTS);
			
			// set PORTC I/O
			SETBITMASK(DDRC, PORTC_OUTS);
			CLEARBITMASK(DDRC, PORTC_INS); 
			
			C_CLEARBIT(IntSwt);		//Select input in
			C_CLEARBIT(ZeroSwt);	//Auto Zero off
			C_SETBIT(InhSwt);		//Inhibit Vref mux
			
		}
	
	// Turns on the specified pins and off the remainder
	static inline void PINs_SetAll(const uint8_t PINMask)
			{
				PORTF = ((PORTF |  PINS_PortF) & ~(PINMask & PINS_PortF));
			}
#endif

