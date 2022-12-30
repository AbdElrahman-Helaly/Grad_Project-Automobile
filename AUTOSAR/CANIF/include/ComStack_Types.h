#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

#include "Std_Types.h"

typedef u8 PduIdType;

typedef u8 PduLengthType ;

typedef struct {

	u8* SduDataPtr;
	u8* MetaDataPtr;
	PduLengthType SduLength ;
}PduInfoType;

typedef u8 IcomConfigIdType ;

typedef enum {
	ICOM_SWITCH_E_OK ,
	ICOM_SWITCH_E_FAILED
}IcomSwitch_ErrorType;

#endif
