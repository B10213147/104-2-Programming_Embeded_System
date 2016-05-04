// Date:   7.11.2011
// Author: Janez Puhan

#include "init.h"
#include "keys_driver.h"
#include "uart_driver.h"

extern int main();                 

// Startup program
void start_up()
{
	// Initialisations
  int clock_mhz=12;
  int div=cclk_4;
  int input = 0;
  int output = 0;
  int value = 0;
  init(clock_mhz, div, input, output, value);  
  
    //creat the pipe 
    keys_pipe = rtos2pipe_create(10);
    uart_tx_pipe = rtos2pipe_create(10);
    uart_rx_pipe = rtos2pipe_create(10);
  
  
	main();	// Start main program
	while(1);
}
