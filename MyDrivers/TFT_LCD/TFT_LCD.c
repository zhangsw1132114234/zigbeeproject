/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"
#include "./TFT_LCD/Font.h"
#include "./TFT_LCD/Font_CHN.h"
//#include "./TFT_LCD/pic_1_240_320.h"
/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/


/* Private function prototypes------------------------------------------------*/
static void Init(void);                                                                         //��ʼ��
static void LCD_Clear(LCD_Color_t);                                                             //����
static void FillColor(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);                     //������
static uint32_t LCD_ReadID(void);                                                               //��ȡоƬID
static void LCD_Disp_Direction(void);                                                           //�л���ʾ����
static void LCD_SetWindows(uint16_t, uint16_t, uint16_t, uint16_t);                             //���ô��ڴ�С
static void LCD_Show_Char(uint16_t, uint16_t, const char, uint16_t, uint16_t, ASCII_Font_t);    //��ʾ�ַ�
static void LCD_Show_String(uint16_t, uint16_t, const char*, uint16_t, uint16_t, ASCII_Font_t); //��ʾ�ַ���
static void LCD_Show_CHN(uint16_t, uint16_t, const char*, uint16_t, uint16_t, CHN_Font_t);      //��ʾ�����ַ�
static void LCD_Show_CHNandENString(uint16_t, uint16_t, const char*, uint16_t, uint16_t, CHN_Font_t, ASCII_Font_t);// ��ʾ���ĺ�Ӣ���ַ���
static void LCD_Show_Picture(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);                  //��ʾͼƬ
static uint16_t  LCD_GetPointPiexl(uint16_t, uint16_t);                                         //��ȡ���ص���ɫ
static void LCD_SetPointPiexl(uint16_t, uint16_t, LCD_Color_t);                                 //�������ص���ɫ
static void LCD_Draw_Line(uint16_t, uint16_t, uint16_t, uint16_t, LCD_Color_t);                 //����
static void LCD_Draw_Circle(uint16_t, uint16_t, uint8_t, LCD_Color_t);                          //��Բ
static void LCD_Draw_Rectangle(uint16_t, uint16_t, uint16_t, uint16_t, Filled_t, LCD_Color_t);  //������
static void lvgl_FillColor(uint16_t xStar, uint16_t yStar, uint16_t xWidth, uint16_t yHeight, uint16_t *color_p);
/* Public variables-----------------------------------------------------------*/
TFT_LCD_t TFT_LCD = {
    0,
    
    Init,
    LCD_SetWindows,
    LCD_Clear,
    FillColor,
    LCD_Show_Char,
    LCD_Show_String,
    LCD_Show_CHN,
    LCD_Show_CHNandENString,
    LCD_Show_Picture,
    LCD_GetPointPiexl,
    LCD_SetPointPiexl,
    LCD_Draw_Line,
    LCD_Draw_Circle,
    LCD_Draw_Rectangle,
    lvgl_FillColor
};
/*
  * @name   LCD_Clear
  * @brief  ����
  * @param  
            usColor             -> ��ɫ
  * @retval None
*/
static void LCD_Clear(LCD_Color_t usColor)
{
    uint32_t uiIndex;
    LCD_SetWindows(0,0,240,320);
    for(uiIndex = 0; uiIndex< 240*320; uiIndex++)
    {
        LCD_Write_DATA(usColor);
    }
}
/*
  * @name   LCD_Draw_Rectangle
  * @brief  ������ 
  * @param  x1                 -> ���x
            y1                 -> ���y
            x2                 -> �յ�x
            y2                 -> �յ�y
            usColor             -> ��ɫ
  * @retval None
*/
void LCD_Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Filled_t Fill, LCD_Color_t usColor)
{
    assert_param(ISFilled(Filled));
    if(Fill == Filled)
    {
        FillColor(x1, y1, x2-x1, y2-y1, usColor);
    }else
    {
        LCD_Draw_Line(x1,y1,x2,y1, usColor);
        LCD_Draw_Line(x1,y1,x1,y2, usColor);
        LCD_Draw_Line(x1,y2,x2,y2, usColor);
        LCD_Draw_Line(x2,y1,x2,y2, usColor);
    }
    
}
/*
  * @name   LCD_Draw_Circle
  * @brief  ��Բ 
  * @param  x0                 -> ԭ��x
            y0                 -> ԭ��y
            r                  -> �뾶
            usColor             -> ��ɫ
  * @retval None
*/
static void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, LCD_Color_t usColor)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       //�ж��¸���λ�õı�־

    while (a <= b)
    {
        LCD_SetPointPiexl(x0 + a, y0 - b, usColor);        //5
        LCD_SetPointPiexl(x0 + b, y0 - a, usColor);        //0
        LCD_SetPointPiexl(x0 + b, y0 + a, usColor);        //4
        LCD_SetPointPiexl(x0 + a, y0 + b, usColor);        //6
        LCD_SetPointPiexl(x0 - a, y0 + b, usColor);        //1
        LCD_SetPointPiexl(x0 - b, y0 + a, usColor);
        LCD_SetPointPiexl(x0 - a, y0 - b, usColor);        //2
        LCD_SetPointPiexl(x0 - b, y0 - a, usColor);        //7
        a++;

        //ʹ��Bresenham�㷨��Բ
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}
/*
  * @name   LCD_Draw_Line
  * @brief  �������ص� 
  * @param  usX                 -> x�������
            usY                 -> y�������
            usColor             -> ��ɫ
  * @retval ɫ������ RGB565
*/
static void LCD_Draw_Line(uint16_t usX_Start, uint16_t usY_Start, uint16_t usX_End, uint16_t usY_End, LCD_Color_t usColor)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = usX_End - usX_Start;              //������������
    delta_y = usY_End - usY_Start;
    uRow = usX_Start;
    uCol = usY_Start;

    if (delta_x > 0)incx = 1;       //���õ�������
    else if (delta_x == 0)incx = 0; //��ֱ��
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x; //ѡȡ��������������
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )    //�������
    {
        LCD_SetPointPiexl(uRow, uCol, usColor);
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
/*
  * @name   LCD_SetPointPiexl
  * @brief  �������ص� 
  * @param  usX                 -> x�������
            usY                 -> y�������
            usColor             -> ��ɫ
  * @retval ɫ������ RGB565
*/
static void LCD_SetPointPiexl(uint16_t usX, uint16_t usY, LCD_Color_t usColor)
{
    if((usX < LCD_WIDTH) && (usY < LCD_HEIGHT))
    {
        // ���ô���
        LCD_SetWindows(usX, usY, 1, 1);
        // д������
        LCD_Write_DATA(usColor);
    }
}
/*
  * @name   LCD_GetPointPiexl
  * @brief  ��ȡ���ص�
  * @param  usX                 -> x�������
            usY                 -> y�������
  * @retval ɫ������ RGB565
*/
static uint16_t  LCD_GetPointPiexl(uint16_t usX, uint16_t usY)
{
    uint16_t usR=0, usG=0, usB=0;
    uint16_t usColor = 0;
    if((usX < LCD_WIDTH) && (usY < LCD_HEIGHT))
    {
        // ���ô���
        LCD_SetWindows(usX, usY, 1, 1);
        // д�������
        LCD_Write_CMD(LCD_CMD_RDgram);
        // ��ȡ����
        usR = LCD_Read_DATA();// ������һ������
        usR = LCD_Read_DATA();
        usG = LCD_Read_DATA();
        usB = LCD_Read_DATA();
        usColor = ((usR>>11)<<11) | ((usG>>10)<<5) | (usB>>11);
        printf("The pixel color is 0x%.4x\r\n",usColor);
    }
    return usColor;
}
/*
  * @name   LCD_Show_Picture
  * @brief  ��ʾͼƬ
  * @param  usX                 -> x�������
            usY                 -> y�������
            Pic_H               -> ͼƬ��
            Pic_V               -> ͼƬ��
            Pic_Num             -> ͼƬ���
  * @retval None
*/
static void LCD_Show_Picture(uint16_t usX, uint16_t usY, uint16_t Pic_H, uint16_t Pic_V, uint8_t Pic_Num)
{
    uint32_t uiIndex;
    const uint8_t *pic = NULL;
    
    // ���ô��ڴ�С
    LCD_SetWindows(usX, usY, Pic_V, Pic_H);
    
    switch(Pic_Num)
    {
//        case 1: pic = pic_1; break;
        // ����µ�ͼƬ
    }
    
    for(uiIndex = 0; uiIndex < Pic_H*Pic_V*2; uiIndex+=2)
    {
        LCD_Write_DATA((pic[uiIndex] << 8) | pic[uiIndex+1]);
    }
}

/*
  * @name   LCD_Show_CHNandENString
  * @brief  ��ʾ���ĺ�Ӣ���ַ���
  * @param  usX                 -> x�������
            usY                 -> y�������
            cStr                -> ��ʾ���ַ���
            usColor_Background  -> ������ɫ
            usColor_Foreground  -> ǰ����ɫ
            CHN_Font            -> ���������С
            ASCII_Font          -> Ӣ�������С
  * @retval None
*/
static void LCD_Show_CHNandENString(uint16_t usX, uint16_t usY, const char *pStr, uint16_t usColor_Background, uint16_t usColor_Foreground, CHN_Font_t CHN_Font, ASCII_Font_t ASCII_Font)
{
    while((*pStr) != '\0')
    {
        // ����
        if((*pStr) > 127)  
        {
            // ����
            if((usX + CHN_Font) > LCD_WIDTH)  
            {
                usX = 0;
                usY = usY + CHN_Font;
            }
            // ��ҳ
            if((usY + CHN_Font) > LCD_HEIGHT)
            {
                usX = 0;
                usY = 0;
            }
            //��ʾ����
            LCD_Show_CHN(usX, usY, pStr, usColor_Background, usColor_Foreground, CHN_Font);
            pStr += 2;
            usX += CHN_Font;
        }
        // Ӣ���ַ�
        else
        {
            if(((*pStr) == '\r') | ((*pStr) == '\n'))
            {
                // ǰһ���ַ�Ϊ����
                if((*(pStr-1)) > 127)
                {
                    usX = 0;
                    usY = usY + CHN_Font;
                }
                else
                {
                    usX = 0;
                    usY = usY + ASCII_Font;
                }
            }
            else
            {
                // ����
                if((usX + ASCII_Font/2) > LCD_WIDTH)  
                {
                    usX = 0;
                    usY = usY + ASCII_Font;
                }
                // ��ҳ
                if((usY + ASCII_Font) > LCD_HEIGHT)
                {
                    usX = 0;
                    usY = 0;
                }
                // ��ʾ�ַ�
                TFT_LCD.LCD_Show_Char(usX, usY, *pStr, usColor_Background, usColor_Foreground, ASCII_Font);
                // ����λ��
                usX = usX + ASCII_Font/2;
            }
            pStr++;
        }
        
    }
}
/*
  * @name   LCD_Show_CHN
  * @brief  ��ʾ�����ַ���
  * @param  usX                -> x�������
            usY                -> y�������
            cStr              -> ��ʾ���ַ���
            usColor_Background -> ������ɫ
            usColor_Foreground -> ǰ����ɫ
            font               -> �����С
  * @retval None
*/
void LCD_Show_CHN(uint16_t usX, uint16_t usY, const char *pStr, uint16_t usColor_Background, uint16_t usColor_Foreground, CHN_Font_t font)
{
    uint8_t ucPage, ucColumn, ucTemp;
    uint16_t usCHN_NUM;
    uint16_t usIndex;
    // �ж������С�Ƿ�Ϸ�
    assert_param(ISCHN_Font(font));
    // �ж����� 16����
    if (font == CHN_Font_16)
    {
        // ͳ�ƺ�������
        usCHN_NUM = sizeof(Font_CHN16) /sizeof(Font_CHN16_t);
        for(usIndex = 0; usIndex < usCHN_NUM; usIndex++)
        {
            // ���ҳɹ�
            if((Font_CHN16[usIndex].Index[0] == *pStr) && (Font_CHN16[usIndex].Index[1] == *(pStr+1)))
            {
                // ���ô���
                LCD_SetWindows(usX, usY, 16, 16);
                // ����д������   16�� 16������
                for(ucPage = 0; ucPage < 32; ucPage++)
                {
                    ucTemp = Font_CHN16[usIndex].CHN_code[ucPage];
                    for(ucColumn = 0; ucColumn < 8; ucColumn++)
                    {
                        if(ucTemp & 0x80)
                            LCD_Write_DATA(usColor_Foreground);
                        else
                            LCD_Write_DATA(usColor_Background);
                        ucTemp <<= 1;
                    }
                }
                break;
            }
        }
    }
    // �ж����� 24����
    if (font == CHN_Font_24)
    {
        // ͳ�ƺ�������
        usCHN_NUM = sizeof(Font_CHN24) /sizeof(Font_CHN24_t);
        for(usIndex = 0; usIndex < usCHN_NUM; usIndex++)
        {
            // ���ҳɹ�
            if((Font_CHN24[usIndex].Index[0] == *pStr) && (Font_CHN24[usIndex].Index[1] == *(pStr+1)))
            {
                // ���ô���
                LCD_SetWindows(usX, usY, 24, 24);
                // ����д������   24�� 24������
                for(ucPage = 0; ucPage < 72; ucPage++)
                {
                    ucTemp = Font_CHN24[usIndex].CHN_code[ucPage];
                    for(ucColumn = 0; ucColumn < 8; ucColumn++)
                    {
                        if(ucTemp & 0x80)
                            LCD_Write_DATA(usColor_Foreground);
                        else
                            LCD_Write_DATA(usColor_Background);
                        ucTemp <<= 1;
                    }
                }
                break;
            }
        }
    }
}
/*
  * @name   LCD_Show_String
  * @brief  ��ʾ�ַ���
  * @param  usX                -> x�������
            usY                -> y�������
            cStr              -> ��ʾ���ַ���
            usColor_Background -> ������ɫ
            usColor_Foreground -> ǰ����ɫ
            font               -> �����С
  * @retval None
*/
static void LCD_Show_String(uint16_t usX, uint16_t usY, const char* pStr, uint16_t usColor_Background, uint16_t usColor_Foreground, ASCII_Font_t font)
{
    while(*pStr != '\0')
    {
        // ����
        if((usX + font/2) > LCD_WIDTH)  
        {
            usX = 0;
            usY = usY + font;
        }
        // ��ҳ
        if((usY + font) > LCD_HEIGHT)
        {
            usX = 0;
            usY = 0;
        }
        // ��ʾ�ַ�
        TFT_LCD.LCD_Show_Char(usX, usY, *pStr, usColor_Background, usColor_Foreground, font);
        // ����λ��
        pStr++;
        usX = usX + font/2;
    }
}
/*
  * @name   LCD_Show_Char
  * @brief  ��ʾ�ַ�
  * @param  usX                -> x�������
            usY                -> y�������
            cChar              -> ��ʾ���ַ�
            usColor_Background -> ������ɫ
            usColor_Foreground -> ǰ����ɫ
            font               -> �����С
  * @retval None
*/
static void LCD_Show_Char(uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground, ASCII_Font_t font)
{
    uint8_t ucTemp, ucIndex, ucPage, ucColumn;
    // �������Ƿ�Ϸ�
    assert_param(ISASCII_Font(font));
    ucIndex = cChar - ' ';
    
    // �ж����� 16����
    if(font == ASCII_Font_16)
    {
        // ���ô���
        LCD_SetWindows(usX, usY, 8, 16);
        // ����д������ 16�� 8�����ص�
        for(ucPage = 0; ucPage < 16; ucPage++)
        {
            ucTemp = usAscii_1608[ucIndex][ucPage];
            for(ucColumn = 0; ucColumn < 8; ucColumn++)
            {
                if(ucTemp & 0x80)
                    LCD_Write_DATA(usColor_Foreground);
                else
                    LCD_Write_DATA(usColor_Background);
                ucTemp <<= 1;
            }
        }
    }
    
    // �ж����� 24����
    if(font == ASCII_Font_24)
    {
        // ���ô���
        LCD_SetWindows(usX, usY, 12, 24);
        // ����д������ 24�� 12�����ص�
        for(ucPage = 0; ucPage < 48; ucPage+=2)
        {
            ucTemp = usAscii_2412[ucIndex][ucPage];
            for(ucColumn = 0; ucColumn < 8; ucColumn++)
            {
                if(ucTemp & 0x80)
                    LCD_Write_DATA(usColor_Foreground);
                else
                    LCD_Write_DATA(usColor_Background);
                ucTemp <<= 1;
            }
            ucTemp = usAscii_2412[ucIndex][ucPage+1];
            for(ucColumn = 0; ucColumn < 4; ucColumn++)
            {
                if(ucTemp & 0x80)
                    LCD_Write_DATA(usColor_Foreground);
                else
                    LCD_Write_DATA(usColor_Background);
                ucTemp <<= 1;
            }
        }
    }
}
/*
  * @name   LCD_SetWindows
  * @brief  ����LCD���� ��������ſ�����ʾ
  * @param  xStar   -> x�������
            yStar   -> y�������
            xWidth  -> ���ڿ��
            yHeight -> ���ڸ߶�
  * @retval None
*/
static void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xWidth, uint16_t yHeight)
{
    LCD_Write_CMD(LCD_CMD_SETxOrgin);
    LCD_Write_DATA(xStar >> 8);
    LCD_Write_DATA(xStar&0x00FF);
    LCD_Write_DATA((xStar+xWidth-1) >> 8);
    LCD_Write_DATA((xStar+xWidth-1)&0x00FF);
    
    LCD_Write_CMD(LCD_CMD_SETyOrgin);
    LCD_Write_DATA(yStar >> 8);
    LCD_Write_DATA(yStar&0x00FF);
    LCD_Write_DATA((yStar+yHeight-1) >> 8);
    LCD_Write_DATA((yStar+yHeight-1)&0x00FF);
    // ��ʼд��Gram
    LCD_Write_CMD(LCD_CMD_WRgram);
}
/*
  * @name   LCD_Disp_Direction
  * @brief  ��ʾ�������
  * @param  None
  * @retval None
*/
static void LCD_Disp_Direction(void)
{
    switch(LCD_DIRECTION)
    {
        case 1:LCD_Write_CMD(0x36); LCD_Write_DATA(1<<3); break;
        case 2:LCD_Write_CMD(0x36); LCD_Write_DATA((1<<3)|(1<<5)|(1<<6)); break;
        case 3:LCD_Write_CMD(0x36); LCD_Write_DATA((1<<3)|(1<<7)|(1<<4)|(1<<6)); break;
        case 4:LCD_Write_CMD(0x36); LCD_Write_DATA((1<<3)|(1<<7)|(1<<5)|(1<<4)); break;
        default:LCD_Write_CMD(0x36); LCD_Write_DATA(1<<3); break;
    }
}
/*
  * @name   LCD_ReadID
  * @brief  ��ȡLCD ID
  * @param  None
  * @retval ������id
*/
static uint32_t LCD_ReadID(void)
{
    uint32_t LCD_ID = 0;
    uint32_t buf[4];
    
    LCD_Write_CMD(0xD3);
    
    buf[0] = LCD_Read_DATA();
    buf[1] = LCD_Read_DATA() & 0x00FF;  // ֻ�е�8λ��Ч
    buf[2] = LCD_Read_DATA() & 0x00FF;
    buf[3] = LCD_Read_DATA() & 0x00FF;
    
    LCD_ID = (buf[1] << 16) + (buf[2] << 8) + buf[3];
    return LCD_ID;
}
/*
  * @name   Init
  * @brief  TFT_LCD ��ʼ��
  * @param  None
  * @retval None
*/
static void Init(void)
{
    TFT_LCD.ID = LCD_ReadID();
    printf("The ID of LCD is 0x%.6X\r\n", TFT_LCD.ID);
    
    // ILT9341��ʼ�� �����ṩ
    LCD_Write_CMD(0xCF);  
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0xC1); 
    LCD_Write_DATA(0X30); 
    LCD_Write_CMD(0xED);  
    LCD_Write_DATA(0x64); 
    LCD_Write_DATA(0x03); 
    LCD_Write_DATA(0X12); 
    LCD_Write_DATA(0X81); 
    LCD_Write_CMD(0xE8);  
    LCD_Write_DATA(0x85); 
    LCD_Write_DATA(0x10); 
    LCD_Write_DATA(0x7A); 
    LCD_Write_CMD(0xCB);  
    LCD_Write_DATA(0x39); 
    LCD_Write_DATA(0x2C); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x34); 
    LCD_Write_DATA(0x02); 
    LCD_Write_CMD(0xF7);  
    LCD_Write_DATA(0x20); 
    LCD_Write_CMD(0xEA);  
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x00); 
    LCD_Write_CMD(0xC0);    //Power control 
    LCD_Write_DATA(0x1B);   //VRH[5:0] 
    LCD_Write_CMD(0xC1);    //Power control 
    LCD_Write_DATA(0x01);   //SAP[2:0];BT[3:0] 
    LCD_Write_CMD(0xC5);    //VCM control 
    LCD_Write_DATA(0x30); 	 //3F
    LCD_Write_DATA(0x30); 	 //3C
    LCD_Write_CMD(0xC7);    //VCM control2 
    LCD_Write_DATA(0XB7); 
