#include "init.h"
#include "timer.h"

extern int main();

void start_up()
{
  
  int input = P0_12 | P0_13 | P0_14 | P0_15;          // glej gpio.h
  int output = P0_4; // glej gpio.h
  int clock_mhz = 12;                     // glej definicijo funk. init v init.c
  int vpbdiv = cclk_4;                    // glej vpb.h  
  int value = 0;                          // glej definicijo funk. init v init.c
  
  init(clock_mhz, vpbdiv, input, output, value);
  
  int prescale = 0;
  int match[4] = {0,0,0,0};
  int control = 0;
  int count = timer;
  
  timer1_init(prescale, match, control, count);
  T1TCR = counter_enable;
  
	main();	// Start main program
	while(1);
}
