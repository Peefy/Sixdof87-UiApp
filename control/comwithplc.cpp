
#include "stdafx.h"
#include "comwithplc.h"

#define SELF_PORT         9000
#define SELF_IP_STRING    "192.168.0.123"

#define PLC_PORT        8000
#define PLC_IP_STRING   "192.168.0.22"

#define PLC_BUFFER_LENGTH 1024

#define PLC_DATA_SCALE 1000.0

static UdpClient udpClient = UdpClient(SELF_PORT);


static inline int16_t PLCExchangeInt16Bit(int16_t data)
{
	return (int16_t)((((uint8_t)data) << 8) + (uint8_t)(data >> 8));
}

static inline int32_t PLCExchangeInt32Bit(int32_t data)
{
	static int8_t b[4] = {0};
	memmove(b, &data, sizeof(int8_t) * 4);
	return (int32_t)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
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
	return package;
}

void PLCDataDoExchange(ComWiwhPLCPackage * data)
{
	PLCExchangeInt16Bit(data->Head);
	PLCExchangeInt16Bit(data->Tail);
	PLCExchangeInt16Bit(data->Time);
	PLCExchangeInt16Bit(data->PackageCount);

	PLCExchangeInt16Bit(data->X);
	PLCExchangeInt16Bit(data->Y);
	PLCExchangeInt16Bit(data->Z);
	PLCExchangeInt16Bit(data->Roll);
	PLCExchangeInt16Bit(data->Pitch);
	PLCExchangeInt16Bit(data->Yaw);

	PLCExchangeInt16Bit(data->XSpeed);
	PLCExchangeInt16Bit(data->YSpeed);
	PLCExchangeInt16Bit(data->ZSpeed);
	PLCExchangeInt16Bit(data->RollSpeed);
	PLCExchangeInt16Bit(data->PitchSpeed);
	PLCExchangeInt16Bit(data->YawSpeed);

	PLCExchangeInt16Bit(data->XAcc);
	PLCExchangeInt16Bit(data->YAcc);
	PLCExchangeInt16Bit(data->ZAcc);
	PLCExchangeInt16Bit(data->RollAcc);
	PLCExchangeInt16Bit(data->PitchAcc);
	PLCExchangeInt16Bit(data->YawAcc);

	PLCExchangeInt16Bit(data->Reseverd1);
	PLCExchangeInt16Bit(data->Reseverd2);
	PLCExchangeInt16Bit(data->Reseverd3);
	PLCExchangeInt16Bit(data->Reseverd4);
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

void PLCDataAdapter::SendData(ControlCommandEnum command, const RoadSpectrumData& roaddata)
{
	static char buffer[PLC_BUFFER_LENGTH];
	static const char * ip = PLCIp.c_str();
	ComWiwhPLCPackage data = PLCPackageNew();

	data.ControlCommand = (uint8_t)command;
	data.X = (int16_t)(roaddata.Position.X / PLC_DATA_SCALE);
	data.Y = (int16_t)(roaddata.Position.Y / PLC_DATA_SCALE);
	data.Z = (int16_t)(roaddata.Position.Z / PLC_DATA_SCALE);
	data.Roll = (int16_t)(roaddata.Position.Roll / PLC_DATA_SCALE);
	data.Pitch = (int16_t)(roaddata.Position.Pitch / PLC_DATA_SCALE);
	data.Yaw = (int16_t)(roaddata.Position.Yaw / PLC_DATA_SCALE);

	data.XSpeed = (int16_t)(roaddata.Speed.X / PLC_DATA_SCALE);
	data.YSpeed = (int16_t)(roaddata.Speed.Y / PLC_DATA_SCALE);
	data.ZSpeed = (int16_t)(roaddata.Speed.Z / PLC_DATA_SCALE);
	data.RollSpeed = (int16_t)(roaddata.Speed.Roll / PLC_DATA_SCALE);
	data.PitchSpeed = (int16_t)(roaddata.Speed.Pitch / PLC_DATA_SCALE);
	data.YawSpeed = (int16_t)(roaddata.Speed.Yaw / PLC_DATA_SCALE);

	data.XAcc = (int16_t)(roaddata.Acc.X / PLC_DATA_SCALE);
	data.YAcc = (int16_t)(roaddata.Acc.Y / PLC_DATA_SCALE);
	data.ZAcc = (int16_t)(roaddata.Acc.Z / PLC_DATA_SCALE);
	data.RollAcc = (int16_t)(roaddata.Acc.Roll / PLC_DATA_SCALE);
	data.PitchAcc = (int16_t)(roaddata.Acc.Pitch / PLC_DATA_SCALE);
	data.YawAcc = (int16_t)(roaddata.Acc.Yaw / PLC_DATA_SCALE);

	PLCDataDoExchange(&data);

	memcpy(buffer, &data, bufferLength);
	udpClient.SendTo(PLCPort, ip, buffer, bufferLength);
}
