/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Timer6_Start_IT(void);
/* Private function prototypes------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/
Timer6_t Timer6 = {
    0,
    Timer6_Start_IT
};


/*
  * @name   Timer6_Start_IT
  * @brief  中断方式启动定时器6
  * @param  None
  * @retval None
*/
static void Timer6_Start_IT(void)
{
    HAL_TIM_Base_Start_IT(&htim6);
}
/********************************************************
  End Of File
********************************************************/
