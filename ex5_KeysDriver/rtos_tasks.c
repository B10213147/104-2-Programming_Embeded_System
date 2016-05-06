#include "rtos_tasks.h"
#include "rtos_pipe.h"
#include "gpio.h"

#define BUFF_LEN 20

void str2lcd(int position, char *str);
void clrscr();

// Priviliged task
voidfuncptr priv_task = generator;

// Scheduler
voidfuncptr sch_tab[] = {keys_driver, keys2lcd, lcd_driver, empty_task};
int sch_tab_size = sizeof(sch_tab);

// empty task (dummy)
void empty_task(){
}

// LCD related stuff
char lcd_buff[33]="                                ";
char *lcd_string=lcd_buff;
void lcd_driver(){
	int i, done=0;
	lcd_write_comm(DDRAM | 0x00);
	for(i = 0; i < 16; i++){
    if(lcd_string[i]=='\0')
      done=1;
    if(done)
      lcd_write_data(' ');
    else
      lcd_write_data(lcd_string[i]);
  }
	lcd_write_comm(DDRAM | 0x40);
	for(; i < 32; i++){
    if(lcd_string[i]=='\0')
      done=1;
    if(done)
      lcd_write_data(' ');
    else
      lcd_write_data(lcd_string[i]);
  }
}

//creat the FIFO
char keys_buff[BUFF_LEN];
struct rtos_pipe keysFifo = {0, 0, BUFF_LEN, keys_buff};
int key_last_state = 0;
void keys_driver(){
	int key_state;
	char c;
	int n;

    key_state = get_keys();

    //store in a buffer as '0'
    if(((key_state & T0) != 0) && ((key_last_state & T0) == 0)){
 		c = '0';
		n = rtos_pipe_write(&keysFifo, &c, 1);
		if(n == 1){

		}
    }
    //store in a buffer as '1'
    if(((key_state & T1) != 0) && ((key_last_state & T1) == 0)){
        c = '1';
		n = rtos_pipe_write(&keysFifo, &c, 1);
		if(n == 1){

		}
    }
    //store in a buffer as '2'
    if(((key_state & T2) != 0) && ((key_last_state & T2) == 0)){
        c = '2';
		n = rtos_pipe_write(&keysFifo, &c, 1);
		if(n == 1){

		}
    }
    //store in a buffer as '3'
    if(((key_state & T3) != 0) && ((key_last_state & T3) == 0)){
        c = '3';
		n = rtos_pipe_write(&keysFifo, &c, 1);
		if(n == 1){

		}
    }

    key_last_state = key_state;
}

void keys2lcd(){
    int i;
    char line[BUFF_LEN];

    for(i=0; (keysFifo.begin+i)%keysFifo.size != keysFifo.end; i++){
      line[i] = keysFifo.data[(keysFifo.begin+i)%keysFifo.size];
    }

    clrscr();

    line[i] = '\0';
    str2lcd(0, line);
}

int buzy = 0;
int cycle = 0;
int delay = 0;
int pulse_num = 0;
void generator(){
	int n;
    char c;

	if(buzy == 0){
		n = rtos_pipe_read(&keysFifo, &c, 1);
        if(n == 0) return;

		switch(c){
        case '0':
			buzy = 1;
			pulse_num = 10;
			break;

		case '1':
			buzy = 1;
			pulse_num = 1;
			break;

		case '2':
			buzy = 1;
			pulse_num = 2;
			break;

		case '3':
			buzy = 1;
			pulse_num = 3;
			break;

		default:
			buzy = 0;
			pulse_num = 0;
            cycle = 0;
		}
	}

	if(buzy == 1){
        if(pulse_num>0){
    		cycle++;
            if(cycle>0 && cycle<=2){
    			set_pin_value(4, 1);
                //cycle++;
    		}
    		else if(cycle>2 && cycle<=5){
    			set_pin_value(4, 0);
                //cycle++;
    		}
    		//finish one pulse
    		if(cycle>=5){
    			cycle = 0;
    			pulse_num--;
    		}
        }
		else{ //end of whole generate
           if(delay<=50){
			    set_pin_value(4, 0);
                delay++;
		    }
		    else{
			    delay = 0;
			    buzy = 0;
                cycle = 0;
		    }
        }
	}//end buzy
}

void str2lcd(int position, char *str){
    int i;

    for(i=0; position+i >= 0 && position+i < 32; i++){
        if(str[i] == '\0') break;
        lcd_string[position+i] = str[i];
    }
}

void clrscr(){
    int i;

    for(i=0; i<32; i++){
        lcd_string[i] = ' ';
    }
    lcd_string[32] = '\0';
}




