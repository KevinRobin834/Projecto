/*
 * Timer_function.c
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


#define SDI12_TIMER GPT0_BASE



//****************************************************timer configuration*******************************************//
void TimerConfigs()//timer A et timer B
{
    //make sure that timer is disable before doing change
    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEN_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEN_DIS;//timer B enable

    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TAEN_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TAEN_DIS;//timer B enable


    //config timer
    HWREG(SDI12_TIMER + GPT_O_CFG) &= ~GPT_CFG_CFG_M; //mask
    HWREG(SDI12_TIMER + GPT_O_CFG) |= GPT_CFG_CFG_16BIT_TIMER;//timer mode 16 bits

    //timer A mode config
    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TCACT_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TCACT_SETCLR_ON_TO ;//Set CCP output pin immediately and clear on Time-Out

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TACINTD_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TACINTD_EN_TO_INTR ;//timer interupt disable

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TAMRSU_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TAMRSU_TOUPDATE;//update TAMATCHR and TAPR on time out

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TASNAPS_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TASNAPS_DIS;//capture mode enable

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TAWOT_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TAWOT_WAIT; // wait trigger command to begin counting

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TAMIE_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TAMIE_EN; //interrupt event enable

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TACDIR_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TACDIR_UP; // counting up

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TAAMS_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TAAMS_CAP_COMP; //capture/compare enable NO PWM

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TACM_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TACM_EDGCNT;// edge time vs edge count ?????

    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TAMR_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TAMR_PERIODIC;//periodic mode

    //timer B mode config same as timer A

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TCACT_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TCACT_SETCLR_ON_TO ;//Set CCP output pin immediately and clear on Time-Out


    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBCINTD_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBCINTD_EN_TO_INTR ;//timer interupt enable

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBMRSU_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBMRSU_TOUPDATE;//update TAMATCHR and TAPR on time out

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBSNAPS_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBSNAPS_DIS;//capture mode enable

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBWOT_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBWOT_WAIT; // wait trigger command to begin counting

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBMIE_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBMIE_EN; //interrupt event enable

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBCDIR_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBCDIR_UP; // counting up

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBAMS_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBAMS_CAP_COMP; //capture/compare enable NO PWM

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBCM_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBCM_EDGCNT;// edge time vs edge count ?????

    HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBMR_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBMR_PERIODIC;//periodic mode

    // timer register control (timer B define before timer A, same order as in the register)

    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TASTALL_M;
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TASTALL_EN;//timer can be freeze by debugger

    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBSTALL_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBSTALL_EN;//timer can be freeze by debugger

    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEVENT_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEVENT_POS;//timer B active on positive edge


    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEVENT_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEVENT_POS;//timer A active on positive edge

    //timer A and timer B active in positive edge

    //timer synchro register //disable all synchro with other timer (like 1,2,3,4)
    HWREG(SDI12_TIMER + GPT_O_SYNC) &= ~0x000000FF;//mask on all the register, all bits reserved excluding 8 LSB Bits
    HWREG(SDI12_TIMER + GPT_O_SYNC) |=GPT_SYNC_SYNC3_NOSYNC;//
    HWREG(SDI12_TIMER + GPT_O_SYNC) |=GPT_SYNC_SYNC2_NOSYNC;//
    HWREG(SDI12_TIMER + GPT_O_SYNC) |=GPT_SYNC_SYNC1_NOSYNC;//
    HWREG(SDI12_TIMER + GPT_O_SYNC) |=GPT_SYNC_SYNC0_NOSYNC;//

    //Interupt mask register
    HWREG(SDI12_TIMER + GPT_O_IMR) &= ~GPT_IMR_TBMIM_M;//mask
    HWREG(SDI12_TIMER + GPT_O_IMR) |=GPT_IMR_TBMIM_EN;//activate one interrupt  for timer B

    HWREG(SDI12_TIMER + GPT_O_IMR) &= ~GPT_IMR_TAMIM_M;//mask
    HWREG(SDI12_TIMER + GPT_O_IMR) |=GPT_IMR_TAMIM_EN;//activate one interrupt  for timer A

    //config interrupt
    int gpt_timer_interrupt_config = 0xFF;
    HWREG(NVIC_EN0)= 1<<15;
    HWREG(NVIC_PRI3)|= (gpt_timer_interrupt_config<<24);
}


