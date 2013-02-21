//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingUtil
// Description: Additional higher-level functions to control the lights 
//    including HSV color model.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingTimer.h"

enum COLOR_t {
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    TURQUOISE,
    FUCHSIA,
    WHITE
};

void random_leds (void);
void fader (void);
void fader_hue (void);
void color_wave (uint8_t width);
void sequence (void);
void rotate(uint8_t color,uint8_t dir);

void set_all_hsv (uint16_t hue, uint8_t sat, uint8_t val) ;
void set_all_byte_hsv (uint8_t data_byte, uint16_t hue, uint8_t sat, uint8_t val);
void set_led_hsv (uint8_t led, uint16_t hue, uint8_t sat, uint8_t val);
