#include "keys_driver.h"
#include "rtos2.h"
#include "gpio.h"

int key_last_state = 0;
void keys_driver(){
	int key_state;
	char c;
	int n;

    key_state = get_keys();

    //store in a buffer as '0'
    if(((key_state & T0) != 0) && ((key_last_state & T0) == 0)){
 		c = '0';
		n = rtos2pipe_write(keys_pipe, &c, 1);
		if(n == 1){

		}
    }
    //store in a buffer as '1'
    if(((key_state & T1) != 0) && ((key_last_state & T1) == 0)){
        c = '1';
		n = rtos2pipe_write(keys_pipe, &c, 1);
		if(n == 1){

		}
    }
    //store in a buffer as '2'
    if(((key_state & T2) != 0) && ((key_last_state & T2) == 0)){
        c = '2';
		n = rtos2pipe_write(keys_pipe, &c, 1);
		if(n == 1){

		}
    }
    //store in a buffer as '3'
    if(((key_state & T3) != 0) && ((key_last_state & T3) == 0)){
        c = '3';
		n = rtos2pipe_write(keys_pipe, &c, 1);
		if(n == 1){

		}
    }

    key_last_state = key_state;
}
