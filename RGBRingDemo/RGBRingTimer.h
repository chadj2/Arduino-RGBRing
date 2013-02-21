//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingTimer
// Description: Provides a rotate effect for the LEDs based on an 
//    interrupt timer.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingLed.h"

enum DIRECTION_t {
        NONE,
    CW,
    CCW
};

extern uint16_t wobble_pattern_1[__leds];
extern uint16_t wobble_pattern_2[__leds];
extern uint16_t wobble_pattern_3[__leds];

void InitIO(void);
void setup_timer2_ovf (void);
void enable_timer2_ovf (void);
void disable_timer2_ovf (void); 
void swaywobble(uint8_t _delay,uint8_t dir);
void setwobble(uint16_t var);
