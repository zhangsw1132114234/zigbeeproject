#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "MyApplication.h"

// 软件模拟SPI用到的管脚
//SCK
#define CLR_TOUCH_SPI_SCK HAL_GPIO_WritePin(TOUCH_SPI_SCK_GPIO_Port, TOUCH_SPI_SCK_Pin, GPIO_PIN_RESET)
#define SET_TOUCH_SPI_SCK HAL_GPIO_WritePin(TOUCH_SPI_SCK_GPIO_Port, TOUCH_SPI_SCK_Pin, GPIO_PIN_SET)
//MISI
#define CLR_TOUCH_SPI_MISI HAL_GPIO_WritePin(TOUCH_SPI_MOSI_GPIO_Port, TOUCH_SPI_MOSI_Pin, GPIO_PIN_RESET)
#define SET_TOUCH_SPI_MISI HAL_GPIO_WritePin(TOUCH_SPI_MOSI_GPIO_Port, TOUCH_SPI_MOSI_Pin, GPIO_PIN_SET)
//READ MISO
#define READ_TOUCH_SPI_MISO HAL_GPIO_ReadPin(TOUCH_SPI_MISO_GPIO_Port, TOUCH_SPI_MISO_Pin)
//CS
#define CLR_TOUCH_SPI_CS HAL_GPIO_WritePin(TOUCH_SPI_CS_GPIO_Port, TOUCH_SPI_CS_Pin, GPIO_PIN_RESET)
#define SET_TOUCH_SPI_CS HAL_GPIO_WritePin(TOUCH_SPI_CS_GPIO_Port, TOUCH_SPI_CS_Pin, GPIO_PIN_SET)

//XT2046读取X轴与Y轴命令
#define Touch_X_CMD     0xD0    //读取X轴命令
#define Touch_Y_CMD     0x90    //读取X轴命令

//用于灵敏度和准确性
#define Touch_READ_TIMES    5   //一次读取触摸值的次数
#define Touch_Error         15  //误差

//ADC坐标值边界
#define Touch_LCD_ADC_xMIN  100
#define Touch_LCD_ADC_yMIN  100
#define Touch_LCD_ADC_xMAX  4000
#define Touch_LCD_ADC_yMAX  4000

//校准参数存放Flash的地址
#define Touch_Calibrate_Para_Addr   0
#define Touch_Calibrate_OK          'Y'

typedef struct{
    uint8_t Calibrate_Flag; //校准标志位
    uint8_t xOffset;        //x轴偏移量
    uint8_t yOffset;        //y轴偏移量
    float   xFactor;        //x轴比例因子
    float   yFactor;        //y轴比例因子
}Touch_Calibrate_Para_t;

typedef struct{
    uint8_t Touch_Flag;             //触摸标志位
    uint8_t Force_Calibrate_Flag;   //强行校准标志位
    
    uint16_t ADC_X; 
    uint16_t ADC_Y;
    uint16_t LCD_X;
    uint16_t LCD_Y;
    
    uint8_t (*Touch_Calibrate)(void);
    uint8_t (*Touch_Read_ADC_XY)(uint16_t*, uint16_t*);
    uint8_t (*Touch_Scan)(void);
    void    (*Palette_DrawPoint)(uint16_t x, uint16_t y, LCD_Color_t color);
    
}Touch_t;
/* extern variables-----------------------------------------------------------*/
extern Touch_t Touch;
extern Touch_Calibrate_Para_t Touch_Calibrate_Para;
/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
