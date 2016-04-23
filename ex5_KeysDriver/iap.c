// Date:   4.3.2008
// Author: Janez Puhan

#include "iap.h"

void find_end(int **ptr)
{
	asm("stmfd sp!, {r1, r2}");
	asm("ldr   r1, =_edata");
	asm("ldr   r2, =_data");
	asm("sub   r1, r1, r2");
	asm("ldr   r2, =_datasrc");
	asm("add   r1, r1, r2");
	asm("str   r1, [r0]");
	asm("ldmfd sp!, {r1, r2}");
}

// Find end of code in flash
// Return first free address
int *end_of_code()
{
	int *end;
	find_end(&end);
	return end;
}

// Find sector where address resides
// address ... flash address [0x00000000-0x0007cfff]
// sector  ... address of the first byte in sector
// length  ... number of bytes in sector
// Return sector number, its address and length
int find_sector(int *address, int **sector, int *length)
{
	int sector_num = 0;
	*sector = 0x00000000;
	*length = _4KB;
	while(!(*sector <= address && *sector + *length / 4 > address))
	{
		sector_num = sector_num + 1;
		*sector = *sector + *length / 4;
		if(sector_num == FIRST_32KB) *length = _32KB;
		if(sector_num == FIRST_4KB) *length = _4KB;
	}
	return sector_num;
}

void iap_function(int *command, int *result)
{
	asm("                stmfd sp!, {r1, r2, lr}");
	asm("                stmfd sp!, {r0}");
	asm("                mov   r2, #0x7ffffff1");
	asm("                ldr   lr, =return_address");
	asm("                bx    r2");
	asm("return_address: mov   r2, r0");
	asm("                ldmfd sp!, {r0}");
	asm("                str   r2, [r0]");
	asm("                ldmfd sp!, {r1, r2, lr}");
}

// Erase sectors
// Warning: Flash memory is not accessible during erase operation. Therefore
// interrupts should be disabled when calling this function. If interrupts cannot be
// avoided then interrupt vectors and handlers must reside in RAM.
// first ... number of first sector to erase [0-26]
// last  ... number of last sector to erase (last >= first) [0-26]
// Return a return code from the IAP routine
int erase_sectors(int first, int last)
{
	int command[5];
	command[0] = PREPARE_SECTOR;
	command[1] = first;
	command[2] = last;
	iap_function(command, command);
	if(command[0] == CMD_SUCCESS)
	{
		command[0] = ERASE_SECTOR;
		command[1] = first;
		command[2] = last;
		command[3] = 12000 * ((PLLSTAT & msel) + 1);
		iap_function(command, command);
	}
	return command[0];
}

// Write RAM to flash
// Warning: Flash memory is not accessible during write operation. Therefore
// interrupts should be disabled when calling this function. If interrupts cannot be
// avoided then interrupt vectors and handlers must reside in RAM.
// to     ... 256-byte aligned destination address in flash memory
// from   ... 32-bit aligned source address in RAM
// length ... number of bytes to copy [256, 512, 1024 or 4096]
// Return a return code from the IAP routine
int write_flash(int *to, int *from, int length)
{
	int command[5];
	command[1] = find_sector(to, (int **)command, command + 3);
	command[2] = find_sector(to + length / 4, (int **)command, command + 3);
	command[0] = PREPARE_SECTOR;
	iap_function(command, command);
	if(command[0] == CMD_SUCCESS)
	{
		command[0] = RAM_TO_FLASH;
		command[1] = (int)to;
		command[2] = (int)from;
		command[3] = length;
		command[4] = 12000 * ((PLLSTAT & msel) + 1);
		iap_function(command, command);
	}
	return command[0];
}
