// Date:   18.9.2013
// Author: Janez Puhan

#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

#include "gpio.h"

// Receive buffer size in slave mode
#define bufsize	20

// Bit masks in PINSEL0 register
#define i2c0_bits	0x000000f0
#define i2c0_values	0x00000050
#define i2c1_bits	0x30c00000
#define i2c1_values	0x30c00000

// Bit masks in I2C0CONSET and I2C1CONSET registers
#define aa			0x00000004
#define sto			0x00000010
#define sta			0x00000020
#define i2en		0x00000040

// Bit masks in I2C0CONCLR and I2C1CONCLR registers
#define aac			0x00000004
#define sic			0x00000008
#define stac		0x00000020
#define i2enc		0x00000040

// Bit mask in I2C0ADR and I2C1ADR registers
#define gc			0x00000001

// Register addresses
#define I2C0CONSET	(*((volatile int *)0xe001c000))	// Control Set
#define I2C0STAT	(*((volatile int *)0xe001c004))	// Status
#define I2C0DAT		(*((volatile int *)0xe001c008))	// Data
#define I2C0SCLH	(*((volatile int *)0xe001c010))	// SCL High Duty Cycle
#define I2C0SCLL	(*((volatile int *)0xe001c014))	// SCL Low Duty Cycle
#define I2C0CONCLR	(*((volatile int *)0xe001c018))	// Control Clear
#define I2C0ADR		(*((volatile int *)0xe001c00c))	// Slave Address
#define I2C1CONSET	(*((volatile int *)0xe005c000))	// Control Set
#define I2C1STAT	(*((volatile int *)0xe005c004))	// Status
#define I2C1DAT		(*((volatile int *)0xe005c008))	// Data
#define I2C1SCLH	(*((volatile int *)0xe005c014))	// SCL High Duty Cycle
#define I2C1SCLL	(*((volatile int *)0xe005c014))	// SCL Low Duty Cycle
#define I2C1CONCLR	(*((volatile int *)0xe005c018))	// Control Clear
#define I2C1ADR		(*((volatile int *)0xe005c00c))	// Slave Address

// Function declarations
extern void i2c0_init(int slave_address, int general_call, int duty, char *tx_buf);
extern void i2c1_init(int slave_address, int general_call, int duty, char *tx_buf);

// R/W flag values
#define read	1
#define write	0

// Master statuses
#define i2c_idle	0
#define i2c_busy	1
#define i2c_error	2

// Global variables
extern int i2c0_rxslv_begin, i2c0_rxslv_end, i2c0_address_rw, i2c0_num_of_bytes,
	i2c0_status, i2c1_rxslv_begin, i2c1_rxslv_end, i2c1_address_rw,
	i2c1_num_of_bytes, i2c1_status;
extern char i2c0_rxslv[bufsize], *i2c0_txslv, *i2c0_buf, i2c1_rxslv[bufsize],
	*i2c1_txslv, *i2c1_buf;

// Interrupt service routines
extern void handle_i2c0_state();
extern void handle_i2c1_state();

#endif
