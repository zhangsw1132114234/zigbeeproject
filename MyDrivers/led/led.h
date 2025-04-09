#ifndef __LED_H__
#define __LED_H__
#include "MyApplication.h"

typedef enum{
    LED1 = (uint8_t)0x01,
    LED2 = (uint8_t)0x02,
}LED_Num_t;

typedef struct{
    void (*LED_FUN)(LED_Num_t, void(*CallBack)(LED_Num_t));
}LED_t;


/* extern variables-----------------------------------------------------------*/
extern LED_t LED;

/* extern function prototypes-------------------------------------------------*/
extern void LED_ON(LED_Num_t);     //打开
extern void LED_OFF(LED_Num_t);    //关闭
extern void LED_Flip(LED_Num_t);   //翻转
#endif
/********************************************************
  End Of File
********************************************************/
