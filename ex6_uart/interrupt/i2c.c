// Date:   15.2.2007
// Author: Janez Puhan

#include "i2c.h"

// Configure i2c0
// slave_address ... 7 bit address of this i2c device in slave mode, if
//                   slave_address = 0x00 and general_call = 0x00, then this device
//                   operates only in master mode
// general_call  ... if not zero, then this i2c device recognises general call
//                   address (0x00)
// duty          ... upper 16 bits define duration of high state in number of VPB
//                   cycles and lower 16 bits define duration of low state in number
//                   of VPB cycles
// tx_buf        ... pointer to data for slave transmit mode
void i2c0_init(int slave_address, int general_call, int duty, char *tx_buf)
{
	i2c0_rxslv_begin = 0;
	i2c0_rxslv_end = 0;
	i2c0_txslv = tx_buf;
	i2c0_status = i2c_idle;
	PINSEL0 = (PINSEL0 & ~i2c0_bits) | i2c0_values;
	I2C0SCLH = (duty & 0xffff0000) >> 16;
	I2C0SCLL = duty & 0x0000ffff;
	I2C0ADR = 2 * slave_address;
	if(general_call) I2C0ADR = I2C0ADR | gc;
	I2C0CONCLR = i2enc | stac | sic | aac;
	if(slave_address || general_call) I2C0CONSET = aa;
}

// Configure i2c1
// slave_address ... 7 bit address of this i2c device in slave mode, if
//                   slave_address = 0x00 and general_call = 0x00, then this device
//                   operates only in master mode
// general_call  ... if not zero, then this i2c device recognises general call
//                   address (0x00)
// duty          ... upper 16 bits define duration of high state in number of VPB
//                   cycles and lower 16 bits define duration of low state in number
//                   of VPB cycles
// tx_buf        ... pointer to data for slave transmit mode
void i2c1_init(int slave_address, int general_call, int duty, char *tx_buf)
{
	i2c1_rxslv_begin = 0;
	i2c1_rxslv_end = 0;
	i2c1_txslv = tx_buf;
	i2c1_status = i2c_idle;
	PINSEL0 = (PINSEL0 & ~i2c1_bits) | i2c1_values;
	I2C1SCLH = (duty & 0xffff0000) >> 16;
	I2C1SCLL = duty & 0x0000ffff;
	I2C1ADR = 2 * slave_address;
	if(general_call) I2C1ADR = I2C1ADR | gc;
	I2C1CONCLR = i2enc | stac | sic | aac;
	if(slave_address || general_call) I2C1CONSET = aa;
}

// Handles events on i2c0 bus
// Slave receiver mode (cyclic FIFO buffer)
int i2c0_rxslv_begin;		// Index of the first received byte
int i2c0_rxslv_end;			// Index of the first empty place
char i2c0_rxslv[bufsize];	// Buffer
// Slave transmitter mode
char *i2c0_txslv;	// Pointer to the string of bytes to be transmitted
// Master modes
int i2c0_address_rw;	// Device address with read/write bit
int i2c0_num_of_bytes;	// Number of bytes to be received/transmitted
int i2c0_status;		// Current master status
char *i2c0_buf;			// Receiving/transmitting buffer
void handle_i2c0_state()
{
	static int i2c0_txslv_cnt, i2c0_num_of_bytes_internal, i2c0_cnt;
	int tmp = I2C0ADR, status = I2C0STAT;
	switch(status)
	{
	case 0x00000000:
		i2c0_status = i2c_error;
		I2C0CONCLR = stac;
		I2C0CONSET = sto;
		if(tmp) I2C0CONSET = aa;
		else I2C0CONCLR = aac;
		break;
	case 0x00000008:
	case 0x00000010:
		i2c0_cnt = 0;
		I2C0DAT = i2c0_address_rw;
		break;
	case 0x00000018:
		i2c0_num_of_bytes_internal = i2c0_num_of_bytes;
	case 0x00000028:
		I2C0CONCLR = stac;
		if(i2c0_num_of_bytes_internal)
		{
			i2c0_num_of_bytes_internal = i2c0_num_of_bytes_internal - 1;
			I2C0DAT = i2c0_buf[i2c0_cnt];
			i2c0_cnt = i2c0_cnt + 1;
		} else
		{
			i2c0_status = i2c_idle;
			i2c0_num_of_bytes = 0;
			I2C0CONSET = sto;
		}
		break;
	case 0x00000030:
		i2c0_num_of_bytes = i2c0_num_of_bytes_internal + 1;
	case 0x00000020:
		i2c0_status = i2c_idle;
		I2C0CONCLR = stac;
		I2C0CONSET = sto;
		break;
	case 0x00000038:
		I2C0CONSET = sta;
		break;
	case 0x00000050:
		i2c0_buf[i2c0_cnt] = I2C0DAT;
		i2c0_cnt = i2c0_cnt + 1;
	case 0x00000040:
		i2c0_num_of_bytes = i2c0_num_of_bytes - 1;
		if(i2c0_num_of_bytes) I2C0CONSET = aa;
		else I2C0CONCLR = aac;
		I2C0CONCLR = stac;
		break;
	case 0x00000058:
		i2c0_buf[i2c0_cnt] = I2C0DAT;
	case 0x00000048:
		if(tmp) I2C0CONSET = aa;
		else I2C0CONCLR = aac;
		i2c0_status = i2c_idle;
		I2C0CONCLR = stac;
		I2C0CONSET = sto;
		break;
	case 0x00000080:
	case 0x00000090:
		tmp = i2c0_rxslv_end;
		i2c0_rxslv[tmp] = I2C0DAT;
		tmp = tmp + 1;
		if(tmp == bufsize) tmp = 0;
		if(tmp != i2c0_rxslv_begin) i2c0_rxslv_end = tmp;
		break;
	case 0x000000a8:
	case 0x000000b0:
		i2c0_txslv_cnt = 0;
	case 0x000000b8:
		I2C0DAT = i2c0_txslv[i2c0_txslv_cnt];
		i2c0_txslv_cnt = i2c0_txslv_cnt + 1;
		break;
	}
	I2C0CONCLR = sic;
}

