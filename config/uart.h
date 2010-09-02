#ifndef __UART_H__
#define __UART_H__

#define SYS_BAUDRATE 	9600

#define RXB8 1
#define TXB8 0
#define UPE  2
#define OVR	 3
#define FE	 4
#define UDRE 5
#define RXC	 7

#define	FRAMING_ERROR  (1<<FE)
#define	PARITY_ERROR   (1<<UPE)
#define	DATA_OVERRUN   (1<<OVR)
#define	RX_COMPLETE    (1<<RXC)

#define REV_EN()    UCSRB |= _BV(RXCIE);
#define REV_DIS()   UCSRB &= ~_BV(RXCIE);

#define TXC_EN()    UCSRB |= _BV(TXCIE);
#define TXC_DIS()   UCSRB &= ~_BV(TXCIE);

#define TBUF_SIZE   32
#define BIT_MASK    0x1F

typedef struct{
	unsigned char T_front;
	unsigned char T_rear;
	unsigned char T_buf[TBUF_SIZE];
	unsigned char T_disabled;
}siocirqueue;

extern siocirqueue RTbuf;

void uart_init(void);
void uart_putchar(char x);
void uart_putstring(char *str);
void uart_putnstring (char *p, unsigned char len) ;
void Com_putchar (char x);
void Com_putstring (char *p,unsigned char len);
void Com_putCommand(CMD *pCmd);
void Com_putData(DATA *pData);
void Com_putAckCommand(void);
void Tbuf_init(void);
bool Tbuf_empty(void);
bool Tbuf_full(void);

#endif
