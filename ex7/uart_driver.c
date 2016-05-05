#include "uart_driver.h"
#include "uart.h"

void uart_driver(){
    int n;
    char tx_data, rx_data;
	char line_status = U1LSR;

	if((line_status&rdr) == 0){	//U1RBR is empty
	}
	else{	//U1RBR contains valid data
		if((line_status&rxfe) == 0){	//U1RBR contains no UART1 RX errors
			rx_data = U1RBR;
			rtos2pipe_write(uart_rx_pipe, &rx_data, 1);
		}
		else{	//UART1 RBR contains at least one UART1 RX error
			U1RBR;
		}
	}
	//U1THR and/or the U1TSR contains valid data
	if((line_status&temt) == 0){
	}
	else{   //U1THR and the U1TSR are empty
        n = rtos2pipe_read(uart_tx_pipe, &tx_data, 1);
        if(n == 1){
            U1THR = tx_data;
        }
	}
}

