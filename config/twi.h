/************************************************
文件：TWI.h
用途：TWI头文件
************************************************/
#ifndef __TWI_H__
#define __TWI_H__

void twi_init(void);
void i2cstart(void);
unsigned char i2cwt(unsigned char data);
unsigned char i2crd(void);
void i2cstop(void);

#endif
