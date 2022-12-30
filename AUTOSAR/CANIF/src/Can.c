#include "Std_Types.h"
#include "BIT_MATH.h"
#include "Can.h"
#include "Can_MemMap.h"
#include "Can_cfg.h"



static Can_StateType CAN_CONTROLLER_STATUS [NUM_OF_CONTOLLERS] = { CAN_CS_UNINIT , CAN_CS_UNINIT } ;
static Can_Module_State module_state = CAN_UNINIT ;
static u8 Disable_Counter[NUM_OF_CONTOLLERS]  ;

static u8 HTH_Flag[MAX_NUM_OF_HOH] = {0};

Can_PduType * PduMsg1 ;
Can_PduType * PduMsg2 ;


CanBaudrateConfig BaudRateCFG[NUM_OF_BAUDRATES];
const Can_ConfigType* Global_config ;


void Can_GetVersionInfo ( Std_VersionInfoType* versioninfo )
{
#if ( Dev_Error_Detect == STD_ON )

	if ( versioninfo == NULL_PTR )
	{
		// Report error function >> CAN_E_PARAM_POINTER
	}
#endif

	versioninfo->moduleID = MODULE_ID ;
	versioninfo->vendorID = VENDOR_ID ;
	versioninfo->instanceID = INSTANCE_ID ;
	versioninfo->sw_major_version = SW_MAJOR_VERSION ;
	versioninfo->sw_minor_version = SW_MINOR_VERSION ;
	versioninfo->sw_patch_version = SW_PATCH_VERSION ;

}

/**************************************************************************************************/

Std_ReturnType Can_SetControllerMode ( u8 Controller, Can_ControllerStateType Transition )
{
	u8 can_status = CAN_CONTROLLER_STATUS[Controller];
	u32 base_address = 0 ;
	u8 return_temp ;


	/**************getting the base address of the can controller********/
	if (Controller == 0)
	{
		base_address = CAN0_BASE_ADDRESS ;
	}
	else if (Controller == 1)
	{
		base_address = CAN1_BASE_ADDRESS ;
	}

	/*[SWS_Can_00198]  If development error detection for the Can module is enabled: if
		the module is not yet initialized, the function Can_SetControllerMode shall raise
		development error CAN_E_UNINIT and return E_NOT_OK.*/

	if (module_state == CAN_UNINIT)
	{
		#if ( Dev_Error_Detect == STD_ON )
		// Report error function >> CAN_E_UNINIT
		#endif
		return_temp = E_NOT_OK ;
	}

	/*[SWS_Can_00199]  If development error detection for the Can module is enabled: if
		the parameter Controller is out of range, the function Can_SetControllerMode
		shall raise development error CAN_E_PARAM_CONTROLLER and return
		E_NOT_OK.*/

	else if ( Controller > NUM_OF_CONTOLLERS )
	{
		#if ( Dev_Error_Detect == STD_ON )
		// Report error function >> CAN_E_PARAM_CONTROLLER
		#endif
		return_temp = E_NOT_OK ;
	}

	/*[SWS_Can_00200]  If development error detection for the Can module is enabled: if
		an invalid transition has been requested, the function Can_SetControllerMode shall
		raise the error CAN_E_TRANSITION and return E_NOT_OK.*/

	else if ( ((Transition == CAN_CS_STARTED)&&(can_status != CAN_CS_STOPPED)) ||
			((Transition ==CAN_CS_SLEEP)&&((can_status!=CAN_CS_SLEEP)||(can_status!=CAN_CS_STOPPED))))
	{
		#if ( Dev_Error_Detect == STD_ON )
		// Report error function >> CAN_E_TRANSITION
		#endif
		return_temp = E_NOT_OK ;
	}

	/**********SETTING NEW MODE****************/
	else
	{
		switch ( Transition )
		{
		case CAN_CS_STARTED :
			REG(base_address + CAN_MCR) &= ~(0x00000003);
			CAN_CONTROLLER_STATUS[Controller] = CAN_CS_STARTED ;
			break;

		case CAN_CS_STOPPED :
			REG(base_address + CAN_MCR) &= ~(0x00000003);
			REG(base_address + CAN_MCR) |= 0x00000001 ;
			CAN_CONTROLLER_STATUS[Controller] = CAN_CS_STOPPED ;
			break;

		case CAN_CS_SLEEP :
			REG(base_address + CAN_MCR) &= ~(0x00000003);
			REG(base_address + CAN_MCR) |= 0x00000002 ;
			CAN_CONTROLLER_STATUS[Controller] = CAN_CS_SLEEP ;
			break;
		default :
			return_temp = E_NOT_OK;
		}
		return_temp = E_OK ;
	}
	return return_temp ;
}

