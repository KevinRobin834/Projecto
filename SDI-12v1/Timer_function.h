/*
 * Timer_function.h
 *
 *  Created on: 20 jun. 2017
 *      Author: kero
 */

#ifndef TIMER_FUNCTION_H_
#define TIMER_FUNCTION_H_

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
void TimerConfigs();//timer A et timer B

/*
 * break : 99  6240 = 13ms
 * marking : 99  4320 = 8.33ms
 */

void Set_TimerA(int prescaler,int value);

void Set_TimerB(int prescaler,int value);

void Launch_TimerA();

void Launch_TimerB();

void Disable_TimerA();

void Disable_TimerB();

void ClearTimerAFlag();

void ClearTimerBFlag();


#endif /* TIMER_FUNCTION_H_ */
