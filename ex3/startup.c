#include "init.h"
#include "timer.h"

extern int main();

void start_up()
{
    int input = P0_12 | P0_13 | P0_14 | P0_15;    // glej gpio.h
    int output = P0_4;    // glej gpio.h
    int clock_mhz = 12;   // glej definicijo funk. init v init.c
    int vpbdiv = cclk_4;  // glej vpb.h
    int value = 0;        // glej definicijo funk. init v init.c

    init(clock_mhz, vpbdiv, input, output, value);

    int prescale = 0;
    int match[4] = {10,0,0,0};
    int control = mr0i | mr0r;
    int count = timer;

    timer1_init(prescale, match, control, count);
    T1TCR = counter_enable;

    int fiq = 0;
    int irq = timer1;
    voidfuncptr function[16] = {pulse_train(),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int interrupt[16] = {timer1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    vic_init(fiq, irq, function, interrupt,0);

	main();	// Start main program
	while(1);
}
