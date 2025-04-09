/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/

//zigbee采集到的数据
int SensorValue[3];

/* Private function prototypes------------------------------------------------*/     
/*
  * @name   Memory_Clr
  * @brief  清除缓存
  * @param  pucBuffer   -> 内存首地址
            LEN         -> 内存长度
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
//延时
void delay_us(uint32_t us) //利用CPU循环实现的非精准应用的微秒延时函数
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 8000000 * us); //使用HAL_RCC_GetHCLKFreq()函数获取主频值，经算法得到1微秒的循环次数
    while (delay--); //循环delay次，达到1微秒延时
}


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

/********************************************************
  End Of File
********************************************************/
