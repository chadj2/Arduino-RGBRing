//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingTimer
// Description: Provides an interrput timer that drives a PCM signal for LED
//     illumination.
//////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#define RR_LEDS 		12
#define RR_LED_MAX 	RR_LEDS - 1
#define RR_LED_BRIGHTNESS_LEVELS 	64
#define RR_LED_BRIGHTNESS_MAX 		RR_LED_BRIGHTNESS_LEVELS-1

extern uint8_t brightness[3][RR_LEDS]; 

void InitTimer(void);
void enable_timer2_ovf (void);
void disable_timer2_ovf (void); 
void setwobble(uint16_t var);
