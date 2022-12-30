#include "Port.h"
#include "Port_MemMap.h"


static ModuleStateType module_state = UNINIT ;
static const Port_ConfigType* port_global_config = NULL_PTR ;








static Port_ID get_port_baseaddress ( Pin_ID pin_id  , volatile u32**base_address)
{
	Port_ID port ;
	port = pin_id / 16 ;
	switch ( port )
	{
	case 0 :
		port = PORTA ;
		*base_address = ((volatile u32*)GPIOA_Base_Address) ;
		break ;

	case 1 :
		port = PORTB ;
		*base_address = ((volatile u32*)GPIOB_Base_Address) ;
		break ;

	case 2 :
		port = PORTC ;
		*base_address = ((volatile u32*)GPIOC_Base_Address) ;
		break ;
	}

	return port ;
}

static Pin_ID get_pin_num ( u8 pin_id )
{
	Pin_ID pin ;
	pin = pin_id % 16 ;
	return pin ;
}



void Port_Init( const Port_ConfigType* ConfigPtr )
{

	volatile u32 * base_address ;
	Pin_ID pin_num = 0 ;
	//Port_ID port ;
	u8 pin ;

	if ( ConfigPtr == NULL_PTR )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_INIT_FAILED
#endif
		return ;
	}

	else if (module_state != UNINIT )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_INIT_FAILED
#endif
		return ;
	}

	port_global_config = ConfigPtr ;

	for ( pin_num = 0 ; pin_num < TOTAL_NUM_OF_PINS ; pin_num++)
	{
		get_port_baseaddress( pin_num , &base_address );
		pin = get_pin_num ( pin_num );
		switch ( ConfigPtr->Port_Channels[pin_num].direction)
		{
		case PORT_PIN_IN :
			if ( pin <= 7 )
			{
				REG(base_address + GPIO_CRL ) &= ~(0x00000003<< (pin_num*4) );
			}
			else if ( pin <= 15 )
			{
				pin = pin - 8 ;
				REG(base_address + GPIO_CRH ) &= ~(0x00000003<< (pin_num*4) );
			}
			break ;

		case PORT_PIN_OUT :
			if ( pin <= 7 )
			{
				REG(base_address + GPIO_CRL ) &= ~(0x00000003<< (pin_num*4) );
				switch ( ConfigPtr->Port_Channels[pin_num].PinSpeed )
				{
				case OUTPUT_SPEED_2MHZ :
					REG(base_address + GPIO_CRL ) |= (0x00000002<< (pin_num*4) );
					break ;

				case OUTPUT_SPEED_10MHZ :
					REG(base_address + GPIO_CRL ) |= (0x00000001<< (pin_num*4) );
					break ;

				case OUTPUT_SPEED_50MHZ :
					REG(base_address + GPIO_CRL ) |= (0x00000003<< (pin_num*4) );
					break ;
				}

			}
			else if ( pin <= 15 )
			{
				pin = pin - 8 ;
				REG(base_address + GPIO_CRH ) &= ~(0x00000003<< (pin_num*4) );
				switch ( ConfigPtr->Port_Channels[pin_num].PinSpeed )
				{
				case OUTPUT_SPEED_2MHZ :
					REG(base_address + GPIO_CRH ) |= (0x00000002<< (pin_num*4) );
					break ;

				case OUTPUT_SPEED_10MHZ :
					REG(base_address + GPIO_CRH ) |= (0x00000001<< (pin_num*4) );
					break ;

				case OUTPUT_SPEED_50MHZ :
					REG(base_address + GPIO_CRH ) |= (0x00000003<< (pin_num*4) );
					break ;
				}
			}
			break ;
		}
		pin = get_pin_num ( pin_num );
		if ( ConfigPtr->Port_Channels[pin_num].direction == PORT_PIN_IN )
		{
			switch (ConfigPtr->Port_Channels[pin_num].Input_type)
			{
			case PIN_INPUT_ANALOG :
				if ( pin <= 7 )
				{
					REG(base_address + GPIO_CRL ) &= ~(0x000000C<< (pin_num*4) );
				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					REG(base_address + GPIO_CRH ) &= ~(0x000000C<< (pin_num*4) );
				}
				break ;

			case PIN_INPUT_FLOATING :
				if ( pin <= 7 )
				{
					REG(base_address + GPIO_CRL ) &= ~(0x000000C<< (pin_num*4) );
					REG(base_address + GPIO_CRL ) |= (0x00000004<< (pin_num*4) );

				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					REG(base_address + GPIO_CRH ) &= ~(0x000000C<< (pin_num*4) );
					REG(base_address + GPIO_CRH ) |= (0x00000004<< (pin_num*4) );
				}
				break ;

			case PIN_INPUT_PULLDOWN :
				if ( pin <= 7 )
				{
					REG(base_address + GPIO_CRL ) &= ~(0x000000C<< (pin_num*4) );
					REG(base_address + GPIO_CRL ) |= (0x00000008<< (pin_num*4) );
					CLR_BIT(REG(base_address+GPIO_ODR),pin_num);

				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					REG(base_address + GPIO_CRH ) &= ~(0x000000C<< (pin_num*4) );
					REG(base_address + GPIO_CRH ) |= (0x00000008<< (pin_num*4) );
					CLR_BIT(REG(base_address+GPIO_ODR),pin_num);
				}
				break ;

			case PIN_INPUT_PULLUP :
				if ( pin <= 7 )
				{
					REG(base_address + GPIO_CRL ) &= ~(0x000000C<< (pin_num*4) );
					REG(base_address + GPIO_CRL ) |= (0x00000008<< (pin_num*4) );
					SET_BIT(REG(base_address+GPIO_ODR),pin_num);

				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					REG(base_address + GPIO_CRH ) &= ~(0x000000C<< (pin_num*4) );
					REG(base_address + GPIO_CRH ) |= (0x00000008<< (pin_num*4) );
					SET_BIT(REG(base_address+GPIO_ODR),pin_num);
				}
				break ;
			}
		}
		else if ( ConfigPtr->Port_Channels[pin_num].direction == PORT_PIN_OUT )
		{
			switch ( ConfigPtr->Port_Channels[pin_num].Output_Type)
			{
			case OUTPUT_PUSH_PULL :
				if ( pin <= 7 )
				{

				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					CLR_BIT( REG(base_address + GPIO_CRH ) , 2 * pin_num );
				}
				break ;

			case OUTPUT_OPEN_DRAIN :
				if ( pin <= 7 )
				{
					SET_BIT( REG(base_address + GPIO_CRL ) , 2 * pin_num );
				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					SET_BIT( REG(base_address + GPIO_CRH ) , 2 * pin_num );
				}
				break ;
			}
			pin = get_pin_num ( pin_num );

			switch ( ConfigPtr->Port_Channels[pin_num].PinMode)
			{
			case AFN :
				if ( pin <= 7 )
				{
					SET_BIT( REG(base_address + GPIO_CRL ) , 3 * pin_num );
				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					SET_BIT( REG(base_address + GPIO_CRH ) , 3 * pin_num );
				}
				break ;

			case DIO :
				if ( pin <= 7 )
				{
					CLR_BIT( REG(base_address + GPIO_CRL ) , 3 * pin_num );
				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					CLR_BIT( REG(base_address + GPIO_CRH ) , 3 * pin_num );
				}
				break ;
			}
			pin = get_pin_num ( pin_num );

			switch ( ConfigPtr->Port_Channels[pin_num].initial_value)
			{
			case GPIO_LOW :
				CLR_BIT( REG(base_address + GPIO_ODR ) , pin_num );
				break;
			case GPIO_HIGH :
				SET_BIT( REG(base_address + GPIO_ODR ) , pin_num );
				break ;
			}
		}
	}
	module_state = READY ;
}


