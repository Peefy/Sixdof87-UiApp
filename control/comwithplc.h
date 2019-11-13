
#ifndef __COM_WITH_PLC_H_
#define __COM_WITH_PLC_H_

#include <stdint.h>
#include <memory>

#include "udp.h"

#include "../signal/roadspectrum.h"

using namespace std;
using namespace Signal;

#define PLC_DATA_HEAD  0xAABB
#define PLC_DATA_TAIL  0xCCDD

#define PLC_DATA_SCALE 0.001

#define IS_PLC_DATA_BIG_ENDIAH 0

typedef enum 
{
	// 0 空指令
	CONTROL_COMMAND_NONE = 0,
	// 1 上升
	CONTROL_COMMAND_RISING = 1,
	// 2 下降
	CONTROL_COMMAND_DOWN = 2,
	// 3 路谱运行
	CONTROL_COMMAND_START_SIGNAL = 3,
	// 4 正弦运行
	CONTROL_COMMAND_START_SINE = 4,
	// 5 平台回中
	CONTROL_COMMAND_MIDDLE = 5,
	// 6 平台停止并回中
	CONTROL_COMMAND_STOP = 6,
	// 7 平台暂停运行
	CONTROL_COMMAND_PAUSE = 7,
	// 8 平台恢复运行
	CONTROL_COMMAND_RECOVER = 8
}ControlCommandEnum;

typedef enum 
{
	// 0 空状态
	SIXDOF_STATE_NONE = 0,   
	// 1 正在上升
	SIXDOF_STATE_RISING = 1,
	// 2 在最低位
	SIXDOF_STATE_DOWN = 2,
	// 3 正在路谱运行
	SIXDOF_STATE_START_SIGNAL = 3,
	// 4 正在正弦运动
	SIXDOF_STATE_START_SINE = 4,
	// 5 平台在中立位
	SIXDOF_STATE_MIDDLE = 5,
	// 6 平台暂停
	SIXDOF_STATE_PAUSE = 6,
}SixdofStateEnum;
  
#pragma pack (1)
typedef struct
{
	uint16_t Head;                  // 首校验 0xAABB  0 1
	uint8_t SixdofState;            // 六自由度平台状态  2
	uint8_t IsEnableData;           // 是否运行路谱 3
	uint8_t IsTest;                 // 是否是正弦运动 4
	uint8_t ControlCommand;         // 六自由度平台控制指令 5
	uint16_t UtcTime;               // Utc时间 6 7
	uint16_t PackageCount;          // 包序，每发送一次+1 8 9
	int32_t X;                      // X线位移 0.001mm 10 11 12 13
	int32_t Y;                      // Y线位移 0.001mm 14 15 16 17
	int32_t Z;                      // Z线位移 0.001mm 18 19 20 21
	int32_t Roll;                   // 横滚角  0.001度 22 23 24 25
	int32_t Pitch;                  // 俯仰角  0.001度 26 27 28 29 
	int32_t Yaw;                    // 偏航角  0.001度 30 31 32 33
	int32_t XSpeed;                 // X线位移速度 0.001mm/s 34 35 36 37
	int32_t YSpeed;                 // Y线位移速度 0.001mm/s 38 39 40 41
	int32_t ZSpeed;                 // Z线位移 0.001mm/s     42 43 44 45
	int32_t RollSpeed;              // 横滚角速度  0.001度/s  46 47 48 49
	int32_t PitchSpeed;             // 俯仰角速度  0.001度/s  50 51 52 53
	int32_t YawSpeed;               // 偏航角速度  0.001度/s
	int32_t XAcc;                   // X线位移加速度 0.001mm/s^2
	int32_t YAcc;                   // Y线位移加速度 0.001mm/s^2
	int32_t ZAcc;                   // Z线位移加速度 0.001mm/s^2
	int32_t RollAcc;                // 横滚角加速度  0.001度^2
	int32_t PitchAcc;               // 俯仰角加速度  0.001度^2
	int32_t YawAcc;                 // 偏航角加速度  0.001度^2
	uint16_t RoadDataTotalCount;    // 路谱数据点总数
	uint16_t RoadDataIndex;         // 路谱数据点索引
	uint16_t Reseverd1;             // 保留WORD 1
	uint16_t Reseverd2;             // 保留WORD 2
	uint16_t Tail;                  // 尾校验 0xCCDD
}ComWiwhPLCPackage;
#pragma pack () 

#pragma pack (1)
typedef struct
{
	uint16_t Head;                  // 首校验 0xAABB  0 1
	uint8_t SixdofState;            // 六自由度平台状态  2
	uint8_t IsEnableData;           // 是否运行路谱 3
	uint8_t IsTest;                 // 是否是正弦运动 4
	uint8_t ControlCommand;         // 六自由度平台控制指令 5
	uint16_t UtcTime;               // Utc时间 6 7
	uint16_t PackageCount;          // 包序，每发送一次+1 8 9
	int32_t X;                      // X线位移 0.001mm 10 11 12 13
	int32_t Y;                      // Y线位移 0.001mm 14 15 16 17
	int32_t Z;                      // Z线位移 0.001mm 18 19 20 21
	int32_t Roll;                   // 横滚角  0.001度 22 23 24 25
	int32_t Pitch;                  // 俯仰角  0.001度 26 27 28 29 
	int32_t Yaw;                    // 偏航角  0.001度 30 31 32 33
	uint16_t Reseverd1;             // 保留WORD 1
	uint16_t Reseverd2;             // 保留WORD 2
	uint16_t Tail;                  // 尾校验 0xCCDD
}ComPackageFromPLC;
#pragma pack () 

class PLCDataAdapter
{
public:
	PLCDataAdapter();
	~PLCDataAdapter();
	void SendData(ControlCommandEnum control, const RoadSpectrumData& roaddata, int dataCount, int dataIndex);
	void RecieveData(RoadSpectrumData* road);
private:
	int bufferLength;
protected:
	void DataInit();
	int SelfPort;
	string SelfIp;
	int PLCPort;
	string PLCIp;
};



#endif // !__COM_WITH_PLC_H_