//		LCD_Write_CMD(0x36);    // Memory Access Control 
//		LCD_Write_DATA(0x48); 
    LCD_Disp_Direction();   // �ı�LCD����
    LCD_Write_CMD(0x3A);   
    LCD_Write_DATA(0x55); 
    LCD_Write_CMD(0xB1);   
    LCD_Write_DATA(0x00);   
    LCD_Write_DATA(0x1A); 
    LCD_Write_CMD(0xB6);    // Display Function Control 
    LCD_Write_DATA(0x0A); 
    LCD_Write_DATA(0xA2); 
    LCD_Write_CMD(0xF2);    // 3Gamma Function Disable 
    LCD_Write_DATA(0x00); 
    LCD_Write_CMD(0x26);    //Gamma curve selected 
    LCD_Write_DATA(0x01); 
    LCD_Write_CMD(0xE0);    //Set Gamma 
    LCD_Write_DATA(0x0F); 
    LCD_Write_DATA(0x2A); 
    LCD_Write_DATA(0x28); 
    LCD_Write_DATA(0x08); 
    LCD_Write_DATA(0x0E); 
    LCD_Write_DATA(0x08); 
    LCD_Write_DATA(0x54); 
    LCD_Write_DATA(0XA9); 
    LCD_Write_DATA(0x43); 
    LCD_Write_DATA(0x0A); 
    LCD_Write_DATA(0x0F); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x00); 		 
    LCD_Write_CMD(0XE1);    //Set Gamma 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x15); 
    LCD_Write_DATA(0x17); 
    LCD_Write_DATA(0x07); 
    LCD_Write_DATA(0x11); 
    LCD_Write_DATA(0x06); 
    LCD_Write_DATA(0x2B); 
    LCD_Write_DATA(0x56); 
    LCD_Write_DATA(0x3C); 
    LCD_Write_DATA(0x05); 
    LCD_Write_DATA(0x10); 
    LCD_Write_DATA(0x0F); 
    LCD_Write_DATA(0x3F); 
    LCD_Write_DATA(0x3F); 
    LCD_Write_DATA(0x0F); 
    LCD_Write_CMD(0x2B); 
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x01);
    LCD_Write_DATA(0x3f);
    LCD_Write_CMD(0x2A); 
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0xef);	 
    LCD_Write_CMD(0x11); //Exit Sleep
    HAL_Delay(120);
    LCD_Write_CMD(0x29); //display on	
    
    TFT_LCD_BL_ON;  // �򿪱���
   
}
/*
  * @name   FillColor
  * @brief  �����ɫ
  * @param  None
  * @retval None
*/
static void FillColor(uint16_t xStar, uint16_t yStar, uint16_t xWidth, uint16_t yHeight, uint16_t Color)
{
    uint16_t i,j;
    // ���ô���
    LCD_SetWindows(xStar, yStar, xWidth, yHeight);
    // �����ɫ
    for(i = xStar; i<(xStar+xWidth); i++)
    {
        for(j = yStar; j<(yStar+yHeight); j++)
        {
            // ��Ȼ��ַû�б仯���ڲ�Ӳ�����Զ�ָ����һ������
            LCD_Write_DATA(Color);
        }
    }
}
// lvglд�뺯��
static void lvgl_FillColor(uint16_t xStar, uint16_t yStar, uint16_t xWidth, uint16_t yHeight, uint16_t *color_p)
{
    uint32_t size = xWidth * yHeight;  // ��������

    // ���ô��ڣ��������꣩
    LCD_SetWindows(xStar, yStar, xWidth, yHeight);

    // ����д����ɫ����
    while (size--) {
        LCD_Write_DATA(*color_p++);
    }
}

/********************************************************
  End Of File
********************************************************/
