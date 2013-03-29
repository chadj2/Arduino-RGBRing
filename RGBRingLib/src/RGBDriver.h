/*
 * RGBDriver.h
 *
 *  Created on: Mar 26, 2013
 *      Author: Chad Juliano
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

	enum color_t { RED, GREEN, BLUE, PURPLE, YELLOW, CYAN, WHITE };

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
	void SetLedAll(color_t color, uint8_t level = _BRIGHT_MAX);
	void SetLed(uint8_t led, color_t color, uint8_t level = _BRIGHT_MAX);
	void SetLedRgb(uint8_t led, uint8_t n_red, uint8_t n_green, uint8_t n_blue);

private:

	void SetupLeds();
	void SetLedPorts(uint8_t n_selector, uint8_t cport_a, uint8_t cport_b);
	void SetLedAllOff();

	/////////////////////////////////////////
	// timer interrupt control
	/////////////////////////////////////////

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

	// store LED color levels
	uint8_t _ledLevelBuffer[_LED_COUNT][3];
};

extern RGBDriver g_rgbDriver;

#endif /* RGBDRIVER_H_ */
