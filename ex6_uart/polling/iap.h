// Date:   18.9.2013
// Author: Janez Puhan

#ifndef IAP_H_INCLUDED
#define IAP_H_INCLUDED

#include "pll.h"

// Sector information
#define _4KB			4096
#define _32KB			32768
#define FIRST_32KB		8		// Number of the first 32kB sector
#define FIRST_4KB		22		// Number of the first 4kB sector after 32kB sectors

// IAP commands
#define PREPARE_SECTOR	50
#define RAM_TO_FLASH	51
#define ERASE_SECTOR	52

// IAP routine return code
#define CMD_SUCCESS		0

// Function declarations
extern int *end_of_code();
extern int find_sector(int *address, int **sector, int *length);
extern int erase_sectors(int first, int last);
extern int write_flash(int *to, int *from, int length);

#endif
