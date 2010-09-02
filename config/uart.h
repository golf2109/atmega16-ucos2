#ifndef __UART_H__
#define __UART_H__

#define SYS_BAUDRATE 	9600
#define TXC_EN()    	UCSRB |= _BV(TXCIE);
#define TXC_DIS()   	UCSRB &= ~_BV(TXCIE);

void uart_init(void);
void uart_putchar(char x);
void uart_putstring(char *str);
void uart_putnstring (char *p, unsigned char len) ;

#endif