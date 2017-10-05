/*
 * gps_top.h
 *
 *  Created on: 12 sept. 2017
 *      Author: kero
 */

#ifndef GPS_TOP_H_
#define GPS_TOP_H_


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


//size_t size=sizeof(buffer);

//void *buffer=

void Callback_Uart_read(UART_Handle uart, void *buffer, size_t total_of_character_to_receive);

void Callback_Uart_write(UART_Handle uart, void *buffer, size_t total_of_character_to_transmit);

void send_NMEA_command_(UART_Handle uart, UART_Params uartParams, uint8_t tx_data[254],size_t total_of_character_to_transmit);


void receive_NMEA_response(UART_Handle uart, UART_Params uartParams, uint8_t rx_data[254],size_t total_of_character_to_receive);








#endif /* GPS_TOP_H_ */
