
#include "stdafx.h"
#include "modbus_dataapdapter.h"

namespace SixdofModbus
{

	static int scale = 1000.0;
	static uint16_t dataBuffers[sizeof(ModbusPackage) / 2];

	inline int16_t ExchangeInt16Bit(int16_t data)
	{
		return (int16_t)((((uint8_t)data) << 8) + (uint8_t)(data >> 8));
	}

	inline int32_t ExchangeInt32Bit(int32_t data)
	{
		static int8_t b[4] = {0};
		memmove(b, &data, sizeof(int8_t) * 4);
		return (int32_t)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
	}

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

	void ModbusDoExchange(ModbusPackage * data)
	{
		ExchangeInt16Bit(data->Head);
		ExchangeInt16Bit(data->Tail);
		ExchangeInt16Bit(data->Time);
		ExchangeInt16Bit(data->PackageCount);

		ExchangeInt16Bit(data->X);
		ExchangeInt16Bit(data->Y);
		ExchangeInt16Bit(data->Z);
		ExchangeInt16Bit(data->Roll);
		ExchangeInt16Bit(data->Pitch);
		ExchangeInt16Bit(data->Yaw);

		ExchangeInt16Bit(data->XSpeed);
		ExchangeInt16Bit(data->YSpeed);
		ExchangeInt16Bit(data->ZSpeed);
		ExchangeInt16Bit(data->RollSpeed);
		ExchangeInt16Bit(data->PitchSpeed);
		ExchangeInt16Bit(data->YawSpeed);

		ExchangeInt16Bit(data->XAcc);
		ExchangeInt16Bit(data->YAcc);
		ExchangeInt16Bit(data->ZAcc);
		ExchangeInt16Bit(data->RollAcc);
		ExchangeInt16Bit(data->PitchAcc);
		ExchangeInt16Bit(data->YawAcc);

		ExchangeInt16Bit(data->Reseverd1);
		ExchangeInt16Bit(data->Reseverd2);
		ExchangeInt16Bit(data->Reseverd3);
		ExchangeInt16Bit(data->Reseverd4);
	}

	void ModbusDataAdapter::DataInit()
	{
		ModbusPackageInit(&data);
		bufferLength = sizeof(ModbusPackage) / 2;
	}

	void ModbusDataAdapter::ModbusInit()
	{
		modbus = modbus_new_tcp(MODBUS_MASTER_IP_STRING, MODBUS_TCP_DEFAULT_PORT);
		if (modbus_connect(modbus) == -1)
		{
			IsConnect = false;
			modbus_free(modbus);
			return;
		}
		modbus_set_slave(modbus, MODBUS_SLAVE_ID);
	}

	void ModbusDataAdapter::ExchangeData(ControlCommandEnum& command, SixdofStateEnum& status, RoadSpectrumData& roaddata)
	{
		if (modbus == nullptr)
			return;
		data.ControlCommand = (uint8_t)command;
		data.X = (int16_t)(roaddata.Position.X / DATA_SCALE);
		data.Y = (int16_t)(roaddata.Position.Y / DATA_SCALE);
		data.Z = (int16_t)(roaddata.Position.Z / DATA_SCALE);
		data.Roll = (int16_t)(roaddata.Position.Roll / DATA_SCALE);
		data.Pitch = (int16_t)(roaddata.Position.Pitch / DATA_SCALE);
		data.Yaw = (int16_t)(roaddata.Position.Yaw / DATA_SCALE);

		data.XSpeed = (int16_t)(roaddata.Speed.X / DATA_SCALE);
		data.YSpeed = (int16_t)(roaddata.Speed.Y / DATA_SCALE);
		data.ZSpeed = (int16_t)(roaddata.Speed.Z / DATA_SCALE);
		data.RollSpeed = (int16_t)(roaddata.Speed.Roll / DATA_SCALE);
		data.PitchSpeed = (int16_t)(roaddata.Speed.Pitch / DATA_SCALE);
		data.YawSpeed = (int16_t)(roaddata.Speed.Yaw / DATA_SCALE);

		data.XAcc = (int16_t)(roaddata.Acc.X / DATA_SCALE);
		data.YAcc = (int16_t)(roaddata.Acc.Y / DATA_SCALE);
		data.ZAcc = (int16_t)(roaddata.Acc.Z / DATA_SCALE);
		data.RollAcc = (int16_t)(roaddata.Acc.Roll / DATA_SCALE);
		data.PitchAcc = (int16_t)(roaddata.Acc.Pitch / DATA_SCALE);
		data.YawAcc = (int16_t)(roaddata.Acc.Yaw / DATA_SCALE);
		WriteData();
		ReadData();
		status = (SixdofStateEnum)data.SixdofState;
	}

	void ModbusDataAdapter::ReadData()
	{
		if (modbus == nullptr)
			return;
		modbus_read_registers(modbus, 0 + (int)IS_PLC_COMM, bufferLength, dataBuffers);
		memmove(&data, dataBuffers, sizeof(uint16_t) * bufferLength);
		if (IS_PLC_BIGENDIAN == true)
		{
			ModbusDoExchange(&data);
		}
	}

	void ModbusDataAdapter::WriteData()
	{
		if (modbus == nullptr)
			return;
		if (IS_PLC_BIGENDIAN == true)
		{
			ModbusDoExchange(&data);
		}	
		memmove(dataBuffers, &data, sizeof(uint16_t) * bufferLength);
		modbus_write_registers(modbus, 0 + (int)IS_PLC_COMM, bufferLength, dataBuffers);
	}

	ModbusDataAdapter::ModbusDataAdapter()
	{
		DataInit();
		//ModbusInit();
	}

	ModbusDataAdapter::~ModbusDataAdapter()
	{
		modbus_close(modbus);
		modbus_free(modbus);
	}

}