/****************************************************************************************************/

Std_ReturnType Can_GetControllerMode ( u8 Controller, Can_ControllerStateType* ControllerModePtr )
{
	u8 return_temp ;

	/*[SWS_Can_91016]  If development error detection for the Can module is enabled:
		The function Can_GetControllerMode shall raise the error CAN_E_UNINIT and
		return E_NOT_OK if the driver is not yet initialized.(SRS_BSW_00406,SRS_BSW_00416)*/

	if (module_state == CAN_UNINIT)
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_UNINIT
			#endif
			return_temp = E_NOT_OK ;
		}
	/*[SWS_Can_91017]  If parameter Controller of Can_GetControllerMode() has an
		invalid value, the CanDrv shall report development error code
		CAN_E_PARAM_CONTROLLER to the Det_ReportError service of the DET.
		(SRS_BSW_00323)*/

	else if ( Controller > NUM_OF_CONTOLLERS )
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_CONTROLLER
			#endif
			return_temp = E_NOT_OK ;
		}

	/*[SWS_Can_91018]  If parameter ControllerModePtr of Can_GetControllerMode()
	has an null pointer, the CanDrv shall report development error code
	CAN_E_PARAM_POINTER to the Det_ReportError service of the DET.
	(SRS_BSW_00323)*/

	else if ( ControllerModePtr == NULL_PTR )
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_POINTER
			#endif
			return_temp = E_NOT_OK ;
		}

	/****************************getting controller state****************************/
	else
	{
		*ControllerModePtr = CAN_CONTROLLER_STATUS[Controller] ;
	}


	return return_temp ;
}

/****************************************************************************************************/

Std_ReturnType Can_GetControllerErrorState ( u8 ControllerId , Can_ErrorStateType* ErrorStatePtr )
{
	u8 return_temp ;
	u32 base_address = 0 ;

	/**************getting the base address of the can controller********/
		if (ControllerId == 0)
		{
			base_address = CAN0_BASE_ADDRESS ;
		}
		else if (ControllerId == 1)
		{
			base_address = CAN1_BASE_ADDRESS ;
		}


	/*[SWS_Can_91005]  If development error detection for the Can module is enabled: if
		the module is not yet initialized, the function Can_GetControllerErrorState shall raise
		development error CAN_E_UNINIT and return E_NOT_OK. (SRS_BSW_00406,
		SRS_BSW_00416)*/
	if (module_state == CAN_UNINIT)
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_UNINIT
			#endif
			return_temp = E_NOT_OK ;
		}

	/*[SWS_Can_91006] If development error detection for the Can module is enabled: if
		the parameter ControllerId is out of range, the function Can_GetControllerErrorState
		shall raise development error CAN_E_PARAM_CONTROLLER and return
		E_NOT_OK. (SRS_BSW_00323)*/
	else if ( ControllerId > NUM_OF_CONTOLLERS )
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_CONTROLLER
			#endif
			return_temp = E_NOT_OK ;
		}

	/*[SWS_Can_91007]  If development error detection for the Can module is enabled: if
		the parameter ErrorStatePtr is a null pointer, the function
		Can_GetControllerErrorState shall raise development error
		CAN_E_PARAM_POINTER and return E_NOT_OK.(SRS_BSW_00323)*/
	else if ( ErrorStatePtr == NULL_PTR)
	{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_POINTER
			#endif
			return_temp = E_NOT_OK ;
	}
	/*********************************getting error state********************************/
	else
	{
		if( (REG(base_address + CAN_ESR)&0x7) == 0b001 )
		{
			*ErrorStatePtr = CAN_ERRORSTATE_ACTIVE;
		}
		else if ( (REG(base_address + CAN_ESR)&0x7) == 0b010)
		{
			*ErrorStatePtr = CAN_ERRORSTATE_PASSIVE;
		}
		else if ( (REG(base_address + CAN_ESR)&0x7) == 0b100)
		{
			*ErrorStatePtr = CAN_ERRORSTATE_BUSOFF;
		}

		return_temp = E_OK ;
	}

	return return_temp ;
}

