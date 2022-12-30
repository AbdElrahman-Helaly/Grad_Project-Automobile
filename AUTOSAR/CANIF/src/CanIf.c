#include "Std_Types.h"
#include "BIT_MATH.h"
#include "Can.h"
#include "CanIf.h"
#include "Can_MemMap.h"
#include "CanIf_Types.h"

//extern Can_StateType CAN_CONTROLLER_STATUS [NUM_OF_CONTOLLERS] ;


// Helper to get the Can Controller refered to by a CanIf Channel
#define ARC_GET_CHANNEL_CONTROLLER(_channel)                \
	CanIf_ConfigPtr->Arc_ChannelToControllerMap[_channel]

/* Global configure */
static const CanIf_ConfigType *CanIf_ConfigPtr;

// Struct of controller private data.

typedef struct
{
    Can_ControllerStateType  ControllerMode;
    CanIf_PduModeType  PduMode;
} CanIf_ChannelPrivateType;

typedef struct
{
    boolean initRun;
    CanIf_ChannelPrivateType channelData[CANIF_CHANNEL_CNT];
} global_canifType;

static global_canifType global_canif;



static const CanIf_ConfigType *CanIf_ConfigPtr;

Std_ReturnType CanIf_SetControllerMode(u8 Controller,Can_ControllerStateType ControllerMode)
{
	u8 return_temp = E_OK ;
	Can_ControllerStateType transition = ControllerMode ;

	/*[SWS_CANIF_00311] d If parameter ControllerId of
		CanIf_SetControllerMode() has an invalid value, the CanIf shall report development error code
		CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET module, when
		CanIf_SetControllerMode() is called. c (SRS_BSW_00323)*/
	if ( Controller > NUM_OF_CONTOLLERS )
	{
		//report dev error CANIF_E_PARAM_CONTROLLERID
		return_temp = E_NOT_OK ;
	}

	/*[SWS_CANIF_00774] d If parameter ControllerMode of
		CanIf_SetControllerMode() has an invalid value (not CAN_CS_STARTED,
		CAN_CS_SLEEP or CAN_CS_STOPPED), the CanIfshall report development error
		code CANIF_E_PARAM_CTRLMODE to the Det_ReportError service of the DET
		module, when CanIf_SetControllerMode() is called. c(SRS_BSW_00323)*/

	else if ((ControllerMode!=CAN_CS_STARTED)&&(ControllerMode!=CAN_CS_STOPPED)&&(ControllerMode!=CAN_CS_SLEEP))
	{
		//report dev error CANIF_E_PARAM_CTRLMODE
		return_temp = E_NOT_OK ;

	}

	/*[SWS_CANIF_00677] d If a controller mode referenced by ControllerId is in state
		CAN_CS_STOPPED and if the PduIdType parameter in a call of CanIf_Transmit()
		is assigned to that CAN Controller, then the call of CanIf_Transmit() does not
		result in a call of Can_Write() (see [SWS_CANIF_00317]) and returns E_NOT_OK. c
		()*/
	else if ( global_canif.channelData[Controller].ControllerMode == CAN_CS_UNINIT )
	{
		// prevent calling can_write+report error
		return_temp = E_NOT_OK ;
	}
	else
	{
		switch ( transition )
		{
		case CAN_CS_STOPPED :
			if ( Can_SetControllerMode(Controller , CAN_CS_STOPPED) == E_OK )
			{
				/*[SWS_CANIF_00866] d If CanIf_SetControllerMode(ControllerId,
						CAN_CS_STOPPED) or CanIf_ControllerBusOff(ControllerId) is called,
						CanIf shall set the PDU channel mode of the corresponding channel to
						CANIF_TX_OFFLINE. c()*/
				CanIf_SetPduMode(Controller , CANIF_TX_OFFLINE );
				return_temp = E_OK ;
				global_canif.channelData[Controller].ControllerMode = CAN_CS_STOPPED ;
			}
			else
			{
				return_temp = E_NOT_OK ;
			}
			break ;

		case CAN_CS_STARTED :
			if ( Can_SetControllerMode(Controller , CAN_CS_STARTED) == E_OK )
			{
				CanIf_SetPduMode(Controller , CANIF_ONLINE );
				return_temp = E_OK ;
				global_canif.channelData[Controller].ControllerMode = CAN_CS_STARTED ;
			}
			else
			{
				return_temp = E_NOT_OK;
			}
			break;

		case CAN_CS_SLEEP :
			if ( Can_SetControllerMode(Controller , CAN_CS_SLEEP) == E_OK )
			{
				return_temp = E_OK ;
				global_canif.channelData[Controller].ControllerMode = CAN_CS_SLEEP ;
				/* [SWS_CANIF_00865] d If CanIf_SetControllerMode(ControllerId,
						CAN_CS_SLEEP) is called, CanIf shall set the PDU channel mode
						of the corresponding channel to CANIF_OFFLINE. c()*/
				CanIf_SetPduMode(Controller , CANIF_OFFLINE );

			}
			else
			{
				return_temp = E_NOT_OK;
			}
			break ;
		}
	}


	return return_temp;
}


