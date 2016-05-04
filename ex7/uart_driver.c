#include "uart_driver.h"
#include "uart.h"

int num = 0;
void uart_driver(){
    char tx_data, rx_data;
	char line_status = U1LSR;

	if((line_status&rdr) == 0){	//U1RBR is empty

	}
	else{	//U1RBR contains valid data
		if((line_status&rxfe) == 0){	//U1RBR contains no UART1 RX errors
			data = U1RBR;
			if(data == 27){
              toggle = !toggle;
            }
            else{
				if(toggle){	//send back to PC
					U1THR = data;
				}
				else{	//display on LCD
					lcd_string[i] = data;
					lcd_string[i+1] = '\0';
					i++;
					lcd_driver();
				}
            }
		}
		else{	//UART1 RBR contains at least one UART1 RX error
			U1RBR;
		}
	}


}
