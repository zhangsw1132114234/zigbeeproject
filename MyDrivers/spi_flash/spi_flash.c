/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/


/* Private function prototypes------------------------------------------------*/
static void Read_ID(void);                              // ��ȡ�豸��ʶ��
static void EraseSector(uint32_t);                      // ����4KB ����
static void EraseTotal(void);                           // ������������
static void WritePage(uint8_t*, uint32_t, uint32_t);    // д��256B��д���ֽڲ��ܳ���256B
static void WriteUnfixed(uint8_t*, uint32_t, uint32_t); // д�벻�̶����ȵ�����
static void ReadUnfixed(uint8_t*, uint32_t, uint32_t);  // ��ȡ���̶���������
    
static uint8_t SPI_FLASH_ReadByte(void);                // ��Flash��һ���ֽ�
static void SPI_FLASH_WriteByte(uint8_t);               // ��Flashдһ���ֽ�

static void SPI_FLASH_WriteEnable(void);                // Flashдʹ��
static void SPI_FLASH_WaitForWriteEnd(void);            // �ȴ�Flashд�����
static void TransferPictureToTFT_LCD(uint8_t);          // ��ȡͼƬ������ʾ����Ļ
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
  * @brief  ��ȡͼƬ������ʾ����Ļ 
  * @param  Pic_Num -> ͼƬ���
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
        // �������ͼƬ
    }
    // Ƭѡ
    CLR_SPI_FLASH_CS;
    // ��ȡ����ָ��
    SPI_FLASH_WriteByte(W25X_ReadData);
    SPI_FLASH_WriteByte((Flash_Pic1_Addr & 0xFF0000) >> 16);
    SPI_FLASH_WriteByte((Flash_Pic1_Addr & 0x00FF00) >> 8);
    SPI_FLASH_WriteByte((Flash_Pic1_Addr & 0x0000FF));
    // ���ô���
    TFT_LCD.LCD_SetWindows(0,0,240,320);
    // ��ȡ����
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
  * @brief  ��Flash��һ���ֽ� 
  * @param  None
  * @retval ����������
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
  * @brief  ��Flashдһ���ֽ� 
  * @param  Byte    ->  ��д���ֽ�
  * @retval None
*/
static void SPI_FLASH_WriteByte(uint8_t Byte)
{
    uint8_t SendByte = Byte;
    HAL_SPI_Transmit(&hspi1, &SendByte, 1, 0x0A);
}
/*
  * @name   SPI_FLASH_WriteEnable
  * @brief  Flashдʹ�� 
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
  * @brief  �ȴ�Flashд�����
  * @param  None
  * @retval None
*/
static void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t Flash_Status1 = 0;
    
    CLR_SPI_FLASH_CS;
    SPI_FLASH_WriteByte(W25X_ReadStatusReg1);
    // ������ȡ״̬�Ĵ���1 ֱ��WELΪ�͵�ƽ
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
  * @brief  ��ȡ�豸��ʶ�� 
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
  * @brief  ����4KB ���� 
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
  * @brief  ������������
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
  * @brief  д��256B����
  * @param  None
  * @retval None
*/
static void WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    // SET WEL
    SPI_FLASH_WriteEnable();
    // ����Ƭѡ
    CLR_SPI_FLASH_CS;
    // ����дҳ����
    SPI_FLASH_WriteByte(W25X_PageProgram);
    // ����24bit��ַ
    SPI_FLASH_WriteByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_WriteByte((WriteAddr & 0x00FF00) >> 8);
    SPI_FLASH_WriteByte((WriteAddr & 0x0000FF));
    // ѭ��д������
    if(NumByteToWrite > SPI_FLASH_PageSize)
    {
        NumByteToWrite = SPI_FLASH_PageSize;
        printf("Error: Flashд�����ݲ��ܳ���256�ֽ�\r\n");
    }
    while(NumByteToWrite--)
    {
        SPI_FLASH_WriteByte(*pBuffer);
        pBuffer++;
    }
    // �ر�Ƭѡ
    SET_SPI_FLASH_CS;
    SPI_FLASH_WaitForWriteEnd();
}
/*
  * @name   WriteUnfixed
  * @brief  д�벻�̶�������
  * @param  None
  * @retval None
*/
static void WriteUnfixed(uint8_t* pWriteBuffer, uint32_t WriteAddr, uint32_t WriteLength)
{
    uint32_t PageNumofWriteLength    = WriteLength /SPI_FLASH_PageSize;
    uint8_t NotEnoughNumofPage       = WriteLength%SPI_FLASH_PageSize;
    uint8_t WriteAddrPageAlignment   = WriteAddr % SPI_FLASH_PageSize;
    uint8_t NotAlignmentNumofPage    = SPI_FLASH_PageSize -WriteAddrPageAlignment;
    
        // д���ַҳ����
    if (WriteAddrPageAlignment == 0) 
    {
        // ��д�����ݲ���һҳ
        if (PageNumofWriteLength == 0) 
        {
            WritePage(pWriteBuffer, WriteAddr, WriteLength);
        }else 
        {
            // ��д�����ݳ���һҳ
            // ��д����ҳ
            while (PageNumofWriteLength--) 
            {
                WritePage(pWriteBuffer, WriteAddr, SPI_FLASH_PageSize);
                pWriteBuffer += SPI_FLASH_PageSize;
                WriteAddr += SPI_FLASH_PageSize;
            }
            // ��д�벻��һҳ������
            if (NotEnoughNumofPage > 0) 
            {
                WritePage(pWriteBuffer, WriteAddr, NotEnoughNumofPage);
            }
        }
    }else
    {
        // ��д�����ݲ���һҳ
        if (PageNumofWriteLength == 0) 
        {
            // ����һҳ������ <= ��ַ�����벿��
            if (NotEnoughNumofPage <= NotAlignmentNumofPage) 
            {
                WritePage(pWriteBuffer, WriteAddr, WriteLength);
            } 
            // ����һҳ������ > ��ַ�����벿��
            else 
            {
                // ��д��ַ�����벿������д�����󳤶�
                WritePage(pWriteBuffer, WriteAddr, NotAlignmentNumofPage);
                pWriteBuffer += NotAlignmentNumofPage;
                WriteAddr += NotAlignmentNumofPage;

                // ��д����һҳ������
                WritePage(pWriteBuffer, WriteAddr, NotEnoughNumofPage - NotAlignmentNumofPage);
            }
        }else
        {
            // ��д��ַ�����벿������д�����󳤶ȣ���ַ��ʱ������
            WritePage(pWriteBuffer, WriteAddr, NotAlignmentNumofPage);
            pWriteBuffer += NotAlignmentNumofPage;
            WriteAddr += NotAlignmentNumofPage;

            // ��ַ��������¼���ʣ������
            WriteLength -= NotAlignmentNumofPage;
            PageNumofWriteLength = WriteLength / SPI_FLASH_PageSize;  // ��д����ҳ��
            NotEnoughNumofPage = WriteLength % SPI_FLASH_PageSize;    // ʣ�಻��һҳ������

            // ��д����ҳ
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
  * @brief  ��ȡ���̶�������
  * @param  pBuffer     -> ��Ŷ�ȡ���ݵĻ���ָ��
            ReadAddr    -> ��ȡ��ַ
            NumByteToRead-> ��ȡ����
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