Std_ReturnType CanIf_GetControllerMode(u8 Controller,Can_ControllerStateType *ControllerModePtr)
{
	u8 return_temp ;

	/*[SWS_CANIF_00313] d If parameter ControllerId of
		CanIf_GetControllerMode() has an invalid, the CanIf shall report development error code
	    CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET, when
	    CanIf_GetControllerMode() is called. c(SRS_BSW_00323)*/
	if ( Controller >  NUM_OF_CONTOLLERS )
	{
		//report dev error CANIF_E_PARAM_CONTROLLERID
		return_temp = E_NOT_OK ;
	}

	/* [SWS_CANIF_00656] d If parameter ControllerModePtr of
		CanIf_GetControllerMode() has an invalid value, the CanIf shall report development
		error code CANIF_E_PARAM_POINTER to the Det_ReportError service of
		the DET, when CanIf_GetControllerMode() is called. c(SRS_BSW_00323)*/
	else if ( ControllerModePtr == NULL_PTR )
	{
		//report dev error CANIF_E_PARAM_POINTER
		return_temp = E_NOT_OK ;
	}
	else
	{
		if ( Can_GetControllerMode(Controller , ControllerModePtr ) == E_OK )
		{
			return_temp = E_OK ;
		}
		else
		{
			return_temp = E_NOT_OK ;
		}
	}
	return return_temp ;
}


Std_ReturnType CanIf_GetControllerErrorState(u8 ControllerId,Can_ErrorStateType* ErrorStatePtr)
{
	u8 return_temp ;

	/* [SWS_CANIF_00898] d If parameter ControllerId of
		CanIf_GetControllerErrorState() has an invalid value, the CanIf
		shall report development error code CANIF_E_PARAM_CONTROLLERID
		to the Det_ReportError service of the DET, when
		CanIf_GetControllerErrorState() is called. c(SRS_BSW_00323)*/
	if ( ControllerId > NUM_OF_CONTOLLERS )
	{
		//report dev error CANIF_E_PARAM_CONTROLLERID
		return_temp = E_NOT_OK ;
	}

	/* [SWS_CANIF_00899] d If parameter ErrorStatePtr of
		CanIf_GetControllerErrorState() is a null pointer, the CanIf shall report
		development error code CANIF_E_PARAM_POINTER to the Det_ReportError
		service of the DET, when CanIf_GetControllerErrorState() is called. c
		(SRS_BSW_00323*/
	else if ( ErrorStatePtr == NULL_PTR )
	{
		//report dev error CANIF_E_PARAM_POINTER
		return_temp = E_NOT_OK ;
	}
	else
	{
		if ( Can_GetControllerErrorState ( ControllerId , ErrorStatePtr ) == E_OK )
		{
			return_temp = E_OK ;
		}
		else
		{
			return_temp = E_NOT_OK ;
		}
	}
	return return_temp ;
}

//_______________________________________________________________________________________________
//_______________________________________________________________________________________________



static CanIf_TxPduConfigType* GetTxPduEntry ( PduIdType id )
{
	if ( id > CanIf_ConfigPtr->InitConfig->CanIfNumberOfCanTXPduIds )
	{
		return NULL_PTR ;
	}
	else
	{
		return &(CanIf_ConfigPtr->InitConfig->CanIfTxPduConfigPtr[id]);
	}
}

