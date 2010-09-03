/************************************************
�ļ���PCF8563.h
��;��PCF8563����ͷ�ļ�
ע�⣺ϵͳʱ��8M
************************************************/
#include "config.h"

unsigned char syserr;
unsigned char const *week_list[7] PROGMEM =
{
	(unsigned char *)"Sunday   ",
	(unsigned char *)"Monday   ",
	(unsigned char *)"Tuesday  ",
	(unsigned char *)"Wednesday",
	(unsigned char *)"Thursday ",
	(unsigned char *)"Friday   ",
	(unsigned char *)"Saturday "
};

unsigned char write_buff[8];
unsigned char read_buff[8];
unsigned char const time_tmp[] ="00:00:00";
unsigned char const data_tmp[] ="2010.09.01";
unsigned char *week_tmp;


unsigned char const time[7] PROGMEM =
{
	0x00,		///*��		
	0x20,		///*��		
	0x09,		///*ʱ		
	0x08,		///*��		
	0x02,		///*����		
	0x06,		///*��/����	
	0x10		///*��		
};

/*************************************************************************
** ��������: clear(unsigned char *p,unsigned char num)
** ��������: ���ָ������ָ�����ȵ�����
** �䡡��: unsigned char *p :��ʼ��ַ   unsigned char num :�����ݳ���
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void clear(unsigned char *p,unsigned char num)
{
	for(; num > 0; num--)
	{
		*p = 0;
		p++;
	}
}
/*************************************************************************
** ��������: PCF8536_wt(unsigned int addr,unsigned char data)
** ��������: ��PCF8563ָ����ַд��һ������
** �䡡��: unsigned int addr    ���߰�λΪ������ַ���Ͱ�λΪ�ڲ��Ĵ�����ַ
unsigned char data  ����Ҫд�������
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void pcf8536_write(unsigned int addr, unsigned char data)
{
	unsigned char t;
	t = addr>>8;
	t <<= 1;
	i2c_start();
	if(i2c_write(W_ADD_COM+t) == SLAW)
	{
		i2c_write(addr);
		i2c_write(data);
	}
	else 
	{
		syserr=ERR_SLAW;
	}
	i2c_stop();
}
/*************************************************************************
** ��������: PCF8536_wt_p(unsigned int addr,unsigned char *p,unsigned char num)
** ��������: ��PCF8563��ַ�����ļĴ���д��ϵ������
** �䡡��: unsigned int addr    ���߰�λΪ������ַ���Ͱ�λΪ�ڲ��Ĵ�����ַ
unsigned char *p    ����Ҫд������������ʼ��ַ
unsigned char num   ��д�����ݵĸ���
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����д����������Ϊ��ַ�����ļĴ���
** ע�⣺
**************************************************************************/
void pcf8536_write_p(unsigned int addr,unsigned char *p,unsigned char num)
{
	unsigned char t;
	t = addr>>8;
	t <<= 1;
	i2c_start();
	
	if(i2c_write(W_ADD_COM+t)==SLAW)
	{
		i2c_write(addr);
		for(;num>0;num--)
		{
			i2c_write(*p);
			p++;
			_NOP();
		}
	}
	else 
	{
		syserr=ERR_SLAW;
	}
	i2c_stop();
}
/*************************************************************************
** ��������: PCF8536_rd(unsigned int addr,unsigned char *p,unsigned char num)
** ��������: ��PCF8563
** �䡡��: unsigned int addr    ���߰�λΪ������ַ���Ͱ�λΪ�ڲ��Ĵ�����ַ
unsigned char *p    �����������ݴ�ŵ�ַ����ʼ��ַ
unsigned char num   ���������ݵĸ���
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void pcf8536_read(unsigned int addr,unsigned char *p,unsigned char num)
{
	unsigned char t;
	
	t = addr >> 8;
	t <<= 1;
	i2c_start();
	
	if(i2c_write(W_ADD_COM + t) == SLAW)
	{
		i2c_write(addr);
	}
	else 
	{
		syserr=ERR_SLAW;
	}

	i2c_start();
	if(i2c_write(R_ADD_COM+t) == SLAW)
	{
		i2c_write(addr);
	}
	else 
	{
		syserr=ERR_SLAW;
	}

	for(;num>0;num--)
	{
		*p=i2c_read();
		p++;
	} 
}
/*************************************************************************
** ��������: pcf8563_init(void)
** ��������: PCF8563��ʼ��
** �䡡��: 
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void pcf8563_init(void)
{
	memset(write_buff,0, 8);
	memset(read_buff, 0, 8);

	//PCF8536_wt(0x00,0x20);//д�Ĵ���1��ֹͣ��ʱ
	//PCF8536_wt_p(0x02,time,7);//�趨ʱ��
	//PCF8536_wt(0x00,0x00);//д�Ĵ���1����ʼ��ʱ
}
/*************************************************************************
** ��������: Updata_time(void)
** ��������: ˢ��ʱ��Ĵ����������������
** �䡡��: 
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
/*void Updata_time(void)
{
PCF8536_rd(0x02,pgm_read_byte(time),7);//��ȡʱ��

time[0]=time[0]&0x7F;
time[1]=time[1]&0x7F;
time[2]=time[2]&0x3F;
time[3]=time[3]&0x3F;
time[4]=time[4]&0x07;
time[5]=time[5]&0x1F;
//����ʱ��
time_tmp[7]=(time[0]&0x0F)+0x30;
time_tmp[6]=(time[0]>>4)+0x30;
time_tmp[4]=(time[1]&0x0F)+0x30;
time_tmp[3]=(time[1]>>4)+0x30;
time_tmp[1]=(time[2]&0x0F)+0x30;
time_tmp[0]=(time[2]>>4)+0x30;
//��������
data_tmp[9]=(time[3]&0x0F)+0x30;
data_tmp[8]=(time[3]>>4)+0x30;
data_tmp[6]=(time[5]&0x0F)+0x30;
data_tmp[5]=(time[5]>>4)+0x30;
data_tmp[3]=(time[6]&0x0F)+0x30;
data_tmp[2]=(time[6]>>4)+0x30;
//��������
week_tmp=week_list[time[4]&0x0F];
}*/
