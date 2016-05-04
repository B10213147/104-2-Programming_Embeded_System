#include "keys_driver.h"
#include "uart_driver.h"
#include "lcd_driver.h"
#include "rtos2.h"

extern char *lcd_string;

int main()
{
    char c;
    while(1){
        keys_driver();
        lcd_string = keys_pipe->buffer;
        lcd_driver();
        rtos2pipe_read(keys_pipe, &c, 1);
    }
    return 0;
}