static CanIf_RxPduConfigType* GetRxPduEntry ( PduIdType id )
{
	if ( id > CanIf_ConfigPtr->InitConfig->CanIfNumberOfCanRxPduIds )
	{
		return NULL_PTR ;
	}
	else
	{
		return &(CanIf_ConfigPtr->InitConfig->CanIfRxPduConfigPtr[id]);
	}
}

static CanIf_HthConfigType* GetTxPduHandler ( CanIf_Arc_ChannelIdType Channel )
{
	const CanIf_InitHohConfigType * config_hoh ;
	const CanIf_HthConfigType * config_hth ;

	config_hoh = CanIf_ConfigPtr->InitConfig->CanIfHohConfigPtr;
	while (! config_hoh->CanIf_Arc_EOL )
	{
		config_hth = config_hoh->CanIfHthConfig ;
		while ( ! config_hth->CanIf_Arc_EOL )
		{
			if ( Channel == config_hth->CanIfCanControllerIdRef)
			{
				return (&config_hth);
			}
			config_hth++;
		}
		config_hoh++;
	}
	return NULL_PTR ;
}


static CanIf_HrhConfigType* GetRxPduHandler ( CanIf_Arc_ChannelIdType Channel )
{
	const CanIf_InitHohConfigType * config_hoh ;
	const CanIf_HrhConfigType * config_hrh ;

	config_hoh = CanIf_ConfigPtr->InitConfig->CanIfHohConfigPtr;
	while (! config_hoh->CanIf_Arc_EOL )
	{
		config_hrh = config_hoh->CanIfHrhConfig ;
		while ( ! config_hrh->CanIf_Arc_EOL )
		{
			if ( Channel == config_hrh->CanIfCanControllerHrhIdRef)
			{
				return &(config_hrh);
			}
			config_hrh++;
		}
		config_hoh++;
	}
	return NULL_PTR ;
}





Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId,const PduInfoType *PduInfoPtr)
{
	u8 return_temp = E_OK ;
	Can_PduType pdu ;
	CanIf_TxPduConfigType *TxEntry ;
	Can_StateType can_mode ;
	CanIf_PduModeType pdu_mode ;
	CanIf_Arc_ChannelIdType channel ;

	TxEntry = GetTxPduEntry(CanTxPduId);


	if(	global_canif.initRun == FALSE  )
	{
		// report dev
		return E_NOT_OK ;
	}
	else if (PduInfoPtr==NULL_PTR)
	{
		// report dev
		return E_NOT_OK ;
	}

	TxEntry = GetTxPduEntry(CanTxPduId);
	if(TxEntry == NULL_PTR)
	{
		//report error
		return E_NOT_OK ;
	}
	channel = TxEntry->CanIfCanTxPduHthRef->CanIfCanControllerIdRef ;

	if ( CanIf_GetControllerMode(channel , &can_mode) == E_NOT_OK )
	{
		return E_NOT_OK ;
	}
	else if (  CanIf_GetPduMode(channel , &pdu_mode) == E_NOT_OK )
	{
		return E_NOT_OK ;
	}
	else if (can_mode != CAN_CS_STARTED )
	{
		return E_NOT_OK ;
	}
	else if ( pdu_mode != CANIF_ONLINE)
	{
		return E_NOT_OK ;
	}

	pdu.swPduHandle = GetTxPduHandler(TxEntry->CanIfCanTxPduHthRef->CanIfCanControllerIdRef);

	pdu.id = TxEntry->CanIfCanTxPduIdCanId ;
	pdu.length = PduInfoPtr->SduLength ;
	pdu.sdu = PduInfoPtr->SduDataPtr ;

	 return_temp = Can_Write(TxEntry->CanIfCanTxPduHthRef->CanIfHthIdSymRef , &pdu) ;
	 if ( return_temp == E_NOT_OK || return_temp == CAN_BUSY )
	 {
		 return E_NOT_OK ;
	 }
	 else
	 {
		 return E_OK ;
	 }

}

void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{
	CanIf_ConfigPtr = ConfigPtr ;
	//to be implemented
}

