//////////////////////////////////////////////////////////////////////////////
// Title: RGBRingDefs
// Description: Basic definitions for the RGB Ring.
//////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <RGBDriver.h>


void setup()
{
	//Serial.begin(38400);
	g_rgbDriver.Setup();
}

void loop()
{
	g_rgbDriver.SetLedAllWhite(0);

	g_rgbDriver.SetLed(0, RGBDriver::RED, RGBDriver::_BRIGHT_MAX);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN, 4);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE, 4);
	delay(300);

	g_rgbDriver.SetLed(0, RGBDriver::RED, 4);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN, RGBDriver::_BRIGHT_MAX);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE, 4);
	delay(300);

	g_rgbDriver.SetLed(0, RGBDriver::RED, 4);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN, 4);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE, RGBDriver::_BRIGHT_MAX);
	delay(300);

	g_rgbDriver.SetLedAllWhite(RGBDriver::_BRIGHT_MAX);
	delay(300);
}