/****************************************************************************************************/

Std_ReturnType Can_GetControllerRxErrorCounter ( u8 ControllerId, u8* RxErrorCounterPtr )
{
	u8 return_temp ;
	u32 base_address = 0 ;
	u32 rx_counter_temp = 0;

	/**************getting the base address of the can controller********/
	if (ControllerId == 0)
	{
		base_address = CAN0_BASE_ADDRESS ;
	}
	else if (ControllerId == 1)
	{
		base_address = CAN1_BASE_ADDRESS ;
	}
	/**************************************************************************/

	/*[SWS_Can_00512]  If development error detection for the Can module is enabled: if
		the module is not yet initialized, the function Can_GetControllerRxErrorCounter shall
		raise development error CAN_E_UNINIT and return E_NOT_OK.
		(SRS_BSW_00406)*/
	if (module_state == CAN_UNINIT)
	{
		#if ( Dev_Error_Detect == STD_ON )
		// Report error function >> CAN_E_UNINIT
		#endif
		return_temp = E_NOT_OK ;
	}

	/*[SWS_Can_00513]  If development error detection for the Can module is enabled: if
	the parameter ControllerId is out of range, the function
	Can_GetControllerRxErrorCounter shall raise development error
	CAN_E_PARAM_CONTROLLER and return E_NOT_OK.  (SRS_BSW_00323)*/
	else if ( ControllerId > NUM_OF_CONTOLLERS )
	{
		#if ( Dev_Error_Detect == STD_ON )
		// Report error function >> CAN_E_PARAM_CONTROLLER
		#endif
		return_temp = E_NOT_OK ;
	}

	/*[SWS_Can_00514]  If development error detection for the Can module is enabled: if
		the parameter RxErrorCounterPtr is a null pointer, the function
		Can_GetControllerRxErrorCounter shall raise development error
		CAN_E_PARAM_POINTER and return E_NOT_OK.  (SRS_BSW_00323)*/
	else if ( RxErrorCounterPtr == NULL_PTR)
	{
		#if ( Dev_Error_Detect == STD_ON )
		// Report error function >> CAN_E_PARAM_POINTER
		#endif
		return_temp = E_NOT_OK ;
	}

	/**********************************getting rx counter**************************/
	else
	{
		rx_counter_temp = REG( base_address + CAN_ESR ) & 0x11000000;
		rx_counter_temp = rx_counter_temp >> 24 ;
		*RxErrorCounterPtr = (u8)rx_counter_temp ;
		return_temp = E_OK;
	}

	return return_temp ;
}

/******************************************************************************************/

Std_ReturnType Can_GetControllerTxErrorCounter ( u8 ControllerId, u8* TxErrorCounterPtr )
{
		u8 return_temp ;
		u32 base_address = 0 ;
		u32 tx_counter_temp = 0;

		/**************getting the base address of the can controller********/
		if (ControllerId == 0)
		{
			base_address = CAN0_BASE_ADDRESS ;
		}
		else if (ControllerId == 1)
		{
			base_address = CAN1_BASE_ADDRESS ;
		}
		/**************************************************************************/

		/*[SWS_Can_00517]  If development error detection for the Can module is enabled: if
			the module is not yet initialized, the function Can_GetControllerRxErrorCounter shall
			raise development error CAN_E_UNINIT and return E_NOT_OK.
			(SRS_BSW_00406)*/
		if (module_state == CAN_UNINIT)
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_UNINIT
			#endif
			return_temp = E_NOT_OK ;
		}

		/*[SWS_Can_00518]  If development error detection for the Can module is enabled: if
		the parameter ControllerId is out of range, the function
		Can_GetControllerRxErrorCounter shall raise development error
		CAN_E_PARAM_CONTROLLER and return E_NOT_OK.  (SRS_BSW_00323)*/
		else if ( ControllerId > NUM_OF_CONTOLLERS )
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_CONTROLLER
			#endif
			return_temp = E_NOT_OK ;
		}

		/*[SWS_Can_00519]  If development error detection for the Can module is enabled: if
			the parameter RxErrorCounterPtr is a null pointer, the function
			Can_GetControllerRxErrorCounter shall raise development error
			CAN_E_PARAM_POINTER and return E_NOT_OK.  (SRS_BSW_00323)*/
		else if ( TxErrorCounterPtr == NULL_PTR)
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_POINTER
			#endif
			return_temp = E_NOT_OK ;
		}

		/**********************************getting tx counter**************************/
		else
		{
			tx_counter_temp = REG( base_address + CAN_ESR ) & 0x00110000;
			tx_counter_temp = tx_counter_temp >> 16 ;
			*TxErrorCounterPtr = (u8)tx_counter_temp ;
			return_temp = E_OK;
		}

		return return_temp ;

}

