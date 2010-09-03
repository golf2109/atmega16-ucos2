//ICC-AVR application builder : 2010-9-2 ���� 12:37:32
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
	UCSRC |= _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);	//8λ���ݣ�������9600��8N1,�޼���
	UCSRB |= _BV(RXEN)  | _BV(TXEN)  | _BV(RXCIE) | _BV(TXCIE);//ʹ�ܷ��ͣ����գ������жϣ������ж�
	
	uart_set_baudrate(SYS_BAUDRATE);				//���ò����ʣ�9600
	Tbuf_init();									//��ս��ջ�����
}

void uart_putchar(char x)
{
	while(!(UCSRA & (1<<UDRE))); 					//��򵥵ķ��ͺ���
	UDR = x;
}

void uart_putstring(char *str)						//���ڼ򵥷�ʽ�ķ����ַ�������
{
	while(*str++)
	{
		uart_putchar(*str);
	}
}

void uart_putnstring (char *p, unsigned char len) 	//���ڼ򵥷�ʽ�ķ���ָ�������ַ�������
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
	if(Tbuf_full())									//�����ͻ�����������ֱ�ӷ���
		return;

	TXC_DIS();										//�رշ�������ж�
	if (RTbuf.T_disable)							//������ͻ������ǲ����õ�
	{
		while(!(UCSRA & (1<<UDRE))); 				//����������ݼĴ�����Ϊ����ȴ�
		UDR = x;									//�ѵ�ǰҪ���͵������ȷ���ȥ
		RTbuf.T_disable = 0;						//�÷��ͻ���������
	}
	else
	{
		RTbuf.T_buf[RTbuf.T_tail]=x;				//�����͵����ݷŵ�������β��
		RTbuf.T_tail=(RTbuf.T_tail+1) & BIT_MASK;	//�ƶ�������β��ָ��
	}
	TXC_EN();										//�򿪷�������ж�
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

bool Tbuf_empty(void)							//��ⷢ�ͻ������Ƿ�Ϊ�գ�1Ϊ�գ�0Ϊ�ǿ�
{
	return (RTbuf.T_tail == RTbuf.T_head);
}

bool Tbuf_full(void)							//��ⷢ�ͻ������Ƿ�������1Ϊ����0Ϊ����
{
	return (((RTbuf.T_tail + 1) & BIT_MASK) == RTbuf.T_head);
}

ISR(USART_RXC_vect)								//��������жϷ����ӳ���
{
	unsigned char status = UCSRA;				//�ж�״̬
	unsigned char data = UDR;					//��ȡ��������

	if((status & (FRAMING_ERROR | DATA_OVERRUN))==0)
	{
		CmdRcvBuf_AddChar(data);
	}
}

ISR(USART_TXC_vect)								//��������жϷ����ӳ���
{
	if (!Tbuf_empty())							//�жϷ��ͻ������Ƿ�Ϊ�գ�������T_disable=1
	{
		UDR = RTbuf.T_buf[RTbuf.T_head];		//�ӷ��ͻ�������ͷ����ʼ����
		RTbuf.T_head = (RTbuf.T_head+1) & BIT_MASK;//����ͷ��λ��
	}
	else 
	{
		RTbuf.T_disable = 1;					//��λ������������
	}
}
