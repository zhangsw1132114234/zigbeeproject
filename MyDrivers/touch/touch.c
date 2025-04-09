/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
#define DEBUG 0
/* Private variables----------------------------------------------------------*/


/* Private function prototypes------------------------------------------------*/
static uint8_t Touch_Calibrate(void);
static void Touch_ReadCalibrateValue(uint16_t, uint16_t, uint16_t*, uint16_t*);
static void LCD_Draw_Cross(uint16_t, uint16_t);

static void     Touch_Write_Byte(uint8_t num); //������ICд��һ���ֽ�����
static uint8_t  Touch_Read_Byte(void);    //������IC��ȡһ���ֽ�����
static uint16_t Touch_Read_ADC(uint8_t XT2046_CMD);
static uint8_t  Touch_Read_ADC_XY(uint16_t*, uint16_t*);
static uint8_t  Touch_Scan(void);
static void Palette_DrawPoint(uint16_t x, uint16_t y, LCD_Color_t color);

/* Public variables-----------------------------------------------------------*/
Touch_Calibrate_Para_t Touch_Calibrate_Para = {
    0,
    0,
    0,
    0.0,
    0.0
};
Touch_t Touch = {
    0,
    0,
    0,
    0,
    0,
    0,
    Touch_Calibrate,
    Touch_Read_ADC_XY,
    Touch_Scan,
    Palette_DrawPoint
};
/*
  * @name   Palette_DrawPoint
  * @brief  ��������
  * @param  x       -> x����
            y       -> y����
            color   -> ��ɫ
  * @retval None
*/
static void Palette_DrawPoint(uint16_t x, uint16_t y, LCD_Color_t color)
{
    TFT_LCD.LCD_SetPointPiexl(x-1, y-1, color);
    TFT_LCD.LCD_SetPointPiexl(x  , y-1, color);
    TFT_LCD.LCD_SetPointPiexl(x+1, y-1, color);
    TFT_LCD.LCD_SetPointPiexl(x-1, y  , color);
    TFT_LCD.LCD_SetPointPiexl(x  , y  , color);
    TFT_LCD.LCD_SetPointPiexl(x+1, y-1, color);
}
/*
  * @name   LCD_Draw_Cross
  * @brief  ��У׼ʮ�ֹ��
  * @param  x -> x����
            y -> y����
  * @retval None
*/
static void LCD_Draw_Cross(uint16_t x, uint16_t y)
{
    TFT_LCD.LCD_Draw_Line(x, y-10, x, y+10, LCD_RED);
    TFT_LCD.LCD_Draw_Line(x-10, y, x+10, y, LCD_RED);
}
/*
  * @name   Touch_ReadCalibrateValue
  * @brief  ��ȡУ׼����
  * @param  None
  * @retval None

*/
static void Touch_ReadCalibrateValue(uint16_t x, uint16_t y, uint16_t* xValue, uint16_t* yValue)
{
    uint8_t cnt = 0;
    // ��ʾУ׼��λ��
    LCD_Draw_Cross(x, y);
    while(1)
    {
        if(Touch_Read_ADC_XY(xValue, yValue))
        {
            if(cnt++ > 10)
            {
                TFT_LCD.FillColor(0,0,320,240,LCD_BLACK);
                break;
            }
        }
    }
}
/*
  * @name   Touch_Calibrate
  * @brief  ������У׼  �л���Ļ�������Ҫ����У׼
  * @param  None
  * @retval 1�д��� 0�޴���
    ���͸��Ŷ�ȡ����
*/
static uint8_t Touch_Calibrate(void)
{
    //5��У׼λ�� ��ʱ��
    uint16_t Calibrate_xyLCD[5][2] = {
        {20, 20},
        {20, LCD_HEIGHT-20},
        {LCD_WIDTH-20, LCD_HEIGHT-20},
        {LCD_WIDTH-20, 20},
        {LCD_WIDTH/2, LCD_HEIGHT/2}
    };
    //5��У׼λ�ö�Ӧ��ADC����ֵ
    uint16_t xValue[5], yValue[5];
    uint16_t xOpposite[2], yOpposite[2];    //����õ��Խ��ߵ�����
    uint16_t Avr_xOpposite, Avr_yOpposite;  //�Խ��������ƽ��ֵ
    uint8_t i;
    //��ȡ5���������ֵ
    TFT_LCD.FillColor(0, 0, 320, 240, LCD_BLACK);
    for(i = 0; i < 5; i++)
    {
        Touch_ReadCalibrateValue(Calibrate_xyLCD[i][0], Calibrate_xyLCD[i][1], &xValue[i], &yValue[i]);
        HAL_Delay(800);
    }
    // �������ε�4��У׼�����ϳɶԽ����㣬���ٴ������
    xOpposite[0] = (xValue[0] + xValue[1]) / 2;
    yOpposite[0] = (yValue[0] + yValue[3]) / 2;
    xOpposite[1] = (xValue[2] + xValue[3]) / 2;
    yOpposite[1] = (yValue[2] + yValue[1]) / 2;

    // ����Խ������ƽ��ֵ
    Avr_xOpposite = (xOpposite[0] + xOpposite[1]) / 2;
    Avr_yOpposite = (yOpposite[0] + yOpposite[1]) / 2;
    
    #if DEBUG
    printf("������ADC����ֵ��xAvr=%u,yAvr=%u\r\n", Avr_xOpposite, Avr_yOpposite);
    printf("������ADC����ֵ��xMid=%u,yMid=%u\r\n", xValue[4], yValue[4]);
    printf("xOpposite[0]=%u,xOpposite[1]=%u\r\n", xOpposite[0], xOpposite[1]);
    printf("yOpposite[0]=%u,yOpposite[1]=%u\r\n", yOpposite[0], yOpposite[1]);
    #endif
    //����У�� 
    if(Avr_xOpposite >= xValue[4])
    {
        if((Avr_xOpposite - xValue[4]) > 100)
        {
            printf("У׼ʧ��\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }else
    {
        if((xValue[4] - Avr_xOpposite) > 100)
        {
            printf("У׼ʧ��\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }
    if(Avr_yOpposite >= yValue[4])
    {
        if((Avr_yOpposite - yValue[4]) > 100)
        {
            printf("У׼ʧ��\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }else
    {
        if((yValue[4] - Avr_yOpposite) > 100)
        {
            printf("У׼ʧ��\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }
    
    // �����������
    Touch_Calibrate_Para.xFactor = fabs((float)(LCD_WIDTH -40) /  (xOpposite[1] - xOpposite[0]));
    Touch_Calibrate_Para.yFactor = fabs((float)(LCD_HEIGHT -40) / (yOpposite[1] - yOpposite[0]));

    // ����ƫ����
    Touch_Calibrate_Para.xOffset = (uint8_t)(Touch_Calibrate_Para.xFactor*Avr_xOpposite - LCD_WIDTH / 2);
    Touch_Calibrate_Para.yOffset = (uint8_t)(Touch_Calibrate_Para.yFactor*Avr_yOpposite - LCD_HEIGHT / 2);
    // ����У׼��־λ
    Touch_Calibrate_Para.Calibrate_Flag = Touch_Calibrate_OK;
    
    printf("У׼�ɹ�\r\n");
    printf("У׼����xFactor��%.2f\r\n", Touch_Calibrate_Para.xFactor);
    printf("У׼����yFactor��%.2f\r\n", Touch_Calibrate_Para.yFactor);
    printf("ƫ����xOffset��%d\r\n", Touch_Calibrate_Para.xOffset);
    printf("ƫ����yOffset��%d\r\n", Touch_Calibrate_Para.yOffset);
    TFT_LCD.LCD_Show_String(12, 160, "Calibrate Success", LCD_BLACK, LCD_RED, ASCII_Font_24);
    HAL_Delay(1000);
    // �������
    SPI_FLASH.EraseSector(Touch_Calibrate_Para_Addr);
    SPI_FLASH.WriteUnfixed(&Touch_Calibrate_Para.Calibrate_Flag, Touch_Calibrate_Para_Addr, sizeof(Touch_Calibrate_Para));
    
    return 1;
}
/*
  * @name   Touch_Scan
  * @brief  ������ɨ��
  * @param  None
  * @retval 1�д��� 0�޴���
    ���͸��Ŷ�ȡ����
*/
static uint8_t Touch_Scan()
{
    uint16_t LCD_X1,LCD_Y1,LCD_X2,LCD_Y2;
	
	//if(HAL_GPIO_ReadPin(TP_NIRQ_GPIO_Port,TP_NIRQ_Pin) == GPIO_PIN_RESET)
	//{
		//��һ�ζ�ȡ������������ֵ
	  if(Touch.Touch_Read_ADC_XY(&Touch.ADC_X,&Touch.ADC_Y) == FALSE)
		{
			return FALSE;
		}
		
		//�ڶ��μ���LCD��Ļ����ֵ
		LCD_X1 = (uint16_t)(Touch.ADC_X*Touch_Calibrate_Para.xFactor) - Touch_Calibrate_Para.xOffset;
		LCD_Y1 = (uint16_t)(Touch.ADC_Y*Touch_Calibrate_Para.yFactor) - Touch_Calibrate_Para.yOffset;
		
		if(Touch.Touch_Read_ADC_XY(&Touch.ADC_X,&Touch.ADC_Y) == FALSE)
		{
			return FALSE;
		}
		
		//�ڶ��μ���LCD��Ļ����ֵ
		LCD_X2 = (uint16_t)(Touch.ADC_X*Touch_Calibrate_Para.xFactor) - Touch_Calibrate_Para.xOffset;
		LCD_Y2 = (uint16_t)(Touch.ADC_Y*Touch_Calibrate_Para.yFactor) - Touch_Calibrate_Para.yOffset;
		
		//�����
		if(LCD_X1 >= LCD_X2)
		{
			if((LCD_X1 - LCD_X2) > 2)
				return FALSE;
		}
		else
		{
			if((LCD_X2 - LCD_X1) > 2)
				return FALSE;
		}
		
		if(LCD_Y1 >= LCD_Y2)
		{
			if((LCD_Y1 - LCD_Y2) > 2)
				return FALSE;
		}
		else
		{
			if((LCD_Y2 - LCD_Y1) > 2)
				return FALSE;
		}
		
		//�������ε�ƽ��ֵ���õ�LCD��Ļ����ֵ
		Touch.LCD_X = (LCD_X1 + LCD_X2)/2;
		Touch.LCD_Y = (LCD_Y1 + LCD_Y2)/2;
    if(LCD_WIDTH == 320) // ���ڵ�����������
    {
      Touch.LCD_X = abs((LCD_X1 + LCD_X2)/2 - 320);
    }
		
		if(Touch.LCD_X > (LCD_WIDTH - 1))
		{
			Touch.LCD_X = LCD_WIDTH - 1;
		}
		
		if(Touch.LCD_Y > (LCD_HEIGHT - 1))
		{
			Touch.LCD_Y = LCD_HEIGHT - 1;
		}
    #if DEBUG
		printf("Touch_Read_LCD_XY:(X,Y)=(%u,%u)\r\n", Touch.LCD_X, Touch.LCD_Y);
    // LCD��Ļ����ʾ����ֵ
    TFT_LCD.LCD_Show_Char(24, LCD_HEIGHT-24, Touch.ADC_X/1000+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    TFT_LCD.LCD_Show_Char(36, LCD_HEIGHT-24, Touch.ADC_X%1000/100+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    TFT_LCD.LCD_Show_Char(48, LCD_HEIGHT-24, Touch.ADC_X%100/10+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    TFT_LCD.LCD_Show_Char(60, LCD_HEIGHT-24, Touch.ADC_X%10+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    
    TFT_LCD.LCD_Show_Char(120, LCD_HEIGHT-24, Touch.ADC_Y/1000+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    TFT_LCD.LCD_Show_Char(132, LCD_HEIGHT-24, Touch.ADC_Y%1000/100+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    TFT_LCD.LCD_Show_Char(144, LCD_HEIGHT-24, Touch.ADC_Y%100/10+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    TFT_LCD.LCD_Show_Char(156, LCD_HEIGHT-24, Touch.ADC_Y%10+'0', LCD_GRAY, LCD_RED, ASCII_Font_24);
    #endif
		return TRUE;
}
/*
  * @name   Touch_Read_ADC_XY
  * @brief  ��ȡ������XY����ֵ
  * @param  xValue -> ��ȡ��x��ADC��ֵ�ĵ�ַ
            yValue -> ��ȡ��y��ADC��ֵ�ĵ�ַ
  * @retval 1�ɹ� 0ʧ��
*/
static uint8_t Touch_Read_ADC_XY(uint16_t *xValue, uint16_t *yValue)
{
    uint16_t xValue_Buf[2], yValue_Buf[2];
    uint16_t xValue_Error, yValue_Error;
    // ��ȡ����
    xValue_Buf[0] = Touch_Read_ADC(Touch_X_CMD);
    yValue_Buf[0] = Touch_Read_ADC(Touch_Y_CMD);
    xValue_Buf[1] = Touch_Read_ADC(Touch_X_CMD);
    yValue_Buf[1] = Touch_Read_ADC(Touch_Y_CMD);
    // �������ζ�ȡ�������
    if(xValue_Buf[0] >= xValue_Buf[1])
    {
        xValue_Error = xValue_Buf[0] - xValue_Buf[1];
    }else
    {
        xValue_Error = xValue_Buf[1] - xValue_Buf[0];
    }
    if(yValue_Buf[0] >= yValue_Buf[1])
    {
        yValue_Error = yValue_Buf[0] - yValue_Buf[1];
    }else
    {
        yValue_Error = yValue_Buf[1] - yValue_Buf[0];
    }
    // �����趨���
    if((xValue_Error > Touch_Error) || (yValue_Error > Touch_Error))
    {
        return 0;
    }
    // ��ƽ��
    *xValue = (xValue_Buf[0] + xValue_Buf[1]) / 2;
    *yValue = (yValue_Buf[0] + yValue_Buf[1]) / 2;
    if(LCD_WIDTH == 320)
    {
      *yValue = (xValue_Buf[0] + xValue_Buf[1]) / 2;
      *xValue = (yValue_Buf[0] + yValue_Buf[1]) / 2;
    }
    // �ж�ֵ�Ƿ���Ч
    if((*xValue > Touch_LCD_ADC_xMAX) || (*xValue < Touch_LCD_ADC_xMIN)) return 0;
    if((*yValue > Touch_LCD_ADC_yMAX) || (*yValue < Touch_LCD_ADC_yMIN)) return 0;
    //printf("Touch_Read_ADC_XY:(X,Y)=(%u,%u)\r\n", *xValue, *yValue);
    return 1;
}
/*
  * @name   Touch_Read_ADC
  * @brief  ��ȡ��������ADCֵ  
  * @param  XT2046_CMD -> ��ȡ����
  * @retval ��ȡ�����ֽ�
*/
static uint16_t Touch_Read_ADC(uint8_t XT2046_CMD)
{
    uint8_t i, j;
    uint16_t Value_Buf[Touch_READ_TIMES], usTemp;
    uint32_t SumValue = 0;
    
    
    // ��ȡ����
    for(i = 0; i < Touch_READ_TIMES; i++)
    {
        CLR_TOUCH_SPI_SCK;  //ʱ������
        CLR_TOUCH_SPI_MISI; //����������
        // Ƭѡ
        CLR_TOUCH_SPI_CS;
        // ���Ϳ�������
        Touch_Write_Byte(XT2046_CMD);
        delay_us(6);
        // ��һ��ʱ�����BUSY
        CLR_TOUCH_SPI_SCK;  //ʱ������
        delay_us(1);
        SET_TOUCH_SPI_SCK;  //ʱ������
        delay_us(1);
        CLR_TOUCH_SPI_SCK;  //ʱ������
        //��ȡ���� ֻ��12λ������Ч
        Value_Buf[i] = Touch_Read_Byte();
//        Value_Buf[i] &= (0x7F);   // ���λ������Ч
        Value_Buf[i] <<= 8;
        Value_Buf[i] += Touch_Read_Byte();
        Value_Buf[i] >>= 4;
        SET_TOUCH_SPI_CS;
    }
    // ���� �Ӵ�С
    for(i = 0; i < (Touch_READ_TIMES-1); i++)
    {
        for(j = i+1; j < Touch_READ_TIMES; j++)
        {
            if(Value_Buf[i] < Value_Buf[j])
            {
                usTemp = Value_Buf[i];
                Value_Buf[i] = Value_Buf[j];
                Value_Buf[j] = usTemp;
            }
        }
    }
    // ȥ�����ֵ����Сֵ�����
    for(i = 1; i < (Touch_READ_TIMES-1); i++)
    {
        SumValue += Value_Buf[i];
    }
    return SumValue/(Touch_READ_TIMES - 2);
}
/*
  * @name   TP_Read_Byte
  * @brief  ������IC��ȡһ���ֽ�����
  * @param  CMD -> ���͵�����
  * @retval ��ȡ�����ֽ�
*/
static uint8_t Touch_Read_Byte(void)
{
    uint8_t readData = 0;
    uint8_t count;
    //ѭ����������
    for(count=0;count<8;count++)
    {
        readData <<= 1;
        CLR_TOUCH_SPI_SCK;  //ʱ������
        delay_us(1);
        SET_TOUCH_SPI_SCK;  //ʱ������
        if(READ_TOUCH_SPI_MISO) readData++;
    }
    return readData;
}
/*
  * @name   TP_Write_Byte
  * @brief  ������ICд��һ���ֽ�����
  * @param  num -> д�������
  * @retval None
*/
static void Touch_Write_Byte(uint8_t num)
{
    uint8_t count;
    
    //ѭ��д������
    for(count = 0; count < 8; count++)
    {
        if(num & 0x80) SET_TOUCH_SPI_MISI;
        else CLR_TOUCH_SPI_MISI;
        num <<=1;
        // CLK���������ط�������
        CLR_TOUCH_SPI_SCK;
        delay_us(1);
        SET_TOUCH_SPI_SCK;
    }
}
/********************************************************
  End Of File
********************************************************/
