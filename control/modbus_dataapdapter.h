

#ifndef __MODBUS_DATA_ADAPTER_H_
#define __MODBUS_DATA_ADAPTER_H_

#include <stdint.h>
#include <memory>

#include "../modbus.h"
#include "../signal/roadspectrum.h"

using namespace Signal;

namespace SixdofModbus
{

#define MODBUS_SLAVE_PORT         9000
#define MODBUS_SLAVE_IP_STRING    "192.168.0.104"

#define MODBUS_MASTER_PORT        502
#define MODBUS_MASTER_IP_STRING   "192.168.0.140"

#define MODBUS_SLAVE_ID  1

#define MODBUS_DATA_HEAD  0xAABB
#define MODBUS_DATA_TAIL  0xCCDD

#define IS_MODBUS_SLAVE  (true)
#define IS_PLC_BIGENDIAN (true)
#define IS_PLC_COMM      (true)

#define DATA_SCALE 0.001

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

typedef enum 
{
	SIXDOF_STATE_NONE,
	SIXDOF_STATE_RISING,
	SIXDOF_STATE_DOWN,
	SIXDOF_STATE_START_SIGNAL,
	SIXDOF_STATE_START_SINE,
	SIXDOF_STATE_STOP,
	SIXDOF_STATE_PAUSE,
	SIXDOF_STATE_RECOVER,
}SixdofStateEnum;

#pragma pack (1)
typedef struct
{
	uint16_t Head;
	uint8_t SixdofState;
	uint8_t IsEnableData;
	uint8_t IsShock;
	uint8_t ControlCommand;
	uint16_t Time;
	uint16_t PackageCount;
	int16_t X;
	int16_t Y;
	int16_t Z;
	int16_t Roll;
	int16_t Pitch;
	int16_t Yaw;
	int16_t XSpeed;
	int16_t YSpeed;
	int16_t ZSpeed;
	int16_t RollSpeed;
	int16_t PitchSpeed;
	int16_t YawSpeed;
	int16_t XAcc;
	int16_t YAcc;
	int16_t ZAcc;
	int16_t RollAcc;
	int16_t PitchAcc;
	int16_t YawAcc;
	uint16_t Reseverd1;
	uint16_t Reseverd2;
	uint16_t Reseverd3;
	uint16_t Reseverd4;
	uint16_t Tail;
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

