//ICC-AVR application builder : 2010-9-2 ÉÏÎç 12:37:32
// Target : M16
// Crystal: 8.0000Mhz

#include "config.h"

siocirqueue RTbuf;

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
	Tbuf_init();
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

void Com_putchar (char x)
{
	if(Tbuf_full())
		return;

	TXC_DIS();
	if (RTbuf.T_disabled)
	{
		UDR = x;
		RTbuf.T_disabled = 0;
	}
	else
	{
		RTbuf.T_buf[RTbuf.T_rear]=x;
		RTbuf.T_rear=(RTbuf.T_rear+1) & BIT_MASK;
	}
	TXC_EN();
}

void Com_putstring (char *p,unsigned char len) 
{
	unsigned char i;
	if ( !len )
		return;

	for(i = 0; i < len; i++)
	{
		Com_putchar(p[i]);
	}
}

void Com_putCommand(CMD *pCmd)
{
	Com_putchar(0xAA);
	Com_putstring((char*)pCmd, pCmd->para_size + 2 );
}

void Com_putData(DATA *pData)
{
	Com_putchar(0xAA);
	Com_putstring((char*)pData, DATA_LENGTH );
}

void Com_putAckCommand(void)
{
	CMD cmd;
	cmd.cmd_code = 0x01;
	cmd.para_size = 0;
	Com_putCommand( &cmd );
}

void Tbuf_init(void)
{
	RTbuf.T_front = 0;
	RTbuf.T_rear  = 0;
	RTbuf.T_disabled = 1;
}

bool Tbuf_empty(void)
{
	return RTbuf.T_rear == RTbuf.T_front;
}

bool Tbuf_full(void)
{
	return ((RTbuf.T_rear + 1) & BIT_MASK) == RTbuf.T_front;
}

ISR(USART_RXC_vect)
{
	unsigned char status = UCSRA;
	unsigned char data = UDR;

	if((status & (FRAMING_ERROR | DATA_OVERRUN))==0)
	{
		CmdRcvBuf_AddChar(data);
	}
}

ISR(USART_TXC_vect)
{
	if (!Tbuf_empty())
	{
		UDR = RTbuf.T_buf[RTbuf.T_front];
		RTbuf.T_front = (RTbuf.T_front+1) & BIT_MASK;
	}
	else 
	{
		RTbuf.T_disabled = 1;
	}
}
