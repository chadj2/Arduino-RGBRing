#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <WProgram.h>
#include <WConstants.h>
#include <util/delay.h>



/*=============================================================================
                                  other functions
=============================================================================*/

#define NEW_LED
//#define UART

#define __leds 12
#define __max_led __leds - 1

#define __brightness_levels 64
#define __max_brightness __brightness_levels-1

// Starting from the D10,Clockwise
#define LED0 PB4
#define LED1 PB3
#define LED2 PB2
#define LED3 PB1
#define LED4 PB0
#define LED5 PB5
// Starting from the D1 ,Clockwise
//#define LED0 PB5
//#define LED1 PB4
//#define LED2 PB3
//#define LED3 PB2
//#define LED4 PB1
//#define LED5 PB0

#if defined NEW_LED
  #define RED_A 	PC1
  #define GREEN_A PC0
  #define BLUE_A 	PC2

  #define RED_B 	PD5
  #define GREEN_B PD6
  #define BLUE_B 	PD7
#elif
  #define RED_A 	PC1
  #define GREEN_A PC2
  #define BLUE_A 	PC0

  #define RED_B 	PD5
  #define GREEN_B PD7
  #define BLUE_B 	PD6
#endif

#define ALED ((1 << RED_A) | (1 << GREEN_A) | (1 << BLUE_A))
#define BLED ((1 << RED_B) | (1 << GREEN_B) | (1 << BLUE_B))
#define CDDR_A	DDRC
#define CPORT_A	PORTC
#define CDDR_B	DDRD
#define CPORT_B	PORTD
#define ALLLED  ((1<<LED0)|(1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4)|(1<<LED5))


#define __TIMER1_MAX 0xFF	// 8 bit CTR
#define __TIMER1_CNT 0x30   // this may have to be adjusted if "__brightness_levels" is changed too much


#ifndef BAUD_RATE
#define BAUD_RATE   19200
#endif

#define	RX_SIZE		64      /* UART receive buffer size (must be 2^n ) <=256 */
#define	TX_SIZE		64      /* UART transmit buffer size (must be 2^n, 16<= ~ <=256 ) */

#define	RX_MASK		(RX_SIZE-1)
#define	TX_MASK		(TX_SIZE-1)


#define COMMAND_SIZE 30

#define __fade__delay_ms  5


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

enum DIRECTION_t {
        NONE,
	CW,
	CCW
};




/*=============================================================================
                                  other functions
=============================================================================*/


uint8_t arrange[6]={(1<<LED0),(1<<LED1),(1<<LED2),(1<<LED3),(1<<LED4),(1<<LED5)};

static uint16_t wobble=0x0FFF;

uint8_t brightness[3][__leds];	/* memory for RED LEDs */

uint8_t ReceivePtr;
uint8_t rx_buf[RX_SIZE];

uint8_t Command_Length;
uint8_t	Command[COMMAND_SIZE];



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



#ifdef UART
void InitUART(void);
#endif
void clearCommand(void);
void savebuff(void);
void random_leds (void);
void fader (void);
void fader_hue (void);
void color_wave (uint8_t width) ;

