/*
 * sdi.h
 *
 *  Created on: 27 jun. 2017
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

//#include "Gpio_function.h"
//#include "Timer_function.h"

#include "GPTimerCC26XX.h"
#include "GPIOCC26XX.h"


#include "Gpio_function.h"
#ifndef SDI_H_
#define SDI_H_


#define CHAR_CR     0x0d
#define CHAR_LF     0x0a
/*
 *  ======== variables ========
 */
#define SDI12_TIMER GPT0_BASE
#define SDI12_Serial_data_line Board_DIO22
#define LED_RED Board_GPIO_LED0
#define LED_GREEN Board_GPIO_LED1

enum sdi12_low_state_e{
    IDLE,
    TRANSMIT,
    RECEIVE
};
typedef enum sdi12_low_state_e sdi12_low_state_t;

enum sdi12_communication_state_status_e{
    SD12_BREAK      = -3,
    SD12_MARKING    =-2,
    SD12_WAKE_END   =-1,
    SD12_START      =0,
    SD12_BIT0       =1,
    SD12_BIT1       =2,
    SD12_BIT2       =3,
    SD12_BIT3       =4,
    SD12_BIT4       =5,
    SD12_BIT5       =6,
    SD12_BIT6       =7,
    SD12_PARITY     =8,
    SD12_STOP       =9,
    SD12_END        = 10
};
typedef enum sdi12_communication_state_status_e sdi12_communication_state_status_t;

enum error_flag_e{
    clear                   =0,
    start_error             =10,
    parity_error            =11,
    stop_error              =12,
    format_error            =13,
    answer_too_long         =14,
    addr_error              =15,
    change_addr_failed      =16,
    read_dio_failed         =17
};

typedef enum error_flag_e error_flag_t;

int Parity_Even(uint8_t RCV_Buffer[9]);

unsigned char Reverse_Byte(char a);

void Sdi_Set_State_Data_Line(sdi12_low_state_t line); //index is SDI12_Serial_data_line


void Sdi_Wake_Sensor();


///*** Timer A will be use for creating bits and timer B will be use for creating SDI frames***///

void Sdi_Transmit_Character(uint8_t data_to_send);

void Sdi_Receive_Character();


uint8_t Sdi_array_to_uint8_t(uint8_t Buffer[]);

void init_component(void);



void TimerBegin(/*GPTimerCC26XX_Handle *hTimerA,GPTimerCC26XX_Handle *hTimerB,GPTimerCC26XX_Params params*/);//timer A et timer B

/*
 * break : 99  6240 = 13ms
 * marking : 99  4320 = 8.33ms
 */

void Set_TimerA(int prescaler,int value,GPTimerCC26XX_Handle hTimerA);

void Set_TimerB(int prescaler,int value,GPTimerCC26XX_Handle hTimerB);

void Launch_TimerA(GPTimerCC26XX_Handle hTimerA);

void Launch_TimerB(GPTimerCC26XX_Handle hTimerB);

void Disable_TimerA(GPTimerCC26XX_Handle hTimerA);

void Disable_TimerB(GPTimerCC26XX_Handle hTimerB);

void ClearTimerAFlag();

void ClearTimerBFlag();


#endif /* SDI_H_ */
