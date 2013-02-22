//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingTimer
// Description: Provides an interrput timer that drives a PCM signal for LED
//     illumination.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingTimer.h"

// 8 bit CTR
#define __TIMER1_MAX 0xFF    

// this may have to be adjusted if "__brightness_levels" is changed too much
#define __TIMER1_CNT 0x30   

uint16_t    wobble = 0x0FFF;

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

#define ALLLED  	((1<<LED0)|(1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4)|(1<<LED5))

#define ALED 		((1 << RED_A) | (1 << GREEN_A) | (1 << BLUE_A))
#define BLED 		((1 << RED_B) | (1 << GREEN_B) | (1 << BLUE_B))
#define CDDR_A    	DDRC
#define CPORT_A    	PORTC
#define CDDR_B    	DDRD
#define CPORT_B    	PORTD

 /* memory for RED LEDs */
uint8_t     brightness[3][__leds];   

uint8_t     arrange[6] = { 
	(1<<LED0), 
	(1<<LED1), 
	(1<<LED2), 
	(1<<LED3), 
	(1<<LED4), 
	(1<<LED5) };

	
void setup_timer2_ovf (void);

// -----------------------Function InitIO-------------------------------//
void InitTimer(void)
{
	DDRB |= ALLLED;    	// set PORTB as output
    PORTB &=~ ALLLED; 	// all pins HIGH --> cathodes HIGH --> LEDs off
    CDDR_A |= ALED;    	// set COLORPORT #5-7 as output
    CPORT_A &= ~ALED; 	// pins #5-7 LOW --> anodes LOW --> LEDs off
    CDDR_B |= BLED;    	// set COLORPORT #5-7 as output
    CPORT_B &= ~BLED; 	// pins #5-7 LOW --> anodes LOW --> LEDs off
	
    setup_timer2_ovf ();        
    enable_timer2_ovf ();    
}

// -----------------------Function setwobble-------------------------------//
void setwobble(uint16_t var)
{
    wobble=var;
}

// -----------------------Function setup_timer2_ovf-------------------------------//
void setup_timer2_ovf (void)
{
    // Arduino runs at 8 Mhz...
    // Timer2 (8bit) Settings:
    // prescaler (frequency divider) values:   CS12    CS11   CS10
    //                                           0       0      0    stopped
    //                                           0       0      1      /1  
    //                                           0       1      0      /8
    //                                           0       1      1      /32  
    //                                           1       0      0      /64
    //                                           1       0      1      /128 
    //                                           1       1      0      /256
    //                                           1       1      1      /1024
    TCCR2B |= ((1 << CS22) | (1 << CS20) | ((1 << CS21)));    //1024###
	
    //normal mode (16bit counter)
    TCCR2B &=~(1 << WGM22);
    TCCR2A =0;
	
    // enable global interrupts flag
    sei ();
}

// -----------------------Function enable_timer2_ovf-------------------------------//
void enable_timer2_ovf (void)
{
    TCNT2 = __TIMER1_MAX - __TIMER1_CNT;
    TIMSK2 |= (1 << TOIE2);
}

// -----------------------Function disable_timer2_ovf-------------------------------//
void disable_timer2_ovf (void) 
{
    PORTB &=~ ALLLED;    
    TIMSK2 &= ~(1 << TOIE2);
}

// -----------------------Framebuffer interrupt routine-------------------------------//
ISR (TIMER2_OVF_vect)
{
    TCNT2 = __TIMER1_MAX - __TIMER1_CNT;
    
    uint8_t cycle;
    uint8_t led;
    uint8_t times;
    uint8_t Atmp;
	uint8_t Btmp;
    uint16_t tp=0x0001;
    
    for (cycle = 0; cycle < __max_brightness; cycle++) 
    {
        tp=0x0001;
        led = 0;
        
        for (times = 0; times < 6; times++)   
        {
			// all relevant anodes LOW --> OFF
            CPORT_A &= ~ALED;    
            CPORT_B &= ~BLED;
			
			// all cathodes HIGH --> OFF
            PORTB &=~ ALLLED;        
            Atmp=0;Btmp=0;
            PORTB |= arrange[times];
            
            if(wobble & tp) {                            
                if (cycle < brightness[1][led])    Atmp |= (1 << GREEN_A);
                if (cycle < brightness[2][led])    Atmp |= (1 << BLUE_A);
                if (cycle < brightness[0][led])    Atmp |= (1 << RED_A);
            }
            
            led++;
            tp = tp<<1;
            //CPORT_A &= ~ALED;
            
            if(wobble & tp) {                                                    
                if (cycle < brightness[1][led])    Btmp |= (1 << GREEN_B);
                if (cycle < brightness[2][led])    Btmp |= (1 << BLUE_B);
                if (cycle < brightness[0][led])    Btmp |= (1 << RED_B);
            }
            
            CPORT_B |= Btmp;    
            CPORT_A |= Atmp;
            asm("nop");
            led++;
            tp = tp<<1;        
            //CPORT_B &= ~BLED;
            //PORTB &=~ ALLLED;
        }
    }
	
     // all relevant anodes LOW --> OFF
    CPORT_A &= ~ALED;   
    CPORT_B &= ~BLED;
	
	// all cathodes HIGH --> OFF
    PORTB &=~ ALLLED;  
}