/************************************************************************************************/

void Can_DeInit ( void )
{
	u8 controllerid = 0 ;
	/*[SWS_Can_91011]  If development error detection for the Can module is enabled:
	The function Can_DeInit shall raise the error CAN_E_TRANSITION if the driver is not
	in state CAN_READY. (SRS_BSW_00369)*/
#if ( Dev_Error_Detect == STD_ON )
	if ( module_state != CAN_READY )
	{
		// Report error function >> CAN_E_TRANSITION
	}

	/*[SWS_Can_91012]  If development error detection for the Can module is enabled:
			The function Can_DeInit shall raise the error CAN_E_TRANSITION if any of the CAN
			controllers is in state STARTED. (SRS_BSW_00369)*/

	for ( controllerid = 0 ; controllerid < NUM_OF_CONTOLLERS ; controllerid ++)
	{
		if ( CAN_CONTROLLER_STATUS[controllerid] == CAN_CS_STARTED )
		{
			// Report error function >> CAN_E_TRANSITION
		}
	}

#endif
	for ( controllerid = 0 ; controllerid < NUM_OF_CONTOLLERS ; controllerid ++)
	{


	}



}

/************************************************************************************************/

void Can_DisableControllerInterrupts ( u8 Controller )
{
	u32 base_address = 0  ;

	/**************getting the base address of the can controller********/

	if (Controller == 0)
	{
		base_address = CAN0_BASE_ADDRESS ;
	}
	else if (Controller == 1)
	{
		base_address = CAN1_BASE_ADDRESS ;
	}
	/************************************************************************/

#if ( Dev_Error_Detect == STD_ON )
	/*[SWS_Can_00205]  If development error detection for the Can module is enabled:
			The function Can_DisableControllerInterrupts shall raise the error CAN_E_UNINIT if
			the driver not yet initialized. ()*/
	if ( module_state == CAN_UNINIT )
	{
		// Report error function >> CAN_E_UNINIT
	}

	/*[SWS_Can_00206]  If development error detection for the Can module is enabled:
		The function Can_DisableControllerInterrupts shall raise the error
		CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range. ()*/

	else if ( Controller > NUM_OF_CONTOLLERS )
	{
		// Report error function >> CAN_E_PARAM_CONTROLLER
	}
#endif

	//disabling interrupts for this controller
	if (Disable_Counter[Controller] == 0 )
	{
		REG(base_address + CAN_IER) = 0 ;
	}


	// increasing disabling counter
	Disable_Counter[Controller]++;

}

/********************************************************************************************/

void Can_EnableControllerInterrupts ( u8 Controller )
{
	u32 base_address  = 0;

	/**************getting the base address of the can controller********/

	if (Controller == 0)
	{
		base_address = CAN0_BASE_ADDRESS ;
	}
	else if (Controller == 1)
	{
		base_address = CAN1_BASE_ADDRESS ;
	}
	/************************************************************************/

#if ( Dev_Error_Detect == STD_ON )
	/*[SWS_Can_00209]  If development error detection for the Can module is enabled:
				The function Can_DisableControllerInterrupts shall raise the error CAN_E_UNINIT if
				the driver not yet initialized. ()*/
	if ( module_state == CAN_UNINIT )
	{
		// Report error function >> CAN_E_UNINIT
	}

	/*[SWS_Can_00210]  If development error detection for the Can module is enabled:
			The function Can_DisableControllerInterrupts shall raise the error
			CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range. ()*/

	else if ( Controller > NUM_OF_CONTOLLERS )
	{
		// Report error function >> CAN_E_PARAM_CONTROLLER
	}
#endif

	if (Disable_Counter[Controller] == 1)
	{
		// code for enabling interrupts
		Disable_Counter[Controller] = 0 ;
	}

	else if ( Disable_Counter[Controller] > 1 )
	{
		Disable_Counter[Controller]-- ;
	}
	else if ( Disable_Counter[Controller] == 0 )
	{
		// do nothing
	}

}

