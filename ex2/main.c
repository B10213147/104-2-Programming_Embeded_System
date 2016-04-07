#include "gpio.h"
#include <string.h>

/* Niz za izpisovanje na LCD prikazovalniku */
char lcd_buff[33]="12345678901234567890123456789012";
char *lcd_string=lcd_buff;
/* Gonilnik za LCD prikazovalnik
*   ob vsakem klicu na prikazovalnik izpiše vsebino niza lcd_string */
void lcd_driver();
void clrscr();
void str2lcd(int position, char *str);
int num2str(int x, char* str);
void lcd_display(int f, int duty);

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

    lcd_display(frequency, (int)(duty*100.0));
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

void clrscr(){
  int i;

  for(i=0; i<32; i++){
    lcd_string[i] = ' ';
  }
  lcd_string[32] = '\0';
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

  for(digit=1; x>0 && digit<16; digit++){
    remainder = x%10;
    str[digit] = remainder + 48;
    x /= 10;
  }

  return digit-1;
}

void lcd_display(int f, int duty){
  int digit, i;
  char temp[16];
  char line1[16] = "f=Hz";
  char line2[16] = "duty=%";

  digit = num2str(f, temp);

  line1[3+digit] = line1[3];
  line1[2+digit] = line1[2];
  for(i=0; digit-i > 0; i++){
    line1[2+i] = temp[digit-i];
  }

  digit = num2str(duty, temp);

  line2[5+digit] = line2[5];
  for(i=0; digit-i > 0; i++){
    line2[5+i] = temp[digit-i];
  }

  clrscr();
  str2lcd(0,line1);
  str2lcd(16,line2);
  lcd_driver();
}

