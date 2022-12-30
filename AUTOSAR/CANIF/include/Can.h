

#ifndef CAN_H
#define CAN_H

#include "Std_Types.h"
#include "ComStack_Types.h"

/*********************************************************************/

#define 	CAN0_BASE_ADDRESS 		0x40006400
#define 	CAN1_BASE_ADDRESS 		0x40006800

#define 	NUM_OF_CONTOLLERS		2
#define 	NUM_OF_HOH_OBJECTS		2
#define 	NUM_OF_BAUDRATES		2

#define 	MAX_NUM_OF_HOH          6
/*********************************************************************/
/*????????????????????????????? version info ????????????????????????*/

#define 	 VENDOR_ID  				0x01
#define	 	 MODULE_ID					0x02
#define  	 INSTANCE_ID				0x03
#define  	 SW_MAJOR_VERSION			0x04
#define  	 SW_MINOR_VERSION			0x05
#define  	 SW_PATCH_VERSION			0x06

/*********************************************************************/

#define		 Dev_Error_Detect 			STD_ON

/*********************************************************************/


#define ENABLE 		0x01
#define DISABLE		0x00


/*********************************************************************/

typedef u8 Can_Module_State ;
#define CAN_UNINIT		0x01
#define CAN_READY		0x02


/**********************************************************************/

#define CAN_E_PARAM_POINTER              (0x01)
#define CAN_E_PARAM_HANDLE               (0x02)
#define CAN_E_PARAM_DATA_LENGTH          (0x03)
#define CAN_E_PARAM_CONTROLLER           (0x04)
#define CAN_E_UNINIT                     (0x05)
#define CAN_E_TRANSITION                 (0x06)
#define CAN_E_PARAM_BAUDRATE             (0x07)
#define CAN_E_ICOM_CONFIG_INVALID        (0x08)
#define CAN_E_INIT_FAILED                (0x09)

/**********************************************************************/

typedef struct {

	u8  CanControllerBaudRateConfigID;
	u16 CanControllerPrescaler;
	u8  CanControllerSeg1;
	u8  CanControllerSeg2;
	u8  CanControllerSyncJumpWidth;


} CanBaudrateConfig;


/**********************************************************************/

typedef u32 Can_IdType ;

/**********************************************************************/

typedef u8 Can_HwHandleType ;
#define Transmit 	(Can_HwHandleType)0x0
#define Receive 	(Can_HwHandleType)0x1

/**********************************************************************/

typedef struct
{
	Can_IdType CanId ;
	Can_HwHandleType Hoh ;
	u8 ControllerId ;

}Can_HwType;

/**********************************************************************/

typedef struct
{
	PduIdType swPduHandle;
	u8 	length ;
	Can_IdType id ;
	u8* sdu ;

}Can_PduType;

/**********************************************************************/

typedef enum {
	CAN_ERRORSTATE_ACTIVE ,
	CAN_ERRORSTATE_PASSIVE ,
	CAN_ERRORSTATE_BUSOFF
}Can_ErrorStateType;

/**********************************************************************/

typedef enum {
	CAN_CS_UNINIT ,
	CAN_CS_STARTED ,
	CAN_CS_STOPPED ,
	CAN_CS_SLEEP
}Can_ControllerStateType;

typedef u8 Can_StateType;


/**********************************************************************/

typedef enum {
	CAN_ERROR_BIT_MONITORING1  = 0x01,
	CAN_ERROR_BIT_MONITORING0  ,
	CAN_ERROR_BIT ,
	CAN_ERROR_CHECK_ACK_FAILED ,
	CAN_ERROR_ACK_DELIMITER ,
	CAN_ERROR_ARBITRATION_LOST  ,
	CAN_ERROR_OVERLOAD ,
	CAN_ERROR_CHECK_FORM_FAILED ,
	CAN_ERROR_CHECK_STUFFING_FAILED ,
	CAN_ERROR_CHECK_CRC_FAILED ,
	CAN_ERROR_BUS_LOCK
}Can_ErrorType;


/**********************************************************************/

typedef struct {

	u32 CanHwFilterCode;
	u32 CanHwFilterMask;

} CanHwFilter;

/***********************************************************************************************/

