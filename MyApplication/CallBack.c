/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      
// 串口回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // Zigbee数据接收
  if(huart == &huart6)
  {
    //USART1_printf("%c", _USART6.New_Data);
    if(_USART6.RX_STA < REC_LEN)
    {
      _USART6.USART_TIMER_EN = 1;
      _USART6.USART_TIMER = 0;
      _USART6.RX_BUF[_USART6.RX_STA++] = _USART6.New_Data;
    }else
    {
        _USART6.RX_STA |= 0x8000;
    }
    HAL_UART_Receive_IT(&huart6,(uint8_t *)&_USART6.New_Data,1);
  }
}

// 定时器回调函数 main.c中由Cubemax生成
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  // 定时器6中断函数 5ms
//  if(htim == &htim6)
//  {
//    // 系统运行时间
//    Timer6.usMCU_Run_Timer++;
//    // 用于按键检测
//    KEY_STA_Machine.Scan_Timer++;
//    KEY_STA_Machine.Double_Click_Timer++;
//    KEY_STA_Machine.Press_Timer++;
//    // 用于SPI_FLash
//    SPI_FLASH.SPI_FLASH_Delay_Timer++;
//    // 串口6定时器
//    _USART6.USART_TIMER++;
//    if(((_USART6.USART_TIMER % TIMER_200ms) == 0) && _USART6.USART_TIMER_EN)
//    {
//      _USART6.RX_BUF[_USART6.RX_STA&0x7FFF] = 0;  // 添加结束符
//      _USART6.RX_STA |= 0x8000;                   // 时间到强制结束
//      _USART6.USART_TIMER_EN = 0;                 // 关闭定定时器
//      _USART6.USART_TIMER = 0;
//    }
//  }
//}
/********************************************************
  End Of File
********************************************************/
