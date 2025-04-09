#ifndef __MYUSART_H__
#define __MYUSART_H__
#include "MyApplication.h"

#define REC_LEN 200     // �������ֽ���



typedef struct{
    UART_HandleTypeDef* UART_Handle; // ���ھ��
    
    uint8_t USART_TIMER_EN; // ���ڶ�ʱ��ʹ��  1����    0���ر�
    uint16_t USART_TIMER;   // ���ڽ������ݵĶ�ʱ��
    
    uint16_t RX_STA;    // ���ձ�־λ bit15��������ɱ�־��bit14�����յ�0x0d��bit13~0�����յ�����Ч�ֽ���Ŀ
    uint8_t New_Data;   // �½��յ�������
    uint8_t RX_BUF[REC_LEN];    // ���ݽ�������
}USART1_t;

typedef struct{
    UART_HandleTypeDef* UART_Handle; // ���ھ��
    
    uint8_t USART_TIMER_EN; // ���ڶ�ʱ��ʹ��  1����    0���ر�
    uint16_t USART_TIMER;   // ���ڽ������ݵĶ�ʱ��
    
    uint16_t RX_STA;    // ���ձ�־λ bit15��������ɱ�־��bit14�����յ�0x0d��bit13~0�����յ�����Ч�ֽ���Ŀ
    uint8_t New_Data;   // �½��յ�������
    uint8_t RX_BUF[REC_LEN];    // ���ݽ�������
}USART6_t;

/* extern variables-----------------------------------------------------------*/
extern USART1_t _USART1; //���ڴ�ӡ
extern USART6_t _USART6; //���ڴ�ӡ
/* extern function prototypes-------------------------------------------------*/
extern void USART1_printf (char *fmt, ...);
#endif
/********************************************************
  End Of File
********************************************************/
