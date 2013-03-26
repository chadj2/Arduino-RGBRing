//////////////////////////////////////////////////////////////////////////////
// Title: RGBRingDefs
// Description: Basic definitions for the RGB Ring.
//////////////////////////////////////////////////////////////////////////////

#ifndef RGBRINGDEFS_H_
#define RGBRINGDEFS_H_

//////////////////////////////////////////////////////////////////////////////
// Pin to LED mapping: Intersection of color port and selector determine
//   the LED that gets powered on. There are 2 color ports and 6 selectors
//   for a total of 12 LED's.
//
// Color ports (anodes LOW --> OFF)
//   PORTC(CPORT_A): 0(RR_CPA_GREEN) 1(RR_CPA_RED)   2(RR_CPA_BLUE)
//   PORTD(RR_CPB_PORT): 5(RR_CPB_RED)   6(RR_CPB_GREEN) 7(RR_CPB_BLUE)
//
// Selector (cathodes HIGH --> OFF)
//   PORTB: 0(RR_SEL_LED4)
//		1(RR_SEL_LED3)
//		2(RR_SEL_LED2)
//		3(RR_SEL_LED1)
//		4(RR_SEL_LED0)
//		5(RR_SEL_LED5)
//////////////////////////////////////////////////////////////////////////////

// LED selector starting from the D10,Clockwise
#define RR_SEL_LED0 	PINB4
#define RR_SEL_LED1 	PINB3
#define RR_SEL_LED2 	PINB2
#define RR_SEL_LED3 	PINB1
#define RR_SEL_LED4 	PINB0
#define RR_SEL_LED5 	PINB5
#define RR_SEL_ALLLED  	(_BV(RR_SEL_LED0)|_BV(RR_SEL_LED1)|_BV(RR_SEL_LED2)|_BV(RR_SEL_LED3)|_BV(RR_SEL_LED4)|_BV(RR_SEL_LED5))
#define RR_SEL_DDR      DDRB
#define RR_SEL_PORT     PORTB

// Color Port A
#define RR_CPA_RED     	PINC1
#define RR_CPA_GREEN   	PINC0
#define RR_CPA_BLUE    	PINC2
#define RR_CPA_WHITE   	(_BV(RR_CPA_RED) | _BV(RR_CPA_GREEN) | _BV(RR_CPA_BLUE))
#define RR_CPA_DDR    	DDRC
#define RR_CPA_PORT    	PORTC

// Color Port B
#define RR_CPB_RED     	PIND5
#define RR_CPB_GREEN   	PIND6
#define RR_CPB_BLUE    	PIND7
#define RR_CPB_WHITE   	(_BV(RR_CPB_RED) | _BV(RR_CPB_GREEN) | _BV(RR_CPB_BLUE))
#define RR_CPB_DDR     	DDRD
#define RR_CPB_PORT    	PORTD

// Buttons
#define RR_BUTTON_1		3
#define RR_BUTTON_2 	4

#endif
