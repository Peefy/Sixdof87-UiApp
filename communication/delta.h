#pragma once

#ifndef _DELTA_H_
#define _DELTA_H_

#include <stdio.h>
#include "../TYPE_DEF.H"

#include "sixdof.h"

#define AXES_NUM 6

#define ESCtMaxCardNum 32

#if IS_DELTA_MOTION

#define MAX_VEL (DIS_PER_R * RISE_RPM)

#define RISE_VEL 4500000 * 2
#define DOWN_VEL 4500000
#define MIDDLE_VEL 3000000

#define DIS_PER_R 1280000

#define RISE_RPM 15
#define DOWN_RPM 30

#else

#define VEL_SCALE 0.002

#define MAX_VEL (I32)(1280000 * 100 * VEL_SCALE)

#define RISE_VEL (I32)(4500000 * VEL_SCALE)
#define DOWN_VEL (I32)(3000000 * VEL_SCALE)
#define MIDDLE_VEL (I32)(3000000 * VEL_SCALE)

#define DIS_PER_R 2048
//#define DIS_PER_R 8388608

//#define DIS_PER_R 2048

#define RISE_RPM 17.5
#define DOWN_RPM 35

#endif

#define MAX_ACC 0.1

#define STEP_DOWN_RPM 2

#define MB_TABLE_NO 1
#define MB_FIFO_NO 1

#define MAX_PULSE_NUM  (RISE_RPM - 2) * DIS_PER_R

#define DOWN_ERROR_CODE     0x5442
#define DOWN_ERROR_REGISTER 0x20
#define DOWN_ALARM_CODE     0x14

#if IS_DELTA_MOTION

#define PlaneAboveHingeLength       100.0
#define PlaneAboveBottomLength      700.0
#define CircleTopRadius             2047.5//680.0
#define CircleBottomRadius          2933//840.0
#define DistanceBetweenHingeTop     190.0
#define DistanceBetweenHingeBottom  190.0

#else

#define PlaneAboveHingeLength       225.0
#define PlaneAboveBottomLength      2105.0
#define CircleTopRadius             2047.5//880.7
#define CircleBottomRadius          2933//1519.0
#define DistanceBetweenHingeTop     200.0
#define DistanceBetweenHingeBottom  300.0

#endif

#define IS_REVERSE_CONTROL 1

class MotionControl
{
public:
    MotionControl();
    ~MotionControl();
    bool InitCard();
    bool FindSlave();
    bool ServoAllOnOff(bool isOn);
    bool ServoStop();
	bool ServoSingleStop(int index);
	bool ServoSingleMove(int index, int pulse, int isAbs);
	bool ResetAlarmSingle(int index);
    bool ResetAlarm();
    bool ResetStatus();
	void Csp(I32 * pulse);
	void Rise();
 	void RiseWithSingle();
	void Middle();
	void Down();
	void DownUsingHomeMode();
	void StepDown();
	void DisableDDA();
	void EnableDDA();
    void Close();
	void LockServo();
	void MoveToZeroPulseNumber();
	int GetDDACount();
	bool SetDDAData(I32 * pulse);
	I32 GetMotionAveragePulse();
	void RenewNowPulse();
	bool GetLimitSwitch();
	U16 GetBufferLength();
    U16 ESCExistCards;
    U16 ESCCardNoList[ESCtMaxCardNum];
    U16 CardNo;
    U16 SlaveNum;
	U16 DDAGroupNo;
    U16 NodeId[AXES_NUM];
    U16 SlotId[AXES_NUM];
	U16 DioSlotId[AXES_NUM];
	I32 NowPluse[AXES_NUM];
	I32 AvrPulse;
	//Sixdof
	SixDofPlatformStatus Status;
	bool IsAtBottoms[AXES_NUM];
	bool IsAllAtBottom();
	void ReadAllSwitchStatus();
	bool CheckStatus(SixDofPlatformStatus& status);
	bool PowerOnSelfTest(SixDofPlatformStatus laststatus, I32 * lastpulse);
private:
    bool disposed;
	void Control(I32* pulse);
	void SixdofControl(I32* pulse);
};



#endif // !_DELTA_H_

