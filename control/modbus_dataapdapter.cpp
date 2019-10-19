
#include "stdafx.h"
#include "modbus_dataapdapter.h"

namespace SixdofModbus
{

	static int scale = 1000.0;

	void ModbusPackageInit(ModbusPackage * data)
	{
		data->Head = MODBUS_DATA_HEAD;
		data->Tail = MODBUS_DATA_TAIL;
	}

	void ModbusPackageSetData(ModbusPackage * data, 
		double x, double y, double z, double roll, double pitch, double yaw)
	{
		ModbusPackageInit(data);	
		data->X = (int32_t)(x * scale);
		data->Y = (int32_t)(y * scale);
		data->Z = (int32_t)(z * scale);
		data->Roll = (int32_t)(roll * scale);
		data->Pitch = (int32_t)(pitch * scale);
		data->Yaw = (int32_t)(yaw * scale);
	}

}
