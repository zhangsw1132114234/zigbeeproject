/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void LED_FUN(LED_Num_t, void(*CallBack)(LED_Num_t));

/* Private function prototypes------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/
LED_t LED = {LED_FUN};

/*
  * @name   LED_FUN
  * @brief  LED中间层
  * @param  LED_Num -> LED编号
            CallBack-> 回调函数
  * @retval None
*/
static void LED_FUN(LED_Num_t LED_Num, void(*CallBack)(LED_Num_t))
{
    (*CallBack)(LED_Num);
}

/*
  * @name   LED_ON
  * @brief  LED打开
  * @param  LED_Num -> LED编号
  * @retval None
*/
void LED_ON(LED_Num_t LED_Num)
{       
    switch(LED_Num)
    {
        case LED1: HAL_GPIO_WritePin(DS0_GPIO_Port, DS0_Pin, GPIO_PIN_RESET); break;
        case LED2: HAL_GPIO_WritePin(DS1_GPIO_Port, DS1_Pin, GPIO_PIN_RESET); break;
    }
}       
/*
  * @name   LED_OFF
  * @brief  LED关闭
  * @param  LED_Num -> LED编号
  * @retval None
*/
void LED_OFF(LED_Num_t LED_Num)
{       
    switch(LED_Num)
    {
        case LED1: HAL_GPIO_WritePin(DS0_GPIO_Port, DS0_Pin, GPIO_PIN_SET); break;
        case LED2: HAL_GPIO_WritePin(DS1_GPIO_Port, DS1_Pin, GPIO_PIN_SET); break;
    }
}    

/*
  * @name   LED_Flip
  * @brief  LED翻转
  * @param  LED_Num -> LED编号
  * @retval None
*/
void LED_Flip(LED_Num_t LED_Num)
{
    switch(LED_Num)
    {
        case LED1: HAL_GPIO_TogglePin(DS0_GPIO_Port, DS0_Pin); break;
        case LED2: HAL_GPIO_TogglePin(DS1_GPIO_Port, DS1_Pin); break;
    }
}

/********************************************************
  End Of File
********************************************************/

