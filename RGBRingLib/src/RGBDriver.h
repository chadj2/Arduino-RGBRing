/*
 * RGBDriver.h
 *
 *  Created on: Mar 26, 2013
 *      Author: cjuliano
 */

#ifndef RGBDRIVER_H_
#define RGBDRIVER_H_

#include "RGBRingDefs.h"
#include <inttypes.h>

class RGBDriver
{
public:

	/////////////////////////////////////////
	// Static definitions
	/////////////////////////////////////////

	enum color_t { RED, GREEN, BLUE };

	// LED brightness levels
	static const uint8_t _BRIGHT_LEVELS = 64;
	static const uint8_t _BRIGHT_MAX = _BRIGHT_LEVELS - 1;

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
	void SetLedAllWhite(uint8_t level);
	void SetLedAll(color_t color, uint8_t level);
	void SetLed(uint8_t led, color_t color, uint8_t level);

private:

	void SetupLeds();

	/////////////////////////////////////////
	// timer control
	/////////////////////////////////////////

	// this may have to be adjusted if "RR_LED_BRIGHTNESS_LEVELS" is changed too much
	static const uint8_t _TIMER1_CNT = 0x30;
	// 8 bit CTR
	static const uint8_t _TIMER1_MAX = 0xFF;

	void SetupTimer();
	void EnableTimer();
	void DisableTimer();

	// map LED to bitmask
	static const uint8_t _ledMap[];

	// store LED color levels
	uint8_t _ledColor[_LED_COUNT][3];
};

extern RGBDriver g_rgbDriver;

#endif /* RGBDRIVER_H_ */
