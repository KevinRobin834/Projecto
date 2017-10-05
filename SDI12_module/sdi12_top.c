/*
 * sdi12_top.c
 *
 *  Created on: 22 jun. 2017
 *      Author: kero

 */
#include "sdi.h"
#include "sdi12_top.h"
#include "Gpio_function.h"

#include <string.h>
#include <stdlib.h>

/*********stuff***********/
int x=0; // increment of log_checking_error[]
int log_checking_error[50]={9};
/*  8 = no reached, blank
 *  0 = no error when function used at that moment
 *  1 = error at that moment
 */
//////////////////////////////////////////////////////////////////////LSB FIRST


uint8_t current_sensor_address; //[10]={1,0,0,0,0,1,1,0,0,1};//=> start+ 0 in ASCII(30) +parity+Stop


uint8_t Rcv_Time_For_Measurement[30]={0};
uint8_t Rcv_Number_of_Measurement[10]={0};

/****************fixed value to be check********************/

uint8_t Carriage_Return[10]={1,1,0,1,1,0,0,0,1,1};//=CR in ASCII(13)

uint8_t Line_Feed[10]={1,1,0,1,0,0,0,0,0,1};//=LF in ASCII(10)

typedef void (*Sdi12_Rcv_CallBack)(uint8_t RCV_Buffer[9]);


static uint8_t Top_TRM_Buffer[4]={0}; //4 chars is the longest command "aAb!"
static uint8_t Top_RCV_Buffer[25]={0}; // 25 chars is the longest response from commande "aI!"

int v=0;
#define COMMAND_SERVICE_REQUEST  '*'

enum sdi12_top_communication_state_status_e{
    SD12_TOP_WAKE       = -1,
    SD12_TOP_CHAR0      = 0,
    SD12_TOP_CHAR1      = 1,
    SD12_TOP_CHAR2      = 2,
    SD12_TOP_CHAR3      = 3,
    SD12_TOP_END        = 4
};
typedef enum sdi12_top_communication_state_status_e sdi12_top_communication_state_status_t;
static sdi12_top_communication_state_status_t top_current_state;


static error_flag_t Top_error=clear;

static uint8_t top_current_command;

static uint8_t top_char_sent;

int seconds_to_wait = 0;
int number_of_measurements = 0;
int measurment_gotten=0;

//extern GPTimerCC26XX_Params params;
//extern GPTimerCC26XX_Handle hTimerA;
//extern GPTimerCC26XX_Handle hTimerB;

/********************************Random function***********************************/
int Get_Requiered_Time(uint8_t Rcv_Time_For_Measurement[30]){


    uint8_t tab[21]={0};
    int u=1,d=0;
    int time=0;
    String string={0};

    //extract data without start bit, parity bits and stop bits and in MSB first
    while(u<21){
        tab[d]=Rcv_Time_For_Measurement[u];
        d++;
        u++;
        if((u==8)|(u==18)|(u==28)){
            u=u+3;
        }
    }

   //conversion
    memcpy(string, tab, 22);//tab + null terminator
    string[x] = '\0';
    time = atoi(string);//convert string into int
    return time;//return time in s
}
int Convert_Time_Into_Value(int time){

    int value = (time *48000000)/256;
    if (value>65536)
    {
        value=value/65536;
    }
    return value;
}
int count_useful_data_in_frame(uint8_t Top_RCV_Buffer[25]){

    uint8_t Top_RCV_Buffer2[25]={0};
    int n=0;
    do{
        Top_RCV_Buffer2[n]=Top_RCV_Buffer[n];
        n++;
    }while(Top_RCV_Buffer2[n]!=CHAR_LF);
    return n;
}
/**********************General Top Level function***************************/
int get_seconds_to_wait()
{
    return seconds_to_wait;
}

int get_number_of_measurements()
{
    return number_of_measurements;
}


uint8_t Sdi12_top_get_time(uint8_t Top_RCV_Buffer[25])
{
    int cientos=0,decenas=0,unidades=0;

    cientos=Sdi_array_to_uint8_t(&Top_RCV_Buffer[1])*100;
    decenas=Sdi_array_to_uint8_t(&Top_RCV_Buffer[2])*10;
    unidades=Sdi_array_to_uint8_t(&Top_RCV_Buffer[3])*1;

    return cientos+decenas+unidades;

}

uint8_t Sdi12_top_get_measurements(uint8_t Top_RCV_Buffer[25])
{
    return Sdi_array_to_uint8_t(&Top_RCV_Buffer[4]);
}