void Port_SetPinDirection( Port_PinType Pin,Port_PinDirectionType Direction )
{
	//u8 port ;
	volatile u32 * base_address ;
	u8 pin_num ;

	if ( module_state == UNINIT )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_UNINIT
#endif
		return ;
	}

	else if ( Pin > TOTAL_NUM_OF_PINS )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_PARAM_PIN
#endif
		return ;
	}
	else if ( port_global_config->Port_Channels[Pin].pin_direction_changeable == NOT_CHANGEABLE )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_DIRECTION_UNCHANGEABLE
#endif
		return ;
	}

	get_port_baseaddress(Pin , &base_address ) ;
	pin_num = get_pin_num(Pin);
	switch ( Direction )
	{
	case PORT_PIN_IN :
		if ( pin_num <= 7 )
		{
			REG ( base_address + GPIO_CRL ) &= ~(0x00000003<<pin_num);
		}
		else if ( pin_num <= 15 )
		{
			pin_num = pin_num - 8 ;
			REG ( base_address + GPIO_CRH ) &= ~(0x00000003<<pin_num);
		}
		break ;

	case PORT_PIN_OUT :
		if ( pin_num <= 7 )
		{
			REG ( base_address + GPIO_CRL ) &= ~(0x00000003<<pin_num);
			REG ( base_address + GPIO_CRL ) |=  (0x00000002<<pin_num);
		}
		else if ( pin_num <= 15 )
		{
			pin_num = pin_num - 8 ;
			REG ( base_address + GPIO_CRH ) &= ~(0x00000003<<pin_num);
			REG ( base_address + GPIO_CRH ) |=  (0x00000002<<pin_num);
		}
		break ;

	}

}