typedef struct {
    /*
    *   This parameter provides the controller ID which is unique in a given CAN
    *   Driver. The value for this parameter starts with 0 and continue without any gaps.
    */
    u8   CanControllerId;

    /*
    *   Specifies the CAN controller base address.
    */
    u32 CanControllerBaseAddress;


    /*
    *   Reference to baudrate configuration container configured for the Can    Controller.
    */
    CanBaudrateConfig* CanControllerDefaultBaudrate;
} CanController;

/***********************************************************************************************/


typedef struct
{


    /*
    *   Number of hardware objects used to implement one HOH. In case of a
    *   HRH this parameter defines the number of elements in the hardware FIFO
    *   or the number of shadow buffers, in case of a HTH it defines the number of
    *   hardware objects used for multiplexed transmission or for a hardware FIFO
    *   used by a FullCAN HTH.
    */
    u16 CanHwObjectCount;

    /*
    *   Specifies the IdValuetype
    */
    Can_IdType CanIdTypeType;

    /*
    *   Holds the handle ID of HRH or HTH. The value of this parameter is unique
    *   in a given CAN Driver, and it should start with 0 and continue without any gaps.
    *   The HRH and HTH Ids share a common ID range.
    *   Example: HRH0-0, HRH1-1, HTH0-2, HTH1-3
    */
    u16 CanObjectId;

    /*
    *   Specifies if the HardwareObject is used as Transmit or as Receive object
    */
    Can_HwHandleType   CanObjectType;


    CanController*      CanControllerRef;

    /*
    *   This container is only valid for HRHs and contains the
    *   configuration (parameters) of one hardware filter.
    */
    CanHwFilter*    CanHwFilterRef;


} CanHardwareObject;





/*********************************************************************/
typedef struct
{
	CanController* CanControllerCfgRef;
	CanHardwareObject* CanHardwareObjectRef;

}Can_ConfigType;

/**********************************************************************/

#define EXTENDED    (0x00)
#define STANDARD    (0x01)

/**********************************************************************/






/*
(done)void Can_Init (const Can_ConfigType* Config );
(done)void Can_GetVersionInfo ( Std_VersionInfoType* versioninfo );
(almost done)void Can_DeInit ( void );
(done)Std_ReturnType Can_SetBaudrate ( u8 Controller, u16 BaudRateConfigID );
(done)Std_ReturnType Can_SetControllerMode ( u8 Controller, Can_ControllerStateType Transition );
(done)void Can_DisableControllerInterrupts ( u8 Controller );
(almost done)void Can_EnableControllerInterrupts ( u8 Controller );
(done)Std_ReturnType Can_GetControllerErrorState ( u8 ControllerId, Can_ErrorStateType* ErrorStatePtr );
(done)Std_ReturnType Can_GetControllerMode ( u8 Controller, Can_ControllerStateType* ControllerModePtr );
(done)Std_ReturnType Can_GetControllerRxErrorCounter ( u8 ControllerId, u8* RxErrorCounterPtr ) ;
(done)Std_ReturnType Can_GetControllerTxErrorCounter ( u8 ControllerId, u8* TxErrorCounterPtr ) ;
(done)Std_ReturnType Can_Write ( Can_HwHandleType Hth, const Can_PduType* PduInfo ) ;

*/

void Can_Init (const Can_ConfigType* Config );
void Can_GetVersionInfo ( Std_VersionInfoType* versioninfo );
void Can_DeInit ( void );
Std_ReturnType Can_SetBaudrate ( u8 Controller, u16 BaudRateConfigID );
Std_ReturnType Can_SetControllerMode ( u8 Controller, Can_ControllerStateType Transition );
void Can_DisableControllerInterrupts ( u8 Controller );
void Can_EnableControllerInterrupts ( u8 Controller );
Std_ReturnType Can_GetControllerErrorState ( u8 ControllerId, Can_ErrorStateType* ErrorStatePtr );
Std_ReturnType Can_GetControllerMode ( u8 Controller, Can_ControllerStateType* ControllerModePtr );
Std_ReturnType Can_GetControllerRxErrorCounter ( u8 ControllerId, u8* RxErrorCounterPtr ) ;
Std_ReturnType Can_GetControllerTxErrorCounter ( u8 ControllerId, u8* TxErrorCounterPtr ) ;
Std_ReturnType Can_Write ( Can_HwHandleType Hth, const Can_PduType* PduInfo ) ;


#endif
