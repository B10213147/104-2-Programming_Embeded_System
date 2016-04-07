#include "pulse_train.h"
#include "vic.h"
#include "timer.h"
#include "gpio.h"

int frequency = 1;
float duty = 0.1;
const int fvpb = 3000000;
static int pin=0;

void pulse_train(){
    int nH,nL;

    //reflash nH & nL
    nH = duty * fvpb / frequency;
    nL = (1-duty) * fvpb / frequency;

    if(pin==0){
        pin = 1;
        T1MR0 = nH;
    }
    else{
        pin = 0;
        T1MR0 = nL;
    }
    set_pin_value(4, pin);

    T1IR = mr0_interrupt;   //clear timer1 interrupt request
    VICVectAddr = 0;        //reset VIC priority circuit
}
