/*********************************************************************/
/*********************************************************************/
/******************    Author : AbdElrahman Elnahas      *************/
/******************    Layer : HAL                       *************/
/******************    SWC: CLCD                         *************/
/******************    Version : 1.00=                   *************/
/*********************************************************************/
/*********************************************************************/

#include"STD_TYPE.h"
#include<util/delay.h>

#include"DIO_interface.h"
#include"CLCD_inteface.h"
#include"CLCD_config.h"


void CLCD_voidSendCommand(u8 Copy_u8Command)
{
	/*Set RS as command*/
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_LOW);

	/*Set RW as write*/
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_LOW);

	/*Send command on data pins*/
	DIO_VidSetPortValue(CLCD_DATA_PORT,Copy_u8Command);

	/*Set Enable pulse*/
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_HIGH);
	_delay_ms(2);
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_LOW);
}


void CLCD_voidSendData(u8 Copy_u8Data)
{
	/*Set RS as Data*/
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_HIGH);

	/*Set RW as write*/
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_LOW);

	/*Send command on data pins*/
	DIO_VidSetPortValue(CLCD_DATA_PORT,Copy_u8Data);

	/*Set Enable pulse*/
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_HIGH);
	_delay_ms(2);
	DIO_VidSetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_LOW);
}


void CLCD_voidInit(void)
{
	/*waiting 30 ms*/
	_delay_ms(30);

	/*Function Set*/
	CLCD_voidSendCommand(0b00111000);

	/*Control ON/OFF*/
	CLCD_voidSendCommand(0b00001100);

	/*Clear Screen*/
	CLCD_voidSendCommand(1);
}

void CLCD_voidSendString(const char * Copy_pcString )
{
	u8 Local_I=0; /*initalize*/
	while(*(Copy_pcString + Local_I)!='\0')
	{
		CLCD_voidSendData(*(Copy_pcString + Local_I));
		Local_I++;
	}
}
