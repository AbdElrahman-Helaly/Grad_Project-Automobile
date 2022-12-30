#ifndef PORT_H_
#define PORT_H_

#include "Std_Types.h"
#include "BIT_MATH.h"
#include "Port_Cfg.h"



#define PORT_VENDOR_ID  		   	    (2022U)
#define PORT_MODULE_ID    			    (124U)
#define PORT_INSTANCE_ID 			    (0U)
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)




typedef enum {
	PORT_E_PARAM_PIN = 0x0A ,
	PORT_E_DIRECTION_UNCHANGEABLE,
	PORT_E_INIT_FAILED,
	PORT_E_PARAM_INVALID_MODE,
	PORT_E_MODE_UNCHANGEABLE,
	PORT_E_UNINIT,
	PORT_E_PARAM_POINTER
}Dev_Errors;



typedef u8 Port_PinType ;


typedef enum {
	PORT_PIN_IN ,
	PORT_PIN_OUT
}Port_PinDirectionType;


typedef u8 Port_PinModeType ;
#define	   DIO		(u8)0
#define	   AFN      (u8)1



typedef enum {
	UNINIT ,
	READY
}ModuleStateType;



typedef enum {
	PINA0 = 0 , PINA1 , PINA2 , PINA3 , PINA4 , PINA5 , PINA6 , PINA7 , PINA8 , PINA9 ,
	PINA10 , PINA11 , PINA12 , PINA13 , PINA14 , PINA15 , PINB0 , PINB1 , PINB2 ,
	PINB3 , PINB4 , PINB5 , PINB6 , PINB7 , PINB8 , PINB9 , PINB10 , PINB11 , PINB12 , PINB13 ,
	PINB14 , PINB15 , PINC13 , PINC14 , PINC15
}Pin_ID ;


typedef enum {
	PORTA ,
	PORTB ,
	PORTC
}Port_ID ;


typedef enum {
	GPIO_LOW ,
	GPIO_HIGH
}Pin_Output_Level;

typedef enum {
	OUTPUT_PUSH_PULL ,
	OUTPUT_OPEN_DRAIN
}Pin_Output_Type;


typedef enum {
	PIN_INPUT_PULLUP,
	PIN_INPUT_PULLDOWN,
	PIN_INPUT_FLOATING,
	PIN_INPUT_ANALOG
}Pin_Input_Type ;

typedef enum {
	OUTPUT_SPEED_2MHZ ,
	OUTPUT_SPEED_10MHZ ,
	OUTPUT_SPEED_50MHZ
}Pin_Output_Speed;

typedef enum {
	NOT_CHANGEABLE ,
	CHANGEABLE
}Pin_Change_state;



typedef struct {
	u8 PinMode ;
	Port_PinDirectionType direction;
	Pin_Output_Level          initial_value;
	Pin_Input_Type		  Input_type;
	Pin_Output_Speed	PinSpeed ;
	Pin_Output_Type		Output_Type ;
	Pin_Change_state   pin_direction_changeable;
	Pin_Change_state     pin_mode_changeable;
}Port_Config_Channel;


typedef struct {
	Port_Config_Channel 	Port_Channels [ TOTAL_NUM_OF_PINS ] ;
}Port_ConfigType;


/*
void Port_Init( const Port_ConfigType* ConfigPtr );
void Port_SetPinDirection( Port_PinType Pin,Port_PinDirectionType Direction );
void Port_RefreshPortDirection( void );
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
void Port_SetPinMode( Port_PinType Pin,Port_PinModeType Mode );
*/



void Port_Init( const Port_ConfigType* ConfigPtr );
void Port_SetPinDirection( Port_PinType Pin,Port_PinDirectionType Direction );
void Port_RefreshPortDirection( void );
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
void Port_SetPinMode( Port_PinType Pin,Port_PinModeType Mode );





#endif
