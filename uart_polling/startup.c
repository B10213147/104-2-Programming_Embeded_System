#include "init.h"
#include "uart.h"

extern int main();

// Startup program
void start_up()
{
	// Initialisations
	int input = P0_12 | P0_13 | P0_14 | P0_15;
	int output = P0_4 | P0_5  | P0_6  | P0_7;
	int clock_mhz = 12;
	int div = cclk_4;
	int value = 0;
    
	init(clock_mhz, div, input, output, value); 

	//UART0 initialization
	int baud_rate = 9600;
	int word_length = word_length_8_bit;
	int stop_bit = one_stop_bit;
	int parity = disable_parity;
	int parity_type = 0;
    int handshake = 0;
	int interrupts = 0;
	
	uart1_init(baud_rate, word_length, stop_bit, parity,
	parity_type, handshake, interrupts);
	U1TER |= txen;	//enable the transmitter
	
	main();	// Start main program
	while(1);
}
