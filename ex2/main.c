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
  int pin_status = 0;

  nH = duty * fvpb / frequency;
  nL = (1-duty) * fvpb / frequency;
  set_pin_value(4, pin_status);
  next = T1TC + nL;

  while(1){
    if((get_pin_value(12)==0)||(get_pin_value(13)==0)||
       (get_pin_value(14)==0)||(get_pin_value(15)==0)){
      //frequency increase 1Hz
      if(get_pin_value(12)==0){
        frequency += 1;
        if(frequency>10)
        {frequency=10;}
      }
      //frequency decrease 1Hz
      if(get_pin_value(13)==0){
        frequency -= 1;
        if(frequency<1)
        {frequency=1;}
      }
      //duty cycle increase 0.1
      if(get_pin_value(14)==0){
        duty += 0.1;
        if(duty>1)
        {duty = 1;}
      }
      //duty cycle decrease 0.1
      if(get_pin_value(15)==0){
        duty -= 0.1;
        if(duty<0)
        {duty = 0;}
      }
      //wait all the switches release
      while((get_pin_value(12)!=1)||(get_pin_value(13)!=1)||
            (get_pin_value(14)!=1)||(get_pin_value(15)!=1));
      //reflash nH & nL
      nH = duty * fvpb / frequency;
      nL = (1-duty) * fvpb / frequency;
    }

    if((T1TC - next)>0){
      if(pin_status == 0){
        next += nH;
        pin_status = 1;
      }
      else{
        next += nL;
        pin_status = 0;
      }
      set_pin_value(4, pin_status);
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

