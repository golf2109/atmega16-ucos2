#ifndef __UART_H__
#define __UART_H__

void uart0_init(void);
void uart_putchar(unsigned char x);
void uart_putstring(unsigned char *str);

#endif