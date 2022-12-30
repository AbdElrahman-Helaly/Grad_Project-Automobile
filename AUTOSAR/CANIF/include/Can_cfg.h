#ifndef  CAN_CFG_H
#define  CAN_CFG_H

#include "Std_Types.h"
#include "BIT_MATH.h"

#include "Can.h"




/*****************************************
 Can working state in debug mode
 options:
 1- WORKING
 2- FROZEN
 ****************************************/
#define CAN_WORKING_IN_DEBUG	WORKING

/****************************************
 the bxCAN performs an internal feedback from its Tx output
 to its Rx input. The actual value of the CANRX input pin is
 disregarded by the bxCAN.
 options:
 OFF --> loopback disabled
 ON --> loopback Enabled
 ****************************************/
#define CAN_LOOPBACK_MODE	ON

/****************************************
 In Silent mode,
 the bxCAN is able to receive valid data frames and valid remote frames,
 but it sends only recessive bits on the CAN bus
 and it cannot start a transmission.
 options:
 OFF --> Normal Mode
 ON --> Silent Mode
 ****************************************/
#define CAN_SILENT_MODE	OFF

/******************************************
 Time Triggered communication mode
 -> send the timer value in the last 2 bytes of data
 options:
 1-OFF
 2-ON
 *******************************************/
#define CAN_TIME_TRIGGERED_COMMUNICATION_MODE	OFF

/*******************************************
 Automatic Bus OFF Control
 options:
 SW_REQUEST
 AUTO_BY_HW
 ******************************************/
#define CAN_BUS_OFF_MANAGMENT	SW_REQUEST

/*******************************************
 Automatic Wake UP Control
 options:
 SW_REQUEST
 AUTO_BY_HW
 ******************************************/
#define CAN_WAKE_UP_MANAGMENT	SW_REQUEST

/***********************************************
 CAN Retransmission Status
 -> in case the frame failed to be transmit successfully
 either by fram error or lost arbitration
 does the transmitter retransmit or not
 options:
 OFF -> if failed retransmit
 ON -> only once
 ***********************************************/
#define CAN_NO_RETRANSMISSION		ON

/**********************************************
 Receive FIFO Locked Mode
 -> if the locked mode is on and the FIFO is FULL
 any incoming data will be discarded
 -> if the locked is off and the FIFO is FULL
 any incoming data will overwrite the last one
 options:
 OFF -> overwrite
 ON  -> discard
 *******************************************/
#define CAN_RECEIVE_FIFO_LOCKED_MODE	ON

/********************************************
 Transmitter FIFO Priority
 -> this options takes effect when there is
 more than one mailbox are pending
 0: priority by identifier ( the lower the value the higher the priority)
 1: priority by chronological order (who requested first)
 ******************************************/
#define CAN_TRANSMIT_FIFO_PRIORITY		1

/////////////////////////////////TIMING///////////////////////

/*************************************************
 BaudRate Prescaler of the CAN used to specify
 -> the length of time quanta
 tq = (baudrate+1)* Tclk
 where Tclk is the clock cycle time for the bus
 on which there is the CAN peripheral (APB1)
 -> this field is 10-bits wide (0:1023)
 ************************************************/
#define CAN_BAUD_RATE_PRESCALER		1

/***************************************************
 These bits define the maximum number of time quanta
 the CAN hardware is allowed to
 lengthen or shorten a bit to perform the resynchronization.
 t_RJW = tq * [SJW+1]
 -> this field is 2-bits wide (0:3)
 **************************************************/
#define CAN_RESYNC_JUMP_WIDTH	16

/***************************************************
 These bits define the number of time quanta in Time Segment 1
 t_BS1 = tq * [TS1+1]
 -> this field is 4-bits wide (0:15)
 **************************************************/
#define CAN_TIME_SIGMENT_1	12

/***************************************************
 These bits define the number of time quanta in Time Segment 2
 t_BS2 = tq * [TS2+1]
 -> this field is 3-bits wide (0:7)
 **************************************************/
#define CAN_TIME_SIGMENT_2	1


#define WORKING		0
#define FROZEN		1

#define OFF			0
#define ON			1

#define SW_REQUEST	0
#define AUTO_BY_HW	1

#define DEFAULT_BAUDRATE        0


#endif
