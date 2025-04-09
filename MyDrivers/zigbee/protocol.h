#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#define TEMP_NODE

#include "MyApplication.h"

/*
  zigbee�Զ���Э�飺
  

*/

typedef uint8_t uint8;
typedef uint16_t uint16;
//zigbeeӦ��Э���ͷ��β
#define HEAD  0x99
#define TAIL  0x01
//���ڲ�ͬ�ڵ����Ͳ�һ�� ��ַ��һ�� ���Խ��к궨��
#ifdef TEMP_NODE
#define NODE_ID     0x0000  //�ڵ�ID
#define NODE_TYPE   0x0     //�ڵ�����
#define DATA_LEN    0x02    //���ݳ���
#define DATA_VALUE  0x2070  //����ֵ
#endif

#ifdef FAN_NODE
#define NODE_ID     0x0001
#define NODE_TYPE   0x1
#define DATA_LEN    0x01
#define DATA_VALUE  0x01
#endif
// ��������Ͳ��
// Э��ͷ | �ڵ�ID | �ڵ����� | ���ݳ��� | ���� | CRC | Э��β
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
// Э�鳤��
#define PROTOCOL_LEN sizeof(ProtocolDataType)
// CRCУ���㷨
uint8 crc8(uint8 *data, uint8 length);
// ���Э��Ϸ���
uint8  ProtocolCheck(uint8 *Data,uint16 DataLen);
// Э�����
void ProtocolParse(uint8 *Data,uint16 DataLen);
// Э���齨
uint16  CreateData(uint8 *Data);
// ���ȿ���
void FanControl(uint8_t Status);


#endif
