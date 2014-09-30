/*
 * globals.h
 *
 * Created: 2014/09/28 11:16:14 AM
 *  Author: RobThePyro
 */ 

#ifndef	GLOBALS_H_
#define GLOBALS_H_

/* Intergration Defines */
#define ZERO_TIME  30
#define INT_TIME  80
#define INT_TIME_US	80000
#define DINT_TIME  160
#define VREF_MV		1086

/* Pin Defines: */
#define RXLED		PORTB, 0 // Active Low
#define SCK			PORTB, 1 // SPI
#define MOSI		PORTB, 2 // SPI
#define MISO		PORTB, 3 // SPI
#define IntSwt		PORTB, 4
#define RefSwt		PORTB, 5
#define ARSwt		PORTB, 6
#define TXLED		PORTB, 7 // Active Low

#define RefPolSwt	PORTC, 6
#define CompIn		PINC, 7	 // Input ICP3

#define ENC_A_IN	PIND, 0  // INT0
#define ENC_B_IN	PIND, 1  // INT1
#define LCD_TXO		PORTD, 2 // USART Tx
#define LCD_RXI		PORTD, 3 // USART Rx
#define ARIn		PIND, 4  // Input ICP1
#define StatusLED	PORTD, 5 // Active High
#define IntInhSwt	PORTD, 6
#define ZeroSwt		PORTD, 7

#define HWB			PORTE, 2 // Tied to GND
#define ENC_Swt		PINE, 6  // INT6

#define VBAT		PORTF, 0 // ADC0
#define BUZZ		PORTF, 1
#define ARX			PORTF, 4
#define ARY			PORTF, 5
#define ARZ			PORTF, 6
#define ARInh		PORTF, 7


#define PORTB_OUTS  (1<<0)|(1<<4)|(1<<5)|(1<<6)|(1<<7)
#define PORTB_INS
#define PORTC_OUTS	(1<<6)
#define PORTC_INS	(1<<7)
#define PORTD_OUTS	(1<<2)|(1<<5)|(1<<6)|(1<<7)
#define PORTD_INS	(1<<0)|(1<<1)|(1<<3)|(1<<4)
#define PORTE_OUTS
#define PORTE_INS	(1<<2)|(1<<6)
#define PORTF_OUTS	(1<<1)|(1<<4)|(1<<5)|(1<<6)|(1<<7)
#define PORTF_INS	(1<<0)

/* Range Defines */
#define RANGE4	 0
#define RANGE40	 1
#define RANGE400 2

/* 9600 baud */
#define UART_BAUD_RATE      9600

/* LCD Command defines */
#define	CMD_RESULT0	0xF0
#define CMD_RESULT1 0xF2

#define	CMD_BATT0	0xF7
#define CMD_BATT1	0xF8

#define CMD_BACKLIGHT0 0xF5
#define CMD_BACKLIGHT1 0xF6

#define CMD_RANGE0 0xF9
#define CMD_RANGE1 0xF1

/* ADC Defines */ 
#define ADC_AVcc_VALUE			5050 //mV
#define ADC_VBAT_CHANNEL	0x00


#endif