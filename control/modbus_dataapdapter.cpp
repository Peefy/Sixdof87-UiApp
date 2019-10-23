
#include "stdafx.h"
#include "modbus_dataapdapter.h"

namespace SixdofModbus
{
	#define BUFFER_MAX_COUNT 200

	static uint16_t dataBuffers[BUFFER_MAX_COUNT];

	static inline int16_t ExchangeInt16Bit(int16_t data)
	{
		return (int16_t)((((uint8_t)data) << 8) + (uint8_t)(data >> 8));
	}

	static inline int32_t ExchangeInt32Bit(int32_t data)
	{
		static int8_t b[4] = {0};
		memmove(b, &data, sizeof(int8_t) * 4);
		return (int32_t)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
	}

	static void ModbusSetFloat32ToReg(uint16_t * regs, int index, float val)
	{
		static uint8_t b[4] = {0};
		memmove(b, &val, sizeof(uint8_t) * 4);
		memmove(regs + index, b, sizeof(uint16_t) * 1);
		memmove(regs + index + 1, b + 2, sizeof(uint16_t) * 1);
	}

	static void ModbusSetRegToFloat32(uint16_t * regs, int index, float* val)
	{
		static int8_t b[4] = {0};
		memmove(b, regs + index, sizeof(uint16_t));
		memmove(b + 2, regs + index + 1, sizeof(uint16_t));
		memmove(val, b, sizeof(float) * 1);
	}

	void ModbusDataAdapter::DataInit()
	{
		
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
		
	}

	void ModbusDataAdapter::SetIsEnableRoadData(bool isEnable)
	{
		if (modbus == nullptr)
			return;
		modbus_write_bit(modbus, ENABLE_ROAD_DATA_MODBUS_ADDRESS, isEnable ? 1 : 0);
	}

	void ModbusDataAdapter::SetRoadData(RoadSpectrumData& roaddata)
	{
		if (modbus == nullptr)
			return;
		int i = 0;
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Position.X));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Position.Y));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Position.Z));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Position.Roll));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Position.Pitch));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Position.Yaw));

		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Speed.X));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Speed.Y));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Speed.Z));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Speed.Roll));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Speed.Pitch));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Speed.Yaw));

		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Acc.X));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Acc.Y));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Acc.Z));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Acc.Roll));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Acc.Pitch));
		ModbusSetFloat32ToReg(dataBuffers, (i++) * 2, static_cast<float>(roaddata.Acc.Yaw));

		modbus_write_registers(modbus, 
			SIXDOF_ROAD_DATA_MODBUS_ADDRESS , SIXDOF_ROAD_DATA_MODBUS_REG_COUNT, dataBuffers);
	}

	void ModbusDataAdapter::SetPlatformStatus(ControlCommandEnum& command)
	{
		if (modbus == nullptr)
			return;
		switch (command)
		{
		// 0 空指令
		case CONTROL_COMMAND_NONE:
			break;
		// 1 上升
		case CONTROL_COMMAND_RISING:
			break;
		// 2 下降
		case CONTROL_COMMAND_DOWN:
			break;
		// 3 路谱运行
		case CONTROL_COMMAND_START_SIGNAL:
			break;
		// 4 正弦运行
		case CONTROL_COMMAND_START_SINE:
			break;
		// 5 平台回中
		case CONTROL_COMMAND_MIDDLE:
			break;
		// 6 平台停止并回中
		case CONTROL_COMMAND_STOP:
			break;
		// 7 平台暂停运行
		case CONTROL_COMMAND_PAUSE:
			break;
		// 8 平台恢复运行
		case CONTROL_COMMAND_RECOVER:
			break;
		default:
			break;
		}
	}

	void ModbusDataAdapter::GetPlatformStatue(SixdofStateEnum& status)
	{
		if (modbus == nullptr)
			return;
		status = SIXDOF_STATE_NONE;
	}

	void ModbusDataAdapter::SetIsEnableAllAxis(bool isEnable)
	{
		if (modbus == nullptr)
			return;
		static uint8_t bits[SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT];
		memset(bits, isEnable == true ? 0 : 1, sizeof(uint8_t) * SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT);
		modbus_write_bits(modbus, SIXDOF_ACTIVE_AXIS_MODBUS_ADDRESS, SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT, bits);
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
