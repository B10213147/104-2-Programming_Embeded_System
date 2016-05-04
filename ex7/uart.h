// Date:   26.9.2007
// Author: Janez Puhan

#include "gpio.h"
#include "pll.h"

// Bit masks in PINSEL0 register
#define uart0_bits				0x0000000f
#define uart0_values			0x00000005
#define uart1_rx_tx_bits		0x000f0000
#define uart1_rx_tx_values		0x00050000
#define uart1_handshake_bits	0xfff00000
#define uart1_handshake_values	0x55500000

// Bit masks in U0IER and U1IER registers
#define rx_data_available		0x00000001
#define thre_ier				0x00000002
#define rx_line_status			0x00000004
#define modem_status			0x00000008

// Interrupt identifiers in U0IIR and U1IIR registers
#define modem_id				0x00000000
#define thre_id					0x00000002
#define rx_data_available_id	0x00000004
#define rx_line_status_id		0x00000006
#define character_time_out_id	0x0000000c

// Bit masks in U0IIR and U1IIR registers
#define interrupt_pending		0x00000001
#define interrupt_id			0x0000000e
#define fifo_enable_iir			0x000000c0

// Bit masks in U0FCR and U1FCR registers
#define fifo_enable_fcr			0x00000001
#define rx_fifo_reset			0x00000002
#define tx_fifo_reset			0x00000004
#define rx_trigger_level		0x000000c0

// UART settings in U0LCR and U1LCR registers
#define word_length_5_bit		0x00000000
#define word_length_6_bit		0x00000001
#define word_length_7_bit		0x00000002
#define word_length_8_bit		0x00000003
#define one_stop_bit			0x00000000
#define two_stop_bits			0x00000004
#define disable_parity			0x00000000
#define enable_parity			0x00000008
#define odd_parity				0x00000000
#define even_parity				0x00000010
#define allways_one_parity		0x00000020
#define allways_zero_parity		0x00000030

// Bit masks in U0LCR and U1LCR registers
#define break_control			0x00000040
#define dlab					0x00000080

// Bit mask in U0LSR and U1LSR registers
#define rdr						0x00000001
#define oe						0x00000002
#define pe						0x00000004
#define fe						0x00000008
#define bi						0x00000010
#define thre_lsr				0x00000020
#define temt					0x00000040
#define rxfe					0x00000080

// Bit mask in U0TER and U1TER registers
#define txen					0x00000080

// Register addresses
#define U0RBR (*((volatile int *)0xe000c000)) // Receiver Buffer
#define U0THR (*((volatile int *)0xe000c000)) // Transmitter Holding
#define U0DLL (*((volatile int *)0xe000c000)) // Divisor Lacth LSB
#define U0DLM (*((volatile int *)0xe000c004)) // Divisor Lacth MSB
#define U0IER (*((volatile int *)0xe000c004)) // Interrupt Enable
#define U0IIR (*((volatile int *)0xe000c008)) // Interrupt ID
#define U0FCR (*((volatile int *)0xe000c008)) // FIFO Control
#define U0LCR (*((volatile int *)0xe000c00c)) // Line Control
#define U0LSR (*((volatile int *)0xe000c014)) // Line Status
#define U0TER (*((volatile int *)0xe000c030)) // Transmit Enable
#define U1RBR (*((volatile int *)0xe0010000)) // Receiver Buffer
#define U1THR (*((volatile int *)0xe0010000)) // Transmitter Holding
#define U1DLL (*((volatile int *)0xe0010000)) // Divisor Lacth LSB
#define U1DLM (*((volatile int *)0xe0010004)) // Divisor Lacth MSB
#define U1IER (*((volatile int *)0xe0010004)) // Interrupt Enable
#define U1IIR (*((volatile int *)0xe0010008)) // Interrupt ID
#define U1FCR (*((volatile int *)0xe0010008)) // FIFO Control
#define U1LCR (*((volatile int *)0xe001000c)) // Line Control
#define U1LSR (*((volatile int *)0xe0010014)) // Line Status
#define U1TER (*((volatile int *)0xe0010030)) // Transmit Enable

// Function declarations
extern void uart0_init(int baud_rate, int word_length, int stop_bit, int parity,
	int parity_type, int interrupts);
extern void uart1_init(int baud_rate, int word_length, int stop_bit, int parity,
	int parity_type, int handshake, int interrupts);