void Sdi12_top_processM_answer(void)
{
    seconds_to_wait = Sdi12_top_get_time(&Top_RCV_Buffer[25]);
    number_of_measurements = Sdi12_top_get_measurements(&Top_RCV_Buffer[25]);
    if (Top_RCV_Buffer[5] != CHAR_CR) {
        // Error, bad format
        Top_error=format_error;
    }
    if (Top_RCV_Buffer[6] != CHAR_LF) {
        // Error, bad format
        Top_error=format_error;
    }
    // If everything is ok ...
    // Program data time-out and switch to receive mode
    top_current_command = COMMAND_SERVICE_REQUEST;  // Service request
}

void Sdi12_top_processSR(void)
{
    if (Top_RCV_Buffer[1] != CHAR_CR) {
        // Error, bad format
        Top_error=format_error;
    }
    if (Top_RCV_Buffer[2] != CHAR_LF) {
        // Error, bad format
        Top_error=format_error;
    }
    // Abort data time-out, send data command
    //SD12_give_me_measurements();
}
void Sdi12_top_processA_answer()
{
    if (Top_RCV_Buffer[1] != CHAR_CR) {
          // Error, bad format
        Top_error=format_error;
    }
    if (Top_RCV_Buffer[2] != CHAR_LF) {
          // Error, bad format
        Top_error=format_error;

    }
}

void Sdi12_top_processExisting_sensor_answer(){

    if (Top_RCV_Buffer[1] != CHAR_CR) {
             // Error, bad format
           Top_error=format_error;
       }
       if (Top_RCV_Buffer[2] != CHAR_LF) {
             // Error, bad format
           Top_error=format_error;
       }
}
void  Sdi12_top_processAny_sensor_answer(){

    if (Top_RCV_Buffer[1] != CHAR_CR) {
                // Error, bad format
              Top_error=format_error;
          }
        if (Top_RCV_Buffer[2] != CHAR_LF) {
                // Error, bad format
              Top_error=format_error;
          }
}
void  Sdi12_top_processD_answer(){

    int v =count_useful_data_in_frame(Top_RCV_Buffer);
    int z=2;



    for(z=1;z<v-2;z++)
    {
        if (Top_RCV_Buffer[z] != '+' || Top_RCV_Buffer[z] != '-') {
               // Error, bad format
               Top_error=format_error;
        }


        if(Top_RCV_Buffer[z] !='0'||Top_RCV_Buffer[z] !='1'||Top_RCV_Buffer[z] !='2'||Top_RCV_Buffer[z] !='3'
           ||Top_RCV_Buffer[z] !='4'||Top_RCV_Buffer[z] !='5'||Top_RCV_Buffer[z] !='6'||Top_RCV_Buffer[z] !='7'
           ||Top_RCV_Buffer[z] !='8'||Top_RCV_Buffer[z] !='9'||Top_RCV_Buffer[z] !='.')
        {
            Top_error=format_error;
        }

        if(Top_RCV_Buffer[z] == '+' || Top_RCV_Buffer[z] == '-'){
            measurment_gotten++;
        }
    }

    if (Top_RCV_Buffer[v-1] != CHAR_CR) {
        // Error, bad format
       Top_error=format_error;
       }
    if (Top_RCV_Buffer[v] != CHAR_LF) {
        // Error, bad format
       Top_error=format_error;
    }
    /*
    if (measurment_gotten<number_of_measurements) {
        uint8_t command[4]={'0','D','1','!'};
        Sdi12_Send_Command(&command);
    }*/
}
/*
 * If a sensor detects a break after it receives an M command, but before it issues a service
request, it must abort its measurement procedure. The sensor address, followed by <CR><LF>,
(or followed by <CRC><CR><LF> if a CRC was requested) should be returned in response to
subsequent D commands. This requirement provides a way for a data recorder to abort a
measurement so it can communicate with another sensor, or this sensor, on the SDI-12 bus.
 */
void Sdi12_abort_measurement(void){
    Sdi_Wake_Sensor();
}

error_flag_t get_top_error(){
    return Top_error;
}

void Sdi12_Tx_Callback(void){

    if (top_current_state == SD12_TOP_WAKE) {
        Sdi12_App_CallbackFromISR_OperationEnd();
        return;
    }

    if ( (top_current_state >= SD12_TOP_END) || (top_char_sent == '!') ) {
        // Change to receive mode to receive sensor answer ...
        Sdi_Receive_Character();//do all requiered configuration
    }
    else {
        top_char_sent = Top_TRM_Buffer[top_current_state++];
        Sdi_Transmit_Character(top_char_sent);
    }
  /*
    if (top_current_state < SD12_TOP_END) {
        if (top_current_state > SD12_TOP_CHAR0) {
            if (Top_TRM_Buffer[top_current_state-1] == '!') {
                //Sdi12_AppTx_Callback();
                return;
            }
        }
        Sdi_Transmit_Character(Top_TRM_Buffer[top_current_state++]);
    }
    else {

        // Change to receive mode to receive sensor answer ...
        Sdi_Receive_Character();//do all requiered configuration
    }
    */
}

