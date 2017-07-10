/*
 *  ======== empty.c ========
 */

#include "sdi.h"
#include "sdi12_top.h"

/****Global Variable *////

static sdi12_communication_state_status_t Current_state=SD12_START;
static int increment =0; //for timer B

static uint8_t RCV_Buffer[10]={0};//default value
static uint8_t TRM_Buffer[10]={0};//default value
static error_flag_t sdi_error=clear;
static sdi12_low_state_t line_state=IDLE;

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

void Sdi_Set_State_Data_Line(sdi12_low_state_t line) //index is SDI12_Serial_data_line
{
    //using DIO22 pour sdi12 (and eventually led)
    switch(line)
    {
    case IDLE:
        DIO_22_ClearPreviousState();
        break;
    case TRANSMIT:
        DIO_22_ConfigAsOutput();
        break;
    case RECEIVE:
        DIO_22_ConfigAsInput();
        Config_DIO_22_Interrupt();
        ClearFlagInterrupt_DIO_22();
        break;

    }
}

void Sdi_Wake_Sensor()
{
    Sdi_Set_State_Data_Line(TRANSMIT);
    Disable_TimerB();

    Current_state=SD12_BREAK;
    GPIO_write(SDI12_Serial_data_line, 1);
    Set_TimerA(99,6240); //break : 99  6240 = 13ms
    Launch_TimerA();
    Current_state=SD12_MARKING;
    /*
    GPIO_write(SDI12_Serial_data_line, 0);
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
    Set_TimerA(99,200); //1/2 bits : 99 200 = 8.33/2us
    Launch_TimerA();
    Disable_TimerB();
}

void Sdi_Receive_Character()//help : how to return a array https://www.tutorialspoint.com/cprogramming/c_return_arrays_from_function.htm
{

    Sdi_Set_State_Data_Line(RECEIVE);
    //activate timer timeout and all the other stuff during interrupt loop
    Set_TimerB(256,65536); ////timeout : 256 65536 = 349ms
    Launch_TimerB();

}


void DefaultGPIO_ISR(void)
{
    GPIO_disableInt(SDI12_Serial_data_line);//disable interrupt
    Set_TimerA(99,200); //1/2 bits : 99 200 = 8.33/2us
    Launch_TimerA();
    Current_state=SD12_START;//begin reception mode
    ClearFlagInterrupt_DIO_22();
}


void DefaultTimerA_ISR(void)
{
    ClearTimerAFlag();//clear flag timer A

    if(line_state==IDLE)
    {
        return;
    }

    Set_TimerA(99,399); //1 bits  : 99  399 = 8.33us
    Launch_TimerA();

    if(line_state==TRANSMIT)
    {
        switch (Current_state) {
        case SD12_BREAK:
            break;
        case SD12_MARKING:
            GPIO_write(SDI12_Serial_data_line, 0);
            Set_TimerA(99,4320); //marking : 99  4320 = 8.33ms
            Launch_TimerA();
            break;
        case SD12_WAKE_END:
        case SD12_END:
            Disable_TimerA();
            // Callback!
            Sdi12_Tx_Callback();
            break;
        default:            // START, BITS, PARITY, and STOP
            // Do transmission of character bits
            GPIO_write(SDI12_Serial_data_line, TRM_Buffer[Current_state]);
            break;
        }
    }

    if (line_state==RECEIVE)
    {
        RCV_Buffer[Current_state]=GPIO_read(SDI12_Serial_data_line);//read current value and store it

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
                Disable_TimerA();
                Disable_TimerB();
                // Callback!
                Sdi12_Rx_Callback(Sdi_array_to_uint8_t(RCV_Buffer), sdi_error);
            }

    }

    Current_state++;//preparing next

}
void DefaultTimerB_ISR(void)
{
    ClearTimerBFlag();// if reached go out
    Set_TimerB(256,65536);//349ms
    Launch_TimerB();
    if (increment == 3)
    {
        Sdi_Set_State_Data_Line(IDLE);// go to idle mode
        Disable_TimerB();
        increment=0;
    }
    increment++;
}





/*int parity_even(int v)
{
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return v & 1;
}*/

/*
void sdi_SendCharacter(struct sdi12_low_info_transmit character)
{
     GPIO_write(SDI12_Serial_data_line, 1);                                            // - start bit
     sdi_waitBit();

     int mask=0x01;                          // mask for reversing byte because lsb first
     for (mask = 0x01; mask; mask<<=1){      //for each bits reversed
         if( character.tx_octet & mask)      //send it
             {
                 GPIO_write(SDI12_Serial_data_line, 0);
             }
             else
             {
                 GPIO_write(SDI12_Serial_data_line, 1);
             }
         sdi_waitBit();
           }

     character.tx_parity = (parity_even(character.tx_octet));               // - parity bit
     if(character.tx_parity==1)
     {
         GPIO_write(SDI12_Serial_data_line, 1);
         sdi_waitBit();
     }
     else
     {
         GPIO_write(SDI12_Serial_data_line, 0);
         sdi_waitBit();
     }

     GPIO_write(SDI12_Serial_data_line, 0);                                            // 4.2.4 - stop bit
     sdi_waitBit();
}
*/
/*
void sdi_ReceiveCharacter(uint_least8_t index)
//int sdi_ReceiveCharacter(struct sdi12_low_info_receive character)
{


    //at this point start bit has been received
    int i=7;

    sdi_waitHalfBit();                              //read between start and data, desynchronize timer and reception
    if((GPIO_read(SDI12_Serial_data_line))==1){     //confirm that it's not a 1 , else error
        //sdi_error=begin_reception_error;            // error not reading 0 between start and data
        return 10;
    }
    sdi_waitBit();

    for(i=7;i>=1;i--)                               // should be now reading data, warnin lsb first
    {
        character.rx_octet = (GPIO_read(SDI12_Serial_data_line) <<i); // read and place bits in right order
        sdi_waitBit();
    }

    if(GPIO_read(SDI12_Serial_data_line)!=parity_even(character.rx_octet)) //reading parity bits and check for matching
    {
        //if different then error
       // sdi_error=parity_error;
        return 11;
    }

    sdi_waitBit();

    if (GPIO_read(SDI12_Serial_data_line)!=1)       //reading stop bits
    {
       // sdi_error=stop_error;                       //error stop bit not 1
        return 12;
    }
        sdi_waitBit();

    return 1;//return 1 to confirm that all is alright
}

*/
/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* 1 second delay */
    uint32_t time = 1;

    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SDSPI_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    while (1) {
        sleep(time);
        GPIO_toggle(Board_GPIO_LED0);
    }
}
