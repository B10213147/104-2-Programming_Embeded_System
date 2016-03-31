#include "gpio.h"
#include <string.h>

/* Niz za izpisovanje na LCD prikazovalniku */
char lcd_buff[33]="12345678901234567890123456789012";
char *lcd_string=lcd_buff;
/* Gonilnik za LCD prikazovalnik
*   ob vsakem klicu na prikazovalnik izpiše vsebino niza lcd_string */
void lcd_driver();

int frequency = 1;
float duty = 0.1;
const long fvpb = 3000000;

int main()
{
  long nH,nL,next;
  int pin_state = 0;
  int key_state, key_last_state = 0;

  set_pin_value(4, pin_state);
  nL = (1-duty) * fvpb / frequency;
  next = T1TC + nL;

  while(1){
    key_state = get_keys();

    //frequency increase 1Hz
    if(((key_state & T0) != 0) && ((key_last_state & T0) == 0)){
      frequency += 1;
      if(frequency>10)
      {frequency = 10;}
    }
    //frequency decrease 1Hz
    if(((key_state & T1) != 0) && ((key_last_state & T1) == 0)){
      frequency -= 1;
      if(frequency<1)
      {frequency = 1;}
    }
    //duty cycle increase 0.1
    if(((key_state & T2) != 0) && ((key_last_state & T2) == 0)){
      duty += 0.07;
      if(duty>1)
      {duty = 0.99;}
    }
    //duty cycle decrease 0.1
    if(((key_state & T3) != 0) && ((key_last_state & T3) == 0)){
      duty -= 0.07;
      if(duty<0)
      {duty = 0.01;}
    }
    //reflash nH & nL
    nH = duty * fvpb / frequency;
    nL = (1-duty) * fvpb / frequency;

    key_last_state = key_state;

    if((T1TC - next)>0){
      if(pin_state == 0){
        next += nH;
        pin_state = 1;
      }
      else{
        next += nL;
        pin_state = 0;
      }
      set_pin_value(4, pin_state);
    }
  }
	return 0;
}

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

