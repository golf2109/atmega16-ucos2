/************************************************
文件：PCF8563.h
用途：PCF8563函数头文件
注意：系统时钟8M
************************************************/
#ifndef __PCF8563_H__
#define __PCF8563_H__

//器件地址格式： 1 0 1 0 0 0 A0 R/W
#define W_ADD_COM 0xa2
#define R_ADD_COM 0xa3

#define SLAW 		0x18
#define ERR_SLAW 	0x01

extern void clear(unsigned char *p,unsigned char num);

extern void PCF8563_init(void);
extern void PCF8536_wt(unsigned int add,unsigned char data);
extern void PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num);
extern void PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num);
extern void Updata_time(void);

extern unsigned char write_buff[8];
extern unsigned char read_buff[8];
extern unsigned char time_tmp[];
extern unsigned char data_tmp[];
extern unsigned char *week_tmp;
extern unsigned char time[7];


#endif
