#include "keys_driver.h"
#include "uart_driver.h"
#include "lcd_driver.h"
#include "rtos2.h"

void str2lcd(int position, char *str);

extern char *lcd_string;

int main()
{
    char str[10];
    int n, num=0;
/*
    rtos2task_create(keys_driver, NULL,	1, 10);
    rtos2task_create(lcd_driver, NULL,	2, 2);
    rtos2task_create(uart_driver, NULL,	3, 1);
*/
    while(1){
        //test uart_driver
        uart_driver();

        n = rtos2pipe_read(uart_rx_pipe, str, 1);
        if(n == 1){
            lcd_string[num] = str[0];
            lcd_string[num+1] = '\0';
            num++;

            str[n] = '\n';
            n = rtos2pipe_write(uart_tx_pipe, str, 2);
        }

        lcd_driver();

        if(num == 32){
            str2lcd(0, lcd_string+16);
            str2lcd(16, "                ");
            //str2lcd(16, "");
            num = 16;
        }
    }

    return 0;
}

void str2lcd(int position, char *str){
    int i;

    for(i=0; position+i >= 0 && position+i < 32; i++){
        if(str[i] == '\0') break;
        lcd_string[position+i] = str[i];
    }
}
