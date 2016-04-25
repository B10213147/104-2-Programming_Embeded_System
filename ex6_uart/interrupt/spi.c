// Date:   12.7.2011
// Author: Janez Puhan

#include "spi.h"

// Configure SPI
// length    ... number of bits of data per transfer
//               [8, 9, 10, 11, 12, 13, 14, 15 or 16]
// mode      ... defines active clock level and sampling edge
//               [high_rising, high_falling, low_rising or low_falling]
// type      ... device type [master or slave]
// first     ... bit to be transferred first [lsb or msb]
// interrupt ... enable interrupt requests [0 or 1]
// div       ... fvpb divider, clock = fvpb / div [even number on interval [8, 254]]
void spi_init(int length, int mode, int type, int first, int interrupt, int div)
{
	S0SPCR = bit_enable | ((length & 0x000000ff) << 8) | mode | type | first |
		(interrupt << 7);
	S0SPCCR = div;
	PINSEL0 = (PINSEL0 & ~pinsel0_spi_bits) | pinsel0_spi_mask;
}
