#ifndef _BIT_MATH_H_
#define _BIT_MATH_H_

#include "Std_Types.h"

#define SET_BIT(REG,PIN_NUM) (REG) |= (1<<(PIN_NUM))
#define CLR_BIT(REG,PIN_NUM) (REG) &= ~(1<<(PIN_NUM))
#define TOGGLE_BIT(REG,PIN_NUM) (REG) ^= (1<<(PIN_NUM))
#define GET_BIT(REG,PIN_NUM) (((REG)>>(PIN_NUM)) & 1)
#define REG(x)     (*((volatile u32 *)(x)))

#endif
