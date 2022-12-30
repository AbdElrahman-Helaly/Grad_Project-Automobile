#ifndef STD_TYPE_H
#define STD_TYPE_H

typedef unsigned char  			u8;
typedef signed char  			s8;
typedef unsigned short int		u16;
typedef short int 				s16;
typedef unsigned long int 		u32;
typedef signed long int 		s32;
typedef unsigned long long 		u64;
typedef long long 				s64;
typedef float 					f32;

typedef struct
{
 u16 vendorID;
 u16 moduleID;
 u8 instanceID;
 u8 sw_major_version;
 u8 sw_minor_version;
 u8 sw_patch_version;
} Std_VersionInfoType;

typedef u8 Std_ReturnType;

#define E_OK 		0b0
#define E_NOT_OK	0b1
#define CAN_BUSY    0x2


#define STD_OFF		0
#define STD_ON		1

#define NULL_PTR  ((void*)0)


#endif
