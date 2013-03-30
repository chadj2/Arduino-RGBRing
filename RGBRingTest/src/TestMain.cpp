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
	g_rgbDriver.SetLedAll(RGBColor(RGBColor::WHITE, 0));
	g_rgbDriver[0].SetColor(RGBColor::RED);
	g_rgbDriver[1].SetColor(RGBColor::GREEN, 16);
	g_rgbDriver[2].SetColor(RGBColor::BLUE, 16);
	delay(200);

	g_rgbDriver[0].SetColor(RGBColor::RED, 16);
	g_rgbDriver[1].SetColor(RGBColor::GREEN);
	g_rgbDriver[2].SetColor(RGBColor::BLUE, 16);
	delay(200);

	g_rgbDriver[0].SetColor(RGBColor::RED, 16);
	g_rgbDriver[1].SetColor(RGBColor::GREEN, 16);
	g_rgbDriver[2].SetColor(RGBColor::BLUE);
	delay(200);

	g_rgbDriver.SetLedAll(RGBColor(RGBColor::WHITE));
	delay(200);

	g_rgbDriver.SetLedAll(RGBColor::WHITE, 0);
	g_rgbDriver[0].SetColor(RGBColor::RED);
	g_rgbDriver[1].SetColor(RGBColor::YELLOW);
	g_rgbDriver[2].SetColor(RGBColor::GREEN);
	g_rgbDriver[3].SetColor(RGBColor::CYAN);
	g_rgbDriver[4].SetColor(RGBColor::BLUE);
	g_rgbDriver[5].SetColor(RGBColor::MAGENTA);
	g_rgbDriver[6].SetColor(RGBColor::WHITE);
	delay(1000);
}

