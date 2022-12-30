#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_


#include "Std_Types.h"


typedef enum {
	vCanIf_Channel_1_CONFIG_0,
	vCanIf_Channel_0_CONFIG_0,
	CANIF_CHANNEL_CONFIGURATION_CNT
}CanIf_Arc_ConfigurationIndexType;

typedef enum {
	vCanIf_Channel_1,
	vCanIf_Channel_0,
	CANIF_CHANNEL_CNT
}CanIf_Arc_ChannelIdType;

/** PDU Can id type */
typedef enum {
  CANIF_CAN_ID_TYPE_29 = 0,
  CANIF_CAN_ID_TYPE_11
} CanIf_CanIdTypeType;

typedef enum {
	FALSE = 0 ,
	TRUE = 1
}boolean ;


typedef enum {
	CAN_CTRL_0_vCanHth,
	CAN_CTRL_1_vCanHth,
	NUM_OF_HTHS
} Can_Arc_HTHType;

typedef enum {
	CAN_CTRL_0_vCanHrh,
	CAN_CTRL_1_vCanHrh,
	NUM_OF_HRHS
} Can_Arc_HRHType;

typedef u8 PduIdType ;


/** Parameters for configuring Can id ranges. Not supported. */
typedef struct {
	/** Lower CAN Identifier of a receive CAN L-PDU for identifier range
	 *  definition, in which all CAN Ids shall pass the software filtering. Range: 11
     *  Bit for Standard CAN Identifier 29 Bit for Extended CAN Identifer */
	u32 CanIfRxPduLowerCanId;

	/** Upper CAN Identifier of a receive CAN L-PDU for identifier range
	 *  definition, in which all CAN Ids shall pass the software filtering. Range: 11
	 *  Bit for Standard CAN Identifier 29 Bit for Extended CAN Identifer */
	u32 CanIfRxPduUpperCanId;
} CanIf_HrhRangeConfigType;


/** Definition of Hardware Receive Handle */
typedef struct {

	/** Selects the hardware receive objects by using the HRH range/list from
	 *  CAN Driver configuration to define, for which HRH a software filtering has
	 *  to be performed at during receive processing. True: Software filtering is
	 *  enabled False: Software filtering is disabled */
	boolean  CanIfSoftwareFilterHrh;

	/** Reference to controller Id to which the HRH belongs to. A controller can
	 *  contain one or more HRHs. */
	CanIf_Arc_ChannelIdType CanIfCanControllerHrhIdRef;

	/** The parameter refers to a particular HRH object in the CAN Driver Module
	 *  configuration. The HRH id is unique in a given CAN Driver. The HRH Ids
	 *  are defined in the CAN Driver Module and hence it is derived from CAN
	 *  Driver Configuration. */
	Can_Arc_HRHType CanIfHrhIdSymRef ;

	/** Defines the parameters required for configuraing multiple
	 *  CANID ranges for a given same HRH. */
	//const CanIf_HrhRangeConfigType *CanIfHrhRangeConfig;

  /** End Of List. Set to TRUE if this is the last object in the list. */
  boolean CanIf_Arc_EOL;
} CanIf_HrhConfigType;



/*
 * CanIfInitHthConfig container
 */
/** Definition of Hardware Transmit Handle */
typedef struct {

  /** Reference to controller Id to which the HTH belongs to. A controller
   *  can contain one or more HTHs */
  CanIf_Arc_ChannelIdType CanIfCanControllerIdRef;

  /** The parameter refers to a particular HTH object in the CAN Driver Module
   *  configuration. The HTH id is unique in a given CAN Driver. The HTH Ids
   *  are defined in the CAN Driver Module and hence it is derived from CAN
   *  Driver Configuration. */
  Can_Arc_HTHType CanIfHthIdSymRef ;

  /** End Of List. Set to TRUE if this is the last object in the list. */
  boolean CanIf_Arc_EOL;
} CanIf_HthConfigType;



/*
 * CanIfInitHohConfig container
 */
