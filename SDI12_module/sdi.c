/*
 *  ======== empty.c ========
 */
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/* Example/Board Header files */
#include "Board.h"

/* Files needed to turn off the external flash on 2650, 1310, 1350, 2640R2 LaunchPads */
#if defined(CC2650_LAUNCHXL) || defined(CC2640R2_LAUNCHXL) || defined(CC1310_LAUNCHXL) || defined(CC1350_LAUNCHXL)
    #include <ti/drivers/SPI.h>
    #include <ti/mw/extflash/ExtFlash.h>
#endif

#include "sdi.h"
#include "sdi12_top.h"
#include "GPTimerCC26XX.h"
#include "CC1350_LAUNCHXL.h"
#include "Timer.h"

/****Global Variable *////

static sdi12_communication_state_status_t Current_state=SD12_START;
static int increment =0; //for timer B

static uint8_t RCV_Buffer[10]={0};//default value
static uint8_t TRM_Buffer[10]={0};//default value
static error_flag_t sdi_error=clear;
sdi12_low_state_t line_state=IDLE;

/* Pin driver handles */
 PIN_Handle InPinHandle;
 PIN_Handle OutPinHandle;

/* Global memory storage for a PIN_Config table */
 PIN_State buttonPinState;
 PIN_State ledPinState;

 PIN_Config OutputPinTable[] = {
     Board_DIO22 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
     Board_PIN_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW  | PIN_PUSHPULL | PIN_DRVSTR_MAX,
     Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW  | PIN_PUSHPULL | PIN_DRVSTR_MAX,
     PIN_TERMINATE
 };

 /*
  * Application button pin configuration table:
  *   - Buttons interrupts are configured to trigger on falling edge.
  */
 PIN_Config InputPinTable[] = {
     Board_DIO22  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
     PIN_TERMINATE
 };

 uint32_t currVal = 0;
#define state_1 1
#define state_0 0
static GPTimerCC26XX_Params params;
static GPTimerCC26XX_Handle hTimerA;
static GPTimerCC26XX_Handle hTimerB;


/*
 *  ======== function ========
 */



int Parity_Even(uint8_t RCV_Buffer[10])
{
    int i=1, value=0;
    for (i=1;i<=7;i++)
    {
        value=value^RCV_Buffer[i];
    }
    return value;
}

unsigned char Reverse_Byte(char a)
{

  return ((a & 0x1)  << 7) | ((a & 0x2)  << 5) |
         ((a & 0x4)  << 3) | ((a & 0x8)  << 1) |
         ((a & 0x10) >> 1) | ((a & 0x20) >> 3) |
         ((a & 0x40) >> 5) | ((a & 0x80)  >> 7);
}

void Sdi_uint8_to_array(uint8_t data_to_send, uint8_t TRM_Buffer[]);

void Sdi_uint8_to_array(uint8_t data_to_send, uint8_t TRM_Buffer[])
{
    uint8_t i, parity = 0, bit_to_send;

    TRM_Buffer[0] = 1;  // Start
    for (i = 0; i < 7; i++) {
        bit_to_send = data_to_send & 0x01;  // LSB first
        parity = parity ^ bit_to_send;
        TRM_Buffer[i+1] = bit_to_send;
        data_to_send = data_to_send >> 1;
    }
    TRM_Buffer[8] = parity;  // Even parity
    TRM_Buffer[9] = 0;  // Stop
}

uint8_t Sdi_array_to_uint8_t(uint8_t Buffer[])
{
    uint8_t i, result = 0, recv_bit;

    for (i = 7; i > 0; i--) {
        recv_bit = Buffer[i] & 0x01;
        result = result | recv_bit;
        result = result << 1;
    }
    return result;
}

void DefaultGPIO_ISR(uint_least8_t index)
{
    //GPIO_disableInt(SDI12_Serial_data_line);//disable interrupt
    ClearFlagInterrupt_DIO_22();
    Set_TimerA(99,200,hTimerA);  //1/2 bits : 99 200 = 8.33/2us
    Launch_TimerA(hTimerA);
    Current_state=SD12_START;//begin reception mode
    //ClearFlagInterrupt_DIO_22();
}

void init_component(void){
    Sdi_Set_State_Data_Line(IDLE);
   // IntRegister(15,DefaultTimerA_ISR);
}

