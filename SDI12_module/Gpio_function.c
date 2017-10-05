/*
 * Gpio_function.c
 *
 *  Created on: 20 jun. 2017
 *      Author: kero
 */



/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpt.h"

/* Board Header file */
#include "Board.h"

#define SDI12_Serial_data_line Board_DIO22

//******If GPIO driver does not work, try to use this *******//
void ClearFlagInterrupt_DIO_22(void)
{
    HWREG(GPIO_BASE + GPIO_O_EVFLAGS31_0) &= ~GPIO_EVFLAGS31_0_DIO22_M;//mask
    HWREG(GPIO_BASE + GPIO_O_EVFLAGS31_0) |= GPIO_EVFLAGS31_0_DIO22; // set 1 for clearing flag
}
void Config_DIO_22_Interrupt(void)
{
    GPIO_setConfig(SDI12_Serial_data_line, GPIO_CFG_IN_PD);//set dio22 as imput with pull down

    GPIO_enableInt(SDI12_Serial_data_line);


}
void DIO_22_ClearPreviousState(void)
{
    //because there is not a function in gpio.h, do clearing myself
    HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) &= ~GPIO_DOUTCLR31_0_DIO22_M;//mask
    HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) |= GPIO_DOUTCLR31_0_DIO22; // set 1 for clearing
    GPIO_clearInt(SDI12_Serial_data_line);//clear interrupt
}

void DIO_22_ConfigAsInput()
{
    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_HYST_EN_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_HYST_EN;//input hysteresis set

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IE_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_IE;//input disabled => output

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_WU_CFG_M;//mask
    //HWREG(IOC_BASE + IOC_O_IOCFG22) |=  0x00;//no wake up

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IOMODE_M;//normal input/output mode

    //configure DIO as GPIO input to generate interrupt on Edge detect
    HWREG(IOC_BASE + IOC_O_IOCFG22) &=~IOC_IOCFG22_EDGE_IRQ_EN_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |= IOC_IOCFG22_EDGE_IRQ_EN ;//enable interupt

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_EDGE_DET_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_EDGE_DET_BOTH;//both edge detection becquse i dont know

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_PULL_CTL_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_PULL_CTL_DWN;//pull down beause input


    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_SLEW_RED_M;//no slew rate

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IOSTR_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_IOSTR_AUTO;//min current mode

    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IOCURR_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_IOCURR_2MA;//min current mode


    HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG15_PORT_ID_M;//mask
    HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_PORT_ID_GPIO ;//gpio

    HWREG(GPIO_BASE + GPIO_O_DIN31_0) &= ~GPIO_DIN31_0_DIO22_M;//mask
    HWREG(GPIO_BASE + GPIO_O_DIN31_0) |= GPIO_DIN31_0_DIO22; // set to input

    HWREG(GPIO_BASE + GPIO_O_EVFLAGS31_0) &= ~GPIO_EVFLAGS31_0_DIO22_M;//mask
    HWREG(GPIO_BASE + GPIO_O_EVFLAGS31_0) |= GPIO_EVFLAGS31_0_DIO22; // set 1 for clearing flag

}

uint8_t DIO_22_Get_Value(void){
    uint8_t grab=2;//if cannot grab value, error
    grab = HWREG(GPIO_BASE + GPIO_O_DIN31_0) & ~GPIO_DIN31_0_DIO22;//target register which has dio22 read value
    return grab;
}

void DIO_22_State_1(void)//do not forget to config first as output
{
    HWREG(GPIO_BASE + GPIO_O_DOUT23_20) &= ~GPIO_DOUT23_20_DIO22_M;//mask
    HWREG(GPIO_BASE + GPIO_O_DOUT23_20) |= GPIO_DOUT23_20_DIO22; // set 1
}
void DIO_22_State_0(void)//do not forget to config first as output
{
    HWREG(GPIO_BASE + GPIO_O_DOUT23_20) &= ~GPIO_DOUT23_20_DIO22_M;//mask
    HWREG(GPIO_BASE + GPIO_O_DOUT23_20) |= 0; // set 0
}

void DIO_22_ConfigAsOutput(){

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_HYST_EN_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_HYST_EN;//input hysteresis set

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IE_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_IE;//input disabled => output

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG15_WU_CFG_M;//mask
       //HWREG(IOC_BASE + IOC_O_IOCFG22) |=  0x00;//no wake up

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IOMODE_M;//normal input/output mode

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_EDGE_IRQ_EN_M;//no edge generation

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_EDGE_DET_M;// no edge detction

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_PULL_CTL_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_PULL_CTL_DIS;//no pull


       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_SLEW_RED_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  0x00;//normal slew rate

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IOSTR_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_IOSTR_AUTO;//min current mode

       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_IOCURR_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_IOCURR_2MA;//min current mode


       HWREG(IOC_BASE + IOC_O_IOCFG22) &= ~IOC_IOCFG22_PORT_ID_M;//mask
       HWREG(IOC_BASE + IOC_O_IOCFG22) |=  IOC_IOCFG22_PORT_ID_GPIO ;//gpio

       HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~GPIO_DOE31_0_DIO22_M;//mask
       HWREG(GPIO_BASE + GPIO_O_DOE31_0) |=  GPIO_DOE31_0_DIO22;//setting pin 22 as output

}


