/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Peripheral_Set(void); 

/* Public variables-----------------------------------------------------------*/
MyInit_t MyInit = 
{
	Peripheral_Set
};

/* Private function prototypes------------------------------------------------*/      

void btn_demo()
{
  lv_obj_t* screen = lv_scr_act();
  // 普通按钮
//  lv_obj_t* btn = lv_btn_create(screen);
//  
//  static lv_style_t style;
//  lv_style_init(&style);
//  
//  lv_style_set_width(&style, 100);
//  lv_style_set_height(&style, 60);
//  
//  lv_obj_add_style(btn, &style, 0);
//  
//  lv_obj_center(btn);
  
  //可选按钮
  lv_obj_t* btn2 = lv_btn_create(screen);
  
  lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 80);
  lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_height(btn2, LV_SIZE_CONTENT);
  
  lv_obj_t* label = lv_label_create(btn2);
  lv_label_set_text(label, "Toggle");
  lv_obj_center(label);

}

/*
	* @name   Peripheral_Set
	* @brief  外设设置
	* @param  None
	* @retval None      
*/
static void Peripheral_Set()
{
  // 用于按键检测
  Timer6.Timer6_Start_IT();
  
  // lvgl
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
  
  
  ui_init();
  
  // 启动UART6接收中断
  HAL_UART_Receive_IT(&huart6,(uint8_t *)&_USART6.New_Data,1);
}

/********************************************************
  End Of File
********************************************************/
