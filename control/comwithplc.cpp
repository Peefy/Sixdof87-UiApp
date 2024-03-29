
#include "stdafx.h"
#include "comwithplc.h"

#define SELF_PORT         9000
//#define SELF_IP_STRING    "127.0.0.1"        // "192.168.0.123"  
#define SELF_IP_STRING       "192.168.0.123"  

#define PLC_PORT        8000
#define PLC_IP_STRING        "192.168.0.22"

#define PLC_BUFFER_LENGTH 1024

static UdpClient udpClient = UdpClient(SELF_PORT);


static inline int16_t PLCExchangeInt16Bit(int16_t data)
{
	return (int16_t)((((uint8_t)data) << 8) + (uint8_t)(data >> 8));
}

static inline uint16_t PLCExchangeUInt16Bit(uint16_t data)
{
	return (uint16_t)((((uint8_t)data) << 8) + (uint8_t)(data >> 8));
}

static inline int32_t PLCExchangeInt32Bit(int32_t data)
{
	static int8_t b[4] = {0};
	memmove(b, &data, sizeof(int8_t) * 4);
	return (int32_t)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
}

static inline uint32_t PLCExchangeUInt32Bit(uint32_t data)
{
	static int8_t b[4] = {0};
	memmove(b, &data, sizeof(int8_t) * 4);
	return (uint32_t)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
}

static void PLCPackageInit(ComWiwhPLCPackage * data)
{
	data->Head = PLC_DATA_HEAD;
	data->Tail = PLC_DATA_TAIL;
}

static ComWiwhPLCPackage PLCPackageNew()
{
	static int count = 0;
	ComWiwhPLCPackage package;
	package.Head = PLC_DATA_HEAD;
	package.Tail = PLC_DATA_TAIL;
	package.PackageCount = count++;
	package.Reseverd1 = 0;
	package.Reseverd2 = 0;
	package.SixdofState = 1;
	package.IsTest = 0;
	package.UtcTime = 0;
	package.IsEnableData = 1;
	package.RoadDataIndex = 0;
	package.RoadDataTotalCount = 0;
	return package;
}

void PLCDataDoExchange(ComWiwhPLCPackage * data)
{
	data->Head = PLCExchangeUInt16Bit(data->Head);
	data->Tail = PLCExchangeUInt16Bit(data->Tail);
	data->UtcTime = PLCExchangeUInt16Bit(data->UtcTime);
	data->PackageCount = PLCExchangeInt16Bit(data->PackageCount);

	data->X = PLCExchangeInt32Bit(data->X);
	data->Y = PLCExchangeInt32Bit(data->Y);
	data->Z = PLCExchangeInt32Bit(data->Z);
	data->Roll = PLCExchangeInt32Bit(data->Roll);
	data->Pitch = PLCExchangeInt32Bit(data->Pitch);
	data->Yaw = PLCExchangeInt32Bit(data->Yaw);

	data->XSpeed = PLCExchangeInt32Bit(data->XSpeed);
	data->YSpeed = PLCExchangeInt32Bit(data->YSpeed);
	data->ZSpeed = PLCExchangeInt32Bit(data->ZSpeed);
	data->RollSpeed = PLCExchangeInt32Bit(data->RollSpeed);
	data->PitchSpeed = PLCExchangeInt32Bit(data->PitchSpeed);
	data->YawSpeed = PLCExchangeInt32Bit(data->YawSpeed);

	data->XAcc = PLCExchangeInt32Bit(data->XAcc);
	data->YAcc = PLCExchangeInt32Bit(data->YAcc);
	data->ZAcc = PLCExchangeInt32Bit(data->ZAcc);
	data->RollAcc = PLCExchangeInt32Bit(data->RollAcc);
	data->PitchAcc = PLCExchangeInt32Bit(data->PitchAcc);
	data->YawAcc = PLCExchangeInt32Bit(data->YawAcc);

	data->RoadDataTotalCount = PLCExchangeUInt16Bit(data->RoadDataTotalCount);
	data->RoadDataIndex = PLCExchangeUInt16Bit(data->RoadDataIndex);

	data->Reseverd1 = PLCExchangeUInt16Bit(data->Reseverd1);
	data->Reseverd2 = PLCExchangeUInt16Bit(data->Reseverd2);
}

void PLCDataAdapter::DataInit()
{
	bufferLength = sizeof(ComWiwhPLCPackage);
	SelfPort = SELF_PORT;
	PLCPort = PLC_PORT;
	SelfIp = SELF_IP_STRING;
	PLCIp = PLC_IP_STRING;
	udpClient.SetPortAndBind(SelfPort);
}

PLCDataAdapter::PLCDataAdapter()
{
	DataInit();
}

PLCDataAdapter::~PLCDataAdapter()
{
}

void PLCDataAdapter::SendData(ControlCommandEnum command, const RoadSpectrumData& roaddata, int dataCount, int dataIndex)
{
	static char buffer[PLC_BUFFER_LENGTH];
	static const char * ip = PLCIp.c_str();
	ComWiwhPLCPackage data = PLCPackageNew();

	data.ControlCommand = (uint8_t)command;
	data.UtcTime = GetTickCount();
	data.RoadDataTotalCount = dataCount;
	data.RoadDataIndex = dataIndex;
	data.X = (int32_t)(roaddata.Position.X / PLC_DATA_SCALE);
	data.Y = (int32_t)(roaddata.Position.Y / PLC_DATA_SCALE);
	data.Z = (int32_t)(roaddata.Position.Z / PLC_DATA_SCALE);
	data.Roll = (int32_t)(roaddata.Position.Roll / PLC_DATA_SCALE);
	data.Pitch = (int32_t)(roaddata.Position.Pitch / PLC_DATA_SCALE);
	data.Yaw = (int32_t)(roaddata.Position.Yaw / PLC_DATA_SCALE);

	data.XSpeed = (int32_t)(roaddata.Speed.X / PLC_DATA_SCALE);
	data.YSpeed = (int32_t)(roaddata.Speed.Y / PLC_DATA_SCALE);
	data.ZSpeed = (int32_t)(roaddata.Speed.Z / PLC_DATA_SCALE);
	data.RollSpeed = (int32_t)(roaddata.Speed.Roll / PLC_DATA_SCALE);
	data.PitchSpeed = (int32_t)(roaddata.Speed.Pitch / PLC_DATA_SCALE);
	data.YawSpeed = (int32_t)(roaddata.Speed.Yaw / PLC_DATA_SCALE);

	data.XAcc = (int32_t)(roaddata.Acc.X / PLC_DATA_SCALE);
	data.YAcc = (int32_t)(roaddata.Acc.Y / PLC_DATA_SCALE);
	data.ZAcc = (int32_t)(roaddata.Acc.Z / PLC_DATA_SCALE);
	data.RollAcc = (int32_t)(roaddata.Acc.Roll / PLC_DATA_SCALE);
	data.PitchAcc = (int32_t)(roaddata.Acc.Pitch / PLC_DATA_SCALE);
	data.YawAcc = (int32_t)(roaddata.Acc.Yaw / PLC_DATA_SCALE);

#if IS_PLC_DATA_BIG_ENDIAH
	PLCDataDoExchange(&data);
#endif

	memcpy(buffer, &data, bufferLength);
	udpClient.SendTo(PLCPort, ip, buffer, bufferLength);
}
