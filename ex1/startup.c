#include "gpio.h"

extern int main();
void set_pin_direction(int input, int output);
void set_pin_value(int input, int value);
int get_pin_value(int input);

int startup(){
    /*
        Put initialization code here
    */
    int i;
    int input_pin = P0_4 | P0_5 | P0_6 | P0_7;
    int output_pin = P0_12 | P0_13 | P0_14 | P0_15;
    //pin[4:7] are LEDs
    set_pin_direction(input_pin,1);
    //pin[12:15] are switches
    set_pin_direction(output_pin,0);

    IO0CLR |= 0x000000F0;

    while(1){
    for(i=12; i<=15; i++){
        if(get_pin_value(i)==0){
            set_pin_value(i-8,1);
        }
        else{
            set_pin_value(i-8,0);
        }
    }
    }

    main();
    while(1);
    return 0;
}

void set_pin_direction(int input, int output){
    /*
        output == 1 means pin output
        output == 0 means pin input
    */
    int pinsel_mask;
    int pin_mask;

    pinsel_mask = ~(0x3) << input*2;
    PINSEL0 &= pinsel_mask;

    pin_mask =1<<input;
    if(output == 1){
        IO0DIR |= pin_mask;
    }
    else{
        IO0DIR &= ~pin_mask;
    }
}

void set_pin_value(int input, int value){
    int pin_mask;

    pin_mask =1<<input;

    if(value == 1){
        IO0SET |= pin_mask;
    }
    else{
        IO0CLR |= pin_mask;
    }
}

int get_pin_value(int input){
    int pin_mask;

    pin_mask =1<<input;

    return IO0PIN & pin_mask;
}
