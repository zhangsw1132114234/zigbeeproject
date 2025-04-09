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
#define KEY_SCAN_TIMER  TIMER_10ms          // ״̬ɨ��ʱ��
#define KEY_Double_Click_Timer  TIMER_200ms // ˫����ʱʱ��
#define KEY_Press_Timer TIMER_2S            // ������ʱ��ʱ��

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
  * @brief  KEY0ɨ�躯��
  * @param  None
  * @retval None
*/
static void Key0_Detect(void)
{
    Key_Detect(&KEY0, &KEY_STA_Machine, KEY0_GPIO_Port, KEY0_Pin, GPIO_PIN_RESET);
}

/*
  * @name   Key0_Detect
  * @brief  KEY0ɨ�躯��
  * @param  None
  * @retval None
*/
static void Key1_Detect(void)
{
    Key_Detect(&KEY1, &KEY_STA_Machine, KEY2_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET);
}

/*
  * @name   Key0_Detect
  * @brief  KEY0ɨ�躯��
  * @param  None
  * @retval None
*/
static void Key2_Detect(void)
{
    Key_Detect(&KEY2, &KEY_STA_Machine, KEY2_GPIO_Port, KEY2_Pin, GPIO_PIN_RESET);
}

/*
  * @name   Key0_Detect
  * @brief  KEY0ɨ�躯��
  * @param  None
  * @retval None
*/
static void KeyUp_Detect(void)
{
    //Key_Detect(&KEY_UP, &KEY_STA_Machine, WK_UP_GPIO_Port, WK_UP_Pin, GPIO_PIN_SET);
}

/*
  * @name   Key_Detect
  * @brief  ����ɨ�躯��
  * @param  key               -> �����ṹ��
            KEY_STA_Machine   -> ״̬���ṹ��
            GPIOx             -> ����GPIO�˿ں�
            GPIO_Pin          -> ����GPIOPIN
            GPIO_PinState     -> �������µ�ƽ
  * @retval None
*/
static void Key_Detect(KEY_t* key, KEY_STA_Machine_t* KEY_STA_Machine, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    static uint8_t Key_Buf = 0;
    if(KEY_STA_Machine->Scan_Timer >= KEY_SCAN_TIMER)    // ����ɨ��ʱ��
    {
        KEY_STA_Machine->Scan_Timer = 0; // ���ɨ�趨ʱ��
        
        switch(KEY_STA_Machine->ucSTA_Machine_State)
        {
            case KEY_Up: //̧��״̬
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // ����
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_DownShake;
                }else // δ����
                {
                    if(Key_Buf == 1 && KEY_STA_Machine->Double_Click_Timer >= KEY_Double_Click_Timer)    // ��������
                    {
                        key->KEY_Flag = 1;
                        key->Click = 1;
                        Key_Buf = 0;
                    }
                }
                break;
                
            case KEY_DownShake: //���¶���
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // ����
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_Down;
                    if(Key_Buf == 1) //˫��
                    {
                        Key_Buf = 0;
                        key->KEY_Flag = 1;
                        key->Double_Click = 1;
                    }else
                    {
                        Key_Buf = 1;    // ��¼��������
                    }
                    
                    KEY_STA_Machine->Double_Click_Timer = 0; // ��˫����ʱ��
                    KEY_STA_Machine->Press_Timer = 0;        // ������ʱ����
                }else // δ����
                {
                    
                }
                break;
                
            case KEY_Down: //����
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // ����
                {
                    
                    if(key->Double_Click == 0)   // ˫���������ȼ����ڳ���
                    {
                        if(KEY_STA_Machine->Press_Timer >= KEY_Press_Timer)  // ��������
                        {
                            KEY_STA_Machine->ucSTA_Machine_State = KEY_UpShake;
                            key->KEY_Flag = 1;
                            key->Press = 1;
                            Key_Buf = 0;
                        }
                    }
                    
                }else // δ����
                {
                    KEY_STA_Machine->ucSTA_Machine_State = KEY_UpShake;
                }
                break;
                
            case KEY_UpShake: //̧�𶶶�
                if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState) // ����
                {
                    
                }else // δ����
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