/** Definition of Hardware Object Handle. */
typedef struct {
  /** Reference to the CAN Driver controller config. */
  //const Can_ConfigSetType   *CanConfigSet;

  /** This container contains contiguration parameters for each hardware receive object. */
  const CanIf_HrhConfigType *CanIfHrhConfig;

  /** This container contains parameters releated to each HTH */
  const CanIf_HthConfigType *CanIfHthConfig;

  /** End Of List. Set to TRUE if this is the last object in the list. */
  boolean CanIf_Arc_EOL;
} CanIf_InitHohConfigType;



/** Definition of Tx PDU (Protocol Data Unit). */
typedef struct {
	/** ECU wide unique, symbolic handle for transmit CAN L-PDU. The
	 *  CanIfCanTxPduId is configurable at pre-compile and post-built time.
	 *  Range: 0..max. number of CantTxPduIds  	PduIdType 	CanTxPduId; */
	PduIdType CanIfTxPduId;

	/** CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-
	 *  PDU transmission. Range: 11 Bit For Standard CAN Identifier ... 29 Bit For
	 *  Extended CAN identifier */
	u32  CanIfCanTxPduIdCanId;

	/** Data length code (in bytes) of transmit CAN L-PDUs used by the CAN
	 *  Driver for CAN L-PDU transmission. The data area size of a CAN L-Pdu
	 *  can have a range from 0 to 8 bytes. */
	u8 		CanIfCanTxPduIdDlc;




	/** Enables and disables transmit confirmation for each transmit CAN L-PDU
	 *  for reading its notification status. True: Enabled False: Disabled */
	boolean		CanIfReadTxPduNotifyStatus;


	/** CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-
	 *  PDU transmission.
	 *  EXTENDED_CAN  The CANID is of type Extended (29 bits).
	 *  STANDARD_CAN  The CANID is of type Standard (11 bits). */
	CanIf_CanIdTypeType		CanIfTxPduIdCanIdType;

	/** Name of target confirmation services to target upper layers (PduR, CanNm
	 *  and CanTp. If parameter is not configured then no call-out function is
	 *  provided by the upper layer for this Tx L-PDU. */
	void (*CanIfUserTxConfirmation)(PduIdType);   /* CANIF 109 */

	/** Handle, that defines the hardware object or the pool of hardware objects
	 *  configured for transmission. The parameter refers HTH Id, to which the L-
	 *  PDU belongs to. */
	const CanIf_HthConfigType *CanIfCanTxPduHthRef;

	/** Reference to the "global" Pdu structure to allow harmonization of handle
	 *  IDs in the COM-Stack. */
	void *PduIdRef;
} CanIf_TxPduConfigType;



/*
 * CanIfRxPduConfig container
 */


/** Definition of Rx PDU (Protocol Data Unit). */
typedef struct {
  /** ECU wide unique, symbolic handle for receive CAN L-PDU. The
   *  CanRxPduId is configurable at pre-compile and post-built time. It shall fulfill
   *  ANSI/AUTOSAR definitions for constant defines. Range: 0..max. number
   *  of defined CanRxPduIds */
  PduIdType CanIfCanRxPduId;

  /** CAN Identifier of Receive CAN L-PDUs used by the CAN Interface. Exa:
   *  Software Filtering. Range: 11 Bit For Standard CAN Identifier ... 29 Bit For
   *  Extended CAN identifier */
	u32 		CanIfCanRxPduCanId;

  /** Data Length code of received CAN L-PDUs used by the CAN Interface.
   *  Exa: DLC check. The data area size of a CAN L-PDU can have a range
   *  from 0 to 8 bytes.  	u8 		CanIfCanRxPduDlc; */
	u8		CanIfCanRxPduDlc;

  /** Enables and disables the Rx buffering for reading of received L-PDU data.
   *  True: Enabled False: Disabled */
	boolean		CanIfReadRxPduData;


#if ( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON )
  /** CanIfReadRxPduNotifyStatus {CANIF_READRXPDU_NOTIFY_STATUS}
   *  Enables and disables receive indication for each receive CAN L-PDU for
   *  reading its' notification status. True: Enabled False: Disabled */
	boolean		CanIfReadRxPduNotifyStatus;
#endif

  /** CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN L-
   *  PDU transmission.
   *  EXTENDED_CAN  The CANID is of type Extended (29 bits)
   *  STANDARD_CAN  The CANID is of type Standard (11 bits) */
	CanIf_CanIdTypeType	CanIfRxPduIdCanIdType;



  /** The HRH to which Rx L-PDU belongs to, is referred through this
   *  parameter. */
	const CanIf_HrhConfigType *CanIfCanRxPduHrhRef;

  /** Reference to the "global" Pdu structure to allow harmonization of handle
   *  IDs in the COM-Stack. */
	void *PduIdRef;

  /** Defines the type of software filtering that should be used
   *  for this receive object. */
	//CanIf_SoftwareFilterTypeType CanIfSoftwareFilterType;

  /** Acceptance filters, 1 - care, 0 - don't care.
   *  Is enabled by the CanIfSoftwareFilterMask in CanIf_HrhConfigType
   *  ArcCore exension */
	u32 CanIfCanRxPduCanIdMask;

} CanIf_RxPduConfigType;




