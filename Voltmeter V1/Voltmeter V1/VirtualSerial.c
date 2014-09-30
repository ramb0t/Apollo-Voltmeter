/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/
// avrdude -p m32u4 -c avr109 -P COM65 -U flash:w:"$(ProjectDir)Debug\Voltmeter V1.hex":i
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

volatile static uint8_t bl = 128;
volatile static uint8_t	range = RANGE4;

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	int16_t res; // used to store result of conversion
	
	SetupHardware();


	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();
	
	fputs("Welcome to Our Dual Slope Controller! \r\n", &USBSerialStream);
	
	
	for (;;)
	{
		CheckBatt();
		char buffer[10];
		SendUInt8LCD(bl, CMD_BACKLIGHT0, CMD_BACKLIGHT1);
		itoa(bl,buffer, 10);
		fputs(buffer, &USBSerialStream);
		fputs(" Backlight \r\n", &USBSerialStream);
		
		//LEDs_ToggleLEDs(LEDS_LED1|LEDS_LED2|LEDS_LED3);
		res = Read_DualSlope();
		SendInt16LCD(res, CMD_RESULT0, CMD_RESULT1);
		itoa(res,buffer, 10);
		fputs(buffer, &USBSerialStream);
		fputs("mV \r\n", &USBSerialStream);
		


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
	
	SetupUSART1();
	init_timer3();
	
	LEDs_Init();
	USB_Init();
	Dual_Slope_Init();
	ENC_Init(); // setup encoder pins and interupts 

}

/** Configures the board USART 1 module */
void SetupUSART1(void)
{

    
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


/** Function to make measurement with Dual Slope hardware. Returns Nana Terayza if error value. Duh. #BaconPancakes */
int16_t Read_DualSlope(void)
{
	C_CLEARBIT(ARInh);      // input on
	bool polFlag = 0;		// used to track polarity
	// step 0, auto zero
	C_SETBIT(IntInhSwt);		// disconnect input
	C_SETBIT(ZeroSwt);		// zero cap
	_delay_ms(ZERO_TIME);	// wait some time
	C_CLEARBIT(ZeroSwt);	// zero off

	// step 1, int for INT_TIME mS (80mS ?) 
	C_CLEARBIT(ARSwt);		// select input
	C_CLEARBIT(RefSwt);		// select input
	C_SETBIT(IntSwt);		// select input
	
	C_CLEARBIT(IntInhSwt);		// turn switch on
	
	
	_delay_ms(INT_TIME/2);	// wait int time/2
	// half way, check polarity
	if(C_CHECKBIT(CompIn)){ // -ve pol
		C_SETBIT(RefPolSwt);
		trig_falling();
		polFlag = 1;
	}else{ // +ve pol
		C_CLEARBIT(RefPolSwt);
		trig_rising();
		polFlag = 0;
	}
	_delay_ms(INT_TIME/2); // wait int time/2
	
	// step 2, dint for up to 160mS waiting for 0 crossing
	C_CLEARBIT(IntSwt);		// select dint
	C_SETBIT(RefSwt);
	start_input_capture();  // start the timer and wait for capture
	
	uint16_t capture =0;
	
	while(!(CHECKBITMASK(TIFR3, (1<<ICF3)|(1<<TOV3)))); // wait for a flag to fire
	if(CHECKBIT(TIFR3, ICF3)){ // we have expected input capture
		capture = get_capture_time();
		char buffer[10];
		ultoa(capture, buffer, 10);
		fputs(buffer, &USBSerialStream);
		fputs("cap\n", &USBSerialStream);
	}else if(CHECKBIT(TIFR3, TOV3)){// Overflow! something went wrong!
		capture = 10000;
	}
	// finally stop timer and clear flags
	stop_and_clear_input_capture();
	//LEDs_SetAllLEDs(LEDS_ALL_LEDS);
	
	// step 3, calc and output
	// formula is (T2/T1) * Vref
	// first we need T2:
	uint32_t T2 = capture; // 4uS per 'tick' so capture time * 4 gives T2 in uS
	T2 = T2 *4;
	char buffer[10];
	ultoa(T2, buffer, 10);
	fputs(buffer, &USBSerialStream);
	fputs("T2\n", &USBSerialStream);
	// do calc (no floating point please! )
	T2 = (T2*VREF_MV)/INT_TIME_US;
	ultoa(T2, buffer, 10);
	fputs(buffer, &USBSerialStream);
	fputs("T22\n", &USBSerialStream);
	int16_t result = T2; // T2 should now be a max of 2460 (mV)
	result = result *2;
	
	if(result > 4000){
		SelectHigherRange();
	}else if(result < 400){
		SelectLowerRange();
	}
	itoa(result, buffer, 10);
	fputs(buffer, &USBSerialStream);
	fputs("result\n", &USBSerialStream);
	//LEDs_SetAllLEDs(LEDS_ALL_LEDS);
	if(polFlag){//-ve
		return result*-1;
	}else{//+ve
		return result;
	}
	
}

void SendInt16LCD(int16_t val, uint8_t cmd0, uint8_t cmd1){
	// we need to send data to the lcd in the correct structure
	
	// send command first
	uart1_putc(cmd0);
	uart1_putc(cmd1);
	
	// now send data
	uart1_putc(0x0F & val >> 12);
	uart1_putc(0x0F & val >> 8);
	uart1_putc(0x0F & val >> 4);
	uart1_putc(0x0F & val );
}

void SendUInt8LCD(uint8_t byte, uint8_t cmd0, uint8_t cmd1){
		// send command first
		uart1_putc(cmd0);
		uart1_putc(cmd1);
		
		// now send data
		uart1_putc(0x0F & byte >> 4);
		uart1_putc(0x0F & byte);
}

void CheckBatt(void){
	uint32_t vbatt = ADC_Start(ADC_VBAT_CHANNEL); // read the ADC voltage
	vbatt = vbatt*ADC_AVcc_VALUE; // * ~5000mV
	vbatt = vbatt >> 10; // /1024 , thus vbatt * 5000/1024 to scale
	SendInt16LCD(vbatt, CMD_BATT0, CMD_BATT1); // should only take lower 16bits?
	char buffer[10];
	ultoa(vbatt, buffer, 10);
	fputs(buffer, &USBSerialStream);
	fputs(" mV Batt\r\n", &USBSerialStream);
}



void SelectRange(uint8_t div){
	if(div == RANGE4){ // lowest range
		C_CLEARBIT(ARX);
		C_CLEARBIT(ARY);
	}else if(div == RANGE40){
		C_CLEARBIT(ARX);
		C_SETBIT(ARY);
	}else if(div == RANGE400){
		C_SETBIT(ARX);
		C_SETBIT(ARY);
	}else{ 
		//huh? 
	}
	SendUInt8LCD(div, CMD_RANGE0, CMD_RANGE1); // send range to lcd
}

void SelectHigherRange(void){
	if(range == RANGE4){
		range = RANGE40;
	}else if(range == RANGE40){
		range = RANGE400;
	}
	SelectRange(range);
}

void SelectLowerRange(void){
	if(range == RANGE400){
		range = RANGE40;
	}else if(range == RANGE40){
		range = RANGE4;
	}
	SelectRange(range);
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

ISR(INT0_vect){
	bl += ENC_ISR();
	if(bl > 250){bl = 255;}
	if(bl < 0){bl = 0;}
	Beep();
}

ISR(INT1_vect){
	bl += ENC_ISR();
	if(bl > 255){bl = 255;}
	if(bl < 0){bl = 0;}
	Beep();

}