void Sdi_Set_State_Data_Line(sdi12_low_state_t line) //index is SDI12_Serial_data_line
{
    //using DIO22 pour sdi12 (and eventually led)
    line_state = line;
    switch(line)
    {
    case IDLE:
        //DIO_22_ClearPreviousState();
       // GPIO_init();
        //PIN_init(OutputPinTable);
        TimerBegin(/*&hTimerA,&hTimerB,params*/);
        //GPIO_clearInt(SDI12_Serial_data_line);
       //GPIO_setConfig(LED_RED, GPIO_CFG_OUTPUT);//red led on when transmit mode.
       // GPIO_setConfig(LED_GREEN, GPIO_CFG_OUTPUT);//green led on when receive mode.
        break;
    case TRANSMIT:

        //DIO_22_ConfigAsOutput();
        //GPIO_setConfig(SDI12_Serial_data_line, GPIO_CFG_OUTPUT);//Pin is an output.
       // GPIO_write(LED_RED, Board_GPIO_LED_ON);//red led on on trqnsmit mode
        //GPIO_write(LED_GREEN, Board_GPIO_LED_OFF);
        //PIN_close(InPinHandle);
       PIN_remove(InPinHandle, Board_DIO22);
       OutPinHandle = PIN_open(&ledPinState, OutputPinTable);
       PIN_setOutputValue(OutPinHandle, SDI12_Serial_data_line, 0);
        //DIO_22_ConfigAsOutput();
        break;
    case RECEIVE:

        //DIO_22_ConfigAsInput();
        //Config_DIO_22_Interrupt();
        //ClearFlagInterrupt_DIO_22();
        //GPIO_setConfig(SDI12_Serial_data_line, GPIO_CFG_INPUT);//Pin is an output.
        //GPIO_write(LED_GREEN, Board_GPIO_LED_ON);//gree led on on receive mode
       // GPIO_write(LED_RED, Board_GPIO_LED_OFF);
     //  GPIO_setCallback(SDI12_Serial_data_line, DefaultGPIO_ISR);
      // GPIO_clearInt(SDI12_Serial_data_line);
      // GPIO_enableInt(SDI12_Serial_data_line);
        PIN_close(OutPinHandle);
        InPinHandle = PIN_open(&ledPinState, InputPinTable);
        PIN_setInterrupt(InPinHandle,SDI12_Serial_data_line|PIN_IRQ_NEGEDGE);
        //DIO_22_ConfigAsInput();
        //ClearFlagInterrupt_DIO_22();
        break;

    }
    line_state = line;
}
/*
 * SDI12 use negative logic which means that H=0 and Low=1
 * */
void Sdi_Wake_Sensor()
{
    Sdi_Set_State_Data_Line(TRANSMIT);
    //Disable_TimerB(hTimerB);
    Current_state=SD12_BREAK;
    //GPIO_write(SDI12_Serial_data_line, state_0);
    PIN_setOutputValue(OutPinHandle, SDI12_Serial_data_line, 0);
    //DIO_22_State_0();
    Set_TimerA(99,6240,hTimerA); //break : 99  6240 = 13ms
    Launch_TimerA(hTimerA);
    Current_state=SD12_MARKING;

    /*GPIO_write(SDI12_Serial_data_line, 0);
    Set_TimerA(99,4320); //marking : 99  4320 = 8.33ms
    Launch_TimerA();


*/
}

///*** Timer A will be use for creating bits and timer B will be use for creating SDI frames***///

void Sdi_Transmit_Character(uint8_t data_to_send)
{
    Sdi_Set_State_Data_Line(TRANSMIT);
    Sdi_uint8_to_array(data_to_send, TRM_Buffer);
    Current_state=SD12_START;
    Set_TimerA(99,200,hTimerA);  //1/2 bits : 99 200 = 8.33/2us////// before was 99,200
    Launch_TimerA(hTimerA);
    Disable_TimerB(hTimerB);
}

void Sdi_Receive_Character()//help : how to return a array https://www.tutorialspoint.com/cprogramming/c_return_arrays_from_function.htm
{

    Sdi_Set_State_Data_Line(RECEIVE);
    //activate timer timeout and all the other stuff during interrupt loop
    Set_TimerB(256,65536,hTimerB); ////timeout : 256 65536 = 349ms
    Launch_TimerB(hTimerB);

}





