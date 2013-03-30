/*
 * RGBColor.h
 *
 *  Created on: Mar 29, 2013
 *      Author: cjuliano
 */

#ifndef RGBCOLOR_H_
#define RGBCOLOR_H_

#include <inttypes.h>

class RGBColor
{
public:

	// LED brightness levels
	static const uint8_t _BRIGHT_MAX = 0xFF;

	enum color_t { RED, GREEN, BLUE, MAGENTA, YELLOW, CYAN, WHITE };

	RGBColor();
	RGBColor(color_t color, uint8_t n_level = _BRIGHT_MAX) { SetColor(color, n_level); }

	void SetRgb(uint8_t n_red, uint8_t n_green, uint8_t n_blue);
	void SetColor(color_t color, uint8_t n_level = _BRIGHT_MAX);

	RGBColor& operator=(const RGBColor& color);

	uint8_t _red;
	uint8_t _green;
	uint8_t _blue;
};

#endif /* RGBCOLOR_H_ */
