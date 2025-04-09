/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
#define DEBUG 0
/* Private variables----------------------------------------------------------*/


/* Private function prototypes------------------------------------------------*/
static uint8_t Touch_Calibrate(void);
static void Touch_ReadCalibrateValue(uint16_t, uint16_t, uint16_t*, uint16_t*);
static void LCD_Draw_Cross(uint16_t, uint16_t);

static void     Touch_Write_Byte(uint8_t num); //向触摸屏IC写入一个字节数据
static uint8_t  Touch_Read_Byte(void);    //向触摸屏IC读取一个字节数据
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
  * @brief  画触摸点
  * @param  x       -> x坐标
            y       -> y坐标
            color   -> 颜色
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
  * @brief  画校准十字光标
  * @param  x -> x坐标
            y -> y坐标
  * @retval None
*/
static void LCD_Draw_Cross(uint16_t x, uint16_t y)
{
    TFT_LCD.LCD_Draw_Line(x, y-10, x, y+10, LCD_RED);
    TFT_LCD.LCD_Draw_Line(x-10, y, x+10, y, LCD_RED);
}
/*
  * @name   Touch_ReadCalibrateValue
  * @brief  读取校准数据
  * @param  None
  * @retval None

*/
static void Touch_ReadCalibrateValue(uint16_t x, uint16_t y, uint16_t* xValue, uint16_t* yValue)
{
    uint8_t cnt = 0;
    // 显示校准的位置
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
  * @brief  触摸屏校准  切换屏幕方向后需要重新校准
  * @param  None
  * @retval 1有触摸 0无触摸
    降低干扰读取两次
*/
static uint8_t Touch_Calibrate(void)
{
    //5个校准位置 逆时针
    uint16_t Calibrate_xyLCD[5][2] = {
        {20, 20},
        {20, LCD_HEIGHT-20},
        {LCD_WIDTH-20, LCD_HEIGHT-20},
        {LCD_WIDTH-20, 20},
        {LCD_WIDTH/2, LCD_HEIGHT/2}
    };
    //5个校准位置对应的ADC坐标值
    uint16_t xValue[5], yValue[5];
    uint16_t xOpposite[2], yOpposite[2];    //计算得到对角线的坐标
    uint16_t Avr_xOpposite, Avr_yOpposite;  //对角线坐标的平均值
    uint8_t i;
    //读取5个点的坐标值
    TFT_LCD.FillColor(0, 0, 320, 240, LCD_BLACK);
    for(i = 0; i < 5; i++)
    {
        Touch_ReadCalibrateValue(Calibrate_xyLCD[i][0], Calibrate_xyLCD[i][1], &xValue[i], &yValue[i]);
        HAL_Delay(800);
    }
    // 将正方形的4个校准点整合成对角两点，减少触摸误差
    xOpposite[0] = (xValue[0] + xValue[1]) / 2;
    yOpposite[0] = (yValue[0] + yValue[3]) / 2;
    xOpposite[1] = (xValue[2] + xValue[3]) / 2;
    yOpposite[1] = (yValue[2] + yValue[1]) / 2;

    // 计算对角两点的平均值
    Avr_xOpposite = (xOpposite[0] + xOpposite[1]) / 2;
    Avr_yOpposite = (yOpposite[0] + yOpposite[1]) / 2;
    
    #if DEBUG
    printf("触摸屏ADC坐标值：xAvr=%u,yAvr=%u\r\n", Avr_xOpposite, Avr_yOpposite);
    printf("触摸屏ADC坐标值：xMid=%u,yMid=%u\r\n", xValue[4], yValue[4]);
    printf("xOpposite[0]=%u,xOpposite[1]=%u\r\n", xOpposite[0], xOpposite[1]);
    printf("yOpposite[0]=%u,yOpposite[1]=%u\r\n", yOpposite[0], yOpposite[1]);
    #endif
    //进行校验 
    if(Avr_xOpposite >= xValue[4])
    {
        if((Avr_xOpposite - xValue[4]) > 100)
        {
            printf("校准失败\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }else
    {
        if((xValue[4] - Avr_xOpposite) > 100)
        {
            printf("校准失败\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }
    if(Avr_yOpposite >= yValue[4])
    {
        if((Avr_yOpposite - yValue[4]) > 100)
        {
            printf("校准失败\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }else
    {
        if((yValue[4] - Avr_yOpposite) > 100)
        {
            printf("校准失败\r\n");
            TFT_LCD.LCD_Show_String(24, 160, "Calibrate Fail", LCD_BLACK, LCD_RED, ASCII_Font_24);
            HAL_Delay(1000);
            return 0;
        }
    }
    
    // 计算比例因子
    Touch_Calibrate_Para.xFactor = fabs((float)(LCD_WIDTH -40) /  (xOpposite[1] - xOpposite[0]));
    Touch_Calibrate_Para.yFactor = fabs((float)(LCD_HEIGHT -40) / (yOpposite[1] - yOpposite[0]));

    // 计算偏移量
    Touch_Calibrate_Para.xOffset = (uint8_t)(Touch_Calibrate_Para.xFactor*Avr_xOpposite - LCD_WIDTH / 2);
    Touch_Calibrate_Para.yOffset = (uint8_t)(Touch_Calibrate_Para.yFactor*Avr_yOpposite - LCD_HEIGHT / 2);
    // 设置校准标志位
    Touch_Calibrate_Para.Calibrate_Flag = Touch_Calibrate_OK;
    
    printf("校准成功\r\n");
    printf("校准因子xFactor：%.2f\r\n", Touch_Calibrate_Para.xFactor);
    printf("校准因子yFactor：%.2f\r\n", Touch_Calibrate_Para.yFactor);
    printf("偏移量xOffset：%d\r\n", Touch_Calibrate_Para.xOffset);
    printf("偏移量yOffset：%d\r\n", Touch_Calibrate_Para.yOffset);
    TFT_LCD.LCD_Show_String(12, 160, "Calibrate Success", LCD_BLACK, LCD_RED, ASCII_Font_24);
    HAL_Delay(1000);
    // 保存参数
    SPI_FLASH.EraseSector(Touch_Calibrate_Para_Addr);
    SPI_FLASH.WriteUnfixed(&Touch_Calibrate_Para.Calibrate_Flag, Touch_Calibrate_Para_Addr, sizeof(Touch_Calibrate_Para));
    
    return 1;
}
/*
  * @name   Touch_Scan
  * @brief  触摸屏扫描
  * @param  None
  * @retval 1有触摸 0无触摸
    降低干扰读取两次
*/
static uint8_t Touch_Scan()
{
    uint16_t LCD_X1,LCD_Y1,LCD_X2,LCD_Y2;
	
	//if(HAL_GPIO_ReadPin(TP_NIRQ_GPIO_Port,TP_NIRQ_Pin) == GPIO_PIN_RESET)
	//{
		//第一次读取触摸屏的坐标值
	  if(Touch.Touch_Read_ADC_XY(&Touch.ADC_X,&Touch.ADC_Y) == FALSE)
		{
			return FALSE;
		}
		
		//第二次计算LCD屏幕坐标值
		LCD_X1 = (uint16_t)(Touch.ADC_X*Touch_Calibrate_Para.xFactor) - Touch_Calibrate_Para.xOffset;
		LCD_Y1 = (uint16_t)(Touch.ADC_Y*Touch_Calibrate_Para.yFactor) - Touch_Calibrate_Para.yOffset;
		
		if(Touch.Touch_Read_ADC_XY(&Touch.ADC_X,&Touch.ADC_Y) == FALSE)
		{
			return FALSE;
		}
		
		//第二次计算LCD屏幕坐标值
		LCD_X2 = (uint16_t)(Touch.ADC_X*Touch_Calibrate_Para.xFactor) - Touch_Calibrate_Para.xOffset;
		LCD_Y2 = (uint16_t)(Touch.ADC_Y*Touch_Calibrate_Para.yFactor) - Touch_Calibrate_Para.yOffset;
		
		//误差检查
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
		
		//计算两次的平均值，得到LCD屏幕坐标值
		Touch.LCD_X = (LCD_X1 + LCD_X2)/2;
		Touch.LCD_Y = (LCD_Y1 + LCD_Y2)/2;
    if(LCD_WIDTH == 320) // 由于电阻屏的特性
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
    // LCD屏幕上显示坐标值
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
  * @brief  读取触摸屏XY坐标值
  * @param  xValue -> 读取到x轴ADC的值的地址
            yValue -> 读取到y轴ADC的值的地址
  * @retval 1成功 0失败
*/
static uint8_t Touch_Read_ADC_XY(uint16_t *xValue, uint16_t *yValue)
{
    uint16_t xValue_Buf[2], yValue_Buf[2];
    uint16_t xValue_Error, yValue_Error;
    // 读取两次
    xValue_Buf[0] = Touch_Read_ADC(Touch_X_CMD);
    yValue_Buf[0] = Touch_Read_ADC(Touch_Y_CMD);
    xValue_Buf[1] = Touch_Read_ADC(Touch_X_CMD);
    yValue_Buf[1] = Touch_Read_ADC(Touch_Y_CMD);
    // 计算两次读取到的误差
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
    // 大于设定误差
    if((xValue_Error > Touch_Error) || (yValue_Error > Touch_Error))
    {
        return 0;
    }
    // 求平均
    *xValue = (xValue_Buf[0] + xValue_Buf[1]) / 2;
    *yValue = (yValue_Buf[0] + yValue_Buf[1]) / 2;
    if(LCD_WIDTH == 320)
    {
      *yValue = (xValue_Buf[0] + xValue_Buf[1]) / 2;
      *xValue = (yValue_Buf[0] + yValue_Buf[1]) / 2;
    }
    // 判断值是否有效
    if((*xValue > Touch_LCD_ADC_xMAX) || (*xValue < Touch_LCD_ADC_xMIN)) return 0;
    if((*yValue > Touch_LCD_ADC_yMAX) || (*yValue < Touch_LCD_ADC_yMIN)) return 0;
    //printf("Touch_Read_ADC_XY:(X,Y)=(%u,%u)\r\n", *xValue, *yValue);
    return 1;
}
/*
  * @name   Touch_Read_ADC
  * @brief  读取触摸屏的ADC值  
  * @param  XT2046_CMD -> 读取命令
  * @retval 读取到的字节
*/
static uint16_t Touch_Read_ADC(uint8_t XT2046_CMD)
{
    uint8_t i, j;
    uint16_t Value_Buf[Touch_READ_TIMES], usTemp;
    uint32_t SumValue = 0;
    
    
    // 读取数据
    for(i = 0; i < Touch_READ_TIMES; i++)
    {
        CLR_TOUCH_SPI_SCK;  //时钟拉低
        CLR_TOUCH_SPI_MISI; //拉低数据线
        // 片选
        CLR_TOUCH_SPI_CS;
        // 发送控制命令
        Touch_Write_Byte(XT2046_CMD);
        delay_us(6);
        // 给一个时钟清除BUSY
        CLR_TOUCH_SPI_SCK;  //时钟拉低
        delay_us(1);
        SET_TOUCH_SPI_SCK;  //时钟拉高
        delay_us(1);
        CLR_TOUCH_SPI_SCK;  //时钟拉低
        //读取数据 只有12位数据有效
        Value_Buf[i] = Touch_Read_Byte();
//        Value_Buf[i] &= (0x7F);   // 最高位数据无效
        Value_Buf[i] <<= 8;
        Value_Buf[i] += Touch_Read_Byte();
        Value_Buf[i] >>= 4;
        SET_TOUCH_SPI_CS;
    }
    // 排序 从大到小
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
    // 去掉最大值和最小值，求和
    for(i = 1; i < (Touch_READ_TIMES-1); i++)
    {
        SumValue += Value_Buf[i];
    }
    return SumValue/(Touch_READ_TIMES - 2);
}
/*
  * @name   TP_Read_Byte
  * @brief  向触摸屏IC读取一个字节数据
  * @param  CMD -> 发送的命令
  * @retval 读取到的字节
*/
static uint8_t Touch_Read_Byte(void)
{
    uint8_t readData = 0;
    uint8_t count;
    //循环读出数据
    for(count=0;count<8;count++)
    {
        readData <<= 1;
        CLR_TOUCH_SPI_SCK;  //时钟拉低
        delay_us(1);
        SET_TOUCH_SPI_SCK;  //时钟拉高
        if(READ_TOUCH_SPI_MISO) readData++;
    }
    return readData;
}
/*
  * @name   TP_Write_Byte
  * @brief  向触摸屏IC写入一个字节数据
  * @param  num -> 写入的数据
  * @retval None
*/
static void Touch_Write_Byte(uint8_t num)
{
    uint8_t count;
    
    //循环写入数据
    for(count = 0; count < 8; count++)
    {
        if(num & 0x80) SET_TOUCH_SPI_MISI;
        else CLR_TOUCH_SPI_MISI;
        num <<=1;
        // CLK产生上升沿发送数据
        CLR_TOUCH_SPI_SCK;
        delay_us(1);
        SET_TOUCH_SPI_SCK;
    }
}
/********************************************************
  End Of File
********************************************************/
