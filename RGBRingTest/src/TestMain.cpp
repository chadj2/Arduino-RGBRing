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
	// set all to black.
	g_rgbDriver.SetLedAll(RGBDriver::WHITE, 0);
	g_rgbDriver.SetLed(0, RGBDriver::RED);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN, 4);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE, 4);
	delay(200);

	g_rgbDriver.SetLed(0, RGBDriver::RED, 4);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE, 4);
	delay(200);

	g_rgbDriver.SetLed(0, RGBDriver::RED, 4);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN, 4);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE);
	delay(200);

	g_rgbDriver.SetLedAll(RGBDriver::WHITE);
	delay(200);

	g_rgbDriver.SetLedAll(RGBDriver::WHITE, 0);
	g_rgbDriver.SetLed(0, RGBDriver::RED);
	g_rgbDriver.SetLed(1, RGBDriver::GREEN);
	g_rgbDriver.SetLed(2, RGBDriver::BLUE);
	g_rgbDriver.SetLed(3, RGBDriver::PURPLE);
	g_rgbDriver.SetLed(4, RGBDriver::YELLOW);
	g_rgbDriver.SetLed(5, RGBDriver::CYAN);
	g_rgbDriver.SetLed(6, RGBDriver::WHITE);
	delay(1000);
}

