

#ifndef __MODBUS_DATA_ADAPTER_H_
#define __MODBUS_DATA_ADAPTER_H_

#include <stdint.h>

#include "../modbus.h"
#include "../modbus-tcp.h"

namespace SixdofModbus
{

#define MODBUS_SLAVE_PORT         9000
#define MODBUS_SLAVE_IP_STRING    "192.168.0.104"

#define MODBUS_MASTER_PORT   502
#define MODBUS_MASTER_IP_STRING   "192.168.0.140"

#define IS_MODBUS_SLAVE (true)

// 注意PLC数据存储格式一般是大端模式
#pragma pack (1)
typedef struct
{
	uint8_t Head;
	uint8_t SixdofState;
	uint8_t IsEnableData;
	uint8_t IsShock;
	uint8_t ControlCommand;
	uint32_t Time;
	uint32_t PackageCount;
	uint32_t X;
	uint32_t Y;
	uint32_t Z;
	uint32_t Roll;
	uint32_t Pitch;
	uint32_t Yaw;
	uint32_t XSpeed;
	uint32_t YSpeed;
	uint32_t ZSpeed;
	uint32_t RollSpeed;
	uint32_t PitchSpeed;
	uint32_t YawSpeed;
	uint32_t XAcc;
	uint32_t YAcc;
	uint32_t ZAcc;
	uint32_t RollAcc;
	uint32_t PitchAcc;
	uint32_t YawAcc;
	uint32_t Reseverd1;
	uint32_t Reseverd2;
	uint32_t Reseverd3;
	uint32_t Reseverd4;
	uint8_t Tail;
}ModbusPackage;
#pragma pack () 

}


#endif

