/************************************************
�ļ���PCF8563.h
��;��PCF8563����ͷ�ļ�
ע�⣺ϵͳʱ��8M
************************************************/
#ifndef __PCF8563_H__
#define __PCF8563_H__

//������ַ��ʽ�� 1 0 1 0 0 0 A0 R/W
#define W_ADD_COM 	0xa2
#define R_ADD_COM 	0xa3

#define SLAW 		0x18
#define ERR_SLAW 	0x01

void clear(unsigned char *p,unsigned char num);

void PCF8563_init(void);
void PCF8536_wt(unsigned int add,unsigned char data);
void PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num);
void PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num);
void Updata_time(void);

#endif
