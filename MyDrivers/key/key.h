#ifndef __KEY_H__
#define __KEY_H__
#include "MyApplication.h"



typedef struct{
    uint8_t volatile KEY_Flag;  // ������־λ
    uint8_t Click;              // ������־λ
    uint8_t Double_Click;       // ˫����־λ
    uint8_t Press;              // ������־λ
    
    void (*Key_Detect)(void);   // ����ɨ��
}KEY_t;  

typedef enum{
    KEY_Up          = (uint8_t)0x01,    // ̧��
    KEY_DownShake   = (uint8_t)0x02,    // ���¶���
    KEY_Down        = (uint8_t)0x03,    // ����
    KEY_UpShake     = (uint8_t)0x04,    // ̧�𶶶�
}KEY_STA_Machine_State_t;

typedef struct{
    KEY_STA_Machine_State_t ucSTA_Machine_State;
    
    uint16_t Scan_Timer;        // ����ɨ�趨ʱ��
    uint16_t Press_Timer;       // ������ʱ��
    uint16_t Double_Click_Timer;// ˫����ʱ��
    
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