void DefaultTimerA_ISR(GPTimerCC26XX_Handle hTimerA, GPTimerCC26XX_IntMask interruptMask)
{
    ClearTimerAFlag();//clear flag timer A

    if(line_state==IDLE)
    {
        return;
    }

    Set_TimerA(99,399,hTimerA);  //1 bits  : 99  399 = 8.33us
    Launch_TimerA(hTimerA);

    if(line_state==TRANSMIT)
    {
        switch (Current_state) {
        case SD12_BREAK:
            break;
        case SD12_MARKING:
            //GPIO_write(SDI12_Serial_data_line, 1);
            PIN_setOutputValue(OutPinHandle, SDI12_Serial_data_line, 0);
            //DIO_22_State_1();
            Set_TimerA(99,4320,hTimerA);  //marking : 99  4320 = 8.33ms
            Launch_TimerA(hTimerA);
            break;
        case SD12_WAKE_END:
        case SD12_END:
            Disable_TimerA(hTimerA);
            // Callback!
            Sdi12_Tx_Callback();
            break;
        default:            // START, BITS, PARITY, and STOP
            // Do transmission of character bits

            //GPIO_write(SDI12_Serial_data_line, TRM_Buffer[Current_state]);
            PIN_setOutputValue(OutPinHandle, SDI12_Serial_data_line, TRM_Buffer[Current_state]);
           /*
            if(TRM_Buffer[Current_state]==1)
            {
                DIO_22_State_1();

            }
            else
            {
                DIO_22_State_0();
            }
            */
            break;
        }
    }

    if (line_state==RECEIVE)
    {

        //RCV_Buffer[Current_state]=GPIO_read(SDI12_Serial_data_line);//read current value and store it
        RCV_Buffer[Current_state]=PIN_getInputValue(SDI12_Serial_data_line);

        if (DIO_22_Get_Value()==2){
            sdi_error = read_dio_failed;

        }
        /*
        else
        {
            RCV_Buffer[Current_state]=DIO_22_Get_Value();
        }
         */
        if (Current_state==SD12_START)
        {
            sdi_error=clear;
        }

        //checking start bits
        if ((RCV_Buffer[0]!=0) && (Current_state==SD12_START))
            {
                sdi_error=start_error;
            }

            //checking stop bits
        if ((RCV_Buffer[9]!=1) && (Current_state==SD12_STOP))
            {
                sdi_error=stop_error;
            }

            //checking parity bit
        if ((Current_state==SD12_PARITY ) && (RCV_Buffer[8]!= Parity_Even(RCV_Buffer)))
            {
                sdi_error=parity_error;
            }

        if(Current_state==SD12_END)
            {
                Disable_TimerA(hTimerA);
                Disable_TimerB(hTimerB);
                // Callback!
                Sdi12_Rx_Callback(Sdi_array_to_uint8_t(RCV_Buffer), sdi_error);
            }

    }

    Current_state++;//preparing next

}
void DefaultTimerB_ISR(GPTimerCC26XX_Handle hTimerB, GPTimerCC26XX_IntMask interruptMask)
{
    ClearTimerBFlag();// if reached go out
    Set_TimerB(256,65536,hTimerB);//349ms
    Launch_TimerB(hTimerB);
    if (increment == 3)
    {
        Sdi_Set_State_Data_Line(IDLE);// go to idle mode
        Disable_TimerB(hTimerB);
        increment=0;
    }
    increment++;
}
//*********************************timer configuration + Gpio*******************************************//
void TimerBegin(/*GPTimerCC26XX_Handle *hTimerA,GPTimerCC26XX_Handle *hTimerB,GPTimerCC26XX_Params params*/)//timer A et timer B
{


       //GPTimerCC26XX_Params params;
       GPTimerCC26XX_Params_init(&params);
       params.width          = GPT_CONFIG_16BIT;
       params.mode           = GPT_MODE_PERIODIC_UP;
       params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_ON;

       unsigned int CC2650_GPTIMER0A=0;
       unsigned int CC2650_GPTIMER0B=1;

       hTimerA = GPTimerCC26XX_open(CC2650_GPTIMER0A, &params);

       hTimerB  = GPTimerCC26XX_open(CC2650_GPTIMER0B, &params);

       GPTimerCC26XX_registerInterrupt(hTimerA, DefaultTimerA_ISR, GPT_INT_TIMEOUT);

       GPTimerCC26XX_registerInterrupt(hTimerB, DefaultTimerB_ISR, GPT_INT_TIMEOUT);

/*
 * break : 99  6240 = 13ms
 * marking : 99  4320 = 8.33ms
 * 1 bits  : 99  399 = 8.33us
 * 1/2 bits : 99 200 = 8.33/2us
 * timeout : 256 65536 = 349ms
 */
}


void Set_TimerA(int prescaler,int value,GPTimerCC26XX_Handle hTimerA){

    HWREG(SDI12_TIMER + GPT_O_TAPR) &= ~GPT_TAPR_TAPSR_M;//mask
    HWREG(SDI12_TIMER + GPT_O_TAPR) |=prescaler;   //prescaler = 100, for easy calculation

    //define value to count to of timer A
    HWREG(SDI12_TIMER + GPT_O_TAILR) &= ~GPT_TAILR_TAILR_M;//mask
    HWREG(SDI12_TIMER + GPT_O_TAILR) |=value; // value = 8.33us

   GPTimerCC26XX_setLoadValue(hTimerA, value);

}

