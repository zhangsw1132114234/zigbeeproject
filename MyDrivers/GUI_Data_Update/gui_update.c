/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/
extern lv_chart_series_t * ui_Chart1_series_1;
// ���ڸ���lvgl�����е�����
void GUI_Update(void)
{
  char temp[5];
  char hum[5];
  // �¶ȸ���
  sprintf(temp, "%d��", SensorValue[0]);
  lv_label_set_text(ui_Label8, temp);
  // ������ʽ��������ݵ�
  lv_chart_set_next_value(ui_Chart1, ui_Chart1_series_1, SensorValue[0]);
  // �����ػ�
  lv_chart_refresh(ui_Chart1);
  // ʪ�ȸ���
  sprintf(hum, "%d%%", SensorValue[1]);
  lv_label_set_text(ui_Label4, hum);
  // ������ʽ��������ݵ�
  lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_1, SensorValue[1]);
  // �����ػ�
  lv_chart_refresh(ui_Chart2);
  // ���ȿ���
  
  // �ƹ����
}

void switch2_event_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        // ���ر���
        // ִ�п�����������򿪷���
        FanControl(1);
    } else {
        // ���ر��ر�
        // ִ�йز���������رշ���
        FanControl(0);
    }
}

void switch3_event_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        // ����
        LED.LED_FUN(LED1, LED_ON);
    } else {
        // �ص�
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
