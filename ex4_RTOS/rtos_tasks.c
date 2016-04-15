#include "rtos_tasks.h"


// Priviliged task
voidfuncptr priv_task = empty_task;

// Scheduler
voidfuncptr sch_tab[] = {keyCheck, prepareOutput,
                        lcd_driver, mainCounter};
int sch_tab_size = sizeof(sch_tab);

char counting = 1;

// empty task (dummy)
void empty_task(){
}

// LCD related stuff
char lcd_buff[33] = "                                ";
char *lcd_string = lcd_buff;
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

int n = 1;
void mainCounter(){
    if(counting == 1){
        n++;
        if(n >= 10){
            time++;
            n = 1;
        }
    }
    else{
        n = 0;
    }
}

void prepareOutput(){
    int digit;
    int hour, min, sec;
    char temp[10];
    char line1[16] = "00:00:00";

    hour = time/3600;
    min = (time-hour*3600)/60;
    sec = time%60;

    digit = num2str(hour, temp);
    modify_Array(digit, temp);
    str2lcd(0,temp);

    digit = num2str(min, temp);
    modify_Array(digit, temp);
    str2lcd(3,temp);

    digit = num2str(sec, temp);
    modify_Array(digit, temp);
    str2lcd(6,temp);

    //str2lcd(2,":");
    //str2lcd(5,":");
}

void modify_Array(int digit, char* temp){
    temp[0] = '0';
    if(digit == 2){
        temp[0] = temp[2];
    }
    temp[2] = '\0';
}

void keyCheck(){
    int key_state, key_last_state = 0;

    key_state = get_keys();

    //increase the hours by 1
    if(((key_state & T0) != 0) && ((key_last_state & T0) == 0)){
        time += 3600;
        if(time>86400){
            time -= 86400;
        }
    }
    //increase the minutes by 1
    if(((key_state & T1) != 0) && ((key_last_state & T1) == 0)){
        time += 60;
        if(time>86400){
            time -= 86400;
        }
    }
    //stop the clock(while the key is pressed)
    if(((key_state & T2) != 0) && ((key_last_state & T2) == 0)){
        counting = 0;
    }
    else{
        counting = 1;
    }
    //reset the time to 00:00:00
    if(((key_state & T3) != 0) && ((key_last_state & T3) == 0)){
        time = 0;
    }

    key_last_state = key_state;
}

void str2lcd(int position, char *str){
    int i;

    for(i=0; position+i >= 0 && position+i < 32; i++){
        if(str[i] == '\0') break;
        lcd_string[position+i] = str[i];
    }
}

int num2str(int x, char* str){
    int digit, remainder;

    digit = 0;

    do{
        digit++;
        remainder = x%10;
        str[digit] = remainder + 48;
        x /= 10;
    }while(x>0 && digit<16);

    return digit;
}
