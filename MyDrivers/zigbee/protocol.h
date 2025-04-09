#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#define TEMP_NODE

#include "MyApplication.h"

/*
  zigbee自定义协议：
  

*/

typedef uint8_t uint8;
typedef uint16_t uint16;
//zigbee应用协议的头和尾
#define HEAD  0x99
#define TAIL  0x01
//由于不同节点类型不一样 地址不一样 所以进行宏定义
#ifdef TEMP_NODE
#define NODE_ID     0x0000  //节点ID
#define NODE_TYPE   0x0     //节点类型
#define DATA_LEN    0x02    //数据长度
#define DATA_VALUE  0x2070  //数据值
#endif

#ifdef FAN_NODE
#define NODE_ID     0x0001
#define NODE_TYPE   0x1
#define DATA_LEN    0x01
#define DATA_VALUE  0x01
#endif
// 用与组包和拆包
// 协议头 | 节点ID | 节点类型 | 数据长度 | 数据 | CRC | 协议尾
typedef struct{
  uint8   Head;
  uint16  NodeId;
  uint8   NodeType;
  uint8   DataLen;
  uint8   Data;
  uint8   Crc;
  uint8   Tail;
}ProtocolDataType;
extern ProtocolDataType  ProtocolData;

extern uint8 Temp_Hum_Data[2];

extern uint8 fan;
// 协议长度
#define PROTOCOL_LEN sizeof(ProtocolDataType)
// CRC校验算法
uint8 crc8(uint8 *data, uint8 length);
// 检查协议合法性
uint8  ProtocolCheck(uint8 *Data,uint16 DataLen);
// 协议解析
void ProtocolParse(uint8 *Data,uint16 DataLen);
// 协议组建
uint16  CreateData(uint8 *Data);
// 风扇控制
void FanControl(uint8_t Status);


#endif
