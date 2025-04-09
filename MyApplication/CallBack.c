/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      
// ���ڻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // Zigbee���ݽ���
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

// ��ʱ���ص����� main.c����Cubemax����
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  // ��ʱ��6�жϺ��� 5ms
//  if(htim == &htim6)
//  {
//    // ϵͳ����ʱ��
//    Timer6.usMCU_Run_Timer++;
//    // ���ڰ������
//    KEY_STA_Machine.Scan_Timer++;
//    KEY_STA_Machine.Double_Click_Timer++;
//    KEY_STA_Machine.Press_Timer++;
//    // ����SPI_FLash
//    SPI_FLASH.SPI_FLASH_Delay_Timer++;
//    // ����6��ʱ��
//    _USART6.USART_TIMER++;
//    if(((_USART6.USART_TIMER % TIMER_200ms) == 0) && _USART6.USART_TIMER_EN)
//    {
//      _USART6.RX_BUF[_USART6.RX_STA&0x7FFF] = 0;  // ��ӽ�����
//      _USART6.RX_STA |= 0x8000;                   // ʱ�䵽ǿ�ƽ���
//      _USART6.USART_TIMER_EN = 0;                 // �رն���ʱ��
//      _USART6.USART_TIMER = 0;
//    }
//  }
//}
/********************************************************
  End Of File
********************************************************/
