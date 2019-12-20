
#include "stdafx.h"
#include "modbus_dataapdapter.h"

namespace SixdofModbus
{
	#define BUFFER_MAX_COUNT 1024

	static uint16_t dataBuffers[BUFFER_MAX_COUNT] = {0};
	static uint8_t bitsBuffers[BUFFER_MAX_COUNT] = {0};

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
		memmove(regs + index + 1, b, sizeof(uint16_t) * 1);
		memmove(regs + index, b + 2, sizeof(uint16_t) * 1);
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
		IsConnect = true;
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
		// 9 平台复位
		case CONTROL_COMMAND_RESETPLAT:
			break;
		// 10 开使能
		case CONTROL_COMMAND_ENABLE_ON:
			break;
		// 11 关使能
		case CONTROL_COMMAND_ENABLE_OFF:
			break;
		// 12 跳出上限位
		case CONTROL_COMMAND_OUT_UPPER_LIMIT:
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
		memset(bits, isEnable == true ? 0 : 1, sizeof(uint8_t) * SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT);     //清空一个结构类型的变量或数组	  
/* 读取线圈状态取得一组逻辑线圈的当前状态（ON/OFF)  Reads the boolean status of bits and sets the array elements in the destination to TRUE or FALSE (single bits). */
		modbus_write_bits(modbus, SIXDOF_ACTIVE_AXIS_MODBUS_ADDRESS, SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT, bits);
	}

	ModbusDataAdapter::ModbusDataAdapter()
	{
		DataInit();
		ModbusInit();				//1127
	}

	ModbusDataAdapter::~ModbusDataAdapter()
	{
		if (modbus != nullptr) {
			modbus_close(modbus);
			modbus_free(modbus);
		}
	}

	void ModbusDataAdapter::SetMotorEnable()
	{
		//modbus_write_bits(modbus, )
	}

	int ModbusDataAdapter::SetPlatformEnable(bool isEnable) 
	{
		if (IsConnect == false)
			return -1;
		bitsBuffers[0] = (uint8_t)isEnable;
		auto error = modbus_write_bits(modbus, ENABLE_PLATFORM_MODBUS_ADDRESS, 1, bitsBuffers);
		return error;
	}

	int ModbusDataAdapter::ReadSixdofData(RoadSpectrumData& roaddata) 
	{
		if (IsConnect == false)
			return -1;
		auto error = modbus_read_registers(modbus, ENABLE_PLATFORM_MODBUS_ADDRESS, SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT, dataBuffers);
		roaddata.Position.X = static_cast<double>(dataBuffers[0]);
		roaddata.Position.Y = static_cast<double>(dataBuffers[1]);
		roaddata.Position.Z = static_cast<double>(dataBuffers[2]);
		roaddata.Position.Roll = static_cast<double>(dataBuffers[3]);
		roaddata.Position.Pitch = static_cast<double>(dataBuffers[4]);
		roaddata.Position.Yaw = static_cast<double>(dataBuffers[5]);
		return error;
	}


	int ModbusDataAdapter::WriteResetError()
	{
		if (IsConnect == false)
			return -1;
		bitsBuffers[0] = (uint8_t)1;
		auto error = modbus_write_bits(modbus, RESET_ERROR_MODBUS_ADDRESS, 1, bitsBuffers);
		return error;
	}

	int ModbusDataAdapter::WriteGotoHome()
	{
		return -1;
	}

	int ModbusDataAdapter::WriteSixdofSetData(SixdofData& am, SixdofData& feq, SixdofData& phase)
	{
		if (modbus == nullptr || IsConnect == false)
			return -1;
		int i = 0;
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(am.X * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(am.Y * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(am.Z * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(am.Roll * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(am.Pitch * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(am.Yaw * 1000));

		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(feq.X * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(feq.Y * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(feq.Z * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(feq.Roll * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(feq.Pitch * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(feq.Yaw * 1000));

		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(phase.X * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(phase.Y * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(phase.Z * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(phase.Roll * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(phase.Pitch * 1000));
		MODBUS_SET_INT32_TO_INT16(dataBuffers, (i++) * 2, static_cast<int32_t>(phase.Yaw * 1000));

		modbus_write_registers(modbus, 
			SIXDOF_SET_DATA_MODBUS_ADDRESS, SIXDOF_SET_DATA_MODBUS_REG_COUNT * 2, dataBuffers);
	}

	int ModbusDataAdapter::WriteSixdofSetData(double * am, double * feq, double * phase) 
	{
		return WriteSixdofSetData(SixdofData(am), SixdofData(feq), SixdofData(phase));
	}
}
