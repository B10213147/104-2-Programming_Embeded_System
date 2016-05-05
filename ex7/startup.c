// Date:   7.11.2011
// Author: Janez Puhan

#include "init.h"
#include "uart.h"
#include "keys_driver.h"
#include "uart_driver.h"

extern int main();

char system_memory[500];

// Startup program
void start_up()
{
    // Initializations
    int clock_mhz = 12;
    int div = cclk_4;
    int input = 0;
    int output = 0;
    int value = 0;
    init(clock_mhz, div, input, output, value);

    //UART1 initialization
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

    rtos2init(system_memory, 500, 10000);

    //creat the pipe
    keys_pipe = rtos2pipe_create(10);
    uart_tx_pipe = rtos2pipe_create(10);
    uart_rx_pipe = rtos2pipe_create(10);

    enable_os();

    main();	// Start main program
    while(1);
}
