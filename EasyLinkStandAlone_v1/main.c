/*
 * main.c
 *
 *  Created on: 31 mar. 2017
 *      Author: kero
 */


#include "EasyLink.h"
#include "Board.h"
#include <ti/drivers/PIN.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "CC1350_LAUNCHXL.h"


//#define LED_Roja 6
//#define LED_Verde 5
void main()
{


    Board_initGeneral();

    EasyLink_init(EasyLink_Phy_625bpsLrm);//modo sub 1ghz larga distancia + 625bps data rate

 /*   typedef struct
    {
            uint8_t dstAddr[8];     /// Dst Address
            uint32_t absTime;        ///Absolute time to Tx packet (0 for immediate)
            ///Layer will use last SeqNum used + 1
            uint8_t len;             ///Payload Length
            uint8_t payload[EASYLINK_MAX_DATA_LENGTH];       ///Payload
    } EasyLink_TxPacket;

    EasyLink_Status EasyLink_transmit(EasyLink_TxPacket *txPacket)
*/
    int data=83;// data a transmittir

    EasyLink_TxPacket data_to_transmit = {"0000000",0,2,data};
    EasyLink_TxPacket* pio;
    pio=&data_to_transmit;


    EasyLink_transmit(pio);//transmit data

    //PIN_setOutputValue(PIN_Handle handle, PIN_Id pinId, uint_t val);
    //Encender la led Roja para significar que hubo transmission
    PIN_State* State =1;

    PIN_init(BoardGpioInitTable[]);//en BoardGpioInitTable esta la configuracion de los GPIOs como LED
    PIN_open(State, gpioPinConfigs[]);
    PIN_setOutputValue(handle, 6, 1);


}
