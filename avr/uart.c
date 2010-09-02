//ICC-AVR application builder : 2010-9-2 ÉÏÎç 12:37:32
// Target : M16
// Crystal: 8.0000Mhz

#include "config.h"

//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9615 (0.2%)
static void uart_set_baudrate(unsigned int baudrate)
{
	unsigned int tmp;
	tmp= F_CPU/baudrate/16-1;

	UBRRH=(unsigned char)(tmp>>8);	//set baud rate lo
	UBRRL=(unsigned char)tmp;		//set baud rate hi
}

void uart_init(void)
{
	UCSRB = 0x00; //disable while setting baud rate
	UCSRA = 0x00;
	UCSRC |= _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);	
	UCSRB |= _BV(RXEN) | _BV(TXEN);
	uart_set_baudrate(SYS_BAUDRATE);
}

void uart_putchar(char x)
{
	TXC_DIS();
	UDR = x;
	TXC_EN();
}

void uart_putstring(char *str)
{
	while(*str++)
	{
		uart_putchar(*str);
	}
}

void uart_putnstring (char *p, unsigned char len) 
{
	unsigned char i;
	if ( !len )
		return;

	for(i = 0; i < len; i++)
	{
		uart_putchar(p[i]);
	}
}