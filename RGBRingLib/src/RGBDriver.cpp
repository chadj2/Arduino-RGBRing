/*
 * RGBDriver.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: cjuliano
 */

#include "RGBDriver.h"
#include <Arduino.h>

// -----------------------Framebuffer interrupt routine-------------------------------//

RGBDriver g_rgbDriver = RGBDriver();

ISR (TIMER2_OVF_vect)
{
	g_rgbDriver.TimerIteration();
}

uint8_t const RGBDriver::_ledMap[] = {
		_BV(RR_SEL_LED0),
		_BV(RR_SEL_LED1),
		_BV(RR_SEL_LED2),
		_BV(RR_SEL_LED3),
		_BV(RR_SEL_LED4),
		_BV(RR_SEL_LED5)
};

RGBDriver::RGBDriver()
{
	SetLedAllWhite(0);
	//Setup();
}

void RGBDriver::Setup()
{
	SetupLeds();

	// setup buttons
	pinMode(RR_BUTTON_1, INPUT);
	pinMode(RR_BUTTON_2, INPUT);
	digitalWrite(RR_BUTTON_1, HIGH);
	digitalWrite(RR_BUTTON_2, HIGH);

	SetupTimer();
	EnableTimer();
}

void RGBDriver::SetupLeds()
{
	// selector
	RR_SEL_DDR |= RR_SEL_ALLLED;  	// set PORTB as output
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

void RGBDriver::SetLedAllWhite(uint8_t n_level)
{
	SetLedAll(RED, n_level);
	SetLedAll(GREEN, n_level);
	SetLedAll(BLUE, n_level);
}

void RGBDriver::SetLedAll(color_t color, uint8_t n_level)
{
	for(int n_led = 0; n_led < _LED_COUNT; n_led++)
	{
		SetLed(n_led, color, n_level);
	}
}

void RGBDriver::SetLed(uint8_t n_led, color_t color, uint8_t n_level)
{
	if(color == GREEN)
	{
		// turn down green a little.
		n_level *= 0.9;
	}
	_ledColor[n_led][color] = n_level;
}

void RGBDriver::SetupTimer()
{
	// Arduino runs at 8 Mhz...
	// Timer2 (8bit) Settings:
	// prescaler (frequency divider) values:   CS12    CS11   CS10
	//                                           0       0      0    stopped
	//                                           0       0      1      /1
	//                                           0       1      0      /8
	//                                           0       1      1      /32
	//                                           1       0      0      /64
	//                                           1       0      1      /128
	//                                           1       1      0      /256
	//                                           1       1      1      /1024
	TCCR2B |= ((1 << CS22) | (1 << CS20) | ((1 << CS21)));    //1024###

	//normal mode (16bit counter)
	TCCR2B &= ~(1 << WGM22);
	TCCR2A = 0;

	// enable global interrupts flag
	sei();
}

void RGBDriver::EnableTimer()
{
	TCNT2 = _TIMER1_MAX - _TIMER1_CNT;
	TIMSK2 |= (1 << TOIE2);
}

void RGBDriver::DisableTimer(void)
{
	RR_SEL_PORT &= ~RR_SEL_ALLLED;
	TIMSK2 &= ~(1 << TOIE2);
}

void RGBDriver::TimerIteration()
{
	TCNT2 = _TIMER1_MAX - _TIMER1_CNT;

	for (int n_level = 0; n_level < _BRIGHT_LEVELS; n_level++)
	{
		uint8_t n_led = 0;

		for (int n_led_row = 0; n_led_row < 6; n_led_row++)
		{
			uint8_t cport_a = 0;
			uint8_t cport_b = 0;

			// all relevant anodes LOW --> OFF
			RR_CPA_PORT &= ~RR_CPA_WHITE;
			RR_CPB_PORT &= ~RR_CPB_WHITE;
			// all cathodes HIGH --> OFF
			RR_SEL_PORT &= ~RR_SEL_ALLLED;

			if (n_level < _ledColor[n_led][GREEN]) cport_a |= _BV(RR_CPA_GREEN);
			if (n_level < _ledColor[n_led][BLUE]) cport_a |= _BV(RR_CPA_BLUE);
			if (n_level < _ledColor[n_led][RED]) cport_a |= _BV(RR_CPA_RED);
			n_led++;

			if (n_level < _ledColor[n_led][GREEN]) cport_b |= _BV(RR_CPB_GREEN);
			if (n_level < _ledColor[n_led][BLUE]) cport_b |= _BV(RR_CPB_BLUE);
			if (n_level < _ledColor[n_led][RED]) cport_b |= _BV(RR_CPB_RED);
			n_led++;

			RR_SEL_PORT = (RR_SEL_PORT & ~RR_SEL_ALLLED) | _ledMap[n_led_row];
			RR_CPA_PORT = (RR_CPA_PORT & ~RR_CPA_WHITE) | cport_a;
			RR_CPB_PORT = (RR_CPB_PORT & ~RR_CPB_WHITE) | cport_b;
			//asm("nop");
		}
	}
}


