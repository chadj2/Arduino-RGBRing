//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingUtil
// Description: Additional higher-level functions to control the lights 
//    including HSV color model.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingUtil.h"
#include <util/delay.h>

#define __fade__delay_ms  5


uint16_t wobble_pattern_1[__leds] = {
    0b0000000000000001,
    0b0000000000000010,
    0b0000000000000100,
    0b0000000000001000,
    0b0000000000010000,
    0b0000000000100000,
    0b0000000001000000,
    0b0000000010000000,
    0b0000000100000000,
    0b0000001000000000,
    0b0000010000000000,
    0b0000100000000000
};

uint16_t wobble_pattern_2[__leds] = {
	0b0000000000000001,
	0b0000100000000010,
	0b0000010000001000,
	0b0000001000001000,
	0b0000000100010000,
	0b0000000010100000,
	0b0000000001000000,
	0b0000000010100000,
	0b0000000100010000,
	0b0000001000001000,
	0b0000010000000100,
	0b0000100000000010
};

uint16_t wobble_pattern_3[__leds] = {
	0b0000000000000001,
	0b0000100000000010,
	0b0000010000001000,
	0b0000001000001000,
	0b0000000100010000,
	0b0000000010100000,
	0b0000000001000000,
	0b0000000010100000,
	0b0000000100010000,
	0b0000001000001000,
	0b0000010000000100,
	0b0000100000000010
};

// -----------------------Function random_leds-------------------------------//
void random_leds (void){
  set_led_hsv ((uint8_t) (random (__leds)), (uint16_t) (random (360)), 255, 255);
}

// -----------------------Function fader-------------------------------//
void fader (void){                /* fade the matrix form BLACK to WHITE and back */
  uint8_t ctr1;
  //uint8_t led;
    for (ctr1 = 0; ctr1 <= __max_brightness; ctr1++)    {
          set_all_rgb (ctr1, ctr1, ctr1);
          delay(__fade__delay_ms);
    }
      for (ctr1 = __max_brightness; (ctr1 >= 0) & (ctr1 != 255); ctr1--)    {
          set_all_rgb (ctr1, ctr1, ctr1);
          delay(__fade__delay_ms);
    }
}

// -----------------------Function fader_hue-------------------------------//
void fader_hue (void){    /* cycle the color of the whole matrix */
  uint16_t ctr1;
  for (ctr1 = 0; ctr1 < 360; ctr1 = ctr1 + 3)
    {
      set_all_hsv (ctr1, 255, 255);
      delay(__fade__delay_ms );
    }
}

// -----------------------Function color_wave-------------------------------//
void color_wave (uint8_t width) {
  uint8_t led;
  static uint16_t shift = 0;
  for (led = 0; led <= __max_led; led++)    {
      set_led_hsv (led, (uint16_t)(led) * (uint16_t)(width) + shift, 255, 255);
  }
  shift++;
}

// -----------------------Function sequence-------------------------------//
void sequence (void) {
  uint8_t led;
  static uint16_t shift = 0;
  uint8_t ls = 0;
    for (ls = 0; ls <= __max_led; ls++)    {
                disable_timer2_ovf();
        for (led = 0; led <= __max_led; led++)    {
            if(wobble_pattern_1[ls] & (0x0001<< led)){
                set_led_hsv (led, (uint16_t)(led) * (uint16_t)(ls) * shift, 255, 255);
                shift++;
            }else{
                set_led_rgb(led,0,0,0);
            }
        }
                 enable_timer2_ovf();
        _delay_ms(10);
    }
}
/*
void swaywobble (void) {
  uint8_t led;
  static uint16_t shift = 0;
  uint8_t ls = 0;
    for (ls = 0; ls <= __max_led; ls++)    {
        for (led = 0; led <= __max_led; led++)    {
            if(wobble_pattern_2[__max_led-ls] & (0x0001<< led)){
                set_led_hsv (led, (uint16_t)(led) * (uint16_t)(ls) * shift, 255, 255);
                shift++;
            }else{
                set_led_rgb(led,0,0,0);
            }
        }
        _delay_ms(20);
    }
}
*/

