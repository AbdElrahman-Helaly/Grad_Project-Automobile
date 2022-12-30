
#include "STD_TYPE.h"
#include "BIT_MATH.h"

#include "UART_interface.h"
#include "UART_register.h"

void UART_voidInit(void)
{
  // Set BaudRate -> 9600/16MhZ
  UBRRL = 51;
  UBRRH = 0;
  // Set Frame Format -> 8 data, 1 stop, No Parity
  UCSRC = 0x86;
  // Enable RX and TX
  UCSRB = 0x18;
}

void UART_voidSendChar(u8 data)
{
  // Wait until transmission Register Empty
  while((UCSRA&0x20) == 0x00);
  UDR_T = data;
}

u8 UART_voidGetChar(void)
{
  u8 Result;
  // Wait until Reception Complete
  while((UCSRA&0x80) == 0x00);
  Result = UDR_R;
  
  /* Clear Flag */
  SET_BIT(UCSRA,7);
  return Result;
  
}

