/*
 * RGBDriver.h
 *
 *  Created on: Mar 26, 2013
 *      Author: Chad Juliano
 */

#ifndef RGBDRIVER_H_
#define RGBDRIVER_H_

#include "RGBRingDefs.h"
#include "RGBColor.h"

class RGBDriver
{
public:

	/////////////////////////////////////////
	// Static definitions
	/////////////////////////////////////////

	// total count of LEDs
	static const uint8_t _LED_COUNT = 12;

	/////////////////////////////////////////
	// Class Members
	/////////////////////////////////////////

	RGBDriver();
	//virtual ~RGBDriver() { }

	void Setup();
	void TimerIteration();

	// LED control
	void SetLedAll(const RGBColor& color);
	void SetLedAll(RGBColor::color_t color, uint8_t level = RGBColor::_BRIGHT_MAX);

	RGBColor& operator[](uint8_t n_led);

	// store LED color levels
	RGBColor _ledColors[_LED_COUNT];
private:

	void SetupLeds();
	void SetLedPorts(uint8_t n_selector, uint8_t cport_a, uint8_t cport_b);
	void SetLedAllOff();

	/////////////////////////////////////////
	// timer interrupt control
	/////////////////////////////////////////

	static const uint8_t _SCALE_DIVISOR = 4;

	// _TIMER1_CNT may have to be adjusted if "RR_LED_BRIGHTNESS_LEVELS"
	// is changed too much. Setting it too small will cause the interrupt to fail.
	// Setting it too large will result in dimmer lights.
	static const uint8_t _TIMER1_CNT = 0x2A;

	// 8 bit CTR
	static const uint8_t _TIMER1_MAX = 0xFF;

	void SetupTimer();
	void EnableTimer();
	void DisableTimer();

	// map LED to bitmask
	static const uint8_t _ledMap[];

};

extern RGBDriver g_rgbDriver;

#endif /* RGBDRIVER_H_ */
