/**************************************************************/
/* Author    : Abdelrahman Abdelhady                          */
/* Date      : 27th september 2021                            */
/* Version   : V01                                            */
/**************************************************************/

#include "Std_Types.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "RCC_private.h"
#include "RCC_config.h"


void RCC_void_EnableClock ( u8 Copy_Bus_Id , u8 Copy_perId )
{
	if ( Copy_perId <=31 )
	{
		switch ( Copy_Bus_Id ) 
		{
			case RCC_AHB  : SET_BIT( RCC_AHBENR  , Copy_perId ) ;  break ;
			case RCC_APB1 : SET_BIT( RCC_APB1ENR , Copy_perId ) ;  break ;
			case RCC_APB2 : SET_BIT( RCC_APB2ENR , Copy_perId ) ;  break ;
		}
	}
	else 
	{
		/* return error */
	}
}

void RCC_void_DisableClock ( u8 Copy_Bus_Id , u8 Copy_perId )
{
	if ( Copy_perId <=31 )
	{
		switch ( Copy_Bus_Id ) 
		{
			case RCC_AHB  : CLR_BIT( RCC_AHBENR  , Copy_perId ) ;  break ;
			case RCC_APB1 : CLR_BIT( RCC_APB1ENR , Copy_perId ) ;  break ;
			case RCC_APB2 : CLR_BIT( RCC_APB2ENR , Copy_perId ) ;  break ;
		}
	}
	else 
	{
		/* return error */
	}
}

void RCC_voidInitSysClock (void)
{
	
	#if     RCC_CLOCK_TYPE == RCC_HSE_CRYSTAL
		RCC_CR   = 0x00010000; /* Enable HSE with no bypass */
		RCC_CFGR = 0x00000001;
		
	#elif   RCC_CLOCK_TYPE == RCC_HSE_RC
		RCC_CR   = 0x00050000; /* Enable HSE with bypass */
		RCC_CFGR = 0x00000001;
		
	#elif   RCC_CLOCK_TYPE == RCC_HSI
		RCC_CR   = 0x00000081; /* Enable HSI + Trimming = 0 */
		RCC_CFGR = 0x00000000;
	
	#elif   RCC_CLOCK_TYPE == RCC_PLL
		#if   RCC_PLL_INPUT == RCC_PLL_IN_HSI_DIV_2
		
		#elif RCC_PLL_INPUT == RCC_PLL_IN_HSE_DIV_2
	
		#elif RCC_PLL_INPUT == RCC_PLL_IN_HSE
		
		#endif
	
	#else
		#error("You chosed Wrong Clock type")
	#endif
}
