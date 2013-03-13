//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingTimer
// Description: Provides an interrput timer that drives a PCM signal for LED
//     illumination.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingTimer.h"
#include "RGBRingDefs.h"
#include <avr/io.h>

// 8 bit CTR
#define RR_TIMER1_MAX 0xFF

// this may have to be adjusted if "RR_LED_BRIGHTNESS_LEVELS" is changed too much
#define RR_TIMER1_CNT 0x30

uint16_t wobble = 0x0FFF;

/* memory for RED LEDs */
uint8_t brightness[3][RR_LEDS];

uint8_t arrange[6] =
{ (1 << RR_SEL_LED0), (1 << RR_SEL_LED1), (1 << RR_SEL_LED2), (1 << RR_SEL_LED3), (1 << RR_SEL_LED4), (1 << RR_SEL_LED5) };

void setup_timer2_ovf(void);

// -----------------------Function InitIO-------------------------------//
void InitTimer(void)
{
	// selector
	RR_SEL_DDR |= RR_SEL_ALLLED;    	// set PORTB as output
	RR_SEL_PORT &= ~RR_SEL_ALLLED; 	// all pins HIGH --> cathodes HIGH --> LEDs off

	// color port A
	RR_CPA_DDR |= RR_CPA_WHITE;    	// set COLORPORT #5-7 as output
	RR_CPA_PORT &= ~RR_CPA_WHITE; 	// pins #5-7 LOW --> anodes LOW --> LEDs off

	// color port B
	RR_CPB_DDR |= RR_CPB_WHITE;    	// set COLORPORT #5-7 as output
	RR_CPB_PORT &= ~RR_CPB_WHITE; 	// pins #5-7 LOW --> anodes LOW --> LEDs off

	setup_timer2_ovf();
	enable_timer2_ovf();
}

// -----------------------Function setwobble-------------------------------//
void setwobble(uint16_t var)
{
	wobble = var;
}

// -----------------------Function setup_timer2_ovf-------------------------------//
void setup_timer2_ovf(void)
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
	TCCR2B &= ~(1 << WGM22);
	TCCR2A = 0;

	// enable global interrupts flag
	sei ();
}

// -----------------------Function enable_timer2_ovf-------------------------------//
void enable_timer2_ovf(void)
{
	TCNT2 = RR_TIMER1_MAX - RR_TIMER1_CNT;
	TIMSK2 |= (1 << TOIE2);
}

// -----------------------Function disable_timer2_ovf-------------------------------//
void disable_timer2_ovf(void)
{
	RR_SEL_PORT &= ~RR_SEL_ALLLED;
	TIMSK2 &= ~(1 << TOIE2);
}

// -----------------------Framebuffer interrupt routine-------------------------------//
ISR (TIMER2_OVF_vect)
{
	TCNT2 = RR_TIMER1_MAX - RR_TIMER1_CNT;

	uint8_t cycle;
	uint8_t led;
	uint8_t times;
	uint8_t Atmp;
	uint8_t Btmp;
	uint16_t tp = 0x0001;

	for (cycle = 0; cycle < RR_LED_BRIGHTNESS_MAX; cycle++)
	{
		tp = 0x0001;
		led = 0;

		for (times = 0; times < 6; times++)
		{
			// all relevant anodes LOW --> OFF
			RR_CPA_PORT &= ~RR_CPA_WHITE;
			RR_CPB_PORT &= ~RR_CPB_WHITE;

			// all cathodes HIGH --> OFF
			RR_SEL_PORT &= ~RR_SEL_ALLLED;
			Atmp = 0;
			Btmp = 0;
			RR_SEL_PORT |= arrange[times];

			if (wobble & tp)
			{
				if (cycle < brightness[1][led])
					Atmp |= (1 << RR_CPA_GREEN);
				if (cycle < brightness[2][led])
					Atmp |= (1 << RR_CPA_BLUE);
				if (cycle < brightness[0][led])
					Atmp |= (1 << RR_CPA_RED);
			}

			led++;
			tp = tp << 1;
			//CPORT_A &= ~ALED;

			if (wobble & tp)
			{
				if (cycle < brightness[1][led])
					Btmp |= (1 << RR_CPB_GREEN);
				if (cycle < brightness[2][led])
					Btmp |= (1 << RR_CPB_BLUE);
				if (cycle < brightness[0][led])
					Btmp |= (1 << RR_CPB_RED);
			}

			RR_CPB_PORT |= Btmp;
			RR_CPA_PORT |= Atmp;
			asm("nop");
			led++;
			tp = tp << 1;
			//RR_CPB_PORT &= ~RR_CPB_WHITE;
			//PORTB &=~ ALLLED;
		}
	}

	// all relevant anodes LOW --> OFF
	RR_CPA_PORT &= ~RR_CPA_WHITE;
	RR_CPB_PORT &= ~RR_CPB_WHITE;

	// all cathodes HIGH --> OFF
	RR_SEL_PORT &= ~RR_SEL_ALLLED;
}
