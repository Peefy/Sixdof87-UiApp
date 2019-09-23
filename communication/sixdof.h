
#ifndef _SIXDOF_H_
#define _SIXDOF_H_

#include <Windows.h> 

#include "../config/inihelper.h"
#include "../TYPE_DEF.H"

#define DLL_NAME "Sixdofdll2010.dll"
#define FUNC_NAME "Control"
#define FUNC_NAGIN  6
#define FUNC_NAGOUT 6

//�׵�����
#define AXES_COUNT 6

#define IS_DELTA_MOTION 0

#if IS_DELTA_MOTION

//ƽ̨�˶����Ƕ�
#define VISION_MAX_DEG 8.0
#define MAX_DEG 10.5
#define DEG_SCALE 0.01
//ƽ̨�˶����λ��
#define VISION_MAX_XYZ 95.0
#define MAX_XYZ 100
#define XYZ_SCALE 0.1
// ƽ̨�˶����Ƶ��
#define MAX_HZ 20
#define MAX_PHASE 360

#define MAX_XYZ_ZERO_POS    30
#define MAX_DEG_ZERO_POS    3

#define MAX_XYZ_X      600
#define MAX_XYZ_Y      550
#define MAX_XYZ_Z      300
#define MAX_DEG_PITCH  25
#define MAX_DEG_ROLL   25
#define MAX_DEG_YAW    28

//�����˿�ܵĵ��� һȦ�����6mm, ������쳤��Ϊ700mm
#define MAX_POLE_LENGTH 700 / 2.0
#define PULSE_PER_R 1280000
#define MM_PER_R 6.00
#define LENGTH_TO_PULSE_SCALE 213333.333333333

#else

//ƽ̨�˶����Ƕ�
#define MAX_DEG 28.0
#define DEG_SCALE 0.01
//ƽ̨�˶����λ��
#define MAX_XYZ 600.0
#define XYZ_SCALE 0.1
//�Ӿ�ƽ̨�˶����Ƕ�
#define VISION_MAX_DEG 28.0
//�Ӿ�ƽ̨�˶����λ��
#define VISION_MAX_XYZ 600.0
// ƽ̨�˶����Ƶ��
#define MAX_HZ 25
#define MAX_PHASE 360

#define MAX_XYZ_ZERO_POS    25.0
#define MAX_DEG_ZERO_POS    600.0

#define MAX_XYZ_X      600
#define MAX_XYZ_Y      550
#define MAX_XYZ_Z      300
#define MAX_DEG_PITCH  25
#define MAX_DEG_ROLL   25
#define MAX_DEG_YAW    28

#define MAX_XYZ_SPEED  0.8  //��λm/s
#define MAX_XYZ_ACC    9.8  //��λm/s^2

#define MAX_DEG_SPEED  25   //��λ��/s
#define MAX_DEG_ACC    200  //��λ��/s^2

//�����˿�ܵĵ��� һȦ�����6mm, ������쳤��Ϊ700mm
#define MAX_POLE_LENGTH 700 / 2.0
#define PULSE_PER_R 2048
#define MM_PER_R 20.00
#define LENGTH_TO_PULSE_SCALE 102.4

#endif

#define FREEDOM_X_INDEX     0
#define FREEDOM_Y_INDEX     1
#define FREEDOM_Z_INDEX     2
#define FREEDOM_ROLL_INDEX  3
#define FREEDOM_PITCH_INDEX 4
#define FREEDOM_YAW_INDEX   5

//��λ����
#define RANGE(x,min,max)   (((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))

typedef enum
{
	SIXDOF_STATUS_BOTTOM,
	SIXDOF_STATUS_READY,
	SIXDOF_STATUS_MIDDLE,
	SIXDOF_STATUS_RUN,
	SIXDOF_STATUS_ISRISING,
	SIXDOF_STATUS_ISFALLING,
}SixDofPlatformStatus;

extern char * SixDofStatusText[];

typedef struct 
{
	//��λ1mm
	double X;
	//��λ1mm
	double Y;
	//��λ1mm
	double Z;
	//��λ1��
	double Yaw;
	//��λ1��
	double Roll;
	//��λ1��
	double Pitch;
}SixdofPackage;

class SixDof
{
public:
	SixDof();
	~SixDof();
	bool IsRegister;
	void GetSixServoPulseNum(I32 * pulse_arr, double * data);
	void GetSixServoPulseNum(double * pulse_arr, double * data);
	void GetSixServoPulseNum(double * pulse_arr, SixdofPackage * data);
	double X;
	double Y;
	double Z;
	double Roll;
	double Yaw;
	double Pitch;
	double PoleLength[AXES_COUNT];
private:
	void DataInit();
	void DllInit();
	double* DuGuControl(double x, double y, double z, double roll, double pitch, double yaw);
};



#endif

