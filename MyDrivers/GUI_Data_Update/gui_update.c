/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/
extern lv_chart_series_t * ui_Chart1_series_1;
// 用于更新lvgl界面中的数据
void GUI_Update(void)
{
  char temp[5];
  char hum[5];
  // 温度更新
  sprintf(temp, "%d°", SensorValue[0]);
  lv_label_set_text(ui_Label8, temp);
  // 滚动方式添加新数据点
  lv_chart_set_next_value(ui_Chart1, ui_Chart1_series_1, SensorValue[0]);
  // 触发重绘
  lv_chart_refresh(ui_Chart1);
  // 湿度更新
  sprintf(hum, "%d%%", SensorValue[1]);
  lv_label_set_text(ui_Label4, hum);
  // 滚动方式添加新数据点
  lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_1, SensorValue[1]);
  // 触发重绘
  lv_chart_refresh(ui_Chart2);
  // 风扇控制
  
  // 灯光控制
}

void switch2_event_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        // 开关被打开
        // 执行开操作，例如打开风扇
        FanControl(1);
    } else {
        // 开关被关闭
        // 执行关操作，例如关闭风扇
        FanControl(0);
    }
}

void switch3_event_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        // 开灯
        LED.LED_FUN(LED1, LED_ON);
    } else {
        // 关灯
        LED.LED_FUN(LED1, LED_OFF);
    }
}

void chart_timer_cb(lv_timer_t * timer)
{
    GUI_Update();
}
/********************************************************
  End Of File
********************************************************/
