//ICC-AVR application builder : 2010-9-2 上午 12:37:32
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

	UBRRH=(unsigned char)(tmp>>8);					//set baud rate lo
	UBRRL=(unsigned char)tmp;						//set baud rate hi
}

void uart_init(void)
{
	UCSRB = 0x00; 									//disable while setting baud rate
	UCSRA = 0x00;
	UCSRC |= _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);	//8位数据，波特率9600，8N1,无检验
	UCSRB |= _BV(RXEN)  | _BV(TXEN)  | _BV(RXCIE) | _BV(TXCIE);//使能发送，接收，发送中断，接收中断
	
	uart_set_baudrate(SYS_BAUDRATE);				//设置波特率，9600
	Tbuf_init();									//清空接收缓冲区
}

void uart_putchar(char x)
{
	while(!(UCSRA & (1<<UDRE))); 					//最简单的发送函数
	UDR = x;
}

void uart_putstring(char *str)						//基于简单方式的发送字符串函数
{
	while(*str++)
	{
		uart_putchar(*str);
	}
}

void uart_putnstring (char *p, unsigned char len) 	//基于简单方式的发送指定长度字符串函数
{
	unsigned char i;
	if (!len)
		return;

	for(i = 0; i < len; i++)
	{
		uart_putchar(p[i]);
	}
}

void Com_putchar (char x)						
{
	if(Tbuf_full())									//若发送缓冲区已满，直接返回
		return;

	TXC_DIS();										//关闭发送完成中断
	if (RTbuf.T_disable)							//如果发送缓冲区是不可用的
	{
		while(!(UCSRA & (1<<UDRE))); 				//如果接收数据寄存器不为空则等待
		UDR = x;									//把当前要发送的数据先发出去
		RTbuf.T_disable = 0;						//置发送缓冲区可用
	}
	else
	{
		RTbuf.T_buf[RTbuf.T_tail]=x;				//将发送的数据放到缓冲区尾部
		RTbuf.T_tail=(RTbuf.T_tail+1) & BIT_MASK;	//移动缓冲区尾部指针
	}
	TXC_EN();										//打开发送完成中断
}

void Com_putstring (char *p,unsigned char len) 
{
	unsigned char i;
	if (!len)
		return;

	for(i = 0; i < len; i++)
	{
		Com_putchar(p[i]);
	}
}

void Com_putCommand(CMD *pCmd)
{
	Com_putchar(FRAME_HEAD);
	Com_putstring((char*)pCmd, pCmd->para_size + 2);
}

void Com_putData(DATA *pData)
{
	Com_putchar(FRAME_HEAD);
	Com_putstring((char*)pData, DATA_LENGTH);
}

void Com_putAckCommand(void)
{
	CMD cmd;
	cmd.cmd_code 	= 0x01;
	cmd.para_size 	= 0;
	Com_putCommand(&cmd);
}

void Tbuf_init(void)
{
	RTbuf.T_head 	 = 0;
	RTbuf.T_tail  	 = 0;
	RTbuf.T_disable  = 1;
}

bool Tbuf_empty(void)							//检测发送缓冲区是否为空，1为空，0为非空
{
	return (RTbuf.T_tail == RTbuf.T_head);
}

bool Tbuf_full(void)							//检测发送缓冲区是否已满，1为满，0为非满
{
	return (((RTbuf.T_tail + 1) & BIT_MASK) == RTbuf.T_head);
}

ISR(USART_RXC_vect)								//接收完成中断服务子程序
{
	unsigned char status = UCSRA;				//判断状态
	unsigned char data = UDR;					//读取接收数据

	if((status & (FRAMING_ERROR | DATA_OVERRUN))==0)
	{
		CmdRcvBuf_AddChar(data);
	}
}

ISR(USART_TXC_vect)								//发送完成中断服务子程序
{
	if (!Tbuf_empty())							//判断发送缓冲区是否为空，空则置T_disable=1
	{
		UDR = RTbuf.T_buf[RTbuf.T_head];		//从发送缓冲区的头部开始发送
		RTbuf.T_head = (RTbuf.T_head+1) & BIT_MASK;//调整头部位置
	}
	else 
	{
		RTbuf.T_disable = 1;					//置位缓冲区不可用
	}
}