/** Container used to create channel init configurations.
 *  @see CanIf_Arc_ConfigurationIndexType
 *  @see CanIf_Arc_ChannelIdType */
typedef struct {


	/** CanIf-specific id of the controller */
	CanIf_Arc_ChannelIdType CanIfControllerIdRef;



	/** Reference to */
	//const Can_ControllerConfigType *CanIfInitControllerRef;

} CanIf_ControllerConfigType;


/** This container contains the references to the configuration setup of each
 *  underlying CAN driver. */
typedef struct {
	/** Not used. */
	u32 CanIfConfigSet;

	/** Size of Rx PDU list. */
	u32 CanIfNumberOfCanRxPduIds;
	/** Size of Tx PDU list. */
	u32 CanIfNumberOfCanTXPduIds;


	//
	// Containers
	//

  /** Hardware Object Handle list */
	const CanIf_InitHohConfigType *CanIfHohConfigPtr;

  /** Rx PDU's list */
	const CanIf_RxPduConfigType *CanIfRxPduConfigPtr;

  /** Tx PDU's list */
	const CanIf_TxPduConfigType *CanIfTxPduConfigPtr;

} CanIf_InitConfigType;




typedef struct {
	/** Reference to the list of channel init configurations. */
	const CanIf_ControllerConfigType 	*ControllerConfig;



	/** This container contains the init parameters of the CAN Interface. */
	const CanIf_InitConfigType 		*InitConfig;



	/** ArcCore: Contains the mapping from CanIf-specific Channels to Can Controllers */
	//const Can_ControllerIdType			*Arc_ChannelToControllerMap;

	const u8							*Arc_ChannelDefaultConfIndex;
} CanIf_ConfigType;



/**********************************************************************************************/


/** Status of the PDU channel group. Current mode of the channel defines its
 *  transmit or receive activity. Communication direction (transmission and/or
 *  reception) of the channel can be controlled separately or together by upper
 *  layers. */
typedef enum {
	/** Channel shall be set to the offline mode
	 *  => no transmission and reception */
	CANIF_OFFLINE = 0,

	/** Transmit path of the corresponding channel
	 *  shall be disabled */
	CANIF_TX_OFFLINE,

	/** Channel shall be set to online mode
	 *  => full operation mode */
	CANIF_ONLINE,

	/** Transmit path of the corresponding channel
	 *  shall be set to the offline active mode
	 *  => notifications are processed but transmit
	 *  requests are blocked. */
	CANIF_TX_OFFLINE_ACTIVE
} CanIf_PduModeType;     /* equals to CanIf_PduSetModeType */



typedef enum {
	/** No transmit or receive event occurred for
	 *  the requested L-PDU. */
	CANIF_NO_NOTIFICATION = 0,
	/** The requested Rx/Tx CAN L-PDU was
	 *  successfully transmitted or received. */
	CANIF_TX_RX_NOTIFICATION

} CanIf_NotifStatusType;












#endif
