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
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "VirtualSerial.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;

int8_t res; // used to store result of conversion 



/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();
	SetupUSART1();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();
	fputs("Welcome to Our Dual Slope Controller! \r\n", &USBSerialStream);
	uint16_t time; 
	for (;;)
	{
		//CheckJoystickMovement();
		LEDs_ToggleLEDs(LEDS_LED1|LEDS_LED2|LEDS_LED3);
		time++;
		_delay_ms(250);
		lcd_clrscr();
		lcd_puts("Hi ");
		char buffer[10];
		itoa(time,buffer,10);
		lcd_puts(buffer);
		lcd_goto(0x40); // second line
		res = Read_DualSlope();
		SendValLCD(res);
		double result1 = res * 4.6875; // convert result to mV
		sprintf(buffer,"%f",result1); 
		itoa(res,buffer, 10);
		fputs(buffer, &USBSerialStream);
		fputs("mV \r\n", &USBSerialStream);
		itoa(res,buffer, 10);
		lcd_puts(buffer);

		/* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
		CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	Joystick_Init();
	
	
	//LEDs_ToggleLEDs(LEDS_LED1|LEDS_LED2|LEDS_LED3);
	
	
	
	LEDs_Init();
	USB_Init();
	lcd_init();
	_delay_ms(100);
	lcd_clrscr();
	lcd_puts("Hello World!");
	Dual_Slope_Init();

}

/** Configures the board USART 1 module */
void SetupUSART1(void)
{
	unsigned int c;
    char buffer[7];
    int  num=134;

    
    /*
     *  Initialize UART library, pass baudrate and AVR cpu clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )
     *  or 
     *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
     */
    uart1_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
    
    
    /*
     *  Transmit string to UART
     *  The string is buffered by the uart library in a circular buffer
     *  and one character at a time is transmitted to the UART using interrupts.
     *  uart_puts() blocks if it can not write the whole string to the circular 
     *  buffer
     */
    //uart1_puts("String stored in SRAM\n");
    
    /*
     * Transmit string from program memory to UART
     */
    //uart1_puts_P("String stored in FLASH\n");
    
        
    /* 
     * Use standard avr-libc functions to convert numbers into string
     * before transmitting via UART
     */     
    //itoa( num, buffer, 10);   // convert interger into string (decimal format)         
    //uart1_puts(buffer);        // and transmit string to UART

    
    /*
     * Transmit single character to UART
     */
    //uart1_putc('\r');
	
}


/** Checks for changes in the position of the board joystick, sending strings to the host upon each change. */
void CheckJoystickMovement(void)
{
	uint8_t     JoyStatus_LCL = Joystick_GetStatus();
	char*       ReportString  = NULL;
	static bool ActionSent    = false;

	if (JoyStatus_LCL & JOY_UP)
	  ReportString = "Joystick Up\r\n";
	else if (JoyStatus_LCL & JOY_DOWN)
	  ReportString = "Joystick Down\r\n";
	else if (JoyStatus_LCL & JOY_LEFT)
	  ReportString = "Joystick Left\r\n";
	else if (JoyStatus_LCL & JOY_RIGHT)
	  ReportString = "Joystick Right\r\n";
	else if (JoyStatus_LCL & JOY_PRESS)
	  ReportString = "Joystick Pressed\r\n";
	else
	  ActionSent = false;

	if ((ReportString != NULL) && (ActionSent == false))
	{
		ActionSent = true;

		/* Write the string to the virtual COM port via the created character stream */
		fputs(ReportString, &USBSerialStream);

		/* Alternatively, without the stream: */
		// CDC_Device_SendString(&VirtualSerial_CDC_Interface, ReportString);
	}
}

/** Function to make measurement with Dual Slope hardware. Returns Nana Terayza if error value. Duh. #BaconPancakes */
int8_t Read_DualSlope(void)
{
	bool polFlag = 0;		// used to track polarity
	// step 0, auto zero
	C_SETBIT(InhSwt);		// disconnect input
	C_CLEARBIT(IntSwt);	
	C_SETBIT(ZeroSwt);		// zero cap
	_delay_ms(ZERO_TIME);	// wait some time
	C_CLEARBIT(ZeroSwt);	// zero off

	// step 1, int for INT_TIME mS (80mS ?) 
	C_CLEARBIT(InhSwt);		// turn switch on
	C_SETBIT(IntSwt);		// select input
	_delay_ms(INT_TIME/2);	// wait int time/2
	// half way, check polarity
	if(C_CHECKBIT(CompIn)){ // -ve pol
		C_SETBIT(VrefSwt);
		polFlag = 1;
	}else{ // +ve pol
		C_CLEARBIT(VrefSwt);
		polFlag = 0;
	}
	_delay_ms(INT_TIME/2); // wait int time/2
	
	// step 2, dint for up to 160mS waiting for 0 crossing
	C_CLEARBIT(IntSwt);		// select dint
	
	_delay_ms(DINT_TIME);
	
	// step 3, calc and output
	int8_t result = 956;
	
	if(polFlag){//-ve
		return result*-1;
	}else{//+ve
		return result;
	}
	
}

void SendValLCD(int8_t val){
	// we need to send data to the lcd in the correct structure
	
	// send command first
	uart1_putc(CMD_RESULT0);
	uart1_putc(CMD_RESULT1);
	
	// now send data
	uart1_putc(0x0F & val >> 12);
	uart1_putc(0x0F & val >> 8);
	uart1_putc(0x0F & val >> 4);
	uart1_putc(0x0F & val );
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

