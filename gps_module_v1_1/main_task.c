/*
 * main_task.c
 *
 *  Created on: 12 sept. 2017
 *      Author: kero
 */

/*
 * maintask.c
 *
 *  Created on: 17 jul. 2017
 *      Author: kero
 */
#include "uart.h"
#include "gps_top.h"
#include "gps_low.h"
#include "main_task.h"
#include <string.h>
#include <stdlib.h>
#include <ti/display/Display.h>//drivers display windows terminal

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

Semaphore_Struct Gps_semStruct;
Semaphore_Handle Gps_semHandle;


void Gps_init_sem(void){
    Semaphore_Params Gps_semParams;

    /* Construct a Semaphore object to be use as a resource lock, inital count 1 */
    Semaphore_Params_init(&Gps_semParams);
    Semaphore_construct(&Gps_semStruct, 1, &Gps_semParams);

    /* Obtain instance handle */
    Gps_semHandle = Semaphore_handle(&Gps_semStruct);
}

void Gps_App_Callback_OperationBegin(void)
{/*
    if (Semaphore_getCount(semHandle) == 0)
    {
         System_printf("Sem already blocked \n");
    }
*/
     /* Get access to resource */
    Semaphore_pend(Gps_semHandle, BIOS_WAIT_FOREVER);
}

void Gps_App_CallbackFromISR_OperationEnd(void)
{
/*
    if (Semaphore_getCount(semHandle) == 0)
      {
           System_printf("Sem successfully unblocked \n");
      }
      */
    /* Unlock resource,give it back */

    Semaphore_post(Gps_semHandle);

}

/*
 *  ======== mainThread ========
 */

void *mainThread(void *arg0)
{
    UART_Handle uart;
    UART_Params uartParams;
    uint8_t tx_data[254]="$PMTK103*30<CR><LF>";
    //uint8_t rx_data[254]={0};
while(1){

  send_NMEA_command_(uart,uartParams,tx_data,13);

  //  receive_NMEA_response(uart,uartParams,rx_data,size_t total_of_character_to_receive);

}

}