/********************************************************************************************/



void Can_Init (const Can_ConfigType* Config )
{
	u32 base_address ;
	u8 controllerID ;
	CanBaudrateConfig* BRConfig;
	u8 HOHcount = 0 ;
	u8 HRHcount = 0 ;


#if ( Dev_Error_Detect == STD_ON )

	/*[SWS_Can_00174]  If development error detection for the Can module is enabled:
			The function Can_Init shall raise the error CAN_E_TRANSITION if the driver is not in
			state CAN_UNINIT. ()*/
	if ( module_state != CAN_UNINIT )
	{
		//REPORT DEV ERROR  CAN_E_TRANSITION
	}

	if ( Config == NULL_PTR)
	{
		//report dev error CAN_E_PARAM_POINTER
	}

#endif

	Global_config = Config ;

	/* [SWS_Can_00408]  If development error detection for the Can module is enabled:
				The function Can_Init shall raise the error CAN_E_TRANSITION if the CAN
				controllers are not in state UNINIT. ()*/
	for ( controllerID = 0 ; controllerID < NUM_OF_CONTOLLERS ; controllerID++)
	{
#if ( Dev_Error_Detect == STD_ON )
		if ( CAN_CONTROLLER_STATUS[controllerID] != CAN_CS_UNINIT )
		{
			//REPORT DEV ERROR CAN_E_TRANSITION
		}
#endif

		base_address = Config->CanControllerCfgRef[controllerID].CanControllerBaseAddress ;
		REG(base_address + CAN_MCR) = 0 ; //clearing register
		REG(base_address + CAN_MCR) = 0x1 ; // entering init mode
		while ( GET_BIT(REG(base_address + CAN_MSR) , 0 ) != 1); // wait till enter init mode
		REG(base_address + CAN_MCR ) = CAN_TRANSMIT_FIFO_PRIORITY<<2 | CAN_RECEIVE_FIFO_LOCKED_MODE<<3 |
				CAN_NO_RETRANSMISSION<<4 | CAN_WAKE_UP_MANAGMENT<<5 | CAN_BUS_OFF_MANAGMENT<<6 |
				CAN_TIME_TRIGGERED_COMMUNICATION_MODE<<7 | CAN_WORKING_IN_DEBUG<<16 ;
		REG(base_address + CAN_IER ) = 0x11111; //interrupt enable

		BRConfig = &Config->CanControllerCfgRef[controllerID].CanControllerDefaultBaudrate[DEFAULT_BAUDRATE];
		// setbaudrate ( &BRConfig)
		CAN_CONTROLLER_STATUS[controllerID]= CAN_CS_STOPPED ;

	}



	for ( HOHcount = 0 ; HOHcount<MAX_NUM_OF_HOH ; HOHcount++)
	{
		if ( Config->CanHardwareObjectRef[HOHcount].CanObjectType == Transmit )
		{

		}
		else if ( Config->CanHardwareObjectRef[HOHcount].CanObjectType == Receive )
		{

			base_address = Config->CanHardwareObjectRef[HOHcount].CanControllerRef->CanControllerBaseAddress;
			// enter filter initialization mode by setting FINIT bit
			SET_BIT(REG(base_address + CAN_FMR), 0);
			// 0: Mask Mode
			CLR_BIT(REG(base_address + CAN_FM1R ) , HRHcount);
			// 1: Single 32-bit
			SET_BIT(REG(base_address + CAN_FS1R ) , HRHcount);
			// 0: FIFO_0
			CLR_BIT(REG(base_address + CAN_FFA1R ) , HRHcount);
			// assign identifiers Value
			REG(base_address + CAN_F0R1 +  HOHcount*8 ) = Config->CanHardwareObjectRef[HOHcount].CanHwFilterRef->CanHwFilterCode ;
			REG(base_address + CAN_F0R2 +  HOHcount*8 ) = Config->CanHardwareObjectRef[HOHcount].CanHwFilterRef->CanHwFilterMask ;
			// activate
			SET_BIT(REG(base_address + CAN_FA1R ) , HRHcount);
			// enter Active Filter Mode mode by Clearing FINIT bit
			CLR_BIT(REG(base_address + CAN_FMR ) , 0);


		}
	}
	 module_state = CAN_READY;

}

