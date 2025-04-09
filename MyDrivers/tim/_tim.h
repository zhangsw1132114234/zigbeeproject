#ifndef __MYTIM_H__
#define __MYTIM_H__
#include "MyApplication.h"

// 定时时间 根据定时器中断周期配置 定时器周期：5ms
typedef enum{
    TIMER_10ms  =   (uint16_t)2,
    TIMER_50ms  =   (uint16_t)10,
    TIMER_100ms =   (uint16_t)20,
    TIMER_200ms =   (uint16_t)40,
    TIMER_500ms =   (uint16_t)100,
    TIMER_1S    =   (uint16_t)200,
    TIMER_2S    =   (uint16_t)400,
    TIMER_3S    =   (uint16_t)600,
    TIMER_5S    =   (uint16_t)1000,
    TIMER_10S   =   (uint16_t)2000,
    TIMER_3min  =   (uint16_t)36000,
}TIMER_Value_t;

typedef struct{
    uint16_t volatile usMCU_Run_Timer;  // 系统运行时间
    
    void (*Timer6_Start_IT)(void);      // 定时器6以中断方式启动
}Timer6_t;


/* extern variables-----------------------------------------------------------*/
extern Timer6_t Timer6;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
