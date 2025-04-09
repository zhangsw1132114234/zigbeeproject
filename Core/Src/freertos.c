/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MyApplication.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId WebServer_TaskHandle;
osThreadId ZigBee_TaskHandle;
osThreadId LVGL_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void WebServerTask(void const * argument);
void ZigBeeTask(void const * argument);
void LVGLTask(void const * argument);

extern void MX_LWIP_Init(void);
extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of WebServer_Task */
  osThreadDef(WebServer_Task, WebServerTask, osPriorityAboveNormal, 0, 4096);
  WebServer_TaskHandle = osThreadCreate(osThread(WebServer_Task), NULL);

  /* definition and creation of ZigBee_Task */
  osThreadDef(ZigBee_Task, ZigBeeTask, osPriorityNormal, 0, 256);
  ZigBee_TaskHandle = osThreadCreate(osThread(ZigBee_Task), NULL);

  /* definition and creation of LVGL_Task */
  osThreadDef(LVGL_Task, LVGLTask, osPriorityBelowNormal, 0, 2048);
  LVGL_TaskHandle = osThreadCreate(osThread(LVGL_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_WebServerTask */
/**
  * @brief  Function implementing the WebServer_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_WebServerTask */
void WebServerTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN WebServerTask */
  printf("system is runnung！\r\n");
  /* Infinite loop */
  for(;;)
  {
    http_server_socket_thread(NULL);
    osDelay(1);
  }
  /* USER CODE END WebServerTask */
}

/* USER CODE BEGIN Header_ZigBeeTask */
/**
* @brief Function implementing the ZigBee_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ZigBeeTask */
void ZigBeeTask(void const * argument)
{
  /* USER CODE BEGIN ZigBeeTask */
  /* Infinite loop */
  for(;;)
  {
    if(_USART6.RX_STA & 0x8000)
    {
      //对接收到的数据进行解析
      ProtocolParse(_USART6.RX_BUF, 8);
      printf("SensorValue[0]:%d,SensorValue[1]:%d\r\n", SensorValue[0], SensorValue[1]);
      _USART6.RX_STA = 0;
    }
    osDelay(1);
  }
  /* USER CODE END ZigBeeTask */
}

/* USER CODE BEGIN Header_LVGLTask */
/**
* @brief Function implementing the LVGL_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LVGLTask */
void LVGLTask(void const * argument)
{
  /* USER CODE BEGIN LVGLTask */
  /* Infinite loop */
  for(;;)
  {
    // LVGL
    lv_tick_inc(1);
    lv_timer_handler();
    // 更新数据
//    GUI_Update();
    osDelay(1);
  }
  /* USER CODE END LVGLTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