void setwobble(uint16_t var);
void set_led_red (uint8_t led, uint8_t red) ;
void set_led_green (uint8_t led, uint8_t green) ;
void set_led_blue (uint8_t led, uint8_t blue) ;
void set_led_rgb (uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
void set_all_rgb (uint8_t red, uint8_t green, uint8_t blue) ;
void set_led_unicolor(uint8_t led, uint8_t rgb, uint8_t var);
void set_all_unicolor(uint8_t rgb, uint8_t var);
void set_all_hsv (uint16_t hue, uint8_t sat, uint8_t val) ;
void set_all_byte_hsv (uint8_t data_byte, uint16_t hue, uint8_t sat, uint8_t val);
void set_led_hsv (uint8_t led, uint16_t hue, uint8_t sat, uint8_t val);

void setup_timer2_ovf (void);
void enable_timer2_ovf (void);
void disable_timer2_ovf (void); 

#define ALLLEDBLACK()  set_all_rgb( 0,0,0)
#define ALLLEDRED()  set_all_rgb( __max_brightness,0,0)
#define ALLLEDYELLO() set_all_rgb( __max_brightness, __max_brightness,0)
#define ALLLEDGREEN()  set_all_rgb( 0,__max_brightness,0)
#define ALLLEDTURQUOISE()  set_all_rgb( 0,__max_brightness, __max_brightness)
#define ALLLEDBLUE()  set_all_rgb( 0,0,__max_brightness)
#define ALLLEDFUCHSIA()  set_all_rgb( __max_brightness,0,__max_brightness)
#define ALLLEDWHITE()  set_all_rgb( __max_brightness,__max_brightness,__max_brightness)





// -----------------------Function InitIO-------------------------------//
void InitIO(void){
  DDRB |= ALLLED;	// set PORTB as output
  PORTB &=~ ALLLED;			// all pins HIGH --> cathodes HIGH --> LEDs off
  CDDR_A |= ALED;	// set COLORPORT #5-7 as output
  CPORT_A &= ~ALED;	// pins #5-7 LOW --> anodes LOW --> LEDs off
  CDDR_B |= BLED;	// set COLORPORT #5-7 as output
  CPORT_B &= ~BLED;	// pins #5-7 LOW --> anodes LOW --> LEDs off

  set_all_rgb (0, 0, 0);
  setup_timer2_ovf ();		
  enable_timer2_ovf ();	
}



// -----------------------Function InitUART-------------------------------//
#ifdef UART
void InitUART(void){
	/* initialize UART(s) depending on CPU defined */
#if defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
    #ifdef DOUBLE_SPEED
      UCSR0A = (1<<U2X0); //Double speed mode USART0
      UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*8L)-1);
      UBRR0H = (F_CPU/(BAUD_RATE*8L)-1) >> 8;
    #else
      UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
      UBRR0H = (F_CPU/(BAUD_RATE*16L)-1) >> 8;
    #endif

    UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
#elif defined __AVR_ATmega8__
	/* m8 */
	UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8; 	// set baud rate
	UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
	UCSRB = (1<<RXCIE) | (1<<RXEN)|(1<<TXEN);  // enable Rx & Tx
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // config USART; 8N1
#endif
    DDRD &= ~_BV(PD0);
    PORTD |= _BV(PD0);
}



// -----------------------Function USART_ISR-------------------------------//
ISR(USART_RX_vect){
    uint8_t status = UCSR0A, data = UDR0;
    status  &= (1<<FE0) | (1<<DOR0) | (1<<UPE0);
    if(status == 0) { /* û�н��մ����� */
        if (data>0xDF)	ReceivePtr=0;
        if (data<0xF8) 
        rx_buf[ReceivePtr] = data;
		ReceivePtr++;
		if(ReceivePtr==RX_MASK) ReceivePtr=COMMAND_SIZE+1;
        //ReceivePtr =(ReceivePtr+1) & RX_MASK;
		else if((ReceivePtr<=COMMAND_SIZE) && (rx_buf[0]==data)) savebuff();		
    }
}
#endif

// -----------------------Function clearCommand-------------------------------//
void clearCommand(void){
    for(char i=0;i<COMMAND_SIZE;i++)  Command[i] = 0;
}

// -----------------------Function savebuff-------------------------------//
void savebuff(void){	
    for(char i=0;i<COMMAND_SIZE;i++){
           Command[i]= rx_buf[i] ;
    }
}






/*=============================================================================
                                  other functions
=============================================================================*/


// -----------------------Function random_leds-------------------------------//
void random_leds (void){
  set_led_hsv ((uint8_t) (random (__leds)), (uint16_t) (random (360)), 255, 255);
}


