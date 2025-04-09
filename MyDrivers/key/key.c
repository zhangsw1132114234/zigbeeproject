/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

//KEY0.Key_Detect();
//if(KEY0.KEY_Flag)
//{
//    if(KEY0.Click)
//    {
//        
//    }
//    
//    if(KEY0.Double_Click)
//    {
//        
//    }
//    if(KEY0.Press)
//    {
//        
//    }
//    
//    KEY0.KEY_Flag = 0;
//    KEY0.Click = 0;
//    KEY0.Double_Click = 0;
//    KEY0.Press = 0;
//}
/* Private define-------------------------------------------------------------*/
#define KEY_SCAN_TIMER  TIMER_10ms          // 状态扫描时间
#define KEY_Double_Click_Timer  TIMER_200ms // 双击超时时间
#define KEY_Press_Timer TIMER_2S            // 长按超时定时器

/* Private variables----------------------------------------------------------*/


/* Private function prototypes------------------------------------------------*/
static void Key_Detect(KEY_t* key, KEY_STA_Machine_t* KEY_STA_Machine, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
static void Key0_Detect(void);
static void Key1_Detect(void);
static void Key2_Detect(void);
static void KeyUp_Detect(void);
/* Public variables-----------------------------------------------------------*/
KEY_t KEY0      = {0, 0, 0, 0, Key0_Detect};
KEY_t KEY1      = {0, 0, 0, 0, Key1_Detect};
KEY_t KEY2      = {0, 0, 0, 0, Key2_Detect};
KEY_t KEY_UP    = {0, 0, 0, 0, KeyUp_Detect};

KEY_STA_Machine_t KEY_STA_Machine = {KEY_Up, 0, 0, 0};

/*
  * @name   Key0_Detect
  * @brief  KEY0扫描函数
  * @param  None
  * @retval None
*/
static void Key0_Detect(void)
{
    Key_Detect(&KEY0, &KEY_STA_Machine, KEY0_GPIO_Port, KEY0_Pin, GPIO_PIN_RESET);
}

/*
  * @name   Key0_Detect
  * @brief  KEY0扫描函数
  * @param  None
  * @retval None
*/
static void Key1_Detect(void)
{
    Key_Detect(&KEY1, &KEY_STA_Machine, KEY2_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET);
}

/*
  * @name   Key0_Detect
  * @brief  KEY0扫描函数
  * @param  None
  * @retval None
*/
static void Key2_Detect(void)
{
    Key_Detect(&KEY2, &KEY_STA_Machine, KEY2_GPIO_Port, KEY2_Pin, GPIO_PIN_RESET);
}

/*
  * @name   Key0_Detect
  * @brief  KEY0扫描函数
  * @param  None
  * @retval None
*/
static void KeyUp_Detect(void)
{
    //Key_Detect(&KEY_UP, &KEY_STA_Machine, WK_UP_GPIO_Port, WK_UP_Pin, GPIO_PIN_SET);
}

/*
  * @name   Key_Detect
  * @brief  按键扫描函数
  * @param  key               -> 按键结构体
            KEY_STA_Machine   -> 状态机结构体
            GPIOx             -> 按键GPIO端口号
            GPIO_Pin          -> 按键GPIOPIN
            GPIO_PinState     -> 按键按下电平
  * @retval None
*/
static void Key_Detect(KEY_t* key, KEY_STA_Machine_t* KEY_STA_Machine, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    static uint8_t Key_Buf = 0;
    if(KEY_STA_Machine->Scan_Timer >= KEY_SCAN_TIMER)    // 超过扫描时间
    {
        KEY_STA_Machine->Scan_Timer = 0; // 清除扫描定时器
        
        switch(KEY_STA_Machine->ucSTA_Machine_State)
        {
            case KEY_Up: //抬起状态
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // 按下
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_DownShake;
                }else // 未按下
                {
                    if(Key_Buf == 1 && KEY_STA_Machine->Double_Click_Timer >= KEY_Double_Click_Timer)    // 单击操作
                    {
                        key->KEY_Flag = 1;
                        key->Click = 1;
                        Key_Buf = 0;
                    }
                }
                break;
                
            case KEY_DownShake: //按下抖动
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // 按下
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_Down;
                    if(Key_Buf == 1) //双击
                    {
                        Key_Buf = 0;
                        key->KEY_Flag = 1;
                        key->Double_Click = 1;
                    }else
                    {
                        Key_Buf = 1;    // 记录按键按下
                    }
                    
                    KEY_STA_Machine->Double_Click_Timer = 0; // 打开双击定时器
                    KEY_STA_Machine->Press_Timer = 0;        // 长按定时器打开
                }else // 未按下
                {
                    
                }
                break;
                
            case KEY_Down: //按下
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // 按下
                {
                    
                    if(key->Double_Click == 0)   // 双击操作优先级大于长按
                    {
                        if(KEY_STA_Machine->Press_Timer >= KEY_Press_Timer)  // 长按操作
                        {
                            KEY_STA_Machine->ucSTA_Machine_State = KEY_UpShake;
                            key->KEY_Flag = 1;
                            key->Press = 1;
                            Key_Buf = 0;
                        }
                    }
                    
                }else // 未按下
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_UpShake;
                }
                break;
                
            case KEY_UpShake: //抬起抖动
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // 按下
                {
                    
                }else // 未按下
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_Up;
                }
                break;
        }
    }
    
}
/********************************************************
  End Of File
********************************************************/
