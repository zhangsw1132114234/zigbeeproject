/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/


/* Private function prototypes------------------------------------------------*/
static void Read_ID(void);                              // 读取设备标识符
static void EraseSector(uint32_t);                      // 擦除4KB 扇区
static void EraseTotal(void);                           // 擦除所有内容
static void WritePage(uint8_t*, uint32_t, uint32_t);    // 写入256B，写入字节不能超过256B
static void WriteUnfixed(uint8_t*, uint32_t, uint32_t); // 写入不固定长度的数据
static void ReadUnfixed(uint8_t*, uint32_t, uint32_t);  // 读取不固定长度数据
    
static uint8_t SPI_FLASH_ReadByte(void);                // 从Flash读一个字节
static void SPI_FLASH_WriteByte(uint8_t);               // 从Flash写一个字节

static void SPI_FLASH_WriteEnable(void);                // Flash写使能
static void SPI_FLASH_WaitForWriteEnd(void);            // 等待Flash写入完成
static void TransferPictureToTFT_LCD(uint8_t);          // 读取图片数据显示到屏幕
/* Public variables-----------------------------------------------------------*/
SPI_FLASH_t SPI_FLASH = {
    0,
    0,
    
    Read_ID,
    EraseSector,
    EraseTotal,
    WritePage,
    WriteUnfixed,
    ReadUnfixed,
    TransferPictureToTFT_LCD
};
/*
  * @name   TransferPictureToTFT_LCD
  * @brief  读取图片数据显示到屏幕 
  * @param  Pic_Num -> 图片编号
  * @retval None
*/
static void TransferPictureToTFT_LCD(uint8_t Pic_Num)
{
    uint32_t DataLength = PIC1_LENGTH;
    uint32_t Pic_Addr = NULL;
    uint16_t Pic_Data;
    
    switch(Pic_Num)
    {
        case 1: Pic_Addr = Flash_Pic1_Addr; break;
        // 添加其他图片
    }
    // 片选
    CLR_SPI_FLASH_CS;
    // 读取数据指令
    SPI_FLASH_WriteByte(W25X_ReadData);
    SPI_FLASH_WriteByte((Flash_Pic1_Addr & 0xFF0000) >> 16);
    SPI_FLASH_WriteByte((Flash_Pic1_Addr & 0x00FF00) >> 8);
    SPI_FLASH_WriteByte((Flash_Pic1_Addr & 0x0000FF));
    // 设置窗口
    TFT_LCD.LCD_SetWindows(0,0,240,320);
    // 读取数据
    while(DataLength)
    {
        Pic_Data = SPI_FLASH_ReadByte();
        Pic_Data <<=8;
        Pic_Data += SPI_FLASH_ReadByte();
        
        LCD_Write_DATA(Pic_Data);
        DataLength = DataLength-2;
    }
    SET_SPI_FLASH_CS;
}
/*
  * @name   SPI_FLASH_ReadByte
  * @brief  从Flash读一个字节 
  * @param  None
  * @retval 读到的数据
*/
static uint8_t SPI_FLASH_ReadByte(void)
{
    uint8_t ReceiveByte;
    
    if(HAL_SPI_Receive(&hspi1, &ReceiveByte, 1, 0x0A) != HAL_OK)
    {
        ReceiveByte = Dummy_Byte;
    }
    return ReceiveByte;
}
/*
  * @name   SPI_FLASH_WriteByte
  * @brief  从Flash写一个字节 
  * @param  Byte    ->  待写入字节
  * @retval None
*/
static void SPI_FLASH_WriteByte(uint8_t Byte)
{
    uint8_t SendByte = Byte;
    HAL_SPI_Transmit(&hspi1, &SendByte, 1, 0x0A);
}
/*
  * @name   SPI_FLASH_WriteEnable
  * @brief  Flash写使能 
  * @param  None
  * @retval None
*/
static void SPI_FLASH_WriteEnable(void)
{
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_WriteEnable);
    SET_SPI_FLASH_CS;
}
/*
  * @name   SPI_FLASH_WaitForWriteEnd
  * @brief  等待Flash写入完成
  * @param  None
  * @retval None
*/
static void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t Flash_Status1 = 0;
    
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_ReadStatusReg1);
    // 连续读取状态寄存器1 直到WEL为低电平
    SPI_FLASH.SPI_FLASH_Delay_Timer = 0;
    do{
        Flash_Status1 = SPI_FLASH_ReadByte();
        if(SPI_FLASH.SPI_FLASH_Delay_Timer > TIMER_10S)
            break;
    }while(Flash_Status1 & 0x0001);
    
    SET_SPI_FLASH_CS;
}
/*
  * @name   Read_ID
  * @brief  读取设备标识符 
  * @param  None
  * @retval None
*/
static void Read_ID(void)
{
    uint8_t buf[3];
    
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_JedecDeviceID);
    buf[0] = SPI_FLASH_ReadByte();
    buf[1] = SPI_FLASH_ReadByte();
    buf[2] = SPI_FLASH_ReadByte();
    SET_SPI_FLASH_CS;
    SPI_FLASH.ID = (buf[0] << 16) + (buf[1] << 8) + buf[2];
    printf("The JEDEC ID of SPI Flash is 0x%.6x\r\n", SPI_FLASH.ID);
}
/*
  * @name   EraseSector
  * @brief  擦除4KB 扇区 
  * @param  None
  * @retval None
*/
static void EraseSector(uint32_t SectorAddr)
{
    SPI_FLASH_WriteEnable();
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_SectorErase);
    SPI_FLASH_WriteByte((SectorAddr & 0xFF0000) >> 16);
    SPI_FLASH_WriteByte((SectorAddr & 0x00FF00) >> 8);
    SPI_FLASH_WriteByte((SectorAddr & 0x0000FF));
    SET_SPI_FLASH_CS;
    SPI_FLASH_WaitForWriteEnd();
}
/*
  * @name   EraseTotal
  * @brief  擦除所有内容
  * @param  None
  * @retval None
*/
static void EraseTotal(void)
{
    SPI_FLASH_WriteEnable();
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_ChipErase);
    SET_SPI_FLASH_CS;
    SPI_FLASH_WaitForWriteEnd();
}
/*
  * @name   WritePage
  * @brief  写入256B数据
  * @param  None
  * @retval None
*/
static void WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    // SET WEL
    SPI_FLASH_WriteEnable();
    // 开启片选
    CLR_SPI_FLASH_CS;
    // 发送写页命令
    SPI_FLASH_WriteByte(W25X_PageProgram);
    // 发送24bit地址
    SPI_FLASH_WriteByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_WriteByte((WriteAddr & 0x00FF00) >> 8);
    SPI_FLASH_WriteByte((WriteAddr & 0x0000FF));
    // 循环写入数据
    if(NumByteToWrite > SPI_FLASH_PageSize)
    {
        NumByteToWrite = SPI_FLASH_PageSize;
        printf("Error: Flash写入数据不能超过256字节\r\n");
    }
    while(NumByteToWrite--)
    {
        SPI_FLASH_WriteByte(*pBuffer);
        pBuffer++;
    }
    // 关闭片选
    SET_SPI_FLASH_CS;
    SPI_FLASH_WaitForWriteEnd();
}
/*
  * @name   WriteUnfixed
  * @brief  写入不固定长数据
  * @param  None
  * @retval None
*/
static void WriteUnfixed(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength)
{
    uint32_t PageNumofWriteLength    = WriteLength /SPI_FLASH_PageSize;
    uint8_t NotEnoughNumofPage       = WriteLength%SPI_FLASH_PageSize;
    uint8_t WriteAddrPageAlignment   = WriteAddr % SPI_FLASH_PageSize;
    uint8_t NotAlignmentNumofPage    = SPI_FLASH_PageSize -WriteAddrPageAlignment;
    
        // 写入地址页对齐
    if (WriteAddrPageAlignment == 0) 
    {
        // 待写入数据不足一页
        if (PageNumofWriteLength == 0) 
        {
            WritePage(pWriteBuffer, WriteAddr, WriteLength);
        }else 
        {
            // 待写入数据超过一页
            // 先写入整页
            while (PageNumofWriteLength--) 
            {
                WritePage(pWriteBuffer, WriteAddr, SPI_FLASH_PageSize);
                pWriteBuffer += SPI_FLASH_PageSize;
                WriteAddr += SPI_FLASH_PageSize;
            }
            // 再写入不足一页的数据
            if (NotEnoughNumofPage > 0) 
            {
                WritePage(pWriteBuffer, WriteAddr, NotEnoughNumofPage);
            }
        }
    }else
    {
        // 待写入数据不足一页
        if (PageNumofWriteLength == 0) 
        {
            // 不足一页的数据 <= 地址不对齐部分
            if (NotEnoughNumofPage <= NotAlignmentNumofPage) 
            {
                WritePage(pWriteBuffer, WriteAddr, WriteLength);
            } 
            // 不足一页的数据 > 地址不对齐部分
            else 
            {
                // 先写地址不对齐部分允许写入的最大长度
                WritePage(pWriteBuffer, WriteAddr, NotAlignmentNumofPage);
                pWriteBuffer += NotAlignmentNumofPage;
                WriteAddr += NotAlignmentNumofPage;

                // 再写不足一页的数据
                WritePage(pWriteBuffer, WriteAddr, NotEnoughNumofPage - NotAlignmentNumofPage);
            }
        }else
        {
            // 先写地址不对齐部分允许写入的最大长度，地址此时对齐了
            WritePage(pWriteBuffer, WriteAddr, NotAlignmentNumofPage);
            pWriteBuffer += NotAlignmentNumofPage;
            WriteAddr += NotAlignmentNumofPage;

            // 地址对齐后，重新计算剩余数据
            WriteLength -= NotAlignmentNumofPage;
            PageNumofWriteLength = WriteLength / SPI_FLASH_PageSize;  // 待写入整页数
            NotEnoughNumofPage = WriteLength % SPI_FLASH_PageSize;    // 剩余不足一页的数据

            // 先写入整页
            while (PageNumofWriteLength--) 
            {
                WritePage(pWriteBuffer, WriteAddr, SPI_FLASH_PageSize);
                pWriteBuffer += SPI_FLASH_PageSize;
                WriteAddr += SPI_FLASH_PageSize;
            }
            if(NotEnoughNumofPage > 0)
            {
                WritePage(pWriteBuffer, WriteAddr, NotEnoughNumofPage);
            }

        }

    }
    

}
/*
  * @name   ReadUnfixed
  * @brief  读取不固定长数据
  * @param  pBuffer     -> 存放读取数据的缓存指针
            ReadAddr    -> 读取地址
            NumByteToRead-> 读取长度
  * @retval None
*/
static void ReadUnfixed(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_ReadData);
    SPI_FLASH_WriteByte((ReadAddr & 0xFF0000) >> 16);
    SPI_FLASH_WriteByte((ReadAddr & 0x00FF00) >> 8);
    SPI_FLASH_WriteByte((ReadAddr & 0x0000FF));
    
    while(NumByteToRead--)
    {
        *pBuffer = SPI_FLASH_ReadByte();
        pBuffer++;
    }
    SET_SPI_FLASH_CS;
}
/********************************************************
  End Of File
********************************************************/