// -----------------------Function fader-------------------------------//
void fader (void){				/* fade the matrix form BLACK to WHITE and back */
  uint8_t ctr1;
  uint8_t led;
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
void fader_hue (void){	/* cycle the color of the whole matrix */
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


// -----------------------Function swaywobble-------------------------------//
void swaywobble (uint8_t _delay,uint8_t dir) {
  uint8_t ls = 0;
  if(dir==CW){
	for (ls = 0; ls <= __max_led; ls++)    {
		setwobble(wobble_pattern_3[ls]);
		delay(_delay);
	}
  }
  if(dir==CCW){
	for (ls = __max_led; ls >= 0; ls--)    {
		setwobble(wobble_pattern_3[ls]);
		delay(_delay);
	}
  }
}


// -----------------------Function turnover-------------------------------//
void  turnover(uint8_t rgb,uint8_t dir){
    uint8_t led, temp, i;    
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
      case RED:	//R:0
		turnover(0,dir);
          break;
      case GREEN:	//G:1
		turnover(1,dir);
          break;
      case BLUE:	//B:2
		turnover(2,dir);
          break;
      case YELLOW:	//YELLOW RG:
		turnover(0,dir);
		turnover(1,dir);
          break;
      case TURQUOISE:	//TURQUOISE GB
		turnover(1,dir);
		turnover(2,dir);
          break;
      case FUCHSIA:	//FUCHSIA RB
		turnover(0,dir);
		turnover(2,dir);
          break;
      case WHITE:	//WHITE RGB
          turnover(0,dir);
          turnover(1,dir);
          turnover(2,dir);
	  break;
    }
}


/*===========================================================================
                 basic functions to set the LEDs
===========================================================================*/


// -----------------------Function setwobble-------------------------------//
void setwobble(uint16_t var){
	wobble=var;
}


// ----------------------Function set_led_red-------------------------------//
void set_led_red (uint8_t led, uint8_t red) {
#ifdef DOTCORR
	int8_t dotcorr =
	(int8_t) (pgm_read_byte (&dotcorr_red[led])) * red / __brightness_levels;
	uint8_t value;
	if (red + dotcorr < 0)    {
		value = 0;
	}else{
		value = red + dotcorr;
	}
	brightness[0][led] = value;
#else
  brightness[0][led] = red*0.8;
#endif
}


// -----------------------Function set_led_green-------------------------------//
void set_led_green (uint8_t led, uint8_t green) {
#ifdef DOTCORR
  int8_t dotcorr =
    (int8_t) (pgm_read_byte (&dotcorr_green[led])) * green /
    __brightness_levels;
  uint8_t value;
  if (green + dotcorr < 0)
    {
      value = 0;
    }
  else
    {
      value = green + dotcorr;
    }
  brightness[1][led] = value;
#else
  brightness[1][led] = green;
#endif
}


// -----------------------Function set_led_blue-------------------------------//
void set_led_blue (uint8_t led, uint8_t blue) {
#ifdef DOTCORR
  int8_t dotcorr =
    (int8_t) (pgm_read_byte (&dotcorr_blue[led])) * blue /
    __brightness_levels;
  uint8_t value;
  if (blue + dotcorr < 0)
    {
      value = 0;
    }
  else
    {
      value = blue + dotcorr;
    }
  brightness[2][led] = value;
#else
  brightness[2][led] = blue;
#endif
}


// -----------------------Function set_led_rgb-------------------------------//
void set_led_rgb (uint8_t led, uint8_t red, uint8_t green, uint8_t blue){

	set_led_red (led, red);
	set_led_green (led, green);
	set_led_blue (led, blue);

}


// -----------------------Function set_all_rgb-------------------------------//
void set_all_rgb (uint8_t red, uint8_t green, uint8_t blue) {
	uint8_t led;
	for (led = 0; led <= __max_led; led++)    {
		set_led_rgb (led, red, green, blue);
	}
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
		if ((data_byte >> led) & (B00000001))	{
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
	uint16_t const mmd = 255 * 255;	/* maximum modulation depth */
	uint16_t top = val * 255;
	uint16_t bottom = val * (255 - sat);	/* (val*255) - (val*255)*(sat/255) */
	uint16_t slope = (uint16_t)(val) * (uint16_t)(sat) / 120;	/* dy/dx = (top-bottom)/(2*60) -- val*sat: modulation_depth dy */
	uint16_t a = bottom + slope * rel_pos;
	uint16_t b = bottom + (uint16_t)(val) * (uint16_t)(sat) / 2 + slope * rel_pos;
	uint16_t c = top - slope * rel_pos;
	uint16_t d = top - (uint16_t)(val) * (uint16_t)(sat) / 2 - slope * rel_pos;

	uint16_t R, G, B;

	if (sector == 0)    {
		R = c;
		G = a;
		B = bottom;
	}else if (sector == 1)	{
		R = d;
		G = b;
		B = bottom;
	}else if (sector == 2)	{
		R = bottom;
		G = c;
		B = a;
	}else if (sector == 3)	{
		R = bottom;
		G = d;
		B = b;
	}else if (sector == 4)	{
		R = a;
		G = bottom;
		B = c;
	}
	else	{
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





/*=============================================================================
              Functions dealing with hardware specific jobs / settings
==============================================================================*/


// -----------------------Function swaywobble-------------------------------//
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
  TCCR2B |= ((1 << CS22) | (1 << CS20) | ((1 << CS21)));	//1024�
  //normal mode (16bit counter)
  TCCR2B &=~(1 << WGM22);
  TCCR2A =0;
  // enable global interrupts flag
  sei ();
}


// -----------------------Function swaywobble-------------------------------//
void enable_timer2_ovf (void){
  TCNT2 = __TIMER1_MAX - __TIMER1_CNT;
  TIMSK2 |= (1 << TOIE2);
}


// -----------------------Function swaywobble-------------------------------//
void disable_timer2_ovf (void) {
	PORTB &=~ ALLLED;	
	TIMSK2 &= ~(1 << TOIE2);
}


// -----------------------Function swaywobble-------------------------------//
ISR (TIMER2_OVF_vect){				/* Framebuffer interrupt routine */
  	TCNT2 = __TIMER1_MAX - __TIMER1_CNT;
  	uint8_t cycle;
	uint8_t led;
	uint8_t times;
	uint8_t Atmp,Btmp;
	uint16_t tp=0x0001;
  	for (cycle = 0; cycle < __max_brightness; cycle++)    {
        tp=0x0001;
        led = 0;
      	for (times = 0; times < 6; times++)	{
			CPORT_A &= ~ALED;	// all relevant anodes LOW --> OFF
			CPORT_B &= ~BLED;
			PORTB &=~ ALLLED;		// all cathodes HIGH --> OFF
			Atmp=0;Btmp=0;
                        PORTB |= arrange[times];
			if(wobble & tp) {							
				if (cycle < brightness[1][led])	Atmp |= (1 << GREEN_A);
				if (cycle < brightness[2][led])	Atmp |= (1 << BLUE_A);
				if (cycle < brightness[0][led])	Atmp |= (1 << RED_A);
			}
			led++;
			tp = tp<<1;
			//CPORT_A &= ~ALED;
			if(wobble & tp) {                        							
				if (cycle < brightness[1][led])	Btmp |= (1 << GREEN_B);
				if (cycle < brightness[2][led])	Btmp |= (1 << BLUE_B);
				if (cycle < brightness[0][led])	Btmp |= (1 << RED_B);
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
	CPORT_A &= ~ALED;	// all relevant anodes LOW --> OFF
	CPORT_B &= ~BLED;
	PORTB &=~ ALLLED;  // all cathodes HIGH --> OFF
}
/*
 * PWM_BLOCK_END: all functions in this block are related to PWM mode !
 */
