/**************************************************************/
/* Author    : Abdelrahman Abdelhady                          */
/* Date      : 27th september 2021                            */
/* Version   : V01                                            */
/**************************************************************/

#include "Std_Types.h"
#include "BIT_MATH.h"

#include "STK_interface.h"
#include "STK_private.h"
#include "STK_config.h"

/* Define Variable for interval mode */
static u8 MSTK_u8ModeOfInterval;

void (*MSTK_CallBack) ( void ) ;

void MSTK_voidInit(void)
{
	/****** adjust clock source *********/
	if ( STK_CLK_SRC == STK_CLK_SRC_AHB )
	{
		SET_BIT(STK_CTRL , 2 ) ;
	}
	else if ( STK_CLK_SRC == STK_CLK_SRC_AHB_8 )
	{
		CLR_BIT(STK_CTRL , 2 ) ;
	}
	
	/****** adjust interrupt enabling *********/
	if ( STK_INT_STATUS == STK_INT_ENABLE )
	{
		SET_BIT(STK_CTRL , 1 ) ;
	}
	else if ( STK_INT_STATUS == STK_INT_DISABLE )
	{
		CLR_BIT(STK_CTRL , 1 ) ;
	}
	
	
	/******* disable SysTick **********/
	CLR_BIT(STK_CTRL , 0 ) ;
	
	
}




void MSTK_voidSetBusyWait( u32 Copy_u32Ticks )
{
	u8 Local_u8Temp ;
	STK_LOAD = Copy_u32Ticks ;
	/******* Enable SysTick **********/
	SET_BIT(STK_CTRL , 0 ) ;
	while (  GET_BIT ( STK_CTRL , 16 ) == 0 ) ;
	SET_BIT(STK_CTRL , 0 );
	STK_VAL = 0 ;
	STK_LOAD = 0 ;
	
}


void MSTK_voidSetIntervalSingle  ( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	CLR_BIT( STK_CTRL , 0 ) ;
	STK_VAL = 0 ;
	
	/* load ticks to load register*/
	STK_LOAD = Copy_u32Ticks ;
	
	
	/* Set CAll back function */
	MSTK_CallBack = Copy_ptr ;
	
	
	/* enable interrupt */
	SET_BIT(STK_CTRL , 1 ) ;
	
	/******* Enable SysTick **********/
	SET_BIT(STK_CTRL , 0 ) ;
}

void MSTK_voidSetIntervalPeriodic( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	/* Load ticks to load register */
  STK_LOAD = Copy_u32Ticks;
	
	/* Start Timer */
	SET_BIT(STK_CTRL, 0);
	
	/* Save CallBack */
	MSTK_CallBack = Copy_ptr;
	
	/* Set Mode to Single */
	MSTK_u8ModeOfInterval = MSTK_PERIOD_INTERVAL;
	
	/* Enable STK Interrupt */
	SET_BIT(STK_CTRL, 1);
}

void MSTK_voidStopInterval(void)
{
	/******* disable SysTick **********/
	CLR_BIT(STK_CTRL , 0 ) ;
	
}

u32  MSTK_u32GetElapsedTime(void)
{
	u32 Local_u8Temp = STK_LOAD - STK_VAL ;
	
	return  Local_u8Temp ;
}

u32  MSTK_u32GetRemainingTime(void)
{
	return STK_VAL ;
	
}


void Sys_Tick_Handler ( void ) 
{
	u8 Local_u8Temporary ;
	if (MSTK_u8ModeOfInterval == MSTK_SINGLE_INTERVAL)
	{
		/* Disable STK Interrupt */
		CLR_BIT(STK_CTRL, 1);
	
		/* Stop Timer */
		SET_BIT(STK_CTRL, 0);
		STK_LOAD = 0;
		STK_VAL  = 0;
	}
	
	/* Callback notification */
	MSTK_CallBack();
	
	/* Clear interrupt flag */
	Local_u8Temporary = GET_BIT(STK_CTRL,16);
}







