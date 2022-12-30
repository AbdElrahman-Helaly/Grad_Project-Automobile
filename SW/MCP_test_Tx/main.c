#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "SPI_Interface.h"
#include "MCP2515_Interface.h"
#include "UART_interface.h"
#include <util/delay.h>

void main(void)
{

	MCP2515_VidInit();

	DIO_VidSetPinDirection(DIO_PORTD,DIO_PIN0,DIO_INPUT);
	DIO_VidSetPinDirection(DIO_PORTD,DIO_PIN1,DIO_OUTPUT);


	UART_voidInit();

	u8 Var = 0 ;

	while(1)
	{
		Var = UART_voidGetChar();

		MCP2515_VidSendCANmsg(0, 0x7E1, &Var, 1);

	}
}
