/*
 * RGBColor.cpp
 *
 *  Created on: Mar 29, 2013
 *      Author: cjuliano
 */

#include "RGBColor.h"

RGBColor::RGBColor()
	: _red(0),
	_green(0),
	_blue(0)
{
	// TODO Auto-generated constructor stub
}

void RGBColor::SetRgb(uint8_t n_red, uint8_t n_green, uint8_t n_blue)
{
	// turn down a little by 0.9. We avoid a decimal multiply
	// because it would require flashing the floating point library.
	//n_green = (n_green*9/10);
	n_red = (n_red*8/10);

	_red = n_red;
	_green = n_green;
	_blue = n_blue;
}

void RGBColor::SetColor(color_t color, uint8_t n_level)
{
	switch(color)
	{
		case WHITE:
			SetRgb(n_level, n_level, n_level);
			break;

		case MAGENTA:
			SetRgb(n_level, 0, n_level);
			break;

		case YELLOW:
			SetRgb(n_level, n_level, 0);
			break;

		case CYAN:
			SetRgb(0, n_level, n_level);
			break;

		case RED:
			SetRgb(n_level, 0, 0);
			break;

		case GREEN:
			SetRgb(0, n_level, 0);
			break;

		case BLUE:
			SetRgb(0, 0, n_level);
			break;

		default:
			break;
	}
}

RGBColor& RGBColor::operator=(const RGBColor& color)
{
	_red = color._red;
	_green = color._green;
	_blue = color._blue;
	return *this;
}

