/************************************************
�ļ���TWI.h
��;��TWIͷ�ļ�
************************************************/
#ifndef __TWI_H__
#define __TWI_H__

void twi_init(void);
void i2c_start(void);
unsigned char i2c_write(unsigned char data);
unsigned char i2c_read(void);
void i2c_stop(void);

#endif
