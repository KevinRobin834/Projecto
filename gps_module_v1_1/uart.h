/*
 * uart.c
 *
 *  Created on: 18 jul. 2017
 *      Author: kero
 */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_uart.h"
#include "hw_prcm.h"
#include "hw_ioc.h"


/* Board Header file */
#include "Board.h"

void Uart_begin( UART_Handle uart, UART_Params uartParams);


void Callback_Uart_read(UART_Handle uart, void *buf, size_t count);


void Callback_Uart_write(UART_Handle uart, void *buf, size_t count);


void Uart_end(UART_Handle uart);

int_fast32_t  Uart_read_character(UART_Handle uart,void *buffer,size_t size,size_t count);


int_fast32_t  Uart_write_character(UART_Handle uart,void *buffer,size_t size,size_t count);






