/*
 * gps_low.c
 *
 *  Created on: 12 sept. 2017
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

#include "gps_low.h"
#include "uart.h"
#include "gps_top.h"

/* Board Header file */
#include "Board.h"



int_fast32_t  Uart_read_character(UART_Handle uart,void *buffer,size_t size,size_t count){

    int_fast32_t not_trash;
    not_trash = UART_read(uart,buffer,size);
    Callback_Uart_read(uart,buffer,count);

    return not_trash;



}
int_fast32_t  Uart_write_character(UART_Handle uart,void *buffer,size_t size, size_t count){

    int_fast32_t trash;
    trash = UART_write(uart,buffer,size);
    Callback_Uart_write(uart,buffer,count);
    return trash;


}





/*





void DefaultUART_ISR(void){

    if((HWREG(UART0_BASE + UART_O_MIS) && UART_MIS_TXMIS)==1)//if transmission  interrupt flag =1
    {

         HWREG(UART0_BASE + UART_O_CTL) &= ~UART_CTL_TXE_M;//mask
         HWREG(UART0_BASE + UART_O_CTL) |= UART_CTL_TXE_EN; // to 1 -> enable

        //uart_clear_transmit_interrupt();

        transmit_character_callback();

    }

    if((HWREG(UART0_BASE + UART_O_MIS) && UART_MIS_TXMIS)==1)//if reception interrupt flag =1
    {
        //uart_clear_receive_interrupt();
        receive_character_callback();
    }
    else {
        //super error
        error_communication_callback();
    }


}

*/
