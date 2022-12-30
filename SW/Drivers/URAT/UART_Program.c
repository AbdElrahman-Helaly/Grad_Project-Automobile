/********************		Author: ABDELRAHMAN ELSAYED		 ***************/
/********************		Layer: MCAL				         ***************/
/********************		SWC: UART				         ***************/
/********************		Version: 1.00			         ***************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


#include "STD.h"
#include "bit.h"
#include "DIO_interface.h"
#include "UART_Interface.h"
#include "UART_Register.h"

void UART_voidInti(void)
{

	/* Set the direction Output for Tx pin and Input for Rx pin */
	DIO_voidSetPinDirection(DIO_u8PORTD,DIO_u8PIN1,DIO_u8PIN_OUTPUT);
	DIO_voidSetPinDirection(DIO_u8PORTD,DIO_u8PIN0,DIO_u8PIN_INPUT);

	u8 Local_u8UCSRC =0;

	/*to choose register UCSRC*/
	set_bit(Local_u8UCSRC,UCSRC_URSEL);

	/*Asynchronous*/
	set_bit(Local_u8UCSRC,UCSRC_UMSEL);

	/*8-bit*/
	set_bit(Local_u8UCSRC,UCSRC_UCSZ1);
	set_bit(Local_u8UCSRC,UCSRC_USCZ0);
	clear_bit(Local_u8UCSRC,UCSRB_UCSZ2);

	/*no parity*/
	clear_bit(Local_u8UCSRC,UCSRC_UPM0);
	clear_bit(Local_u8UCSRC,UCSRC_UPM1);

	/*1-bit stop*/
	clear_bit(Local_u8UCSRC,UCSRC_USBS);

	UCSRC = Local_u8UCSRC;

	set_bit(UCSRB,UCSRB_TXEN);
	set_bit(UCSRB,UCSRB_RXEN);

	UBRRL =51;

}
u8 UART_Reseve(void)
{
	while(get(UCSRA,UCSRA_RXC)==0);

	return UDR;

}
void UART_send(u8 Copy_Data){
	while(get(UCSRA,UCSRA_UDRE)==0);

	UDR = Copy_Data;

}

