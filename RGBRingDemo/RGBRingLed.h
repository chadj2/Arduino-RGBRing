//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingLed
// Description: Basic routines to control brightness and color of LED's
//////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

// Starting from the D10,Clockwise
#define LED0 PB4
#define LED1 PB3
#define LED2 PB2
#define LED3 PB1
#define LED4 PB0
#define LED5 PB5

#define NEW_LED

#if defined NEW_LED
  #define RED_A     PC1
  #define GREEN_A PC0
  #define BLUE_A     PC2

  #define RED_B     PD5
  #define GREEN_B PD6
  #define BLUE_B     PD7
#elif
  #define RED_A     PC1
  #define GREEN_A PC2
  #define BLUE_A     PC0

  #define RED_B     PD5
  #define GREEN_B PD7
  #define BLUE_B     PD6
#endif

#define ALED 		((1 << RED_A) | (1 << GREEN_A) | (1 << BLUE_A))
#define BLED 		((1 << RED_B) | (1 << GREEN_B) | (1 << BLUE_B))
#define CDDR_A    	DDRC
#define CPORT_A    	PORTC
#define CDDR_B    	DDRD
#define CPORT_B    	PORTD
#define ALLLED  	((1<<LED0)|(1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4)|(1<<LED5))

#define __leds 		12
#define __max_led 	__leds - 1
#define __brightness_levels 	64
#define __max_brightness 		__brightness_levels-1

extern uint8_t brightness[3][__leds]; 

void InitLED(void);
void set_led_red(uint8_t led, uint8_t red);
void set_led_green(uint8_t led, uint8_t green);
void set_led_blue(uint8_t led, uint8_t blue);
void set_all_rgb(uint8_t red, uint8_t green, uint8_t blue);
void set_led_rgb(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
void set_led_unicolor(uint8_t led, uint8_t rgb, uint8_t var);
void set_all_unicolor(uint8_t rgb, uint8_t var);

/* macro definitions for LED control */ 
#define ALLLEDBLACK()  set_all_rgb( 0,0,0)
#define ALLLEDRED()  set_all_rgb( __max_brightness,0,0)
#define ALLLEDYELLO() set_all_rgb( __max_brightness, __max_brightness,0)
#define ALLLEDGREEN()  set_all_rgb( 0,__max_brightness,0)
#define ALLLEDTURQUOISE()  set_all_rgb( 0,__max_brightness, __max_brightness)
#define ALLLEDBLUE()  set_all_rgb( 0,0,__max_brightness)
#define ALLLEDFUCHSIA()  set_all_rgb( __max_brightness,0,__max_brightness)
#define ALLLEDWHITE()  set_all_rgb( __max_brightness,__max_brightness,__max_brightness)
