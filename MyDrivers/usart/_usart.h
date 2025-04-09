#ifndef __MYUSART_H__
#define __MYUSART_H__
#include "MyApplication.h"

#define REC_LEN 200     // 最大接受字节数



typedef struct{
    UART_HandleTypeDef* UART_Handle; // 串口句柄
    
    uint8_t USART_TIMER_EN; // 串口定时器使能  1：打开    0：关闭
    uint16_t USART_TIMER;   // 用于接收数据的定时器
    
    uint16_t RX_STA;    // 接收标志位 bit15：接收完成标志，bit14：接收到0x0d，bit13~0：接收到的有效字节数目
    uint8_t New_Data;   // 新接收到的数据
    uint8_t RX_BUF[REC_LEN];    // 数据接收数组
}USART1_t;

typedef struct{
    UART_HandleTypeDef* UART_Handle; // 串口句柄
    
    uint8_t USART_TIMER_EN; // 串口定时器使能  1：打开    0：关闭
    uint16_t USART_TIMER;   // 用于接收数据的定时器
    
    uint16_t RX_STA;    // 接收标志位 bit15：接收完成标志，bit14：接收到0x0d，bit13~0：接收到的有效字节数目
    uint8_t New_Data;   // 新接收到的数据
    uint8_t RX_BUF[REC_LEN];    // 数据接收数组
}USART6_t;

/* extern variables-----------------------------------------------------------*/
extern USART1_t _USART1; //用于打印
extern USART6_t _USART6; //用于打印
/* extern function prototypes-------------------------------------------------*/
extern void USART1_printf (char *fmt, ...);
#endif
/********************************************************
  End Of File
********************************************************/
