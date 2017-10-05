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

#include "gps_top.h"


/*
 *  HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEN_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEN_DIS;//timer B enable

 * */


/* Board Header file */
#include "Board.h"



void Uart_begin(UART_Handle uart, UART_Params uartParams){



       // Initialize the UART driver.
       UART_init();
       // Create a UART with data processing on.
           UART_Params_init(&uartParams);
           uartParams.readMode=UART_MODE_CALLBACK;//no bocking
           uartParams.writeMode=UART_MODE_CALLBACK;//no blocking
           uartParams.readCallback=Callback_Uart_read;
           uartParams.writeCallback=Callback_Uart_write;
           uartParams.writeDataMode = UART_DATA_TEXT; //data processing on.
           uartParams.readDataMode = UART_DATA_TEXT;  //data processing on.
           uartParams.readReturnMode = UART_RETURN_NEWLINE; ///*! Unblock/callback when newline character is received. */
           uartParams.readEcho = UART_ECHO_OFF;// echo off
           uartParams.baudRate = 9600;//dont know baud rate
           uartParams.dataLength = UART_LEN_8;//8 bits data
           uartParams.stopBits = UART_STOP_ONE;// 1 stop bits i guess
           uartParams.parityType = UART_PAR_NONE;//dont know for now

           // Open an instance of the UART drivers
           uart = UART_open(Board_UART0, &uartParams);

}

void Uart_end(UART_Handle uart){

    UART_close(uart);

}

