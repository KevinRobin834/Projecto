/*
 * maintask.c
 *
 *  Created on: 17 jul. 2017
 *      Author: kero
 */
#include "sdi.h"
#include "sdi12_top.h"
#include "Gpio_function.h"

#include <string.h>
#include <stdlib.h>
#include <ti/display/Display.h>//drivers display windows terminal

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

Semaphore_Struct semStruct;
Semaphore_Handle semHandle;


void Sdi12_init_sem(void){
    Semaphore_Params semParams;

    /* Construct a Semaphore object to be use as a resource lock, inital count 1 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semStruct, 1, &semParams);

    /* Obtain instance handle */
    semHandle = Semaphore_handle(&semStruct);
}

void Sdi12_App_Callback_OperationBegin(void)
{/*
    if (Semaphore_getCount(semHandle) == 0)
    {
         System_printf("Sem already blocked \n");
    }
*/
     /* Get access to resource */
    Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);
}

void Sdi12_App_CallbackFromISR_OperationEnd(void)
{
/*
    if (Semaphore_getCount(semHandle) == 0)
      {
           System_printf("Sem successfully unblocked \n");
      }
      */
    /* Unlock resource,give it back */

    Semaphore_post(semHandle);

}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    //Display_Handle    displayHandle;
    //Display_Params    displayParams;
    //Display_Params_init(&displayParams);
    //displayHandle = Display_open(Display_Type_UART, NULL);
    //Display_printf(displayHandle, 1, 0, "Pio Pio Pio");
    ///GPIO_init();

    init_component();
    Sdi12_init_sem();
    //GPTimerCC26XX_Params params;
    //GPTimerCC26XX_Handle hTimerA;
    //GPTimerCC26XX_Handle hTimerB;

    while(1){
        //Sdi_Set_State_Data_Line(TRANSMIT);




/*  // test A ; working
        __delay_cycles(1000);
        init_component();
        DIO_22_ConfigAsOutput();
        DIO_22_State_1();
        __delay_cycles(1000);
        DIO_22_State_0();
        __delay_cycles(1000);

  */

    // test B ; workimg ? ISR loop in conflited, error when try to reach max range of value (256,65556), hopefully, it s a detection error of the main program that is shutting down all
       // cannot run more than 30s
      /*  init_component();
        DIO_22_ConfigAsOutput();

        ClearTimerAFlag();
        DIO_22_State_1();
        Set_TimerA(254,65534);
        Launch_TimerA();

        ClearTimerAFlag();
        DIO_22_State_0();
        Set_TimerA(254,65534);
        Launch_TimerA();

 */

       // GPIO_write(LED_RED, Board_GPIO_LED_ON);//red led on on trqnsmit mode
        //GPIO_write(LED_GREEN, Board_GPIO_LED_ON);//gree led on on receive mode
       // GPIO_write(SDI12_Serial_data_line, 1);
      // displayHandle = Display_open(Display_Type_UART, NULL);
      Sdi12_Send_Break();
       // Display_printf(displayHandle, 1, 0, "1");
      Sdi12_Send_Command_2_char("?!");//check if someone is here
       // Display_printf(displayHandle, 1, 0, "2");
        Sdi12_Send_Command("0!");//check if someone with address 0 is here
       // Display_printf(displayHandle, 1, 0, "3");
        Sdi12_Send_Command("0M!");//start measurement
       // Display_printf(displayHandle, 1, 0, "4");
        Sdi12_Send_Command("0D0!");//get measurement
       // Display_printf(displayHandle, 1, 0, "5");
        Sdi12_Send_Command("0D1!");//get measurement
     //  Display_printf(displayHandle, 1, 0, "6");
      //  Display_close(displayHandle);
    }
}