static CanIf_Arc_ChannelIdType GetHrHChannel ( Can_Arc_HRHType hrh )
{
	const CanIf_InitHohConfigType * configHOH ;
	const CanIf_HrhConfigType * configHRH ;
	configHOH = CanIf_ConfigPtr->InitConfig->CanIfHohConfigPtr ;
	configHOH--;

	do
	{
		configHOH++ ;
		configHRH = configHOH->CanIfHrhConfig ;
		configHRH--;
		do {
			configHRH++;
			if(configHRH->CanIfHrhIdSymRef == hrh)
			{
				return configHRH->CanIfCanControllerHrhIdRef;
			}
		   }while(configHRH->CanIf_Arc_EOL!=TRUE);
	} while( configHOH->CanIf_Arc_EOL != TRUE);

	//report DET error
	return  CANIF_CHANNEL_CNT;
}



Std_ReturnType CanIf_SetPduMode(u8 ControllerId,CanIf_PduModeType PduModeRequest)
{
	CanIf_PduModeType transition = PduModeRequest ;
	CanIf_Arc_ChannelIdType channel = (CanIf_Arc_ChannelIdType)ControllerId ;
	u8 return_temp ;

	/*[SWS_CANIF_00874] d The service CanIf_SetPduMode() shall not accept any request and
	 	 	 shall return E_NOT_OK, if the controller mode referenced by ControllerId
			 is not in state CAN_CS_STARTED. c()*/

	if ( global_canif.channelData[channel].ControllerMode != CAN_CS_STARTED )
	{
		//report dev error
		return E_NOT_OK;
	}
	/*[SWS_CANIF_00341] d If CanIf_SetPduMode() is called with invalid ControllerId, CanIf shall report development error code
			CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the
			DET module. c(SRS_BSW_00323) */
	else if (ControllerId > NUM_OF_CONTOLLERS)
	{
		// report dev error CANIF_E_PARAM_CONTROLLERID ;
		return E_NOT_OK ;
	}
	else if ( global_canif.initRun == FALSE )
		{
			//report dev error
			return E_NOT_OK ;
		}
	switch ( transition )
	{
	case CANIF_OFFLINE :
		global_canif.channelData[channel].PduMode = CANIF_OFFLINE ;
		return_temp = E_OK;
		break ;

	case CANIF_TX_OFFLINE :
		global_canif.channelData[channel].PduMode = CANIF_TX_OFFLINE ;
		return_temp = E_OK;
		break ;

	case CANIF_ONLINE :
		global_canif.channelData[channel].PduMode = CANIF_ONLINE ;
		return_temp = E_OK;
		break ;

	case CANIF_TX_OFFLINE_ACTIVE :
		global_canif.channelData[channel].PduMode = CANIF_TX_OFFLINE_ACTIVE ;
		return_temp = E_OK;
		break ;

	default :
		/* [SWS_CANIF_00860] d If CanIf_SetPduMode() is called with invalid PduModeRequest,
		    	CanIf shall report development error code CANIF_E_PARAM_PDU_MODE
				to the Det_ReportError service of the DET module. c(SRS_BSW_00323)*/
		//report dev error  CANIF_E_PARAM_PDU_MODE
		return E_NOT_OK ;
		break ;

	}

	return return_temp;

}


Std_ReturnType CanIf_GetPduMode(u8 ControllerId,CanIf_PduModeType* PduModePtr)
{
	CanIf_Arc_ChannelIdType channel ;
	channel = ( CanIf_Arc_ChannelIdType ) ControllerId ;

	/*[SWS_CANIF_00346] d If CanIf_GetPduMode() is called with invalid ControllerId,
	   	    CanIf shall report development error code
			CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the
			DET module. c(SRS_BSW_00323)*/
	if ( ControllerId > NUM_OF_CONTOLLERS)
	{
		//report dev error CANIF_E_PARAM_CONTROLLERID
		return E_NOT_OK ;
	}

	/*[SWS_CANIF_00657] d If CanIf_GetPduMode() is called with invalid PduModePtr,
	   	   	    CanIf shall report development error code CANIF_E_PARAM_POINTER to the
				Det_ReportError service of the DET module. c(SRS_BSW_00323) */
	else if ( PduModePtr == NULL_PTR )
	{
		//REPOR DEV ERROR CANIF_E_PARAM_POINTER
		return E_NOT_OK ;
	}
	else if ( global_canif.initRun == FALSE )
	{
		//report dev error
		return E_NOT_OK ;
	}

	*PduModePtr = global_canif.channelData[channel].PduMode ;

	return E_OK ;
}


