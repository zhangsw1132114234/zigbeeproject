#ifndef __MyInit_H__
#define __MyInit_H__
#include "MyApplication.h"

//����ṹ������
typedef struct
{
	void (*Peripheral_Set)(void);
} MyInit_t;


/* extern variables-----------------------------------------------------------*/
extern MyInit_t  MyInit;
/* extern function prototypes-------------------------------------------------*/
void delay_us(uint32_t us);
#endif
/********************************************************
  End Of File
********************************************************/