/*******************************************************************************************/

Std_ReturnType Can_SetBaudrate ( u8 Controller, u16 BaudRateConfigID )
{
	u32 base_address  = 0;
	u8 return_temp ;
	/**************getting the base address of the can controller********/

		if (Controller == 0)
		{
			base_address = CAN0_BASE_ADDRESS ;
		}
		else if (Controller == 1)
		{
			base_address = CAN1_BASE_ADDRESS ;
		}
	/************************************************************************/

	CanBaudrateConfig * Baudrate = &(Global_config->CanControllerCfgRef[Controller].CanControllerDefaultBaudrate[BaudRateConfigID]);



	/*[SWS_Can_00492]  If development error detection for the Can module is enabled: if
			the module is not yet initialized, the function Can_SetControllerMode shall raise
			development error CAN_E_UNINIT and return E_NOT_OK.*/

		if (module_state == CAN_UNINIT)
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_UNINIT
			#endif
			return_temp = E_NOT_OK ;
		}

	/*[SWS_Can_00493]  If development error detection for the Can module is enabled:
		The function Can_SetBaudrate shall raise the error CAN_E_PARAM_BAUDRATE
		and return E_NOT_OK if the parameter BaudRateConfigID has an invalid value. ()*/

		else if ( BaudRateConfigID > NUM_OF_BAUDRATES )
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_BAUDRATE
			#endif
			return_temp = E_NOT_OK ;
		}

	/*SWS_Can_00494]  If development error detection for the Can module is enabled
		the function Can_SetBaudrate shall raise the error CAN_E_PARAM_CONTROLLER
		and return E_NOT_OK if the parameter Controller is out of range ()*/
		else if ( Controller > NUM_OF_CONTOLLERS )
		{
			#if ( Dev_Error_Detect == STD_ON )
			// Report error function >> CAN_E_PARAM_CONTROLLER
			#endif
			return_temp = E_NOT_OK ;
		}

		else if ( CAN_CONTROLLER_STATUS [ Controller ] == CAN_CS_STOPPED )
		{
			CLR_BIT(REG(base_address+CAN_MCR) , 1 );
			SET_BIT(REG(base_address+CAN_MCR) , 0 );
			while (GET_BIT(REG(base_address+CAN_MSR),1)==1);
			while (GET_BIT(REG(base_address+CAN_MSR),0)==0);

			REG(base_address + CAN_BTR) = 0 ;
			REG(base_address + CAN_BTR)= (Baudrate->CanControllerSeg1)<<16 |
					(Baudrate->CanControllerSeg2)<<20 | (Baudrate->CanControllerSyncJumpWidth)<<24 |
					Baudrate->CanControllerPrescaler  ;
			return_temp = E_OK ;
		}
		else
		{
			return_temp = E_NOT_OK ;
		}
		return return_temp ;
}

/*********************************************************************************************/


