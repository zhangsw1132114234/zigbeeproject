#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__
#include "MyApplication.h"

// CS管脚控制
#define CLR_SPI_FLASH_CS HAL_GPIO_WritePin(SPI_Flash_CS_GPIO_Port, SPI_Flash_CS_Pin, GPIO_PIN_RESET)
#define SET_SPI_FLASH_CS HAL_GPIO_WritePin(SPI_Flash_CS_GPIO_Port, SPI_Flash_CS_Pin, GPIO_PIN_SET)

//指令集表
#define W25X_WriteEnable         0x06
#define W25X_WriteDisable        0x04
#define W25X_ReadStatusReg1      0x05
#define W25X_ReadStatusReg2      0x35
#define W25X_ReadStatusReg3      0x15
#define W25X_WriteStatusReg1     0x01
#define W25X_WriteStatusReg2     0x31
#define W25X_WriteStatusReg3     0x11
#define W25X_ReadData            0x03
#define W25X_FastReadData        0x0B
#define W25X_FastReadDual        0x3B
#define W25X_PageProgram         0x02
#define W25X_BlockErase          0xD8
#define W25X_SectorErase         0x20
#define W25X_ChipErase           0xC7
#define W25X_PowerDown           0xB9
#define W25X_ReleasePowerDown    0xAB
#define W25X_DeviceID            0xAB
#define W25X_ManufactDeviceID    0x90
#define W25X_JedecDeviceID       0x9F
#define W25X_Enable4ByteAddr     0xB7
#define W25X_Exit4ByteAddr       0xE9

#define SPI_FLASH_PageSize  256      // 页大小
#define Dummy_Byte          0xFF   // 无效数据
// 存储的图片数据
#define PIC1_LENGTH (uint32_t)153600
#define Flash_Pic1_Addr 0x00000000

typedef struct
{
    uint32_t ID;                                        // 设备标识符 制造商+内存类型+容量
    uint16_t SPI_FLASH_Delay_Timer;                     // spi延时定时器
    
    void (*Read_ID)(void);                              // 读取设备标识符
    void (*EraseSector)(uint32_t);                      // 擦除4KB 扇区
    void (*EraseTotal)(void);                           // 擦除所有内容
    void (*WritePage)(uint8_t*, uint32_t, uint32_t);    // 写入256B，写入字节不能超过256B
    void (*WriteUnfixed)(uint8_t*, uint32_t, uint32_t); // 写入不固定长度的数据
    void (*ReadUnfixed)(uint8_t*, uint32_t, uint32_t);  // 读取不固定长度数据
    void (*TransferPictureToTFT_LCD)(uint8_t);          // 读取图片数据显示到屏幕
}SPI_FLASH_t;

/* extern variables-----------------------------------------------------------*/
extern SPI_FLASH_t SPI_FLASH;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
