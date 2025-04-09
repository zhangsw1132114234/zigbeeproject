/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/
USART1_t _USART1 = {
    &huart1,
    0,
    0,
    0,
    0,
    {0}
};

USART6_t _USART6 = {
    {0}
};

/*
  * @name   USART1_printf
  * @brief  串口1打印函数
  * @param  fmt -> 打印格式
            ... -> 可变长字符
  * @retval None
*/
void USART1_printf (char *fmt, ...)
{
    char buff[REC_LEN + 1];
    uint16_t i=0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buff, REC_LEN+1, fmt,  arg_ptr);//数据转换
    i=strlen(buff);//得出数据长度
    if(strlen(buff)>REC_LEN)i=REC_LEN;//如果长度大于最大值，则长度等于最大值（多出部分忽略）
    HAL_UART_Transmit(&huart1,(uint8_t  *)buff,i,0xffff);//串口发送函数（串口号，内容，数量，溢出时间）
    va_end(arg_ptr);
}
/********************************************************
  End Of File
********************************************************/