Std_ReturnType Can_Write ( Can_HwHandleType Hth, const Can_PduType* PduInfo )
{

	u8 HOHcount = 0 ;
	u8 HTH_num ;
	u32 base_address;


	/* [SWS_Can_00216]  If development error detection for the Can module is enabled:
			The function Can_Write shall raise the error CAN_E_UNINIT and shall return
			E_NOT_OK if the driver is not yet initialized. ()*/

	if ( module_state != CAN_READY )
	{
#if ( Dev_Error_Detect == STD_ON )
		//report dev error CAN_E_UNINIT
#endif
		return E_NOT_OK ;
	}

	/* [SWS_Can_00219]  If development error detection for CanDrv is enabled:
			Can_Write() shall raise CAN_E_PARAM_POINTER and shall return E_NOT_OK if the
			parameter PduInfo is a null pointer. ()*/

	else if ( PduInfo == NULL_PTR )
	{
#if ( Dev_Error_Detect == STD_ON )
		//report dev error CAN_E_PARAM_POINTER
#endif
		return E_NOT_OK ;
	}

	/*[SWS_Can_00218]  The function Can_Write shall return E_NOT_OK and if
			development error detection for the CAN module is enabled shall raise the error
			CAN_E_PARAM_DATA_LENGTH:
			 If the length is more than 64 byte.
			 If the length is more than 8 byte and the CAN controller is not in CAN FD
			mode (no CanControllerFdBaudrateConfig).
			 If the length is more than 8 byte and the CAN controller is in CAN FD mode
			(valid CanControllerFdBaudrateConfig), but the CAN FD flag in
			Can_PduType->id is not set (refer Can_IdType).( SRS_Can_01005)*/

	else if ( (PduInfo->length) > 8 )
	{
#if ( Dev_Error_Detect == STD_ON )
		//report dev error CAN_E_PARAM_DATA_LENGTH
#endif
		return E_NOT_OK ;
	}

	for (HOHcount = 0 ; HOHcount < MAX_NUM_OF_HOH  ; HOHcount++)
		{
		    if (Hth == Global_config->CanHardwareObjectRef[HOHcount].CanObjectId)
			{
		    	HTH_num = HOHcount ;
				break ;
			}
		}

	/*[SWS_Can_00217]  If development error detection for the Can module is enabled:
			The function Can_Write shall raise the error CAN_E_PARAM_HANDLE and shall
			return E_NOT_OK if the parameter Hth is not a configured Hardware Transmit
			Handle. ()*/
	if ( Global_config->CanHardwareObjectRef[HTH_num].CanObjectType != Transmit )
	{
#if ( Dev_Error_Detect == STD_ON )
		//report dev error CAN_E_PARAM_HANDLE
#endif
		return E_NOT_OK ;
	}


	if ( HTH_Flag [HTH_num]==0)
	{
		HTH_Flag [HTH_num]=1 ;
		base_address = Global_config->CanHardwareObjectRef[HTH_num].CanControllerRef->CanControllerBaseAddress ;

		/*[SWS_Can_00213] The function Can_Write shall perform no actions if the
				hardware transmit object is busy with another transmit request for an L-PDU:
 	 	 	 	1. The transmission of the other L-PDU shall not be cancelled and the function
				Can_Write is left without any actions.
 	 	 	 	2. The function Can_Write shall return CAN_BUSY. (SRS_Can_01049).*/
		if ( GET_BIT( REG(base_address + CAN_TSR ) , 26) == 0)
		{
			return CAN_BUSY ;
		}
		else
		{
			REG(base_address + CAN_TI0R )  = 0 ;
			REG(base_address + CAN_TDT0R ) = 0 ;
			REG(base_address + CAN_TDL0R ) = 0 ;
			REG(base_address + CAN_TDH0R ) = 0 ;

			CLR_BIT(REG(base_address + CAN_TI0R) , 1);
			if ( Global_config->CanHardwareObjectRef[HTH_num].CanIdTypeType == STANDARD )
			{
				CLR_BIT(REG(base_address + CAN_TI0R) , 2);
				REG(base_address + CAN_TI0R) |= (PduInfo->id)<<21 ;
			}
			else if ( Global_config->CanHardwareObjectRef[HTH_num].CanIdTypeType == EXTENDED )
			{
				SET_BIT(REG(base_address + CAN_TI0R) , 2);
				REG(base_address + CAN_TI0R) |= (PduInfo->id)<<3 ;
			}

			REG(base_address + CAN_TDT0R ) |= PduInfo->length ;

			REG(base_address + CAN_TDL0R) = PduInfo->sdu[3] << 24 | PduInfo->sdu[2] << 16
						| PduInfo->sdu[1] << 8 | PduInfo->sdu[0];

			REG(base_address + CAN_TDH0R) = PduInfo->sdu[7] << 24 | PduInfo->sdu[6] << 16
						| PduInfo->sdu[5] << 8 | PduInfo->sdu[4];

			SET_BIT(REG(base_address + CAN_TI0R ) , 0 );

			HTH_Flag [HTH_num] = 0 ;

			return E_OK ;

		}
	}
	return E_OK ;
}
