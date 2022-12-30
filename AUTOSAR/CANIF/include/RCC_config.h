/**************************************************************/
/* Author    : Abdelrahman Abdelhady                          */
/* Date      : 27th september 2021                            */
/* Version   : V01                                            */
/**************************************************************/

#ifndef _RCC_CONFIG_H
#define _RCC_CONFIG_H

/* Options :  	RCC_HSE_CRYSTAL     
				RCC_HSE_RC   	    
				RCC_HSI		        
				RCC_PLL  	        
*/
#define RCC_CLOCK_TYPE      RCC_HSE_CRYSTAL
//-----------------------------------------

/* Options :  	RCC_PLL_IN_HSI_DIV_2     
				RCC_PLL_IN_HSE_DIV_2   	    
				RCC_PLL_IN_HSE		          	        
*/
#if RCC_CLOCK_TYPE == RCC_PLL
	#define RCC_PLL_INPUT      RCC_PLL_IN_HSE_DIV_2
#endif
//------------------------------------------

/* Options :  	2:16		   */
#if RCC_CLOCK_TYPE == RCC_PLL
	#define RCC_PLL_MUL      4
#endif



#endif