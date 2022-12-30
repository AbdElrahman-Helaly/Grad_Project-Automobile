#ifndef CANIF_H
#define CANIF_H

#include "Can.h"
#include "ComStack_Types.h"
#include "Std_Types.h"
#include "CanIf_Types.h"



#define 	CANIF_INSTANCEID			0x01
#define 	CANIF_MODULEID				0x02
#define 	CANIF_SW_MAJOR_VERSION		0x03
#define 	CANIF_SW_MINOR_VERSION		0x04
#define 	CANIF_SW_PATCH_VERSION		0x05
#define 	CANIF_VENDOR_ID				0x06



#define 	CANIF_READRXPDU_DATA_API 		ENABLE




/*

void CanIf_Init(const CanIf_ConfigType* ConfigPtr);
void CanIf_DeInit(void);
Std_ReturnType CanIf_SetControllerMode(u8 Controller,Can_ControllerStateType ControllerMode);
Std_ReturnType CanIf_GetControllerMode(u8 Controller,Can_ControllerStateType *ControllerModePtr);
Std_ReturnType CanIf_GetControllerErrorState(u8 ControllerId,Can_ErrorStateType* ErrorStatePtr);
Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId,const PduInfoType *PduInfoPtr);
Std_ReturnType CanIf_CancelTransmit(PduIdType TxPduId);
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,PduInfoType* CanIfRxInfoPtr);
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId);
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId);
Std_ReturnType CanIf_SetPduMode(u8 ControllerId,CanIf_PduModeType PduModeRequest);
Std_ReturnType CanIf_GetPduMode(u8 ControllerId,CanIf_PduModeType* PduModePtr);
void CanIf_GetVersionInfo(Std_VersionInfoType* VersionInfo);
Std_ReturnType CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource);
CanIf_NotifStatusType CanIf_GetTxConfirmationState(u8 ControllerId);
Std_ReturnType CanIf_SetBaudrate(u8 ControllerId,u16 BaudRateConfigID);
Std_ReturnType CanIf_SetIcomConfiguration(u8 ControllerId,IcomConfigIdType ConfigurationId);
Std_ReturnType CanIf_TriggerTransmit(PduIdType TxPduId,PduInfoType* PduInfoPtr);
void CanIf_TxConfirmation(PduIdType CanTxPduId);
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);
void CanIf_ControllerBusOff(u8 ControllerId);
void CanIf_ConfirmPnAvailability(u8 TransceiverId);
void CanIf_ControllerModeIndication(u8 ControllerId,Can_ControllerStateType ControllerMode);
*/
Std_ReturnType CanIf_GetPduMode(u8 ControllerId,CanIf_PduModeType* PduModePtr);
Std_ReturnType CanIf_SetPduMode(u8 ControllerId,CanIf_PduModeType PduModeRequest);


Std_ReturnType CanIf_SetControllerMode(u8 Controller,Can_ControllerStateType ControllerMode);
Std_ReturnType CanIf_GetControllerMode(u8 Controller,Can_ControllerStateType *ControllerModePtr);
Std_ReturnType CanIf_GetControllerErrorState(u8 ControllerId,Can_ErrorStateType* ErrorStatePtr);
Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId,const PduInfoType *PduInfoPtr);

void CanIf_GetVersionInfo(Std_VersionInfoType* VersionInfo);

void CanIf_ControllerBusOff(u8 ControllerId);


Std_ReturnType CanIf_SetBaudrate(u8 ControllerId,u16 BaudRateConfigID);



CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId);
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId);


void CanIf_TxConfirmation(PduIdType CanTxPduId);
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);








#endif
