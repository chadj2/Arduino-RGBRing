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
	RGBDriver();
	virtual ~RGBDriver() { }

	void Setup();

	static uint8_t _ledMap[];
};

#endif /* RGBDRIVER_H_ */
