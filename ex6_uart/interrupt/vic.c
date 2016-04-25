// Date:   7.2.2007
// Author: Janez Puhan

#include "vic.h"

// Vector interrupt controller (VIC) initialisation
// fiq       ... FIQ mask determines which interrupts are FIQ
// irq       ... IRQ mask determines which interrupts are IRQ
// function  ... array of pointers to ISRs for each sloted IRQ
// interrupt ... array of sloted IRQs
// def       ... pointer to unsloted ISR
void vic_init(int fiq, int irq, voidfuncptr *function, int *interrupt,
	voidfuncptr def)
{
	int i, j;
	VICIntSelect = fiq;
	VICVectAddr0 = (int)function[0];
	VICVectAddr1 = (int)function[1];
	VICVectAddr2 = (int)function[2];
	VICVectAddr3 = (int)function[3];
	VICVectAddr4 = (int)function[4];
	VICVectAddr5 = (int)function[5];
	VICVectAddr6 = (int)function[6];
	VICVectAddr7 = (int)function[7];
	VICVectAddr8 = (int)function[8];
	VICVectAddr9 = (int)function[9];
	VICVectAddr10 = (int)function[10];
	VICVectAddr11 = (int)function[11];
	VICVectAddr12 = (int)function[12];
	VICVectAddr13 = (int)function[13];
	VICVectAddr14 = (int)function[14];
	VICVectAddr15 = (int)function[15];
	for(i = interrupt[0], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl0 = j | 0x00000020;
	else VICVectCntl0 = 0;
	for(i = interrupt[1], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl1 = j | 0x00000020;
	else VICVectCntl1 = 0;
	for(i = interrupt[2], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl2 = j | 0x00000020;
	else VICVectCntl2 = 0;
	for(i = interrupt[3], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl3 = j | 0x00000020;
	else VICVectCntl3 = 0;
	for(i = interrupt[4], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl4 = j | 0x00000020;
	else VICVectCntl4 = 0;
	for(i = interrupt[5], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl5 = j | 0x00000020;
	else VICVectCntl5 = 0;
	for(i = interrupt[6], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl6 = j | 0x00000020;
	else VICVectCntl6 = 0;
	for(i = interrupt[7], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl7 = j | 0x00000020;
	else VICVectCntl7 = 0;
	for(i = interrupt[8], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl8 = j | 0x00000020;
	else VICVectCntl8 = 0;
	for(i = interrupt[9], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl9 = j | 0x00000020;
	else VICVectCntl9 = 0;
	for(i = interrupt[10], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl10 = j | 0x00000020;
	else VICVectCntl10 = 0;
	for(i = interrupt[11], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl11 = j | 0x00000020;
	else VICVectCntl11 = 0;
	for(i = interrupt[12], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl12 = j | 0x00000020;
	else VICVectCntl12 = 0;
	for(i = interrupt[13], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl13 = j | 0x00000020;
	else VICVectCntl13 = 0;
	for(i = interrupt[14], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl14 = j | 0x00000020;
	else VICVectCntl14 = 0;
	for(i = interrupt[15], j = -1; i; i = i >> 1, j = j + 1);
	if(j > -1) VICVectCntl15 = j | 0x00000020;
	else VICVectCntl15 = 0;
	VICDefVectAddr = (int)def;
	VICIntEnable = fiq | irq;
	VICVectAddr = 0;
}
