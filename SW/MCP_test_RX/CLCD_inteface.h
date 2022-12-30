/*********************************************************************/
/*********************************************************************/
/******************    Author : AbdElrahman Elnahas      *************/
/******************    Layer : HAL                       *************/ 
/******************    SWC: CLCD                         *************/
/******************    Version : 1.00=                   *************/
/*********************************************************************/
/*********************************************************************/

/*Processor file guard*/
#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_


void CLCD_voidSendCommand(u8 Copy_u8Command);

void CLCD_voidSendData(u8 Copy_u8Data);

void CLCD_voidInit(void);

void CLCD_voidSendString(const char * Copy_pcString );



#endif