void Sdi12_Rx_Callback(uint8_t recv_data, uint8_t sdi12_error){
    if (recv_data > sizeof(uint8_t))
    {
        Top_error = answer_too_long;
    }
    else {
        Top_RCV_Buffer[v++] = recv_data;
        if (sdi12_error!=clear )
        {
            Top_error = (error_flag_t)sdi12_error;
        }
    }
    if ((recv_data != CHAR_LF))
    {
        Sdi_Receive_Character();//do all requiered configuration for next reception
    }

    if ((recv_data == CHAR_LF) && (sdi12_error != 0))
    {
        // Process sensor answer, stored into Top_RCV_Buffer
        if (Top_RCV_Buffer[0] != current_sensor_address) {
            if(top_current_command!='A')
            {
                // Error, answer from wrong sensor
                Top_error = addr_error;
            }
            if(top_current_command=='A')
            {
                            // Error, answer from wrong sensor
                            Top_error = change_addr_failed;
           }

        }
       // int u=0;

         // for(u=0;u<25;u++){

        //  Display_printf(displayHandle, 1, 0, "status = %d", Top_RCV_Buffer[u]);



        //  }
        switch (top_current_command)
        {

        case 'M':
            Sdi12_top_processM_answer();
            break;
        case COMMAND_SERVICE_REQUEST:
            Sdi12_top_processSR();
            break;
        case 'A':
            Sdi12_top_processA_answer();
            break;
        case '!':
            if (current_sensor_address=='?'){
               Sdi12_top_processAny_sensor_answer();
            }
            else
            {
            Sdi12_top_processExisting_sensor_answer();
            }
            break;
        case 'D' :
            Sdi12_top_processD_answer();
        }
        //Sdi12_App_CallbackFromISR_OperationEnd();//when i hqve receive full response of sensor, release sem and do other command
    }
    v=0;
    Sdi12_App_CallbackFromISR_OperationEnd();
}

void Sdi12_Send_Break(){

    Sdi12_App_Callback_OperationBegin();
    top_current_state = SD12_TOP_WAKE;
    Sdi_Wake_Sensor();
    //return;
}

void Sdi12_wait(GPTimerCC26XX_Handle hTimerB){
    int z=0;
    int pacha = Convert_Time_Into_Value(get_seconds_to_wait());// get number of second to wait
    if (pacha<65536){
         Set_TimerA(256,pacha,hTimerB);
        Launch_TimerB(hTimerB);

    }
    else{
        pacha=pacha/65536;
        for(z=0;z<pacha;z++){
           Set_TimerA(256,65536,hTimerB);//349ms
            Launch_TimerB(hTimerB);
        }

    }


}
void Sdi12_Send_Command(uint8_t Sdi12_command[4]){

    Sdi12_App_Callback_OperationBegin();
    top_char_sent = '\0';
    top_current_state = SD12_TOP_CHAR0;
    current_sensor_address = Top_TRM_Buffer[0] = Sdi12_command[0];
    top_current_command =    Top_TRM_Buffer[1] = Sdi12_command[1];
    Top_TRM_Buffer[2] = Sdi12_command[2];
    Top_TRM_Buffer[3] = Sdi12_command[3];


    Sdi12_Tx_Callback();// if not break to that

}

