// AVR application builder : 2010-9-2 ���� 12:37:32
// Target : M16
// Crystal: 8.0000Mhz

#include "config.h"

SiocirQueue tx_buf;

static void tx_buf_init(void)
{
	tx_buf.head 	 = 0;
	tx_buf.tail  	 = 0;
	tx_buf.disable  = 1;							//������������
}

static bool tx_buf_is_empty(void)					//��ⷢ�ͻ������Ƿ�Ϊ�գ�1Ϊ�գ�0Ϊ�ǿ�
{
	return (tx_buf.tail == tx_buf.head);
}

static bool tx_buf_is_full(void)					//��ⷢ�ͻ������Ƿ�������1Ϊ����0Ϊ����
{
	return (((tx_buf.tail + 1) & BIT_MASK) == tx_buf.head);
}

//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9615 (0.2%)
static void uart_set_baudrate(unsigned int baudrate)
{
	unsigned int tmp;
	tmp= F_CPU/baudrate/16-1;						//F_CPU was defined in Makefile

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
	tx_buf_init();									//��ս��ջ�����
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

void com_putchar (char x)						
{
	if(tx_buf_is_full())							//�����ͻ�����������ֱ�ӷ���
		return;										//��ֱ�ӷ��ػ���������ȴ�������������

	TXC_DIS();										//�رշ�������ж�
	if (tx_buf.disable)								//������ͻ������ǲ����õ�
	{
		while(!(UCSRA & (1<<UDRE))); 				//����������ݼĴ�����Ϊ����ȴ�
		UDR = x;									//�ѵ�ǰҪ���͵������ȷ���ȥ
		tx_buf.disable = 0;							//�÷��ͻ���������
	}
	else
	{
		tx_buf.buf[tx_buf.tail]=x;					//�����͵����ݷŵ�������β��
		tx_buf.tail=(tx_buf.tail+1) & BIT_MASK;		//�ƶ�������β��ָ��
	}
	TXC_EN();										//�򿪷�������ж�
}

void com_putstring (char *p,unsigned char len) 
{
	unsigned char i;
	if (!len)
		return;

	for(i = 0; i < len; i++)
	{
		com_putchar(p[i]);
	}
}

void com_putcommand(CMD *pCmd)
{
	com_putchar(FRAME_HEAD);
	com_putstring((char*)pCmd, pCmd->para_size + 2);
}

void Com_putdata(DATA *pData)
{
	com_putchar(FRAME_HEAD);
	com_putstring((char*)pData, DATA_LENGTH);
}

void Com_put_ackcommand(void)
{
	CMD cmd;
	cmd.cmd_code 	= 0x01;
	cmd.para_size 	= 0;
	com_putcommand(&cmd);
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
	if (!tx_buf_is_empty())						//�жϷ��ͻ������Ƿ�Ϊ�գ�������disable=1
	{
		UDR = tx_buf.buf[tx_buf.head];			//�ӷ��ͻ�������ͷ����ʼ����
		tx_buf.head = (tx_buf.head+1) & BIT_MASK;//����ͷ��λ��
	}
	else 
	{
		tx_buf.disable = 1;						//��λ������������
	}
}
