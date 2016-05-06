#include "init.h"
#include "timer.h"

extern int main();

void start_up()
{
    //cpu initialization
    int input = P0_12 | P0_13 | P0_14 | P0_15;
    int output = P0_4;
    int clock_mhz = 12;
    int vpbdiv = cclk_4;
    int value = 0;
    init(clock_mhz, vpbdiv, input, output, value);

    //timer initialization
    int prescale = 0;
    int match[4] = {0,0,0,0};
    int control = 0;
    int count = timer;
    timer1_init(prescale, match, control, count);
    T1TCR = counter_enable;

    main();	// Start main program
    while(1);
}