// -----------------------Function turnover-------------------------------//
void  turnover(uint8_t rgb,uint8_t dir){
    uint8_t led, temp;    
    if(dir==CCW){
        temp=brightness[rgb][0]; 
        for (led = 0; led < __max_led; led++)    {
            brightness[rgb][led]=brightness[rgb][led+1];
        }
        brightness[rgb][led]=temp;
    }
    if(dir==CW){
        temp=brightness[rgb][__max_led];
        for (led = __max_led; led >0; led--)    {
            brightness[rgb][led]=brightness[rgb][led-1];
        }
        brightness[rgb][0]=temp;
    }
}

// -----------------------Function rotate-------------------------------//
void rotate(uint8_t color,uint8_t dir){
    switch (color){
      case RED:    //R:0
        turnover(0,dir);
          break;
      case GREEN:    //G:1
        turnover(1,dir);
          break;
      case BLUE:    //B:2
        turnover(2,dir);
          break;
      case YELLOW:    //YELLOW RG:
        turnover(0,dir);
        turnover(1,dir);
          break;
      case TURQUOISE:    //TURQUOISE GB
        turnover(1,dir);
        turnover(2,dir);
          break;
      case FUCHSIA:    //FUCHSIA RB
        turnover(0,dir);
        turnover(2,dir);
          break;
      case WHITE:    //WHITE RGB
          turnover(0,dir);
          turnover(1,dir);
          turnover(2,dir);
      break;
    }
}

// -----------------------Function set_all_hsv-------------------------------//
void set_all_hsv (uint16_t hue, uint8_t sat, uint8_t val) {
    uint8_t led;
    for (led = 0; led <= __max_led; led++)    {
        set_led_hsv (led, hue, sat, val);
    }
}

void set_all_byte_hsv (uint8_t data_byte, uint16_t hue, uint8_t sat, uint8_t val){
    uint8_t led;
    for (led = 0; led <= __max_led; led++)    {
        if ((data_byte >> led) & (B00000001))    {
            set_led_hsv (led, hue, sat, val);
        }else{
            set_led_rgb (led, 0, 0, 0);
        }
    }
}

// -----------------------Function set_led_hsv-------------------------------//
void set_led_hsv (uint8_t led, uint16_t hue, uint8_t sat, uint8_t val){
    /* BETA */

    /* finally thrown out all of the float stuff and replaced with uint16_t
    * hue: 0-->360 (hue, color)
    * sat: 0-->255 (saturation)
    * val: 0-->255 (value, brightness)
    */

    hue = hue % 360;
    uint8_t sector = hue / 60;
    uint8_t rel_pos = hue - (sector * 60);
    uint16_t const mmd = 0xFFFF;    /* maximum modulation depth */
    uint16_t top = val * 255;
    uint16_t bottom = val * (255 - sat);    /* (val*255) - (val*255)*(sat/255) */
    uint16_t slope = (uint16_t)(val) * (uint16_t)(sat) / 120;    /* dy/dx = (top-bottom)/(2*60) -- val*sat: modulation_depth dy */
    uint16_t a = bottom + slope * rel_pos;
    uint16_t b = bottom + (uint16_t)(val) * (uint16_t)(sat) / 2 + slope * rel_pos;
    uint16_t c = top - slope * rel_pos;
    uint16_t d = top - (uint16_t)(val) * (uint16_t)(sat) / 2 - slope * rel_pos;

    uint16_t R, G, B;

    if (sector == 0)    {
        R = c;
        G = a;
        B = bottom;
    }else if (sector == 1)    {
        R = d;
        G = b;
        B = bottom;
    }else if (sector == 2)    {
        R = bottom;
        G = c;
        B = a;
    }else if (sector == 3)    {
        R = bottom;
        G = d;
        B = b;
    }else if (sector == 4)    {
        R = a;
        G = bottom;
        B = c;
    }
    else    {
        R = b;
        G = bottom;
        B = d;
    }

    uint16_t scale_factor = mmd / __max_brightness;

    R = (uint8_t) (R / scale_factor);
    G = (uint8_t) (G / scale_factor);
    B = (uint8_t) (B / scale_factor);

    set_led_rgb (led, R, G, B);
}

// -----------------------Function swaywobble-------------------------------//
void swaywobble (uint8_t _delay,uint8_t dir)
{
  uint8_t ls = 0;
  
  if(dir==CW)
  {
    for (ls = 0; ls <= __max_led; ls++)
	{
        setwobble(wobble_pattern_3[ls]);
        delay(_delay);
    }
  }
  if(dir==CCW)
  {
    for (ls = __max_led; ls >= 0; ls--)
	{
        setwobble(wobble_pattern_3[ls]);
        delay(_delay);
    }
  }
}

