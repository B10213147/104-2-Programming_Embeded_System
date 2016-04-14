#include "init.h"
#include "rtos.h"

extern int main();

// Startup program
void start_up()
{
	// Initialisations
  int input = P0_12 | P0_13 | P0_14 | P0_15;
  int output = P0_4 | P0_5  | P0_6  | P0_7;
  int clock_mhz = 12;
  int div = cclk_4;
  int value = 0;

  init(clock_mhz, div, input, output, value);

  sch_on(25000);    //slice = 25ms

  main();	// Start main program
  while(1);
}

