#ifndef __KEY_H__
#define __KEY_H__
#include "MyApplication.h"



typedef struct{
    uint8_t volatile KEY_Flag;  // 按键标志位
    uint8_t Click;              // 单击标志位
    uint8_t Double_Click;       // 双击标志位
    uint8_t Press;              // 长按标志位
    
    void (*Key_Detect)(void);   // 按键扫描
}KEY_t;  

typedef enum{
    KEY_Up          = (uint8_t)0x01,    // 抬起
    KEY_DownShake   = (uint8_t)0x02,    // 按下抖动
    KEY_Down        = (uint8_t)0x03,    // 按下
    KEY_UpShake     = (uint8_t)0x04,    // 抬起抖动
}KEY_STA_Machine_State_t;

typedef struct{
    KEY_STA_Machine_State_t ucSTA_Machine_State;
    
    uint16_t Scan_Timer;        // 按键扫描定时器
    uint16_t Press_Timer;       // 长按定时器
    uint16_t Double_Click_Timer;// 双击定时器
    
}KEY_STA_Machine_t;


/* extern variables-----------------------------------------------------------*/
extern KEY_t KEY0;
extern KEY_t KEY1;
extern KEY_t KEY2;
extern KEY_t KEY_UP;

extern KEY_STA_Machine_t KEY_STA_Machine;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
