/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*****************     Author : AbdElrahman Elnahas      *************/
/******************    Layer : MCAL                      *************/ 
/******************    SWC: PORT                         *************/
/******************    Version : 1.00=                   *************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

#include"STD_TYPE.h"
#include"BIT_MATH.h"

#include"PORT_register.h"
#include"PORT_config.h"
#include"PORT_interface.h"
#include"PORT_private.h"

void PORT_voidInit(void)
{
	DDRA=PORTA_DIR ;
	DDRB=PORTB_DIR ;
	DDRC=PORTC_DIR ;
	DDRD=PORTD_DIR ;
	
	PORTA=PORTA_INIT_VAL ;
	PORTB=PORTB_INIT_VAL ;
	PORTC=PORTC_INIT_VAL ;
    PORTD=PORTD_INIT_VAL ;

}
