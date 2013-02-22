//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingTimer
// Description: Provides an interrput timer that drives a PCM signal for LED
//     illumination.
//////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#define __leds 		12
#define __max_led 	__leds - 1
#define __brightness_levels 	64
#define __max_brightness 		__brightness_levels-1

extern uint8_t brightness[3][__leds]; 

void InitTimer(void);
void enable_timer2_ovf (void);
void disable_timer2_ovf (void); 
void setwobble(uint16_t var);
