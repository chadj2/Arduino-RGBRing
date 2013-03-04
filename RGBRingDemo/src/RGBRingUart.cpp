//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingUart
// Description: Serial control for the lights. This is redundant with 
//    with arduino "Wire" class and is enabled by setting the "UART"
//    preprocessor definition.
//////////////////////////////////////////////////////////////////////////////

#include "RGBRingUart.h"



#ifndef BAUD_RATE
#define BAUD_RATE      19200
#endif

uint8_t ReceivePtr;
uint8_t rx_buf[RX_SIZE];
uint8_t Command_Length;
uint8_t Command[COMMAND_SIZE];

// -----------------------Function clearCommand-------------------------------//
void clearCommand(void)
{
    for(uint8_t i=0;i<COMMAND_SIZE;i++)  Command[i] = 0;
}

// -----------------------Function savebuff-------------------------------//
void savebuff(void)
{    
    for(uint8_t i=0;i<COMMAND_SIZE;i++){
           Command[i]= rx_buf[i] ;
    }
}

// -----------------------Function InitUART-------------------------------//
#ifdef UART
void InitUART(void)
{
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
    UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8;     // set baud rate
    UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
    UCSRB = (1<<RXCIE) | (1<<RXEN)|(1<<TXEN);  // enable Rx & Tx
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // config USART; 8N1
#endif
    DDRD &= ~_BV(PD0);
    PORTD |= _BV(PD0);
}

// -----------------------Function USART_ISR-------------------------------//
ISR(USART_RX_vect)
{
	uint8_t status = UCSR0A, data = UDR0;
    status  &= (1<<FE0) | (1<<DOR0) | (1<<UPE0);
	
    if(status == 0) { /* ############################## */
        if (data>0xDF)    ReceivePtr=0;
        if (data<0xF8) 
        rx_buf[ReceivePtr] = data;
        ReceivePtr++;
        if(ReceivePtr==RX_MASK) ReceivePtr=COMMAND_SIZE+1;
        //ReceivePtr =(ReceivePtr+1) & RX_MASK;
        else if((ReceivePtr<=COMMAND_SIZE) && (rx_buf[0]==data)) savebuff();        
    }
}
#endif
