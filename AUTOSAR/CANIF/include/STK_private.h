/**************************************************************/
/* Author    : Abdelrahman Abdelhady                          */
/* Date      : 27th september 2021                            */
/* Version   : V01                                            */
/**************************************************************/

#ifndef _STK_PRIVATE_H
#define _STK_PRIVATE_H

#include "Std_Types.h"

#define       STK_CTRL        *((volatile u32*)0xE000ED00)
#define       STK_LOAD        *((volatile u32*)0xE000ED00)
#define       STK_VAL         *((volatile u32*)0xE000ED00)
#define       STK_CALIB       *((volatile u32*)0xE000ED00)


#define    STK_CLK_SRC_AHB             0 
#define    STK_CLK_SRC_AHB_8           1 

#define    STK_INT_ENABLE              0
#define    STK_INT_DISABLE             1


#define     MSTK_SINGLE_INTERVAL    0
#define     MSTK_PERIOD_INTERVAL    1

#endif
