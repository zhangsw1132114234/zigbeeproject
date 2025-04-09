#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "MyApplication.h"

// ���ģ��SPI�õ��Ĺܽ�
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

//XT2046��ȡX����Y������
#define Touch_X_CMD     0xD0    //��ȡX������
#define Touch_Y_CMD     0x90    //��ȡX������

//���������Ⱥ�׼ȷ��
#define Touch_READ_TIMES    5   //һ�ζ�ȡ����ֵ�Ĵ���
#define Touch_Error         15  //���

//ADC����ֵ�߽�
#define Touch_LCD_ADC_xMIN  100
#define Touch_LCD_ADC_yMIN  100
#define Touch_LCD_ADC_xMAX  4000
#define Touch_LCD_ADC_yMAX  4000

//У׼�������Flash�ĵ�ַ
#define Touch_Calibrate_Para_Addr   0
#define Touch_Calibrate_OK          'Y'

typedef struct{
    uint8_t Calibrate_Flag; //У׼��־λ
    uint8_t xOffset;        //x��ƫ����
    uint8_t yOffset;        //y��ƫ����
    float   xFactor;        //x���������
    float   yFactor;        //y���������
}Touch_Calibrate_Para_t;

typedef struct{
    uint8_t Touch_Flag;             //������־λ
    uint8_t Force_Calibrate_Flag;   //ǿ��У׼��־λ
    
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
