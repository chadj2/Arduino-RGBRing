//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingLed
// Description: Basic routines to control brightness and color of LED's
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingLed.h"

 /* memory for RED LEDs */
uint8_t     brightness[3][__leds];   

void InitLED(void)
{
    DDRB |= ALLLED;    	// set PORTB as output
    PORTB &=~ ALLLED; 	// all pins HIGH --> cathodes HIGH --> LEDs off
    CDDR_A |= ALED;    	// set COLORPORT #5-7 as output
    CPORT_A &= ~ALED; 	// pins #5-7 LOW --> anodes LOW --> LEDs off
    CDDR_B |= BLED;    	// set COLORPORT #5-7 as output
    CPORT_B &= ~BLED; 	// pins #5-7 LOW --> anodes LOW --> LEDs off
	
    set_all_rgb (0, 0, 0);
}

// -----------------------Function set_all_rgb-------------------------------//
void set_all_rgb (uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t led;
    for (led = 0; led <= __max_led; led++)
    {
        set_led_rgb (led, red, green, blue);
    }
}

// -----------------------Function set_led_rgb-------------------------------//
void set_led_rgb (uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
    set_led_red (led, red);
    set_led_green (led, green);
    set_led_blue (led, blue);
}

// ----------------------Function set_led_red-------------------------------//
void set_led_red (uint8_t led, uint8_t red)
{
#ifdef DOTCORR

    int8_t dotcorr = (int8_t) (pgm_read_byte (&dotcorr_red[led])) * red / __brightness_levels;
    uint8_t value;
    
    if (red + dotcorr < 0) { value = 0; }
    else { value = red + dotcorr; }
    brightness[0][led] = value;
    
#else
    brightness[0][led] = red*0.8;
#endif
}

// -----------------------Function set_led_green-------------------------------//
void set_led_green (uint8_t led, uint8_t green)
{
#ifdef DOTCORR

    int8_t dotcorr = (int8_t) (pgm_read_byte (&dotcorr_green[led])) * green / __brightness_levels;
    uint8_t value;

    if (green + dotcorr < 0) { value = 0; }
    else { value = green + dotcorr; }
    
    brightness[1][led] = value;

#else
    brightness[1][led] = green;
#endif
}

// -----------------------Function set_led_blue-------------------------------//
void set_led_blue (uint8_t led, uint8_t blue)
{
#ifdef DOTCORR
    int8_t dotcorr = (int8_t) (pgm_read_byte (&dotcorr_blue[led])) * blue / __brightness_levels;
    uint8_t value;
    
    if (blue + dotcorr < 0) { value = 0; }
    else { value = blue + dotcorr; }
    
    brightness[2][led] = value;
#else
    brightness[2][led] = blue;
#endif
}

// -----------------------Function set_led_unicolor-------------------------------//
void set_led_unicolor(uint8_t led, uint8_t rgb, uint8_t var){
    if(rgb>2 || led>__max_led) return;
    brightness[rgb][led] = var;
}

// -----------------------Function set_all_unicolor-------------------------------//
void set_all_unicolor(uint8_t rgb, uint8_t var){
    uint8_t led;
//disable_timer2_ovf();
    for (led = 0; led <= __max_led; led++)    {
        set_led_unicolor (led, rgb, var);
    }
//enable_timer2_ovf();
}
