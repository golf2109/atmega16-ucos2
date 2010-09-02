#include "config.h"

float ResTable[6] = { 330.0, 330.0, 470.0, 470.0, 470.0, 150.0 };
float Rev_ResTable[6] = 
{ 
	0.0030303, 
	0.0030303, 
	0.0021277,
	0.0021277,
	0.0021277,
	0.0066667 };

void ResCtrl_init(void)
{
	RES_PORT = 0x3F;
	RES_DDR	 = 0x3F;
}

void SetRes(unsigned char mask, unsigned char value)
{
	RES_PORT &= ~(0x3F & mask);
	RES_PORT |= ((0x3F & mask) & value);
}
