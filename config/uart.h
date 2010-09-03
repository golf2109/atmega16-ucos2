#ifndef __UART_H__
#define __UART_H__

#define SYS_BAUDRATE 	9600

#define FRAME_HEAD		0xAA

#define RXB8 			1
#define TXB8 			0
#define UPE  			2
#define OVR	 			3	
#define FE	 			4
#define UDRE 			5
#define RXC	 			7

#define	FRAMING_ERROR  (1<<FE)
#define	PARITY_ERROR   (1<<UPE)
#define	DATA_OVERRUN   (1<<OVR)
#define	RX_COMPLETE    (1<<RXC)

#define REV_EN()    	UCSRB |= _BV(RXCIE);
#define REV_DIS()   	UCSRB &= ~_BV(RXCIE);

#define TXC_EN()    	UCSRB |= _BV(TXCIE);
#define TXC_DIS()   	UCSRB &= ~_BV(TXCIE);

#define TBUF_SIZE   	32
#define BIT_MASK    	0x1F

typedef struct
{
	unsigned char head;
	unsigned char tail;
	unsigned char buf[TBUF_SIZE];
	unsigned char disable;
}SiocirQueue;

extern SiocirQueue tx_buf;

void uart_init(void);
void uart_putchar(char x);
void uart_putstring(char *str);
void uart_putnstring (char *p, unsigned char len) ;
void com_putchar (char x);
void com_putstring (char *p,unsigned char len);
void com_putcommand(CMD *pCmd);
void com_putdata(DATA *pData);
void com_put_ackcommand(void);

#endif
