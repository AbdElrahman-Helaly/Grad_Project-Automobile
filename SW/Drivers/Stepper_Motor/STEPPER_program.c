/****************************************************************************/
/****************************************************************************/
/*******************       eng/abdelrahman helaly         *******************/
/*******************        Layer:HAL                     *******************/
/*******************        SEC: Stepper Motor            *******************/
/*******************        version:1.00                  *******************/
/****************************************************************************/
/****************************************************************************/
#include "LIB/TYPS/STD.h"
#include "LIB/MATH/bit.h"
#define F_CPU    8000000UL
#include <util/delay.h>

#include "DIO_interface.h"

#include "STEPPER_interface.h"
#include "STEPPER_config.h"

void STEPPER_voidInit(void)
{
	//Set the direction output for the Pins connected to the Stepper Motor
	DIO_voidSetPinDirection(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_OUTPUT);
}
void STEPPER_voidRotateACW(void)
{
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_HIGH);
	_delay_ms(5);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_HIGH);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_LOW);
	_delay_ms(5);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_HIGH);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_LOW);
	_delay_ms(5);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_HIGH);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_LOW);
	_delay_ms(5);
}
void STEPPER_voidRotateCW(void)
{
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_HIGH);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_LOW);
	_delay_ms(5);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_HIGH);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_LOW);
	_delay_ms(5);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_HIGH);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_LOW);
	_delay_ms(5);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Blue,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Pink,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Yellow,DIO_u8_LOW);
	DIO_voidSetPinValue(STEPPER_u8_COIL_PORT,STEPPER_u8_COIL_Orange,DIO_u8_HIGH);
	_delay_ms(5);
}



