#ifndef __RESCTRL_H_
#define __RESCTRL_H_

#define		RES_1				0x10
#define		RES_2				0x20
#define		RES_3				0x02
#define		RES_4				0x04
#define		RES_5				0x08
#define		RES_6				0x01

#define		RES_PORT			PORTA
#define		RES_DDR				DDRA
#define		RES_PIN				PINA

#define		EnableRes(bit);		{RES_PORT |= (bit);}
#define		DisableRes(bit);	{RES_PORT &= ~(bit);}
#define		ResIsEnable(bit);	{((RES_PORT & (bit)) != 0);}

float ResTable[6];
float Rev_ResTable[6];
void  ResCtrl_init(void);

void SetRes(unsigned char mask, unsigned char value);

#endif
