#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__
#include "MyApplication.h"

// TFT 背光控制
#define TFT_LCD_BL_ON    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
#define TFT_LCD_BL_OFF   HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

// 存储地址
/**************************************************************************************************
1.FSMC外部存储器划分为：NOR/PSRAM   NAND FLASH*2   PC卡，各占256MB
2.NOR/PSRAM的地址是 6000 0000~6FFF FFFF
    NOR/PSRAM1：6000 0000~63FF FFFF
    NOR/PSRAM2：6400 0000~67FF FFFF
    NOR/PSRAM3：6800 0000~6BFF FFFF
    NOR/PSRAM4：6C00 0000~6FFF FFFF
3.正点原子硬件接的是 NOR/PSRAM4 连接的TFT 地址范围：6C00 0000~6FFF FFFF
4.FSMC_A6 接的LCD的D/CX(命令/数据选择)脚
    命令地址 = 0x6C00 0000  A6=0
    数据地址 = 0x6C00 0080  A6=1    TFT_LCD数据宽度8位HADDR[25:0]与FSMC_A[25:0]连接、TFT_LCD数据宽度16位HADDR[25:1]与FSMC_A[24:0]连接 HADDR[0]
**************************************************************************************************/ 
#define FSMC_LCD_CMD_ADDR  ((uint32_t)0x6C000000)
#define FSMC_LCD_DATA_ADDR ((uint32_t)0x6C000080)

// LCD读写函数宏定义
#define LCD_Write_CMD(CMD)      *((__IO uint16_t *)FSMC_LCD_CMD_ADDR)  = (uint16_t)CMD
#define LCD_Write_DATA(DATA)    *((__IO uint16_t *)FSMC_LCD_DATA_ADDR) = (uint16_t)DATA
#define LCD_Read_DATA()         *((__IO uint16_t *)FSMC_LCD_DATA_ADDR)

// 显示方向选择
//#define LCD_DIRECTION   1   //竖屏 逆时针选中0°   原点在左上角  240*320
//#define LCD_DIRECTION   2   //横屏 逆时针选中90°  原点在左上角  320*240
//#define LCD_DIRECTION   3   //竖屏 逆时针选中180° 原点在左上角  240*320
#define LCD_DIRECTION   4   //横屏 逆时针选中270° 原点在左上角  320*240

// LCD屏幕的宽度和高度
#if (LCD_DIRECTION == 1 || LCD_DIRECTION == 3)
    #define LCD_WIDTH    240
    #define LCD_HEIGHT   320
#else
    #define LCD_WIDTH    320
    #define LCD_HEIGHT   240
#endif

// LCD命令
#define LCD_CMD_SETxOrgin   0x2A
#define LCD_CMD_SETyOrgin   0x2B
#define LCD_CMD_WRgram      0x2C
#define LCD_CMD_RDgram      0x2E

// 定义枚举类型
// 颜色定义
typedef enum{
    LCD_WHITE        = 0xFFFF, //白色
    LCD_BLACK        = 0x0000, //黑色
    LCD_BLUE         = 0x001F, //蓝色
    LCD_RED          = 0xF800, //红色
    LCD_GREEN        = 0x07E0, //绿色
    LCD_YELLOW       = 0xFFE0, //黄色
    LCD_BROWN 	     = 0xBC40, //棕色
    LCD_BRRED 	     = 0xFC07, //棕红色
    LCD_GRAY  	     = 0x8430 //灰色
}LCD_Color_t;

// ASCII码字体
typedef enum{
    ASCII_Font_16 =  16,
    ASCII_Font_24 =  24,
}ASCII_Font_t;
#define ISASCII_Font(font) ((font) == ASCII_Font_16) || ((font) == ASCII_Font_24)
// 中文字体
typedef enum{
    CHN_Font_16 =  16,
    CHN_Font_24 =  24,
}CHN_Font_t;
#define ISCHN_Font(font) ((font) == CHN_Font_16) || ((font) == CHN_Font_24)
// 画图填充
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
