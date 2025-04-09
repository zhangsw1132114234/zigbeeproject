#ifndef __CALLBACK_H__
#define __CALLBACK_H__
#include "MyApplication.h"




/* extern variables-----------------------------------------------------------*/

/* extern function prototypes-------------------------------------------------*/
extern void DMA_Receive_Set(void);
extern void HAL_UART_IdleCallback(UART_HandleTypeDef *huart);
#endif
/********************************************************
  End Of File
********************************************************/
