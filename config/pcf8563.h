/************************************************
文件：pcf8563.h
用途：pcf8563函数头文件
注意：系统时钟8M
************************************************/
#ifndef __PCF8563_H__
#define __PCF8563_H__

//器件地址格式： 1 0 1 0 0 0 A0 R/W
#define W_ADD_COM 	0xa2
#define R_ADD_COM 	0xa3

#define SLAW 		0x18
#define ERR_SLAW 	0x01

void clear(unsigned char *p,unsigned char num);
void pcf8563_init(void);
void pcf8536_write(unsigned int add,unsigned char data);
void pcf8536_write_p(unsigned int add,unsigned char *p,unsigned char num);
void pcf8536_read(unsigned int add,unsigned char *p,unsigned char num);
void Updata_time(void);

#endif