void CanIf_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
	VersionInfo->instanceID = CANIF_INSTANCEID ;
	VersionInfo->moduleID = CANIF_MODULEID;
	VersionInfo->sw_major_version = CANIF_SW_MAJOR_VERSION ;
	VersionInfo->sw_minor_version = CANIF_SW_MINOR_VERSION ;
	VersionInfo->sw_patch_version = CANIF_SW_PATCH_VERSION ;
	VersionInfo->vendorID = CANIF_VENDOR_ID ;

}



void CanIf_ControllerBusOff(u8 ControllerId)
{

	/* [SWS_CANIF_00429] d If parameter ControllerId of
			CanIf_ControllerBusOff() has an invalid value, CanIf shall report development error
			code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError
			service of the DET module, when CanIf_ControllerBusOff() is called. c
			(SRS_BSW_00323) */
	if ( ControllerId > NUM_OF_CONTOLLERS )
	{
		//REPORT DEV ERROR CANIF_E_PARAM_CONTROLLERID
	}
	/* [SWS_CANIF_00431] d If CanIf was not initialized before calling
			CanIf_ControllerBusOff(), CanIf shall not execute BusOff notification,
			when CanIf_ControllerBusOff(), is called. c()*/
	else if ( global_canif.initRun == FALSE )
	{
		//REPORT DEV ERROR
	}
	else
	{
		CanIf_SetBaudrate( ControllerId , CAN_CS_STOPPED );
	}

}

/*
Std_ReturnType CanIf_SetBaudrate(u8 ControllerId,u16 BaudRateConfigID)
{
	 [SWS_CANIF_00869] d If CanIf_SetBaudrate() is called with invalid ControllerId,
	 	 	 CanIf shall report development error code
			 CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the
			 DET module. c(SRS_BSW_00323)
	if ( ControllerId > NUM_OF_CONTOLLERS )
	{
		//	REPORT DEV ERROR CANIF_E_PARAM_CONTROLLERID
		return E_NOT_OK ;
	}
	else if ( global_canif.initRun == FALSE )
	{
		//report dev error
		return E_NOT_OK ;
	}
	else
	{
		if ( Can_SetBaudrate(ControllerId , BaudRateConfigID) == E_OK)
		{
			return E_OK ;
		}
		else
		{
			return E_NOT_OK ;
		}
	}

}
*/

CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId)
{
	CanIf_TxPduConfigType * entrypdu ;
	entrypdu = GetTxPduEntry( CanIfTxSduId );

	/* [SWS_CANIF_00336] d If parameter CanIfRxSduId of
				CanIf_ReadRxNotifStatus() is out of range or if status for CanRxPduId
				was requested whereas CANIF_READRXPDU_DATA_API is disabled or if no status
				information was configured for this CAN Rx L-SDU, CanIf shall report development
				error code CANIF_E_INVALID_RXPDUID to the Det_ReportError service of
				the DET, when CanIf_ReadRxNotifStatus() is called. c(SRS_BSW_00323)*/

	if (CanIfTxSduId > CanIf_ConfigPtr->InitConfig->CanIfNumberOfCanTXPduIds )
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}
	//else if ( CANIF_READRXPDU_DATA_API == ENABLE )
	//{
		//report dev error CANIF_E_INVALID_RXPDUID
	//}
	else if ( entrypdu == NULL_PTR )
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}
	else if(entrypdu->CanIfReadTxPduNotifyStatus == FALSE)
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}
	// to be supported

	return CANIF_NO_NOTIFICATION ;

}


CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId)
{

	CanIf_RxPduConfigType * entrypdu ;
	entrypdu = GetRxPduEntry( CanIfRxSduId );

	/* [SWS_CANIF_00336] d If parameter CanIfRxSduId of
			CanIf_ReadRxNotifStatus() is out of range or if status for CanRxPduId
			was requested whereas CANIF_READRXPDU_DATA_API is disabled or if no status
			information was configured for this CAN Rx L-SDU, CanIf shall report development
			error code CANIF_E_INVALID_RXPDUID to the Det_ReportError service of
			the DET, when CanIf_ReadRxNotifStatus() is called. c(SRS_BSW_00323)*/

	if (CanIfRxSduId > CanIf_ConfigPtr->InitConfig->CanIfNumberOfCanRxPduIds )
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}
	else if ( CANIF_READRXPDU_DATA_API == ENABLE )
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}
	else if ( entrypdu == NULL_PTR )
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}
	else if(entrypdu->CanIfReadRxPduData == FALSE)
	{
		//report dev error CANIF_E_INVALID_RXPDUID
	}

	// to be supported

	return CANIF_NO_NOTIFICATION ;
}



