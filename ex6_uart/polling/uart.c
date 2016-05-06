// Date:   13.6.2007
// Author: Janez Puhan

#include "uart.h"

// Reset and configure uart0
// baud_rate   ... baud rate (bit/sec)
// word_length ... word length [word_length_5_bit, word_length_6_bit,
//                 word_length_7_bit or word_length_8_bit]
// stop_bit    ... number of stop bits [one_stop_bit or two_stop_bits]
// parity      ... parity checking [disable_parity or enable_parity]
// parity_type ... type of parity if enabled [odd_parity, even_parity,
//                 allways_one_parity or allways_zero_parity]
// interrupts  ... enabled interrupts [any combination of 0x00000000,
//                 rx_data_available, thre_ier, rx_line_status and modem_status]
void uart0_init(int baud_rate, int word_length, int stop_bit, int parity,
	int parity_type, int interrupts)
{
	int divisor, div = 4;
	PINSEL0 = (PINSEL0 & ~uart0_bits) | uart0_values;
	U0TER = 0x00000000;
	U0LCR = dlab;
	if(VPBDIV) div = VPBDIV;
	divisor = 750000 * ((PLLSTAT & msel) + 1) / (div * baud_rate);
	U0DLL = divisor & 0x000000ff;
	U0DLM = divisor / 256;
	U0LCR = word_length | stop_bit | parity | parity_type;
	U0IER = 0x00000000;
	U0FCR = fifo_enable_fcr | rx_fifo_reset | tx_fifo_reset;
	U0IER = interrupts;
}

// Reset and configure uart1
// baud_rate   ... baud rate (bit/sec)
// word_length ... word length [word_length_5_bit, word_length_6_bit,
//                 word_length_7_bit or word_length_8_bit]
// stop_bit    ... number of stop bits [one_stop_bit or two_stop_bits]
// parity      ... parity checking [disable_parity or enable_parity]
// parity_type ... type of parity if enabled [odd_parity, even_parity,
//                 allways_one_parity or allways_zero_parity]
// handshake   ... configure (enable) hardware handshaking pins [0 or 1]
// interrupts  ... enabled interrupts [any combination of 0x00000000,
//                 rx_data_available, thre_ier, rx_line_status and modem_status]
void uart1_init(int baud_rate, int word_length, int stop_bit, int parity,
	int parity_type, int handshake, int interrupts)
{
	int divisor, div = 4;
	PINSEL0 = (PINSEL0 & ~uart1_rx_tx_bits) | uart1_rx_tx_values;
	if(handshake)
		PINSEL0 = (PINSEL0 & ~uart1_handshake_bits) | uart1_handshake_values;
	U1TER = 0x00000000;
	U1LCR = dlab;
	if(VPBDIV) div = VPBDIV;
	divisor = 750000 * ((PLLSTAT & msel) + 1) / (div * baud_rate);
	U1DLL = divisor & 0x000000ff;
	U1DLM = divisor / 256;
	U1LCR = word_length | stop_bit | parity | parity_type;
	U1IER = 0x00000000;
	U1FCR = fifo_enable_fcr | rx_fifo_reset | tx_fifo_reset;
	U1IER = interrupts;
}
