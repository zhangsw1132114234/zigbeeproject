/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Run(void); 
static void Error_Handler(void);
static void Assert_Failed(void);
	
/* Public variables-----------------------------------------------------------*/
System_t System = 
{
	Run,
	Error_Handler,
	Assert_Failed
};

extern int SensorValue[3];

/* Private function prototypes------------------------------------------------*/      
// fatfs 测试代码
void FATFS_Init()
{
  uint8_t retUSB;
  char USBPath[4];
  FATFS USBFatFS;
  FIL USBFile;
  char SensorBuff[100];
  uint32_t u32Wbytes;
  
  retUSB = FATFS_LinkDriver(&USBH_Driver, USBPath);
  
  if(f_mount(&USBFatFS, USBPath, 1) == FR_OK)
  {
    if(f_open(&USBFile, (const char*)"sensor.csv", FA_CREATE_ALWAYS|FA_WRITE) == FR_OK)
    {
      sprintf(SensorBuff, "序号,温度,湿度\r\n");
      f_write(&USBFile, SensorBuff, strlen(SensorBuff), &u32Wbytes);
      for(int i = 0; i < 10; i++)
      {
        sprintf(SensorBuff, "%d,%d,%d\r\n",i,i+10,i+100);
        f_write(&USBFile, SensorBuff, strlen(SensorBuff), &u32Wbytes);
      }
      f_close(&USBFile);
    }
  }
}
/*
	* @name   Run
	* @brief  系统运行
	* @param  None
	* @retval None      
*/

    
static void Run()
{

  // LVGL
  lv_tick_inc(1);
  lv_timer_handler();
  
  if(_USART6.RX_STA & 0x8000)
  {
    //对接收到的数据进行解析
    ProtocolParse(_USART6.RX_BUF, 8);
    printf("SensorValue[0]:%d,SensorValue[1]:%d\r\n", SensorValue[0], SensorValue[1]);
    _USART6.RX_STA = 0;
  }
  
  //发送控制数据
  KEY0.Key_Detect();
  if(KEY0.KEY_Flag)
  {
      if(KEY0.Click)
      {
          FanControl(1);
      }
      
      if(KEY0.Double_Click)
      {
          FanControl(0);
      }
      if(KEY0.Press)
      {
          
      }
      
      KEY0.KEY_Flag = 0;
      KEY0.Click = 0;
      KEY0.Double_Click = 0;
      KEY0.Press = 0;
  }
}

/*
	* @name   Error_Handler
	* @brief  系统错误处理
	* @param  None
	* @retval None      
*/
static void Error_Handler()
{
	/* User can add his own implementation to report the HAL error return state */
}

/*
	* @name   Assert_Failed
	* @brief  函数参数错误处理
	* @param  None
	* @retval None      
*/
static void Assert_Failed()
{
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
/********************************************************
  End Of File
********************************************************/