void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
	const CanIf_TxPduConfigType *pdu_config ;
	CanIf_PduModeType pdu_mode ;

	/* [SWS_CANIF_00410] d If parameter CanTxPduId of CanIf_TxConfirmation()
			has an invalid value, CanIf shall report development error code
			CANIF_E_PARAM_LPDU to the Det_ReportError service of the DET module,
			when CanIf_TxConfirmation() is called. c(SRS_BSW_00323)*/

	pdu_config = &(CanIf_ConfigPtr ->InitConfig->CanIfTxPduConfigPtr[CanTxPduId]);
	if ( CanTxPduId > CanIf_ConfigPtr->InitConfig->CanIfNumberOfCanTXPduIds )
	{
		//REPORT DEV ERROR CANIF_E_PARAM_LPDU
		return ;
	}

	/* [SWS_CANIF_00412] d If CanIf was not initialized before calling CanIf_TxConfirmation(),
	 	 	 CanIf shall not call the service <User_TxConfirmation>() and shall not set
	 	 	 the Tx confirmation status, when CanIf_TxConfirmation() is called. c()*/
	else if ( global_canif.initRun != TRUE )
	{
		return ;
	}

	if ( pdu_config ->CanIfUserTxConfirmation == NULL_PTR )
	{
		return ;
	}
	else
	{
		CanIf_GetPduMode(pdu_config->CanIfCanTxPduHthRef->CanIfCanControllerIdRef , &pdu_mode );
		if ( pdu_mode != CANIF_ONLINE )
		{
			return ;
		}
		else
		{
			pdu_config->CanIfUserTxConfirmation(pdu_config->CanIfTxPduId) ;
		}
	}
}


void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr)
{
	CanIf_PduModeType pdu_mode ;
	CanIf_Arc_ChannelIdType channel ;
	CanIf_RxPduConfigType * pdu_config ;
	//CanIf_HrhConfigType* hrh_config ;
	/*[SWS_CANIF_00417] d If parameter Mailbox->CanId of
			CanIf_RxIndication() has an invalid value, CanIf shall report development
			error code CANIF_E_PARAM_CANID to the Det_ReportError service of the DET
			module, when CanIf_RxIndication() is called. c(SRS_BSW_00323)*/


	/*[SWS_CANIF_00416] d If parameter Mailbox->Hoh of CanIf_RxIndication()
			has an invalid value, CanIf shall report development error code
			CANIF_E_PARAM_HOH to the Det_ReportError service of the DET module,
			when CanIf_RxIndication() is called. c(SRS_BSW_00323)*/
	channel = GetHrHChannel((Can_Arc_HRHType)Mailbox->ControllerId);

	//hrh_config= GetRxPduHandler(channel);

	if (channel == CANIF_CHANNEL_CNT )
	{
		//REPORT DEV ERROR  CANIF_E_PARAM_HOH
		return ;
	}
	/*[SWS_CANIF_00419] d If parameter PduInfoPtr or Mailbox of
			CanIf_RxIndication() has an invalid value, CanIf shall report development
			error code CANIF_E_PARAM_POINTER to the Det_ReportError service of the DET
			module, when CanIf_RxIndication() is called. c(SRS_BSW_00323)*/
	else if ( (Mailbox == NULL_PTR) || (PduInfoPtr==NULL_PTR) )
	{
		//REPORT DEV ERROR CANIF_E_PARAM_POINTER
		return ;
	}
	/*[SWS_CANIF_00421] d If CanIf was not initialized before calling
			CanIf_RxIndication(), CanIf shall not execute Rx indication handling, when
			CanIf_RxIndication(), is called. c()*/
	else if ( global_canif .initRun != TRUE )
	{
		return ;
	}

	if ( 	CanIf_GetPduMode(channel , &pdu_mode ) == E_OK)
	{
		if ( pdu_mode != CANIF_ONLINE )
		{
			return ;
		}
	}
	else
	{
		return ;
	}
	//hrh_config= GetRxPduHandler(channel);

	//to be completed

}




