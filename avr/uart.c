//ICC-AVR application builder : 2010-9-2 ÉÏÎç 12:37:32
// Target : M16
// Crystal: 8.0000Mhz

#include <avr/io.h>

#define TXC_EN()    UCSRB |= _BV(TXCIE);
#define TXC_DIS()   UCSRB &= ~_BV(TXCIE);
//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9615 (0.2%)
void uart0_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);	
 UBRRL = 0x33; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x18;
}

void uart_putchar(unsigned char x)
{
	TXC_DIS();
	UDR = x;
	TXC_EN();
}

void uart_putstring(unsigned char *str)
{
	while(*str++)
	{
		uart_putchar(*str);
	}
}