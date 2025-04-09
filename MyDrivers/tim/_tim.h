#ifndef __MYTIM_H__
#define __MYTIM_H__
#include "MyApplication.h"

// ��ʱʱ�� ���ݶ�ʱ���ж��������� ��ʱ�����ڣ�5ms
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
    uint16_t volatile usMCU_Run_Timer;  // ϵͳ����ʱ��
    
    void (*Timer6_Start_IT)(void);      // ��ʱ��6���жϷ�ʽ����
}Timer6_t;


/* extern variables-----------------------------------------------------------*/
extern Timer6_t Timer6;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
