#include "config.h"

float const ResTable[6] PROGMEM = { 330.0, 330.0, 470.0, 470.0, 470.0, 150.0 };
float const Rev_ResTable[6] PROGMEM = 
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