void Set_TimerB(int prescaler,int value,GPTimerCC26XX_Handle hTimerB){

    //Disable_TimerB();//disable timer due of the setting change
    //timer pre-scale register B
    HWREG(SDI12_TIMER + GPT_O_TBPR) &= ~GPT_TBPR_TBPSR_M;//mask
    HWREG(SDI12_TIMER + GPT_O_TBPR) |=prescaler;   //prescaler = 100, for easy calculation

    //define value to count to of timer B
    HWREG(SDI12_TIMER + GPT_O_TBILR) &= ~GPT_TBILR_TBILR_M;//mask
    HWREG(SDI12_TIMER + GPT_O_TBILR) |=value; // value = 13ms ( sdi 12 datasheet said "break at least 12ms")

    GPTimerCC26XX_setLoadValue(hTimerB, value);
}

void Launch_TimerA(GPTimerCC26XX_Handle hTimerA){


    //HWREG(SDI12_TIMER + GPT_O_TAR) |=  0;//optionnaly, begin counting at 0 , no bits reserved = no mask
  //enable timer B
   // HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TAEN_M;//mask
  //  HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TAEN_EN;//timer B enable

    GPTimerCC26XX_start(hTimerA);

}

void Launch_TimerB(GPTimerCC26XX_Handle hTimerB){



    //HWREG(SDI12_TIMER + GPT_O_TBR) |=  0;//optionnaly, begin counting at 0 , no bits reserved = no mask
  //enable timer B
    HWREG(SDI12_TIMER + GPT_O_CTL) &= ~GPT_CTL_TBEN_M;//mask
    HWREG(SDI12_TIMER + GPT_O_CTL) |=  GPT_CTL_TBEN_EN;//timer B enable

    GPTimerCC26XX_start(hTimerB);

}

void Disable_TimerA(GPTimerCC26XX_Handle hTimerA){

    GPTimerCC26XX_stop(hTimerA);
    GPTimerCC26XX_close(hTimerA);
}

void Disable_TimerB(GPTimerCC26XX_Handle hTimerB){

    GPTimerCC26XX_stop( hTimerB);
    GPTimerCC26XX_close(hTimerB);

}


void ClearTimerAFlag()
{
    HWREG(SDI12_TIMER + GPT_O_ICLR) =  GPT_ICLR_TATOCINT;//clear Flag timer A interrupt
}

void ClearTimerBFlag()
{

    HWREG(SDI12_TIMER + GPT_O_ICLR) =  GPT_ICLR_TBTOCINT;//clear Flag timer B interrupt
}
/*
void DefaultTimerA_ISR(){

}

void DefaultTimerB_ISR(GPTimerCC26XX_Handle hTimerB, GPTimerCC26XX_IntMask interruptMask){


}*/

/*
   Timer_Handle    handleA;
   Timer_Handle    handleB;
   Timer_Params    params2;

void timer_set_params_TimerA(Timer_Handle handleA,Timer_Params params2,uint32_t period){
   Timer_Params_init(&params2);
   params.periodUnits = Timer_PERIOD_US;
   params.period = period;
   params.timerMode  = Timer_CONTINUOUS_CALLBACK;
   params.timerCallback = DefaultTimerA_ISR;

   handleA = Timer_open(Board_GPTIMER0A, &params);



}

void timer_set_params_TimerB(Timer_Handle handleB,Timer_Params params2,uint32_t period){
   Timer_Params_init(&params2);
   params.periodUnits = Timer_PERIOD_US;
   params.period = period;
   params.timerMode  = Timer_CONTINUOUS_CALLBACK;
   params.timerCallback = DefaultTimerB_ISR;

   handleB = Timer_open(Board_GPTIMER0B, &params);

}

void timer_unleashed_timerA(Timer_Handle handleA){
   Timer_start(handleA);

}
void timer_unleashed_timerB(Timer_Handle handleB){
   Timer_start(handleB);
}

void timer_halt_timerA(){
    Timer_stop(handleA);
}
void timer_halt_timerB(Timer_Handle handleB){
    Timer_stop(handleB);
}
void timer_close_timerA(){
    Timer_close(handleA);
}
void timer_close_timerB(Timer_Handle handleB){
    Timer_close(handleB);
}
*/
