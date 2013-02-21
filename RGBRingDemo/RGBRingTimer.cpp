//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingTimer
// Description: Provides a rotate effect for the LEDs based on an 
//    interrupt timer.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingTimer.h"

// 8 bit CTR
#define __TIMER1_MAX 0xFF    

// this may have to be adjusted if "__brightness_levels" is changed too much
#define __TIMER1_CNT 0x30   

uint16_t    wobble = 0x0FFF;

uint8_t     arrange[6] = { 
	(1<<LED0), 
	(1<<LED1), 
	(1<<LED2), 
	(1<<LED3), 
	(1<<LED4), 
	(1<<LED5) };

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

// -----------------------Function InitIO-------------------------------//
void InitIO(void)
{
	InitLED();
    setup_timer2_ovf ();        
    enable_timer2_ovf ();    
}

// -----------------------Function setwobble-------------------------------//
void setwobble(uint16_t var)
{
    wobble=var;
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
