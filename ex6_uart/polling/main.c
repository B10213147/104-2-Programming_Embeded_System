#include "uart.h"
#include "rtos_tasks.h"

void str2lcd(int position, char *str);

char line_status;
char toggle = 0;
int main(){
	char data;
	int i = 0;
while(1){
	line_status = U1LSR;
	if((line_status&rdr) == 0){	//U1RBR is empty

	}
	else{	//U1RBR contains valid data
		if((line_status&rxfe) == 0){	//U1RBR contains no UART1 RX errors
			data = U1RBR;
			if(data == 27){
              toggle = ~toggle;
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

	if(i == 32){
		str2lcd(0, lcd_string+16);
        str2lcd(16, "                ");
		i=16;
	}
}
	return 0;
}

void str2lcd(int position, char *str){
    int i;

    for(i=0; position+i >= 0 && position+i < 32; i++){
        if(str[i] == '\0') break;
        lcd_string[position+i] = str[i];
    }
}



