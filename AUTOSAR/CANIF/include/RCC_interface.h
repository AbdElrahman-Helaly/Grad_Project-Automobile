/**************************************************************/
/* Author    : Abdelrahman Abdelhady                          */
/* Date      : 27th september 2021                            */
/* Version   : V01                                            */
/**************************************************************/

#ifndef _RCC_INTERFACE_H
#define _RCC_INTERFACE_H

#include "Std_Types.h"

#define RCC_AHB	     	0
#define RCC_APB1		1
#define RCC_APB2		2


void RCC_void_EnableClock ( u8 Copy_Bus_Id , u8 Copy_perId );
void RCC_void_DisableClock ( u8 Copy_Bus_Id , u8 Copy_perId );
void RCC_voidInitSysClock (void);

#endif
