
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
	// 0 ��ָ��
	CONTROL_COMMAND_NONE = 0,
	// 1 ����
	CONTROL_COMMAND_RISING = 1,
	// 2 �½�
	CONTROL_COMMAND_DOWN = 2,
	// 3 ·������
	CONTROL_COMMAND_START_SIGNAL = 3,
	// 4 ��������
	CONTROL_COMMAND_START_SINE = 4,
	// 5 ƽ̨����
	CONTROL_COMMAND_MIDDLE = 5,
	// 6 ƽֹ̨ͣ������
	CONTROL_COMMAND_STOP = 6,
	// 7 ƽ̨��ͣ����
	CONTROL_COMMAND_PAUSE = 7,
	// 8 ƽ̨�ָ�����
	CONTROL_COMMAND_RECOVER = 8
}ControlCommandEnum;

typedef enum 
{
	// 0 ��״̬
	SIXDOF_STATE_NONE = 0,   
	// 1 ��������
	SIXDOF_STATE_RISING = 1,
	// 2 �����λ
	SIXDOF_STATE_DOWN = 2,
	// 3 ����·������
	SIXDOF_STATE_START_SIGNAL = 3,
	// 4 ���������˶�
	SIXDOF_STATE_START_SINE = 4,
	// 5 ƽ̨������λ
	SIXDOF_STATE_MIDDLE = 5,
	// 6 ƽ̨��ͣ
	SIXDOF_STATE_PAUSE = 6,
}SixdofStateEnum;
  
#pragma pack (1)
typedef struct
{
	uint16_t Head;                  // ��У�� 0xAABB  0 1
	uint8_t SixdofState;            // �����ɶ�ƽ̨״̬  2
	uint8_t IsEnableData;           // �Ƿ�����·�� 3
	uint8_t IsTest;                 // �Ƿ��������˶� 4
	uint8_t ControlCommand;         // �����ɶ�ƽ̨����ָ�� 5
	uint16_t UtcTime;               // Utcʱ�� 6 7
	uint16_t PackageCount;          // ����ÿ����һ��+1 8 9
	int32_t X;                      // X��λ�� 0.001mm 10 11 12 13
	int32_t Y;                      // Y��λ�� 0.001mm 14 15 16 17
	int32_t Z;                      // Z��λ�� 0.001mm 18 19 20 21
	int32_t Roll;                   // �����  0.001�� 22 23 24 25
	int32_t Pitch;                  // ������  0.001�� 26 27 28 29 
	int32_t Yaw;                    // ƫ����  0.001�� 30 31 32 33
	int32_t XSpeed;                 // X��λ���ٶ� 0.001mm/s 34 35 36 37
	int32_t YSpeed;                 // Y��λ���ٶ� 0.001mm/s 38 39 40 41
	int32_t ZSpeed;                 // Z��λ�� 0.001mm/s     42 43 44 45
	int32_t RollSpeed;              // ������ٶ�  0.001��/s  46 47 48 49
	int32_t PitchSpeed;             // �������ٶ�  0.001��/s  50 51 52 53
	int32_t YawSpeed;               // ƫ�����ٶ�  0.001��/s
	int32_t XAcc;                   // X��λ�Ƽ��ٶ� 0.001mm/s^2
	int32_t YAcc;                   // Y��λ�Ƽ��ٶ� 0.001mm/s^2
	int32_t ZAcc;                   // Z��λ�Ƽ��ٶ� 0.001mm/s^2
	int32_t RollAcc;                // ����Ǽ��ٶ�  0.001��^2
	int32_t PitchAcc;               // �����Ǽ��ٶ�  0.001��^2
	int32_t YawAcc;                 // ƫ���Ǽ��ٶ�  0.001��^2
	uint16_t RoadDataTotalCount;    // ·�����ݵ�����
	uint16_t RoadDataIndex;         // ·�����ݵ�����
	uint16_t Reseverd1;             // ����WORD 1
	uint16_t Reseverd2;             // ����WORD 2
	uint16_t Tail;                  // βУ�� 0xCCDD
}ComWiwhPLCPackage;
#pragma pack () 

#pragma pack (1)
typedef struct
{
	uint16_t Head;                  // ��У�� 0xAABB  0 1
	uint8_t SixdofState;            // �����ɶ�ƽ̨״̬  2
	uint8_t IsEnableData;           // �Ƿ�����·�� 3
	uint8_t IsTest;                 // �Ƿ��������˶� 4
	uint8_t ControlCommand;         // �����ɶ�ƽ̨����ָ�� 5
	uint16_t UtcTime;               // Utcʱ�� 6 7
	uint16_t PackageCount;          // ����ÿ����һ��+1 8 9
	int32_t X;                      // X��λ�� 0.001mm 10 11 12 13
	int32_t Y;                      // Y��λ�� 0.001mm 14 15 16 17
	int32_t Z;                      // Z��λ�� 0.001mm 18 19 20 21
	int32_t Roll;                   // �����  0.001�� 22 23 24 25
	int32_t Pitch;                  // ������  0.001�� 26 27 28 29 
	int32_t Yaw;                    // ƫ����  0.001�� 30 31 32 33
	uint16_t Reseverd1;             // ����WORD 1
	uint16_t Reseverd2;             // ����WORD 2
	uint16_t Tail;                  // βУ�� 0xCCDD
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

