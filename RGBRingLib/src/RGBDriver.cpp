/*
 * RGBDriver.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: cjuliano
 */

#include "RGBDriver.h"
#include <Arduino.h>

uint8_t RGBDriver::_ledMap[] = {
		_BV(RR_SEL_LED0),
		_BV(RR_SEL_LED1),
		_BV(RR_SEL_LED2),
		_BV(RR_SEL_LED3),
		_BV(RR_SEL_LED4),
		_BV(RR_SEL_LED5)
};

RGBDriver::RGBDriver()
{
	//Setup();
}

void RGBDriver::Setup()
{
	// setup buttons
	//pinMode(RR_BUTTON_1, INPUT);
	//pinMode(RR_BUTTON_2, INPUT);
	//digitalWrite(RR_BUTTON_1, HIGH);
	//digitalWrite(RR_BUTTON_2, HIGH);

	// selector
	RR_SEL_DDR |= RR_SEL_ALLLED;    	// set PORTB as output
	RR_SEL_PORT &= ~RR_SEL_ALLLED; 	// all pins HIGH --> cathodes HIGH --> LEDs off

	// color port A
	RR_CPA_DDR |= RR_CPA_WHITE;    	// set COLORPORT #5-7 as output
	RR_CPA_PORT &= ~RR_CPA_WHITE; 	// pins #5-7 LOW --> anodes LOW --> LEDs off

	// color port B
	RR_CPB_DDR |= RR_CPB_WHITE;    	// set COLORPORT #5-7 as output
	RR_CPB_PORT &= ~RR_CPB_WHITE; 	// pins #5-7 LOW --> anodes LOW --> LEDs off

	// set LED1 to green.
	RR_SEL_PORT |= _BV(RR_SEL_LED1);
	RR_CPB_PORT |= _BV(RR_CPB_GREEN);

}
