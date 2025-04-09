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
  * @brief  ����1��ӡ����
  * @param  fmt -> ��ӡ��ʽ
            ... -> �ɱ䳤�ַ�
  * @retval None
*/
void USART1_printf (char *fmt, ...)
{
    char buff[REC_LEN + 1];
    uint16_t i=0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buff, REC_LEN+1, fmt,  arg_ptr);//����ת��
    i=strlen(buff);//�ó����ݳ���
    if(strlen(buff)>REC_LEN)i=REC_LEN;//������ȴ������ֵ���򳤶ȵ������ֵ��������ֺ��ԣ�
    HAL_UART_Transmit(&huart1,(uint8_t  *)buff,i,0xffff);//���ڷ��ͺ��������ںţ����ݣ����������ʱ�䣩
    va_end(arg_ptr);
}
/********************************************************
  End Of File
********************************************************/

