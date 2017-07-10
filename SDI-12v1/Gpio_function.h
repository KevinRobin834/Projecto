/*
 * Gpio_function.h
 *
 *  Created on: 20 jun. 2017
 *      Author: kero
 */

#ifndef GPIO_FUNCTION_H_
#define GPIO_FUNCTION_H_

//******If GPIO driver does not work, try to use this *******//
void ClearFlagInterrupt_DIO_22(void);

void Config_DIO_22_Interrupt(void);

void DIO_22_ClearPreviousState(void);

void DIO_22_ConfigAsInput();

void DIO_22_ConfigAsOutput();

void DIO_22_State_1(void);//do not forget to config first as output

void DIO_22_State_0(void);//do not forget to config first as output







#endif /* GPIO_FUNCTION_H_ */
