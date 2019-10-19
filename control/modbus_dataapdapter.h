

#ifndef __MODBUS_DATA_ADAPTER_H_
#define __MODBUS_DATA_ADAPTER_H_

#include <stdint.h>

#include "../modbus.h"
#include "../modbus-tcp.h"

namespace SixdofModbus
{

#define MODBUS_SLAVE_PORT         9000
#define MODBUS_SLAVE_IP_STRING    "192.168.0.104"

#define MODBUS_MASTER_PORT        502
#define MODBUS_MASTER_IP_STRING   "192.168.0.140"

#define IS_MODBUS_SLAVE (true)

#define MODBUS_DATA_HEAD  0x55
#define MODBUS_DATA_TAIL  0x88

typedef enum 
{
	CONTROL_COMMAND_NONE,
	CONTROL_COMMAND_RISING,
	CONTROL_COMMAND_DOWN,
	CONTROL_COMMAND_START_SIGNAL,
	CONTROL_COMMAND_START_SINE,
	CONTROL_COMMAND_STOP,
	CONTROL_COMMAND_PAUSE,
	CONTROL_COMMAND_RECOVER,
}ControlCommandEnum;

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
	int32_t X;
	int32_t Y;
	int32_t Z;
	int32_t Roll;
	int32_t Pitch;
	int32_t Yaw;
	int32_t XSpeed;
	int32_t YSpeed;
	int32_t ZSpeed;
	int32_t RollSpeed;
	int32_t PitchSpeed;
	int32_t YawSpeed;
	int32_t XAcc;
	int32_t YAcc;
	int32_t ZAcc;
	int32_t RollAcc;
	int32_t PitchAcc;
	int32_t YawAcc;
	uint32_t Reseverd1;
	uint32_t Reseverd2;
	uint32_t Reseverd3;
	uint32_t Reseverd4;
	uint8_t Tail;
}ModbusPackage;
#pragma pack () 

void ModbusPackageInit(ModbusPackage * data);
void ModbusPackageSetData(ModbusPackage * data, 
						  double x, double y, double z, double roll, double pitch, double yaw);

}


#endif

