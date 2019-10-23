

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
#define IS_PLC_COMM      (1)

#define DATA_SCALE 0.001

#define SIXDOF_SINE_DATA_MODBUS_ADDRESS        (10025 - IS_PLC_COMM)
#define SIXDOF_SINE_DATA_MODBUS_REG_COUNT      (18)

#define ENABLE_PLATFORM_MODBUS_ADDRESS         (1004 - IS_PLC_COMM)

#define RESET_ERROR_MODBUS_ADDRESS             (1011 - IS_PLC_COMM)
#define EMERGENCY_STOP_MODBUS_ADDRESS          (1012 - IS_PLC_COMM)
#define MOVE_TO_INIT_MODBUS_ADDRESS            (1013 - IS_PLC_COMM)
#define MOVE_TO_MIN_PLATFORM_MODBUS_ADDRESS    (1014 - IS_PLC_COMM)
#define GO_TO_HOME_PLATFORM_MODBUS_ADDRESS     (1015 - IS_PLC_COMM)
#define IS_PLAT_HOME_PLATFORM_MODBUS_ADDRESS   (1016 - IS_PLC_COMM)
#define SINE_START_PLATFORM_MODBUS_ADDRESS     (1017 - IS_PLC_COMM)
#define SINE_STOP_PLATFORM_MODBUS_ADDRESS      (1018 - IS_PLC_COMM)
#define SINE_PAUSE_PLATFORM_MODBUS_ADDRESS     (1019 - IS_PLC_COMM)
#define IS_PLAT_INIT_PLATFORM_MODBUS_ADDRESS   (1020 - IS_PLC_COMM)

#define SIXDOF_ACTIVE_AXIS_MODBUS_ADDRESS      (1033 - IS_PLC_COMM)
#define SIXDOF_ACTIVE_AXIS_MODBUS_REG_COUNT    (6)

#define ENABLE_ROAD_DATA_MODBUS_ADDRESS        (1060 - IS_PLC_COMM)
#define SIXDOF_ROAD_DATA_MODBUS_ADDRESS        (10101 - IS_PLC_COMM)
#define SIXDOF_ROAD_DATA_MODBUS_REG_COUNT      (36)

class ModbusDataAdapter
{
public:
	ModbusDataAdapter();
	~ModbusDataAdapter();
	void ExchangeData(ControlCommandEnum& command, SixdofStateEnum& status, RoadSpectrumData& roaddata);
	bool IsConnect;
protected:
	modbus_t* modbus;
private:
	void DataInit();	
	void ModbusInit();
protected:
	void SetIsEnableRoadData(bool isEnable);
	void SetRoadData(RoadSpectrumData& roaddata);
	void SetPlatformStatus(ControlCommandEnum& command);
	void GetPlatformStatue(SixdofStateEnum& status);
	void SetIsEnableAllAxis(bool isEnable);
};

}


#endif

