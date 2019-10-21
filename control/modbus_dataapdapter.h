

#ifndef __MODBUS_DATA_ADAPTER_H_
#define __MODBUS_DATA_ADAPTER_H_

#include <stdint.h>
#include <memory>

#include "../modbus.h"
#include "../signal/roadspectrum.h"

#include "comwithplc.h"

using namespace Signal;

namespace SixdofModbus
{

#define MODBUS_SLAVE_PORT         9000
#define MODBUS_SLAVE_IP_STRING    "192.168.0.123"

#define MODBUS_MASTER_PORT        502
#define MODBUS_MASTER_IP_STRING   "192.168.0.22"

#define MODBUS_SLAVE_ID  1

#define MODBUS_DATA_HEAD  0xAABB
#define MODBUS_DATA_TAIL  0xCCDD

#define IS_MODBUS_SLAVE  (true)
#define IS_PLC_BIGENDIAN (true)
#define IS_PLC_COMM      (true)

#define DATA_SCALE 0.001

#pragma pack (1)
typedef struct
{
	uint16_t Head;          // 首校验 0xAABB
	uint8_t SixdofState;    // 六自由度平台状态
	uint8_t IsEnableData;   // 是否运行路谱
	uint8_t IsTest;         // 是否是正弦运动
	uint8_t ControlCommand; // 六自由度平台控制指令
	uint16_t Time;          // Utc时间
	uint16_t PackageCount;  // 包序，每发送一次+1
	int32_t X;              // X线位移 0.001mm
	int32_t Y;              // Y线位移 0.001mm
	int32_t Z;              // Z线位移 0.001mm
	int32_t Roll;           // 横滚角  0.001度
	int32_t Pitch;          // 俯仰角  0.001度
	int32_t Yaw;            // 偏航角  0.001度
	int32_t XSpeed;         // X线位移速度 0.001mm/s
	int32_t YSpeed;         // Y线位移速度 0.001mm/s
	int32_t ZSpeed;         // Z线位移 0.001mm/s
	int32_t RollSpeed;      // 横滚角速度  0.001度/s
	int32_t PitchSpeed;     // 俯仰角速度  0.001度/s
	int32_t YawSpeed;       // 偏航角速度  0.001度/s
	int32_t XAcc;           // X线位移加速度 0.001mm/s^2
	int32_t YAcc;           // Y线位移加速度 0.001mm/s^2
	int32_t ZAcc;           // Z线位移加速度 0.001mm/s^2
	int32_t RollAcc;        // 横滚角加速度  0.001度^2
	int32_t PitchAcc;       // 俯仰角加速度  0.001度^2
	int32_t YawAcc;         // 偏航角加速度  0.001度^2
	uint16_t Reseverd1;     // 保留1
	uint16_t Reseverd2;     // 保留2
	uint16_t Reseverd3;     // 保留3
	uint16_t Reseverd4;     // 保留4
	uint16_t Tail;          //尾校验 0xCCDD
}ModbusPackage;
#pragma pack () 

class ModbusDataAdapter
{
public:
	ModbusDataAdapter();
	~ModbusDataAdapter();
	void ExchangeData(ControlCommandEnum& command, SixdofStateEnum& status, RoadSpectrumData& roaddata);
	bool IsConnect;
protected:
	int bufferLength;
	ModbusPackage data;
	modbus_t* modbus;
private:
	void ReadData();
	void WriteData();
	void DataInit();	
	void ModbusInit();
};

void ModbusPackageInit(ModbusPackage * data);
void ModbusPackageSetData(ModbusPackage * data, 
						  double x, double y, double z, double roll, double pitch, double yaw);

}


#endif

