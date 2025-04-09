#ifndef __MyApplication_H__
#define __MyApplication_H__

/* Includes ------------------------------------------------------------------*/
// CubeMX���ɵ�ͷ�ļ�
#include "main.h"
#include "fatfs.h"
#include "lwip.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fsmc.h"
// FreeRTOSͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
// MyApplication�е�ͷ�ļ�
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "System.h"
#include "Public.h"
#include "MyInit.h"
#include "CallBack.h"
#include "math.h"
// LVGL���
#include "lvgl/lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "ui.h"
// MyDrivers�е�ͷ�ļ�
#include "./led/led.h"
#include "./key/key.h"
#include "./tim/_tim.h"
#include "./usart/_usart.h"
#include "./TFT_LCD/TFT_LCD.h"
#include "./touch/touch.h"
#include "./spi_flash/spi_flash.h"
#include "./zigbee/protocol.h"
#include "./webServer/httpserver-socket.h"
#include "./GUI_Data_Update/gui_update.h"
#endif
/********************************************************
  End Of File
********************************************************/
