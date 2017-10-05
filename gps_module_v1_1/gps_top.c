/*
 * gps_top.c
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

uint8_t total_of_character_to_transmit=0;
uint8_t total_of_character_to_receive=0;

uint8_t number_of_character_transmitted=0;
uint8_t number_of_character_received=0;

uint8_t rx_data[254]={0};
uint8_t tx_data[254]={0};

//size_t size=sizeof(buffer);

//void *buffer=

void Callback_Uart_read(UART_Handle uart, void *buffer, size_t total_of_character_to_receive){

    /*if enters, asume that full character is in buffer*/
    rx_data[number_of_character_received]=buffer;

    number_of_character_received++;
    total_of_character_to_receive--;

    if(total_of_character_to_receive!=0){
        Uart_read_character(uart,buffer,sizeof(buffer),total_of_character_to_receive);
    }
    else{

        number_of_character_received=0;

        Uart_end(uart);
        Data_manager();
    }
}
void Callback_Uart_write(UART_Handle uart, void *buffer, size_t total_of_character_to_transmit){

    /*if enters, asume that full character was sent from buffer*/

    total_of_character_to_transmit--;
    number_of_character_transmitted++;
    tx_data[number_of_character_transmitted]=buffer;


    if(total_of_character_to_transmit!=0){
        Uart_write_character(uart,buffer,sizeof(buffer),total_of_character_to_transmit);
    }
    else{
        number_of_character_transmitted=0;

        Uart_end(uart);
    }
}


void send_NMEA_command_(UART_Handle uart, UART_Params uartParams, uint8_t tx_data[254],size_t total_of_character_to_transmit){

    //number_of_character_to_transmitted=0;
    //total_of_character_to_transmit = strlen(data);
    Uart_begin(uart,uartParams);
    Uart_write_character(uart,(uint8_t*) tx_data[0],sizeof(uint8_t),total_of_character_to_transmit);


    /*

    int i=0;
   //begin
    //preamble
    uart_transmit('$');
    //talker id
    uart_transmit('P');
    uart_transmit('M');
    uart_transmit('T');
    uart_transmit('K');
    //packet type for gps module PA6HMTK339 seem to be 103
    uart_transmit('1');
    uart_transmit('0');
    uart_transmit('3');
    //data
    uart_transmit('$');
    //end data
    uart_transmit('*');
    //cheksum
    uart_transmit('$');
    uart_transmit('$');
    //cr and Lf
    uart_transmit('CR');//cr
    uart_transmit('LF');//lf
*/

}

void receive_NMEA_response(UART_Handle uart, UART_Params uartParams, uint8_t rx_data[254],size_t total_of_character_to_receive){

    Uart_begin(uart,uartParams);
   // rx_data[254]={0};
    Uart_read_character(uart,(uint8_t*)rx_data[0],sizeof(uint8_t),total_of_character_to_receive);

}

void Data_manager(){

}
