/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/

//zigbee�ɼ���������
int SensorValue[3];

/* Private function prototypes------------------------------------------------*/     
/*
  * @name   Memory_Clr
  * @brief  �������
  * @param  pucBuffer   -> �ڴ��׵�ַ
            LEN         -> �ڴ泤��
  * @retval None
*/
void Memory_Clr(uint8_t* pucBuffer, uint16_t LEN)
{
    uint16_t i;
    
    for(i = 0; i< LEN; i++)
    {
        *(pucBuffer + i) = (uint8_t)0;
    }
}
//��ʱ
void delay_us(uint32_t us) //����CPUѭ��ʵ�ֵķǾ�׼Ӧ�õ�΢����ʱ����
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 8000000 * us); //ʹ��HAL_RCC_GetHCLKFreq()������ȡ��Ƶֵ�����㷨�õ�1΢���ѭ������
    while (delay--); //ѭ��delay�Σ��ﵽ1΢����ʱ
}


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

/********************************************************
  End Of File
********************************************************/
