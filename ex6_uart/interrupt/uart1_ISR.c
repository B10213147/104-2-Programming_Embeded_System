#include "uart1_ISR.h"
#include "uart.h"
#include "rtos_tasks.h"

void str2lcd(int position, char *str);

char toggle = 0;
int num = 0;
void uart1_ISR(){
    char data;

	char line_status = U1IIR;

	if((line_status&interrupt_pending) == 0){	//At least one interrupt is pending
		switch(line_status&interrupt_id){
		case rx_data_available_id:	// Receive Data Available
			data = U1RBR;

			if(data == 27){
			  toggle = ~toggle;
			}
			else{
				if(toggle){	//send back to PC
					U1THR = data;
				}
				else{	//display on LCD
					lcd_string[num] = data;
					lcd_string[num+1] = '\0';
					num++;
					lcd_driver();
				}
			}

			break;

		case character_time_out_id:	//Character Time-out Indicator
			U1RBR;
			break;

		default:
            break;
		}
	}
	else{	//No interrupt is pending
	}

	if(num == 32){
		str2lcd(0, lcd_string+16);
        str2lcd(16, "                ");
		num = 16;
	}

    VICVectAddr = 0;        //reset VIC priority circuit
}

void str2lcd(int position, char *str){
    int i;

    for(i=0; position+i >= 0 && position+i < 32; i++){
        if(str[i] == '\0') break;
        lcd_string[position+i] = str[i];
    }
}
