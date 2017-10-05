/*
 * sdi12_top.h
 *
 *  Created on: 22 jun. 2017
 *      Author: kero
 */

#include "sdi.h"


#ifndef SDI12_TOP_H_
#define SDI12_TOP_H_

extern void Sdi12_init_sem(void);
extern void Sdi12_App_Callback_OperationBegin(void);
extern void Sdi12_App_CallbackFromISR_OperationEnd(void);

void Sdi12_Check_End_Carriage(uint8_t *Sdi_Receive_Character());

void Sdi12_Check_Line_Feed(uint8_t RCV_Buffer[9]);

void Sdi12_Check_Adress_Sensors(uint8_t RCV_Buffer[9],uint8_t address_sensor );




void Sdi12_Top_check_Sensor_Exist();

int get_requiered_Time(uint8_t Rcv_Time_For_Measurement[30]);

void Sdi12_Rx_Callback(uint8_t recv_data, uint8_t sdi12_error);

void Sdi12_Tx_Callback(void);

void Sdi12_Send_Break();

void Sdi12_Send_Command(uint8_t Sdi12_command[4]);

void Sdi12_Send_Command_2_char(uint8_t Sdi12_command[2]);

void Sdi12_Send_Command_3_char(uint8_t Sdi12_command[3]);

void Sdi12_Send_Command_4_char(uint8_t Sdi12_command[4]);

#endif /* SDI12_TOP_H_ */