// Handles events on i2c1 bus
// Slave receiver mode (cyclic FIFO buffer)
int i2c1_rxslv_begin;		// Index of the first received byte
int i2c1_rxslv_end;			// Index of the first empty place
char i2c1_rxslv[bufsize];	// Buffer
// Slave transmitter mode
char *i2c1_txslv;	// Pointer to the string of bytes to be transmitted
// Master modes
int i2c1_address_rw;	// Device address with read/write bit
int i2c1_num_of_bytes;	// Number of bytes to be received/transmitted
int i2c1_status;		// Current master status
char *i2c1_buf;			// Receiving/transmitting buffer
void handle_i2c1_state()
{
	static int i2c1_txslv_cnt, i2c1_num_of_bytes_internal, i2c1_cnt;
	int tmp = I2C1ADR, status = I2C1STAT;
	switch(status)
	{
	case 0x00000000:
		i2c1_status = i2c_error;
		I2C1CONCLR = stac;
		I2C1CONSET = sto;
		if(tmp) I2C1CONSET = aa;
		else I2C1CONCLR = aac;
		break;
	case 0x00000008:
	case 0x00000010:
		i2c1_cnt = 0;
		I2C1DAT = i2c1_address_rw;
		break;
	case 0x00000018:
		i2c1_num_of_bytes_internal = i2c1_num_of_bytes;
	case 0x00000028:
		I2C1CONCLR = stac;
		if(i2c1_num_of_bytes_internal)
		{
			i2c1_num_of_bytes_internal = i2c1_num_of_bytes_internal - 1;
			I2C1DAT = i2c1_buf[i2c1_cnt];
			i2c1_cnt = i2c1_cnt + 1;
		} else
		{
			i2c1_status = i2c_idle;
			i2c1_num_of_bytes = 0;
			I2C1CONSET = sto;
		}
		break;
	case 0x00000030:
		i2c1_num_of_bytes = i2c1_num_of_bytes_internal + 1;
	case 0x00000020:
		i2c1_status = i2c_idle;
		I2C1CONCLR = stac;
		I2C1CONSET = sto;
		break;
	case 0x00000038:
		I2C1CONSET = sta;
		break;
	case 0x00000050:
		i2c1_buf[i2c1_cnt] = I2C1DAT;
		i2c1_cnt = i2c1_cnt + 1;
	case 0x00000040:
		i2c1_num_of_bytes = i2c1_num_of_bytes - 1;
		if(i2c1_num_of_bytes) I2C1CONSET = aa;
		else I2C1CONCLR = aac;
		I2C1CONCLR = stac;
		break;
	case 0x00000058:
		i2c1_buf[i2c1_cnt] = I2C1DAT;
	case 0x00000048:
		if(tmp) I2C1CONSET = aa;
		else I2C1CONCLR = aac;
		i2c1_status = i2c_idle;
		I2C1CONCLR = stac;
		I2C1CONSET = sto;
		break;
	case 0x00000080:
	case 0x00000090:
		tmp = i2c1_rxslv_end;
		i2c1_rxslv[tmp] = I2C1DAT;
		tmp = tmp + 1;
		if(tmp == bufsize) tmp = 0;
		if(tmp != i2c1_rxslv_begin) i2c1_rxslv_end = tmp;
		break;
	case 0x000000a8:
	case 0x000000b0:
		i2c1_txslv_cnt = 0;
	case 0x000000b8:
		I2C1DAT = i2c1_txslv[i2c1_txslv_cnt];
		i2c1_txslv_cnt = i2c1_txslv_cnt + 1;
		break;
	}
	I2C1CONCLR = sic;
}