/*
 * break : 99  6240 = 13ms
 * marking : 99  4320 = 8.33ms
 * 1 bits  : 99  399 = 8.33us
 * 1/2 bits : 99 200 = 8.33/2us
 * timeout : 256 65536 = 349ms
 */
void Disable_TimerA(){

    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TAEN_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TAEN_DIS;//timer A disable
}

void Disable_TimerB(){

     HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEN_M;//mask
     HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEN_DIS;//timer B disable

}
void Set_TimerA(int prescaler,int value){

    Disable_TimerA();//disable timer due of the setting change
    //timer pre-scale register A
    HWREG(SDI12_TIMER + GPT_O_TAPR) &= ~GPT_TAPR_TAPSR_M;//mask
    HWREG(SDI12_TIMER + GPT_O_TAPR) |=prescaler;   //prescaler = 100, for easy calculation

    //define value to count to of timer A
    HWREG(SDI12_TIMER + GPT_O_TAMATCHR) &= ~GPT_TAMATCHR_TAMATCHR_M;//mask
    HWREG(SDI12_TIMER + GPT_O_TAMATCHR) |=value; // value = 8.33us

    //only do one break --> one shot mode
    HWREG(SDI12_TIMER + GPT_O_TAMR) &= ~GPT_TAMR_TAMR_M; //mask
    HWREG(SDI12_TIMER + GPT_O_TAMR) |=  GPT_TAMR_TAMR_PERIODIC;//periodic mode

}

void Set_TimerB(int prescaler,int value){

      Disable_TimerB();//disable timer due of the setting change
      //timer pre-scale register B
      HWREG(SDI12_TIMER + GPT_O_TBPR) &= ~GPT_TBPR_TBPSR_M;//mask
      HWREG(SDI12_TIMER + GPT_O_TBPR) |=prescaler;   //prescaler = 100, for easy calculation

      //define value to count to of timer B
      HWREG(SDI12_TIMER + GPT_O_TBMATCHR) &= ~GPT_TBMATCHR_TBMATCHR_M;//mask
      HWREG(SDI12_TIMER + GPT_O_TBMATCHR) |=value; // value = 13ms ( sdi 12 datasheet said "break at least 12ms")

      //only do one break --> one shot mode
      HWREG(SDI12_TIMER + GPT_O_TBMR) &= ~GPT_TBMR_TBMR_M; //mask
      HWREG(SDI12_TIMER + GPT_O_TBMR) |=  GPT_TBMR_TBMR_ONE_SHOT;//one shot mode

}

void Launch_TimerA(){

      HWREG(SDI12_TIMER + GPT_O_TAR) |=  0;//optionnaly, begin counting at 0 , no bits reserved = no mask
    //enable timer B
      HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TAEN_M;//mask
      HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TAEN_EN;//timer B enable
}

void Launch_TimerB(){

      HWREG(SDI12_TIMER + GPT_O_TBR) |=  0;//optionnaly, begin counting at 0 , no bits reserved = no mask
    //enable timer B
      HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEN_M;//mask
      HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEN_EN;//timer B enable
}



void ClearTimerAFlag()
{
    HWREG(SDI12_TIMER + GPT_O_ICLR) &= ~GPT_ICLR_TAMCINT_M;//mask
    HWREG(SDI12_TIMER + GPT_O_ICLR) |=  GPT_ICLR_TAMCINT_S;//clear Flag timer A interrupt
}

void ClearTimerBFlag()
{

    HWREG(SDI12_TIMER + GPT_O_ICLR) &= ~GPT_ICLR_TBMCINT_M;//mask
    HWREG(SDI12_TIMER + GPT_O_ICLR) |=  GPT_ICLR_TBMCINT_S;//clear Flag timer B interrupt
}
