#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__
#include "MyApplication.h"

// TFT �������
#define TFT_LCD_BL_ON    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
#define TFT_LCD_BL_OFF   HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

// �洢��ַ
/**************************************************************************************************
1.FSMC�ⲿ�洢������Ϊ��NOR/PSRAM   NAND FLASH*2   PC������ռ256MB
2.NOR/PSRAM�ĵ�ַ�� 6000 0000~6FFF FFFF
    NOR/PSRAM1��6000 0000~63FF FFFF
    NOR/PSRAM2��6400 0000~67FF FFFF
    NOR/PSRAM3��6800 0000~6BFF FFFF
    NOR/PSRAM4��6C00 0000~6FFF FFFF
3.����ԭ��Ӳ���ӵ��� NOR/PSRAM4 ���ӵ�TFT ��ַ��Χ��6C00 0000~6FFF FFFF
4.FSMC_A6 �ӵ�LCD��D/CX(����/����ѡ��)��
    �����ַ = 0x6C00 0000  A6=0
    ���ݵ�ַ = 0x6C00 0080  A6=1    TFT_LCD���ݿ��8λHADDR[25:0]��FSMC_A[25:0]���ӡ�TFT_LCD���ݿ��16λHADDR[25:1]��FSMC_A[24:0]���� HADDR[0]
**************************************************************************************************/ 
#define FSMC_LCD_CMD_ADDR  ((uint32_t)0x6C000000)
#define FSMC_LCD_DATA_ADDR ((uint32_t)0x6C000080)

// LCD��д�����궨��
#define LCD_Write_CMD(CMD)      *((__IO uint16_t *)FSMC_LCD_CMD_ADDR)  = (uint16_t)CMD
#define LCD_Write_DATA(DATA)    *((__IO uint16_t *)FSMC_LCD_DATA_ADDR) = (uint16_t)DATA
#define LCD_Read_DATA()         *((__IO uint16_t *)FSMC_LCD_DATA_ADDR)

// ��ʾ����ѡ��
//#define LCD_DIRECTION   1   //���� ��ʱ��ѡ��0��   ԭ�������Ͻ�  240*320
//#define LCD_DIRECTION   2   //���� ��ʱ��ѡ��90��  ԭ�������Ͻ�  320*240
//#define LCD_DIRECTION   3   //���� ��ʱ��ѡ��180�� ԭ�������Ͻ�  240*320
#define LCD_DIRECTION   4   //���� ��ʱ��ѡ��270�� ԭ�������Ͻ�  320*240

// LCD��Ļ�Ŀ�Ⱥ͸߶�
#if (LCD_DIRECTION == 1 || LCD_DIRECTION == 3)
    #define LCD_WIDTH    240
    #define LCD_HEIGHT   320
#else
    #define LCD_WIDTH    320
    #define LCD_HEIGHT   240
#endif

// LCD����
#define LCD_CMD_SETxOrgin   0x2A
#define LCD_CMD_SETyOrgin   0x2B
#define LCD_CMD_WRgram      0x2C
#define LCD_CMD_RDgram      0x2E

// ����ö������
// ��ɫ����
typedef enum{
    LCD_WHITE        = 0xFFFF, //��ɫ
    LCD_BLACK        = 0x0000, //��ɫ
    LCD_BLUE         = 0x001F, //��ɫ
    LCD_RED          = 0xF800, //��ɫ
    LCD_GREEN        = 0x07E0, //��ɫ
    LCD_YELLOW       = 0xFFE0, //��ɫ
    LCD_BROWN 	     = 0xBC40, //��ɫ
    LCD_BRRED 	     = 0xFC07, //�غ�ɫ
    LCD_GRAY  	     = 0x8430 //��ɫ
}LCD_Color_t;

// ASCII������
typedef enum{
    ASCII_Font_16 =  16,
    ASCII_Font_24 =  24,
}ASCII_Font_t;
#define ISASCII_Font(font) ((font) == ASCII_Font_16) || ((font) == ASCII_Font_24)
// ��������
typedef enum{
    CHN_Font_16 =  16,
    CHN_Font_24 =  24,
}CHN_Font_t;
#define ISCHN_Font(font) ((font) == CHN_Font_16) || ((font) == CHN_Font_24)
// ��ͼ���
typedef enum{
    Filled      = 1,
    unFilled    = 0,
}Filled_t;
#define ISFilled(ucFilled) ((ucFilled) == Filled) || ((ucFilled) == unFilled)

typedef struct{
    uint32_t ID;
    
    void (*Init)(void);  
    void (*LCD_SetWindows)(uint16_t, uint16_t, uint16_t, uint16_t);
    void (*LCD_Clear)(LCD_Color_t);
    void (*FillColor)(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t); 
    void (*LCD_Show_Char)(uint16_t, uint16_t, const char, uint16_t, uint16_t, ASCII_Font_t);    
    void (*LCD_Show_String)(uint16_t, uint16_t, const char*, uint16_t, uint16_t, ASCII_Font_t); 
    void (*LCD_Show_CHN)(uint16_t, uint16_t, const char*, uint16_t, uint16_t, CHN_Font_t);    
    void (*LCD_Show_CHNandENString)(uint16_t, uint16_t, const char*, uint16_t, uint16_t, CHN_Font_t, ASCII_Font_t);
    void (*LCD_Show_Picture)(uint16_t usX, uint16_t usY, uint16_t Pic_H, uint16_t Pic_V, uint8_t Pic_Num);
    uint16_t (*LCD_GetPointPiexl)(uint16_t usX, uint16_t usY);
    void (*LCD_SetPointPiexl)(uint16_t usX, uint16_t usY, LCD_Color_t usColor);
    void (*LCD_Draw_Line)(uint16_t usX_Start, uint16_t usY_Start, uint16_t usX_End, uint16_t usY_End, LCD_Color_t usColor);
    void (*LCD_Draw_Circle)(uint16_t, uint16_t, uint8_t, LCD_Color_t);
    void (*LCD_Draw_Rectangle)(uint16_t, uint16_t, uint16_t, uint16_t, Filled_t, LCD_Color_t);
    void (*lvgl_FillColor)(uint16_t xStar, uint16_t yStar, uint16_t xWidth, uint16_t yHeight, uint16_t *color_p);
}TFT_LCD_t;


/* extern variables-----------------------------------------------------------*/
extern TFT_LCD_t TFT_LCD;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
