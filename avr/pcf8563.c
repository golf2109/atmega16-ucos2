/************************************************
文件：PCF8563.h
用途：PCF8563函数头文件
注意：系统时钟8M
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
	0x00,		///*秒		
	0x20,		///*分		
	0x09,		///*时		
	0x08,		///*天		
	0x02,		///*星期		
	0x06,		///*月/世纪	
	0x10		///*年		
};

/*************************************************************************
** 函数名称: clear(unsigned char *p,unsigned char num)
** 功能描述: 清除指定区域指定长度的数据
** 输　入: unsigned char *p :起始地址   unsigned char num :零数据长度
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
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
** 函数名称: PCF8536_wt(unsigned int addr,unsigned char data)
** 功能描述: 向PCF8563指定地址写入一条数据
** 输　入: unsigned int addr    ：高八位为器件地址，低八位为内部寄存器地址
unsigned char data  ：需要写入的数据
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
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
** 函数名称: PCF8536_wt_p(unsigned int addr,unsigned char *p,unsigned char num)
** 功能描述: 向PCF8563地址连续的寄存器写入系列数据
** 输　入: 	unsigned int addr    ：高八位为器件地址，低八位为内部寄存器地址
			unsigned char *p     ：需要写入的数据域的起始地址
			unsigned char num    ：写入数据的个数
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：写入数据区域为地址连续的寄存器
** 注意：
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
** 函数名称: PCF8536_rd(unsigned int addr,unsigned char *p,unsigned char num)
** 功能描述: 读PCF8563
** 输　入: unsigned int addr    ：高八位为器件地址，低八位为内部寄存器地址
unsigned char *p    ：读出的数据存放地址的起始地址
unsigned char num   ：读出数据的个数
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
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
** 函数名称: pcf8563_init(void)
** 功能描述: PCF8563初始化
** 输　入: 
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
void pcf8563_init(void)
{
	memset(write_buff,0, 8);
	memset(read_buff, 0, 8);

	//PCF8536_wt(0x00,0x20);//写寄存器1，停止计时
	//PCF8536_wt_p(0x02,time,7);//设定时间
	//PCF8536_wt(0x00,0x00);//写寄存器1，开始计时
}
/*************************************************************************
** 函数名称: Updata_time(void)
** 功能描述: 刷新时间寄存器及相关数组内容
** 输　入: 
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
/*void Updata_time(void)
{
PCF8536_rd(0x02,pgm_read_byte(time),7);//读取时间

time[0]=time[0]&0x7F;
time[1]=time[1]&0x7F;
time[2]=time[2]&0x3F;
time[3]=time[3]&0x3F;
time[4]=time[4]&0x07;
time[5]=time[5]&0x1F;
//处理时间
time_tmp[7]=(time[0]&0x0F)+0x30;
time_tmp[6]=(time[0]>>4)+0x30;
time_tmp[4]=(time[1]&0x0F)+0x30;
time_tmp[3]=(time[1]>>4)+0x30;
time_tmp[1]=(time[2]&0x0F)+0x30;
time_tmp[0]=(time[2]>>4)+0x30;
//处理日期
data_tmp[9]=(time[3]&0x0F)+0x30;
data_tmp[8]=(time[3]>>4)+0x30;
data_tmp[6]=(time[5]&0x0F)+0x30;
data_tmp[5]=(time[5]>>4)+0x30;
data_tmp[3]=(time[6]&0x0F)+0x30;
data_tmp[2]=(time[6]>>4)+0x30;
//处理星期
week_tmp=week_list[time[4]&0x0F];
}*/
