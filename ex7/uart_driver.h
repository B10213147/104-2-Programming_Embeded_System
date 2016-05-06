#ifndef UART_DRIVER_H
#define UART_DRIVER_H

void uart_driver();

struct rtos2pipe *uart_rx_pipe;
struct rtos2pipe *uart_tx_pipe;

#endif
