//////////////////////////////////////////////////////////////////////////////
// Module: RGBRingUart
// Description: Serial control for the lights. This is redundant with 
//    with arduino "Wire" class and is enabled by setting the "UART"
//    preprocessor definition.
//////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

//#define UART

#define COMMAND_SIZE   30
#define RX_SIZE        64      /* UART receive buffer size (must be 2^n ) <=256 */
#define TX_SIZE        64      /* UART transmit buffer size (must be 2^n, 16<= ~ <=256 ) */
#define RX_MASK        (RX_SIZE-1)
#define TX_MASK        (TX_SIZE-1)

#ifdef UART
void InitUART(void);
#endif

extern uint8_t Command_Length;
extern uint8_t Command[COMMAND_SIZE];
extern uint8_t ReceivePtr;
extern uint8_t rx_buf[RX_SIZE];

void clearCommand(void);
void savebuff(void);