void Sdi12_Send_Command_2_char(uint8_t Sdi12_command[2]){

    Sdi12_App_Callback_OperationBegin();
    top_char_sent = '\0';
    top_current_state = SD12_TOP_CHAR0;
    current_sensor_address = Top_TRM_Buffer[0] = Sdi12_command[0];
    top_current_command =    Top_TRM_Buffer[1] = Sdi12_command[1];


    top_char_sent = Top_TRM_Buffer[top_current_state];
    Sdi_Transmit_Character(top_char_sent);


    Sdi12_Tx_Callback();// if not break to that

}
void Sdi12_Send_Command_3_char(uint8_t Sdi12_command[3]){

    Sdi12_App_Callback_OperationBegin();
    top_char_sent = '\0';
    top_current_state = SD12_TOP_CHAR0;
    current_sensor_address = Top_TRM_Buffer[0] = Sdi12_command[0];
    top_current_command =    Top_TRM_Buffer[1] = Sdi12_command[1];
    Top_TRM_Buffer[2] = Sdi12_command[2];

    top_char_sent = Top_TRM_Buffer[top_current_state];
    Sdi_Transmit_Character(top_char_sent);



}
void Sdi12_Send_Command_4_char(uint8_t Sdi12_command[4]){

    Sdi12_App_Callback_OperationBegin();
    top_char_sent = '\0';
    top_current_state = SD12_TOP_CHAR0;
    current_sensor_address = Top_TRM_Buffer[0] = Sdi12_command[0];
    top_current_command =    Top_TRM_Buffer[1] = Sdi12_command[1];
    Top_TRM_Buffer[2] = Sdi12_command[2];
    Top_TRM_Buffer[3] = Sdi12_command[3];

    top_char_sent = Top_TRM_Buffer[top_current_state];
    Sdi_Transmit_Character(top_char_sent);


}

/*

void Sdi12_Start_Measurement(uint8_t sensor_address, uint8_t send_break){

    int waiting_time=0;
    current_sensor_address = sensor_address;

    Top_TRM_Buffer[0] = current_sensor_address;
    Top_TRM_Buffer[1] = top_current_command = 'M';
    Top_TRM_Buffer[2] = '!';
    Top_TRM_Buffer[3] = '\0';

    if (send_break)
    {
        top_current_state = SD12_TOP_WAKE;
        Sdi_Wake_Sensor();
        return;
    }

    Sdi12_Tx_Callback;// if not break to that
*/
    /*
    Sdi_Transmit_Character(current_sensor_address);
    Sdi_Transmit_Character('M');
    Sdi_Transmit_Character('!');
    */

/*
    if(memcmp(Sdi_Receive_Character(),current_sensor_address,sizeof(current_sensor_address))==0)//48 == "0"
    {
       //now receiving time for meaurement 3 characher to be receved
        strcat((char *)Rcv_Time_For_Measurement, (char *)Sdi_Receive_Character());
        strcat((char *)Rcv_Time_For_Measurement, (char *)Sdi_Receive_Character());
        strcat((char *)Rcv_Time_For_Measurement, (char *)Sdi_Receive_Character());

        //now receiving number of measurement to be send next
        strcat((char *)Rcv_Number_of_Measurement, (char *)Sdi_Receive_Character());

        if(memcmp(Sdi_Receive_Character(),Carriage_Return,sizeof(Carriage_Return))==0)
        {
            if(memcmp(Sdi_Receive_Character(),Line_Feed,sizeof(Line_Feed))==0)
            {
                log_checking_error[x]=0;// correct response detected
                x++;
            }
        }
    }
    else
    {
        log_checking_error[x]=1; // wrong response detected
        x++;
    }
*/

    // when transmission command done wait the amount store in Rcv_Time_For_Measurement
    /*
     * If ttt is not equal to zero (000), the data recorder must wait for the specified time to elapse.
The ttt time period begins upon completion of the transmission of the line feed character. Then it
wakes the sensor with a break and issues the D0 command. If, however, the sensor has the
measurement ready before ttt seconds elapse, it will send a service request to the recorder. This
tells the recorder to stop marking time and issue the D0 command.
*/
/*
    waiting_time=Convert_Time_Into_Value(Get_Requiered_Time(Rcv_Time_For_Measurement));// get number of second to wait
    Sdi_Set_State_Data_Line(RECEIVE);//activate receive mode in case sensor has measurement before elapse time
    int t=0;
    for(t=0; t<waiting_time;t++)
    {

        Set_TimerB(256,65536);//=max value -> 349ms
        Launch_TimerB();

    }




}
*//*
void SD12_give_me_measurements()
{
    Sdi_Wake_Sensor();
    Sdi_Transmit_Character(current_sensor_address);
    Sdi_Transmit_Character("D");
    Sdi_Transmit_Character("0");
    Sdi_Transmit_Character("!");

   //  if(memcmp(Sdi_Receive_Character(),current_sensor_address,sizeof(current_sensor_address))==0)//48 == "0"
    {

         //value treatememnt





        //if(memcmp(Sdi_Receive_Character(),Carriage_Return,sizeof(Carriage_Return))==0)
        {
       //     if(memcmp(Sdi_Receive_Character(),Line_Feed,sizeof(Line_Feed))==0)
            {
                log_checking_error[x]=0;// correct response detected
                x++;

            }
        }
    }

}*/

