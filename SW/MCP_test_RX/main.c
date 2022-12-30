
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "SPI_Interface.h"
#include "MCP2515_Interface.h"
//#include "UART_Interface.h"
#include <util/delay.h>
#include "CLCD_inteface.h"
#include "PORT_interface.h"
#define SREG *((volatile u8 *) 0x5F)

extern u8 readBuffer[];

void main(void)
{
	PORT_voidInit();
	MCP2515_VidReceiveInit();
	STEPPER_voidInit;

	DIO_VidSetPinDirection(DIO_PORTD, DIO_PIN2, DIO_INPUT);
	DIO_VidSetPinDirection(DIO_PORTD, DIO_PIN5, DIO_OUTPUT);
	//DIO_VidSetPinValue(DIO_PORTD , DIO_PIN5,DIO_HIGH);

	CLCD_voidInit();

	u8 dataR = 0;


	while(1)
	{
		if(DIO_U8GetPinValue(DIO_PORTD, DIO_PIN2) == 0)
		{
			DIO_VidSetPinValue(DIO_PORTD , DIO_PIN5,DIO_HIGH);

			MCP2515_VidReceiveCANmsg();
			DIO_VidSetPinValue(DIO_PORTD , DIO_PIN5,DIO_HIGH);

			dataR = readBuffer[6];

			if (dataR == 'T')
			{
				CLCD_voidInit();

				CLCD_voidSendString("TRUSTED");
				_delay_ms(2000);
				CLCD_voidInit();
				CLCD_voidSendString("CAR IS ACTIVE");
				_delay_ms(2000);
         
		 //FINALLY CAR IS ACTIVE
		 void STEPPER_voidRotateACW(void);


			}
			else if (dataR == 'F')
			{
				DIO_VidSetPinValue(DIO_PORTD , DIO_PIN5,DIO_HIGH);
				_delay_ms(100);
				DIO_VidSetPinValue(DIO_PORTD , DIO_PIN5,DIO_LOW);
				_delay_ms(100);
				CLCD_voidInit();
				CLCD_voidSendString("NOT TRUSTED");


			}

		}

	}
}