void Port_RefreshPortDirection( void )
{
	u8 pin_num , pin ;
	volatile u32 * base_address ;

	if ( module_state == UNINIT)
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_UNINIT
#endif
		return ;
	}
	for ( pin_num = 0 ; pin_num < TOTAL_NUM_OF_PINS ; pin_num++)
	{
		if ( port_global_config->Port_Channels[pin_num].pin_direction_changeable == NOT_CHANGEABLE)
		{
			continue ;
		}
		else
		{
			get_port_baseaddress( pin_num , &base_address );
			pin = get_pin_num ( pin_num );
			switch ( port_global_config->Port_Channels[pin_num].direction)
			{
			case PORT_PIN_IN :
				if ( pin <= 7 )
				{
					REG(base_address + GPIO_CRL ) &= ~(0x00000003<< (pin_num*4) );
				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					REG(base_address + GPIO_CRH ) &= ~(0x00000003<< (pin_num*4) );
				}
				break ;

			case PORT_PIN_OUT :
				if ( pin <= 7 )
				{
					REG(base_address + GPIO_CRL ) &= ~(0x00000003<< (pin_num*4) );
					switch ( port_global_config->Port_Channels[pin_num].PinSpeed )
					{
					case OUTPUT_SPEED_2MHZ :
						REG(base_address + GPIO_CRL ) |= (0x00000002<< (pin_num*4) );
						break ;

					case OUTPUT_SPEED_10MHZ :
						REG(base_address + GPIO_CRL ) |= (0x00000001<< (pin_num*4) );
						break ;

					case OUTPUT_SPEED_50MHZ :
						REG(base_address + GPIO_CRL ) |= (0x00000003<< (pin_num*4) );
						break ;
					}

				}
				else if ( pin <= 15 )
				{
					pin = pin - 8 ;
					REG(base_address + GPIO_CRH ) &= ~(0x00000003<< (pin_num*4) );
					switch ( port_global_config->Port_Channels[pin_num].PinSpeed )
					{
					case OUTPUT_SPEED_2MHZ :
						REG(base_address + GPIO_CRH ) |= (0x00000002<< (pin_num*4) );
						break ;

					case OUTPUT_SPEED_10MHZ :
						REG(base_address + GPIO_CRH ) |= (0x00000001<< (pin_num*4) );
						break ;

					case OUTPUT_SPEED_50MHZ :
						REG(base_address + GPIO_CRH ) |= (0x00000003<< (pin_num*4) );
						break ;
					}
				}
				break ;
			}
		}
	}
}


void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
	if ( versioninfo == NULL_PTR)
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//dev report error PORT_E_PARAM_POINTER
		return ;
	}
#endif
	versioninfo->instanceID = PORT_INSTANCE_ID ;
	versioninfo->moduleID = PORT_MODULE_ID ;
	versioninfo->sw_major_version = PORT_SW_MAJOR_VERSION  ;
	versioninfo->sw_minor_version = PORT_SW_MINOR_VERSION ;
	versioninfo->sw_patch_version = PORT_SW_PATCH_VERSION ;
	versioninfo->vendorID = PORT_VENDOR_ID ;

}




void Port_SetPinMode( Port_PinType Pin,Port_PinModeType Mode )
{
	//u8 port ;
	volatile u32 * base_address ;
	u8 pin_num ;

	if ( module_state == UNINIT )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_UNINIT
#endif
		return ;
	}

	else if ( Pin > TOTAL_NUM_OF_PINS )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_PARAM_PIN
#endif
		return ;
	}
	else if ( port_global_config->Port_Channels[Pin].pin_direction_changeable == NOT_CHANGEABLE )
	{
#if (DEV_ERROR_DETECT == STD_ON)
		//report error PORT_E_DIRECTION_UNCHANGEABLE
#endif
		return ;
	}

	get_port_baseaddress(Pin , &base_address ) ;
	pin_num = get_pin_num(Pin);
	switch ( Mode )
	{
	case DIO :
		if ( pin_num <= 7 )
		{
			REG ( base_address + GPIO_CRL ) &= ~(0x0000000C<<pin_num);
		}
		else if ( pin_num <= 15 )
		{
			REG ( base_address + GPIO_CRH ) &= ~(0x0000000C<<pin_num);
		}
		break ;

	case AFN :
		if ( pin_num <= 7 )
		{
			REG ( base_address + GPIO_CRL ) &= ~(0x0000000C<<pin_num);
			REG ( base_address + GPIO_CRL ) |=  (0x00000004<<pin_num);
		}
		else if ( pin_num <= 15 )
		{
			REG ( base_address + GPIO_CRH ) &= ~(0x0000000C<<pin_num);
			REG ( base_address + GPIO_CRH ) |=  (0x00000004<<pin_num);
		}
		break ;

	}

}
