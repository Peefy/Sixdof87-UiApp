
#include "StdAfx.h"
#include "ECATSample.h"
#include "ECATSampleDlg.h"

#include <math.h>
#include <time.h>
#include <fstream>
#include <deque>
#include <queue>

#include "DialogRegister.h"
#include "DIALOGLogin.h"
#include "DIALOGGenerate.h"
#include "DIALOGProcessing.h"
#include "DIALOGReproduce.h"

#include "EtherCAT_DLL.h"
#include "EtherCAT_DLL_Err.h"

#include "chart.h"

#include "communication/sixdof.h"
#include "communication/communication.h"
#include "communication/delta.h"
#include "communication/SerialPort.h"

#include "control/sensor.h"
#include "control/pid.h"
#include "control/kalman_filter.h"
#include "control/illusion.h"
#include "control/comwithplc.h"
#include "control/modbus_dataapdapter.h"

#include "config/recordpath.h"

#include "ui/uiconfig.h"

#include "util/mytime.h"
#include "util/model.h"

#include "signal/roadspectrum.h"

#include "register/register.h"

#include "Sixdofdll2010.h"

#include "glut.h"
#include "opengl/sixdofopenglhelper.h"

#include "cwnds/panel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace SixdofModbus;

#define COLOR_RED     RGB(255, 123, 123)
#define COLOR_GREEN   RGB(123, 255, 123)

#define TIMER_MS 10

#define SIXDOF_CONTROL_DELEY     10
#define SCENE_THREAD_DELAY       1000
#define SENSOR_THREAD_DELAY      1000
#define DATA_BUFFER_THREAD_DELAY 1000

#define DDA_UP_COUNT    400
#define DDA_ONCE_COUNT  100

#define CHIRP_TIME    2.5
#define ZERO_POS_TIME    2.5
#define ENABLE_CHIRP  true
#define ENABLE_SHOCK  false
#define ENABLE_ZERO_POS true

#define PATH_DATA_USE_DDA 0
#define IS_USE_MESSAGEBOX 1

#define MAX_REPRODUCE_LINE 1000
#define DATA_COL_NUM       18

bool enableShock = ENABLE_SHOCK;
bool enableChirp = ENABLE_CHIRP;
bool enableZeroPos = ENABLE_ZERO_POS;
bool stopSCurve = false;

void SixdofControl();
void SensorRead();

U8 InitialFlag = 0;

bool closeDataThread = true;
bool stopAndMiddle = true;

volatile HANDLE DataThread;
volatile HANDLE SensorThread;
volatile HANDLE SceneThread;
volatile HANDLE DataBufferThread;

I32 pulse_test[AXES_COUNT];
I32 lastStartPulse[AXES_COUNT];
double pulse_cal[AXES_COUNT];
double poleLength[AXES_COUNT];
int poleLengthRenderCount = 0;

DataPackage data;

SixDofPlatformStatus status = SIXDOF_STATUS_BOTTOM;
SixDofPlatformStatus lastStartStatus = SIXDOF_STATUS_BOTTOM;

#if PATH_DATA_USE_DDA

MovingAverageFilterType rollFiter = {2, 1};
MovingAverageFilterType yawFiter = {2, 1};
MovingAverageFilterType pitchFiter = {2, 1};

MovingAverageFilterType xFiter = {2, 1};
MovingAverageFilterType yFiter = {2, 1};
MovingAverageFilterType zFiter = {2, 1};
double ShockVal = 0.1;
double ShockHz = 8.0;

#else

MovingAverageFilterType rollFiter = {2, 1};
MovingAverageFilterType yawFiter = {2, 1};
MovingAverageFilterType pitchFiter = {2, 1};

MovingAverageFilterType xFiter = {2, 1};
MovingAverageFilterType yFiter = {2, 1};
MovingAverageFilterType zFiter = {2, 1};
double ShockVal = 1.0;
double ShockHz = 188.0;

double hpfAccWn = 1.0;
double lpfAccWn = 0.5; 
double hpfAngleSpdWn = 0.2;

#endif

CChartCtrl m_ChartCtrl1; 
CChartLineSerie *pLineSerie1;
CChartLineSerie *pLineSerie2;
CChartLineSerie *pLineSerie3;
CChartLineSerie *pLineSerie4;
CChartLineSerie *pLineSerie5;
CChartLineSerie *pLineSerie6;

double sin_time_pulse = 0;
double sin_time_pulse_delta = 0.01;

bool isStart = false;
bool isAutoInit = true;
bool isTest = true;
bool isPlayData = false;
bool isCosMode = false;

double testVal[FREEDOM_NUM] = {0};
double testHz[FREEDOM_NUM] = {0};
double testPhase[FREEDOM_NUM] = {0};
double testZeroPos[FREEDOM_NUM] = {0};

double chartBottomAxisPoint[CHART_POINT_NUM] = { 0 };

double chartXValPoint[CHART_POINT_NUM] = { 0 };
double chartYValPoint[CHART_POINT_NUM] = { 0 };
double chartZValPoint[CHART_POINT_NUM] = { 0 };
double chartRollValPoint[CHART_POINT_NUM] = { 0 };
double chartPitchValPoint[CHART_POINT_NUM] = { 0 };
double chartYawValPoint[CHART_POINT_NUM] = { 0 };

int runTime = 0;
double chartTime = 0;
double controltime = 0;
double dataChartTime = 0;

double t = 0;

char fileName[100] = "";

bool isCsp = false;
U16 Counter = 0;

int visionCtrlComand = 0;

// 线程锁
CRITICAL_SECTION cs;
CRITICAL_SECTION ctrlCommandLockobj;

// 路谱数据
Signal::RoadSpectrumData roaddata;
// 路谱数据序列
Signal::RoadSpectrum roadSpectrum;
// 控制PLC,注意PLC地址+1
PLCDataAdapter plcData;
//SixdofModbus::ModbusDataAdapter modbusdata;
// 控制PLC六自由度平台的控制指令
ControlCommandEnum sendStatus = CONTROL_COMMAND_NONE;
// 读取PLC六自由度平台的状态
SixdofStateEnum sixdofState = SIXDOF_STATE_NONE;

DWORD WINAPI DataTransThread(LPVOID pParam)
{
	while (true)
	{
		DWORD start_time = 0;
		start_time = GetTickCount();

		SixdofControl();
		plcData.SendData(sendStatus, roaddata);
		//modbusdata.ExchangeData(sendStatus, sixdofState, roaddata);

		DWORD end_time = GetTickCount();
		runTime = end_time - start_time;		
	}
	return 0;
}

DWORD WINAPI SensorInfoThread(LPVOID pParam)
{
	while (true)
	{
		Sleep(SENSOR_THREAD_DELAY);
	}
	return 0;
}

void CECATSampleDlg::JudgeControlCommand()
{
	EnterCriticalSection(&ctrlCommandLockobj);
	switch (visionCtrlComand)
	{
	case ILLUSION_CTL_CMD_START_INT32:
		// Rise
		if (status == SIXDOF_STATUS_BOTTOM || status == SIXDOF_STATUS_ISFALLING)
		{
			OnBnClickedBtnRise();
		}
		else if (status == SIXDOF_STATUS_MIDDLE || status == SIXDOF_STATUS_RUN)
		{
			OnCommandStopme();
		}
		break;
	case ILLUSION_CTL_CMD_STOP_INT32:
		// Down
		OnBnClickedBtnDown();
		break;
	case ILLUSION_CTL_CMD_CONNECT_INT32:
		// Run
		OnBnClickedBtnStart();
		break;
	case ILLUSION_CTL_CMD_DISCONNECT_INT32:
		// StopAndMiddle
		OnCommandStopme();
		break;
	case ILLUSION_CTL_CMD_PAUSE_INT32:
		// StopAndMiddle
		OnCommandStopme();
		break;
	case ILLUSION_CTL_CMD_RECOVER_INT32:
		// Run
		OnBnClickedBtnStart();
		break;
	default:
		break;
	}
	visionCtrlComand = 0;
	LeaveCriticalSection(&ctrlCommandLockobj);
}

void VisionDataDeal()
{	

}

DWORD WINAPI SceneInfoThread(LPVOID pParam)
{
	while (true)
	{
		Sleep(SCENE_THREAD_DELAY);
	}
	return 0;
}

DWORD WINAPI DataBufferInfoThread(LPVOID pParam)
{
	while (true)
	{
		// modbus poll
		Sleep(DATA_BUFFER_THREAD_DELAY);
	}
	return 0;
}

void CloseThread()
{
	try
	{
		if (DataThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(DataThread);
			DataThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{
		printf("%d\r\n", e->ReportError());
	}
	try
	{
		if (SensorThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(SensorThread);
			SensorThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{
		printf("%d\r\n", e->ReportError());
	}
	try
	{
		if (SceneThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(SceneThread);
			SceneThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{
		printf("%d\r\n", e->ReportError());
	}
	try
	{
		if (DataBufferThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(DataBufferThread);
			DataBufferThread = INVALID_HANDLE_VALUE;
		}
	}
	catch (CException* e)
	{
		printf("%d\r\n", e->ReportError());
	}
	//DeleteCriticalSection(&cs);
	//DeleteCriticalSection(&ctrlCommandLockobj);
}

void OpenThread()
{
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&ctrlCommandLockobj);
	DataThread = (HANDLE)CreateThread(NULL, 0, DataTransThread, NULL, 0, NULL);
	//SensorThread = (HANDLE)CreateThread(NULL, 0, SensorInfoThread, NULL, 0, NULL);
	//SceneThread = (HANDLE)CreateThread(NULL, 0, SceneInfoThread, NULL, 0, NULL);
	//DataBufferThread = (HANDLE)CreateThread(NULL, 0, DataBufferInfoThread, NULL, 0, NULL);
}

void SensorRead()
{
	Sleep(SENSOR_THREAD_DELAY);
}

double vision_x = 0;
double vision_y = 0;
double vision_z = 0;
double vision_roll = 0;
double vision_pitch = 0;
double vision_yaw = 0;

void SixdofControl()
{
	Sleep(SIXDOF_CONTROL_DELEY);
	if (closeDataThread == false)
	{	
		U16 upCount = DDA_UP_COUNT;

		if (isCsp == false)
		{
			Counter = 0;
			if (Counter < upCount)
			{
				I32 dis[AXES_COUNT] = {DIS_PER_R, DIS_PER_R, DIS_PER_R, DIS_PER_R, DIS_PER_R, DIS_PER_R};
				if (true)
				{
					for (auto i = 0; i < 1; ++i)
					{
						// 正弦测试运动
						if (isTest == true)
						{
							double nowHz[AXES_COUNT] = {testHz[0], testHz[1], testHz[2], testHz[3], testHz[4], testHz[5]};
							double chirpTime = CHIRP_TIME;
							double zeroposTime = ZERO_POS_TIME;
							double pi = 3.1415926;
							double nowt = t;
							double x = 0;
							double y = 0;
							double z = 0;
							double roll = 0;
							double pitch = 0;
							double yaw = 0;
							double stopTime = 0;
							double deltat = 0.02;
							if (t <= chirpTime && enableChirp == true)
							{
								for (auto i = 0; i < AXES_COUNT; ++i)
								{
									nowHz[i] = t * testHz[i] / chirpTime / chirpTime; 
								}
								x = sin(2 * pi * nowHz[0] * nowt) * testVal[0];
								y = sin(2 * pi * nowHz[1] * nowt) * testVal[1];
								z = sin(2 * pi * nowHz[2] * nowt) * testVal[2];
								roll = sin(2 * pi * nowHz[3] * nowt) * testVal[3];
								pitch = sin(2 * pi * nowHz[4] * nowt) * testVal[4];
								yaw = sin(2 * pi * nowHz[5] * nowt) * testVal[5];
							}		
							else if (enableChirp == true)
							{
								nowt = t - chirpTime;
								x = sin(2 * pi * nowHz[0] * nowt + 2 * pi * testHz[0]) * testVal[0];
								y = sin(2 * pi * nowHz[1] * nowt + 2 * pi * testHz[1]) * testVal[1];
								z = sin(2 * pi * nowHz[2] * nowt + 2 * pi * testHz[2]) * testVal[2];
								roll = sin(2 * pi * nowHz[3] * nowt + 2 * pi * testHz[3]) * testVal[3];
								pitch = sin(2 * pi * nowHz[4] * nowt + 2 * pi * testHz[4]) * testVal[4];
								yaw = sin(2 * pi * nowHz[5] * nowt + 2 * pi * testHz[5]) * testVal[5];
							}
							else if (isCosMode == false)
							{
								x = sin(2 * pi * nowHz[0] * nowt) * testVal[0];
								y = sin(2 * pi * nowHz[1] * nowt) * testVal[1];
								z = sin(2 * pi * nowHz[2] * nowt) * testVal[2];
								roll = sin(2 * pi * nowHz[3] * nowt) * testVal[3];
								pitch = sin(2 * pi * nowHz[4] * nowt) * testVal[4];
								yaw = sin(2 * pi * nowHz[5] * nowt) * testVal[5];
							}
							else
							{

							}
							if (isCosMode == true)
							{
								double nowphase_t[AXES_COUNT] = {t, t, t, t, t, t};
								for (int i = 0;i < AXES_COUNT; ++i)
								{
									if (nowHz[i] != 0)
									{
										auto phase_t = t - 1.0 / nowHz[i] * (testPhase[i] / 360.0);
										nowphase_t[i] = DOWN_RANGE(phase_t, 0);
									}			
									else
									{
										nowphase_t[i] = t;
									}					
								}
								x = sin(2 * pi * nowHz[0] * nowphase_t[0]) * testVal[0];
								y = sin(2 * pi * nowHz[1] * nowphase_t[1]) * testVal[1];
								z = sin(2 * pi * nowHz[2] * nowphase_t[2]) * testVal[2];
								roll = sin(2 * pi * nowHz[3] * nowphase_t[3]) * testVal[3];
								pitch = sin(2 * pi * nowHz[4] * nowphase_t[4]) * testVal[4];
								yaw = sin(2 * pi * nowHz[5] * nowphase_t[5]) * testVal[5];
							}
							if (enableZeroPos == true)
							{
								auto minZeroPosTime = min(t, zeroposTime) / zeroposTime;
								x += testZeroPos[0] * minZeroPosTime;
								y += testZeroPos[1] * minZeroPosTime;
								z += testZeroPos[2] * minZeroPosTime;
								roll += testZeroPos[3] * minZeroPosTime;
								pitch += testZeroPos[4] * minZeroPosTime;
								yaw += testZeroPos[5] * minZeroPosTime;
							}
							data.X = (int16_t)(x * 10);
							data.Y = (int16_t)(y * 10);
							data.Z = (int16_t)(z * 10);
							data.Roll = (int16_t)(roll * 100);
							data.Pitch = (int16_t)(pitch * 100);
							data.Yaw = (int16_t)(yaw * 100);
							roaddata.SetPositions(x, y, z, yaw, roll, pitch);
							double* pulse_dugu = Control(x, y, z, roll, yaw, pitch);
							for (auto ii = 0; ii < AXES_COUNT; ++ii)
							{
								poleLength[ii] = pulse_dugu[ii];
								pulse_cal[ii] = pulse_dugu[ii];
								pulse_cal[ii] *= LENGTH_TO_PULSE_SCALE;
								auto pulse = pulse_cal[ii];
								dis[ii] = (int)pulse;
							}
							t += deltat;
							if (stopSCurve == true && isCosMode == false)
							{
								int index = 0;
								auto maxHz = util::MaxValue(testHz, AXES_COUNT);
								for (int i = 0;i < 100; ++i)
								{
									if (pi * i + 0.5 * pi >= nowt * 2 * pi * maxHz)
									{
										index = i;
										break;
									}
								}
								stopTime = (pi * index + 0.5 * pi) / (2 * pi * maxHz) + chirpTime - 1;
								stopSCurve = false;
							}
							if (stopTime == 0 || nowt <= stopTime) 
							{
								//delta.SetDDAData(dis);
							}	
							else if (nowt > stopTime)
							{
								t -= deltat;
							}
						}
						// 路谱
						else
						{
							EnterCriticalSection(&cs);
							if (roadSpectrum.DataBuffer.size() > 0) {
								auto datatmp = roadSpectrum.DataBuffer.front();
								roadSpectrum.DataBuffer.pop_front();
								vision_x = datatmp.Position.X;
								vision_y = datatmp.Position.Y;
								vision_z = datatmp.Position.Z;
								vision_roll = datatmp.Position.Roll;
								vision_pitch = datatmp.Position.Pitch;
								vision_yaw = datatmp.Position.Yaw;
								roaddata.Speed = datatmp.Speed;
								roaddata.Acc = datatmp.Acc;
							}
							else {			
								if (status != SIXDOF_STATUS_RUN) {
									sin_time_pulse = 0;
									t = 0;
									dataChartTime = 0;
									closeDataThread = true;
									isStart = false;	
								}
							}
							LeaveCriticalSection(&cs);
							double pi = 3.1415926;
							double shockVal = ShockVal;
							double shockHz = ShockHz;
							auto shockz = sin(2 * pi * shockHz * t) * shockVal;
							auto x = RANGE(MyMAFilter(&xFiter, vision_x), -VISION_MAX_XYZ, VISION_MAX_XYZ);
							auto y = RANGE(MyMAFilter(&yFiter, vision_y), -VISION_MAX_XYZ, VISION_MAX_XYZ);
							auto z = RANGE(MyMAFilter(&zFiter, vision_z), -VISION_MAX_XYZ, VISION_MAX_XYZ);
							auto roll = RANGE(MyMAFilter(&rollFiter, vision_roll), -VISION_MAX_DEG, VISION_MAX_DEG);
							auto pitch = RANGE(MyMAFilter(&pitchFiter, vision_pitch), -VISION_MAX_DEG, VISION_MAX_DEG);
							auto yaw = RANGE(MyMAFilter(&yawFiter, vision_yaw), -VISION_MAX_DEG, VISION_MAX_DEG);
							z += (enableShock == true ? shockz : 0);
							roaddata.SetPositions(x, y, z, yaw, roll, pitch);
							double* pulse_dugu = Control(x, y, z, roll, yaw, pitch);
							for (auto ii = 0; ii < AXES_COUNT; ++ii)
							{
								poleLength[ii] = pulse_dugu[ii];
								pulse_cal[ii] = pulse_dugu[ii];
								pulse_cal[ii] *= LENGTH_TO_PULSE_SCALE;
								auto pulse = pulse_cal[ii];
								dis[ii] = (int)pulse;
							}
							data.X = (int16_t)(x * 10);
							data.Y = (int16_t)(y * 10);
							data.Z = (int16_t)(z * 10);
							data.Roll = (int16_t)(roll * 100);
							data.Pitch = (int16_t)(pitch * 100);
							data.Yaw = (int16_t)(yaw * 100);
						}
					}
				}
			}
		}
	}
}

void MoveValPoint()
{
	chartTime += 0.067;
	for (auto i = 0; i < CHART_POINT_NUM; ++i)
	{
		chartBottomAxisPoint[i] = chartBottomAxisPoint[i + 1];

		chartXValPoint[i] = chartXValPoint[i + 1];
		chartYValPoint[i] = chartYValPoint[i + 1];
		chartZValPoint[i] = chartZValPoint[i + 1];
		chartRollValPoint[i] = chartRollValPoint[i + 1];
		chartPitchValPoint[i] = chartPitchValPoint[i + 1];
		chartYawValPoint[i] = chartYawValPoint[i + 1];
	}

	chartBottomAxisPoint[CHART_POINT_NUM - 1] = chartTime;
	chartXValPoint[CHART_POINT_NUM - 1] = data.X * XYZ_SCALE;
	chartYValPoint[CHART_POINT_NUM - 1] = data.Y * XYZ_SCALE; 
	chartZValPoint[CHART_POINT_NUM - 1] = data.Z * XYZ_SCALE;
	chartRollValPoint[CHART_POINT_NUM - 1] = data.Roll * DEG_SCALE;
	chartPitchValPoint[CHART_POINT_NUM - 1] = data.Pitch * DEG_SCALE;
	chartYawValPoint[CHART_POINT_NUM - 1] = data.Yaw * DEG_SCALE;

	m_ChartCtrl1.EnableRefresh(false);

	pLineSerie1->ClearSerie();
	pLineSerie2->ClearSerie();
	pLineSerie3->ClearSerie();
	pLineSerie4->ClearSerie();
	pLineSerie5->ClearSerie();
	pLineSerie6->ClearSerie();

	pLineSerie1->AddPoints(chartBottomAxisPoint, chartXValPoint, CHART_POINT_NUM);
	pLineSerie2->AddPoints(chartBottomAxisPoint, chartYValPoint, CHART_POINT_NUM);
	pLineSerie3->AddPoints(chartBottomAxisPoint, chartZValPoint, CHART_POINT_NUM);
	pLineSerie4->AddPoints(chartBottomAxisPoint, chartRollValPoint, CHART_POINT_NUM);
	pLineSerie5->AddPoints(chartBottomAxisPoint, chartPitchValPoint, CHART_POINT_NUM);
	pLineSerie6->AddPoints(chartBottomAxisPoint, chartYawValPoint, CHART_POINT_NUM);

	m_ChartCtrl1.EnableRefresh(true);

}

CECATSampleDlg::CECATSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CECATSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

HBRUSH CECATSampleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush m_brush(RGB(244,247,252)); 
	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID()==IDC_STATIC_STATUS1) //Picture Control控件icon背景颜色设置
	{
		pDC->SetBkColor(RGB(244,247,252));
		hbr = (HBRUSH)m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CECATSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHART_CTRL, m_ChartCtrl1);
}

BEGIN_MESSAGE_MAP(CECATSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CECATSampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_InitialCard, OnBTNInitialCard)
	ON_BN_CLICKED(IDC_BTN_FindSlave, OnBTNFindSlave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_SVON, OnChkSvon)
	ON_BN_CLICKED(IDC_CHK_ABS, OnChkAbs)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_Rise, &CECATSampleDlg::OnBnClickedBtnRise)
	ON_BN_CLICKED(IDC_BTN_Middle, &CECATSampleDlg::OnBnClickedBtnMiddle)
	ON_BN_CLICKED(IDC_BTN_Start, &CECATSampleDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_StopMe, &CECATSampleDlg::OnBnClickedBtnStopme)
	ON_BN_CLICKED(IDC_BTN_Down, &CECATSampleDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDOK, &CECATSampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CECATSampleDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_Resetme, &CECATSampleDlg::OnBnClickedBtnResetme)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CECATSampleDlg::OnBnClickedBtnDisconnect)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SINGLE_UP, &CECATSampleDlg::OnBnClickedBtnSingleUp)
	ON_BN_CLICKED(IDC_BTN_SINGLE_DOWN, &CECATSampleDlg::OnBnClickedBtnSingleDown)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CECATSampleDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CECATSampleDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_TEST3, &CECATSampleDlg::OnBnClickedButtonTest3)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CECATSampleDlg::OnBnClickedButtonStopTest)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CECATSampleDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_PROCESSING, &CECATSampleDlg::OnBnClickedButtonProcessing)
	ON_BN_CLICKED(IDC_BUTTON_REPRODUCE, &CECATSampleDlg::OnBnClickedButtonReproduce)
	ON_BN_CLICKED(IDC_BUTTON_DATA, &CECATSampleDlg::OnBnClickedButtonData)
	ON_BN_CLICKED(IDC_BUTTON_VISIBLE_TEST, &CECATSampleDlg::OnBnClickedButtonVisibleTest)
END_MESSAGE_MAP()

void CECATSampleDlg::AppConfigInit()
{
	double result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_HPF_ACC_WN_KEY);
	if (result != NULL)
	{
		hpfAccWn = result;
	}
	result= config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_LPF_ACC_WN_KEY);
	if (result != NULL)
	{
		lpfAccWn = result;
	}
	result= config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_ANG_SPD_WN_KEY);
	if (result != NULL)
	{
		hpfAngleSpdWn = result;
	}
	result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_ROLL_SCALE_KEY);
	if (result != NULL)
	{
		rollFiter.Scale = result;
	}
	result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_YAW_SCALE_KEY);
	if (result != NULL)
	{
		yawFiter.Scale = result;
	}
	result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_PITCH_SCALE_KEY);
	if (result != NULL)
	{
		pitchFiter.Scale = result;
	}
	result = config::ParseIntJsonFromFile(JSON_PARA_FILE_NAME, JSON_ROLL_FILTER_LEVEL_KEY);
	if (result != NULL)
	{
		rollFiter.FilterLevel = (unsigned char)result;
	}
	result = config::ParseIntJsonFromFile(JSON_PARA_FILE_NAME, JSON_YAW_FILTER_LEVEL_KEY);
	if (result != NULL)
	{
		yawFiter.FilterLevel = (unsigned char)result;
	}
	result = config::ParseIntJsonFromFile(JSON_PARA_FILE_NAME, JSON_PITCH_FILTER_LEVEL_KEY);
	if (result != NULL)
	{
		pitchFiter.FilterLevel = (unsigned char)result;
	}

	result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_X_SCALE_KEY);
	xFiter.Scale = result;
	result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_Y_SCALE_KEY);
	yFiter.Scale = result;
	result = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, JSON_Z_SCALE_KEY);
	zFiter.Scale = result;
	result = config::ParseIntJsonFromFile(JSON_PARA_FILE_NAME, JSON_X_FILTER_LEVEL_KEY);
	if (result != NULL)
	{
		xFiter.FilterLevel = (unsigned char)result;
	}
	result = config::ParseIntJsonFromFile(JSON_PARA_FILE_NAME, JSON_Y_FILTER_LEVEL_KEY);
	if (result != NULL)
	{
		yFiter.FilterLevel = (unsigned char)result;
	}
	result = config::ParseIntJsonFromFile(JSON_PARA_FILE_NAME, JSON_Z_FILTER_LEVEL_KEY);
	if (result != NULL)
	{
		zFiter.FilterLevel = (unsigned char)result;
	}
}

void CECATSampleDlg::KalmanFilterInit()
{

}

void CECATSampleDlg::ChartInit()
{
	CRect rect, rectChart;
	GetDlgItem(IDC_CHART_CTRL)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rectChart = rect;
	rectChart.top = rect.bottom + 3;
	rectChart.bottom = rectChart.top + rect.Height();

	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl1.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_ChartCtrl1.GetTitle()->AddString(_T(CHART_TITLE));

	m_ChartCtrl1.
		GetLeftAxis()->
		GetLabel()->
		SetText(_T(CHART_LEFT_AXIS_TITLE));

	m_ChartCtrl1.EnableRefresh(false);

	m_ChartCtrl1.RemoveAllSeries();
	m_ChartCtrl1.GetLegend()->SetVisible(true);

	pLineSerie1 = m_ChartCtrl1.CreateLineSerie();
	pLineSerie1->SetSeriesOrdering(poNoOrdering);
	pLineSerie1->SetWidth(2);
	pLineSerie1->SetName(_T(IDC_STATIC_X_VAL_SHOW_TEXT));
	
	pLineSerie2 = m_ChartCtrl1.CreateLineSerie();
	pLineSerie2->SetSeriesOrdering(poNoOrdering);
	
	pLineSerie2->SetWidth(2);
	pLineSerie2->SetName(_T(IDC_STATIC_Y_VAL_SHOW_TEXT)); 

	pLineSerie3 = m_ChartCtrl1.CreateLineSerie();
	pLineSerie3->SetSeriesOrdering(poNoOrdering);
	pLineSerie3->SetWidth(2);
	pLineSerie3->SetName(_T(IDC_STATIC_Z_VAL_SHOW_TEXT));

	pLineSerie4 = m_ChartCtrl1.CreateLineSerie();
	pLineSerie4->SetSeriesOrdering(poNoOrdering);
	pLineSerie4->SetWidth(2);
	pLineSerie4->SetName(_T(IDC_STATIC_ROLL_VAL_SHOW_TEXT));

	pLineSerie5 = m_ChartCtrl1.CreateLineSerie();
	pLineSerie5->SetSeriesOrdering(poNoOrdering); 
	pLineSerie5->SetWidth(2);
	pLineSerie5->SetName(_T(IDC_STATIC_PITCH_VAL_SHOW_TEXT));

	pLineSerie6 = m_ChartCtrl1.CreateLineSerie();
	pLineSerie6->SetSeriesOrdering(poNoOrdering);  
	pLineSerie6->SetWidth(2);
	pLineSerie6->SetName(_T(IDC_STATIC_YAW_VAL_SHOW_TEXT));

	m_ChartCtrl1.EnableRefresh(true);

}

void CECATSampleDlg::AppInit()
{
	int statusTemp = 0;
	auto isRegister = TestAppIsRegister();
	if (isRegister == false)
	{
		DialogRegister * dlg = new DialogRegister(this);
		dlg->DoModal();
		dlg->Create(IDD_DIALOG_REGISTER, this);
		dlg->ShowWindow(SW_SHOW);
		delete dlg;
		if (DialogRegister::IsRegister == false)
		{
			OnBnClickedOk();
		}
		else
		{
			MessageBox(_T("注册成功！"));
		}
	}

////LOGIN界面/////////////////////////////////////////////////////////////////////////////////////////////////////////
	CDIALOGLogin * dlg = new CDIALOGLogin(this);
		dlg->DoModal();
		dlg->Create(IDD_LOGIN, this);
		dlg->ShowWindow(SW_SHOW);
		delete dlg;
		if (CDIALOGLogin:: user_FLAG == 0)
		{
			OnBnClickedOk();
		}
		else
		{
			MessageBox(_T("登录成功！"));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	config::ReadStatusAndPulse(statusTemp, lastStartPulse);
	//stopAndMiddle = config::ReadIsAutoStopAndMiddle();
	lastStartStatus = (SixDofPlatformStatus)statusTemp;

////跳转按钮文字显示//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_BUTTON_GENERATE, _T(IDC_BUTTON_GENERATE_SHOW_TEXT));
	SetDlgItemText(IDC_BUTTON_PROCESSING, _T(IDC_BUTTON_PROCESSING_SHOW_TEXT));
	SetDlgItemText(IDC_BUTTON_REPRODUCE, _T(IDC_BUTTON_REPRODUCE_SHOW_TEXT));
	SetDlgItemText(IDC_BUTTON_DATA, _T(IDC_BUTTON_DATA_SHOW_TEXT));
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SetDlgItemText(IDC_EDIT_X_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_Y_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_Z_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_ROLL_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_PITCH_VAL, _T("0"));
	SetDlgItemText(IDC_EDIT_YAW_VAL, _T("0"));
	
	SetDlgItemText(IDC_EDIT_X_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_Y_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_Z_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_ROLL_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_PITCH_HZ, _T("0"));
	SetDlgItemText(IDC_EDIT_YAW_HZ, _T("0"));

	SetDlgItemText(IDC_EDIT_X_PHASE, _T("0"));
	SetDlgItemText(IDC_EDIT_Y_PHASE, _T("0"));
	SetDlgItemText(IDC_EDIT_Z_PHASE, _T("0"));
	SetDlgItemText(IDC_EDIT_ROLL_PHASE, _T("0"));
	SetDlgItemText(IDC_EDIT_PITCH_PHASE, _T("0"));
	SetDlgItemText(IDC_EDIT_YAW_PHASE, _T("0"));

	SetDlgItemText(IDC_EDIT_X_ZERO_POS, _T("0"));
	SetDlgItemText(IDC_EDIT_Y_ZERO_POS, _T("0"));
	SetDlgItemText(IDC_EDIT_Z_ZERO_POS, _T("0"));
	SetDlgItemText(IDC_EDIT_ROLL_ZERO_POS, _T("0"));
	SetDlgItemText(IDC_EDIT_PITCH_ZERO_POS, _T("0"));
	SetDlgItemText(IDC_EDIT_YAW_ZERO_POS, _T("0"));

	CDialog::SetWindowTextW(_T(WINDOW_TITLE));
	GetDlgItem(IDC_BTN_Start)->SetWindowTextW(_T(IDC_BTN_START_SHOW_TEXT));
	GetDlgItem(IDC_BTN_SINGLE_UP)->SetWindowTextW(_T(IDC_BTN_SINGLE_UP_SHOW_TEXT));
	GetDlgItem(IDC_BTN_SINGLE_DOWN)->SetWindowTextW(_T(IDC_BTN_SINGLE_DOWN_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_POSE)->SetWindowTextW(_T(IDC_STATIC_POSE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_LENGTH)->SetWindowTextW(_T(IDC_STATIC_LENGTH_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_SENSOR)->SetWindowTextW(_T(IDC_STATIC_SENSOR_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_X_VAL)->SetWindowTextW(_T(IDC_STATIC_X_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Y_VAL)->SetWindowTextW(_T(IDC_STATIC_Y_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Z_VAL)->SetWindowTextW(_T(IDC_STATIC_Z_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_ROLL_VAL)->SetWindowTextW(_T(IDC_STATIC_ROLL_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_PITCH_VAL)->SetWindowTextW(_T(IDC_STATIC_PITCH_VAL_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_YAW_VAL)->SetWindowTextW(_T(IDC_STATIC_YAW_VAL_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_X_HZ)->SetWindowTextW(_T(IDC_STATIC_X_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Y_HZ)->SetWindowTextW(_T(IDC_STATIC_Y_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Z_HZ)->SetWindowTextW(_T(IDC_STATIC_Z_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_ROLL_HZ)->SetWindowTextW(_T(IDC_STATIC_ROLL_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_PITCH_HZ)->SetWindowTextW(_T(IDC_STATIC_PITCH_HZ_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_YAW_HZ)->SetWindowTextW(_T(IDC_STATIC_YAW_HZ_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_X_PHASE)->SetWindowTextW(_T(IDC_STATIC_X_PHASE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Y_PHASE)->SetWindowTextW(_T(IDC_STATIC_Y_PHASE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Z_PHASE)->SetWindowTextW(_T(IDC_STATIC_Z_PHASE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_ROLL_PHASE)->SetWindowTextW(_T(IDC_STATIC_ROLL_PHASE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_PITCH_PHASE)->SetWindowTextW(_T(IDC_STATIC_PITCH_PHASE_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_YAW_PHASE)->SetWindowTextW(_T(IDC_STATIC_YAW_PHASE_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_X_ZERO_POS)->SetWindowTextW(_T(IDC_STATIC_X_ZERO_POS_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Y_ZERO_POS)->SetWindowTextW(_T(IDC_STATIC_Y_ZERO_POS_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_Z_ZERO_POS)->SetWindowTextW(_T(IDC_STATIC_Z_ZERO_POS_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_ROLL_ZERO_POS)->SetWindowTextW(_T(IDC_STATIC_ROLL_ZERO_POS_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_PITCH_ZERO_POS)->SetWindowTextW(_T(IDC_STATIC_PITCH_ZERO_POS_SHOW_TEXT));
	GetDlgItem(IDC_STATIC_YAW_ZERO_POS)->SetWindowTextW(_T(IDC_STATIC_YAW_ZERO_POS_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_TEST)->SetWindowTextW(_T(IDC_STATIC_TEST_SHOW_TEXT));
	GetDlgItem(IDC_BUTTON_TEST)->SetWindowTextW(_T(IDC_BUTTON_TEST_SHOW_TEXT));

	GetDlgItem(IDC_STATIC_APP_STATUS)->SetWindowTextW(_T(CORPORATION_NAME));
	GetDlgItem(IDC_STATIC_APP_TITLE)->SetWindowTextW(_T(APP_TITLE));
	CFont* font = new CFont();
	font->CreatePointFont(APP_TITLE_FONT_SIZE, _T("Times New Roman"));
	GetDlgItem(IDC_STATIC_APP_TITLE)->SetFont(font);

#ifdef _DEBUG
	GetDlgItem(IDC_BUTTON_VISIBLE_TEST)->ShowWindow(SW_SHOW);
#else
	GetDlgItem(IDC_BUTTON_VISIBLE_TEST)->ShowWindow(SW_HIDE);
#endif
	EanbleButton(1);
	ChartInit();
	AppConfigInit();
	for (auto i = 1; i <= AXES_COUNT; ++i)
	{
		CString xx;
		xx.Format(_T("%d"), i);
		((CComboBox*)GetDlgItem(IDC_CBO_SingleNo))->AddString(xx);
	}
	((CComboBox*)GetDlgItem(IDC_CBO_SingleNo))->SetCurSel(0);
	KalmanFilterInit();
	double planeAboveBottomLength = config::ParseDoubleJsonFromFile(JSON_PARA_FILE_NAME, PlaneAboveBottomLength_KEY);

	SetPlatformPara(PlaneAboveHingeLength, planeAboveBottomLength, 
		CircleTopRadius, CircleBottomRadius, DistanceBetweenHingeTop,
		DistanceBetweenHingeBottom);
	SetWashOutFilterPara(hpfAccWn, lpfAccWn, hpfAngleSpdWn, ILLUSION_DT);
	OpenThread();
}

double CECATSampleDlg::GetCEditNumber(int cEditId)
{
	CString str;
	GetDlgItemText(cEditId, str);
	auto val = _tstof(str);
	return val;
}

void CECATSampleDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

BOOL CECATSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	
	
	AppInit();
	InitOpenGlControl();
	SetTimer(0, TIMER_MS, NULL);
	return TRUE;  
}

int isShowSingleUpDown = 0;
BOOL CECATSampleDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_SHIFT)
		{
			isShowSingleUpDown = !isShowSingleUpDown;
			((CButton*)GetDlgItem(IDC_BTN_SINGLE_UP))->ShowWindow(isShowSingleUpDown);
			((CButton*)GetDlgItem(IDC_BTN_SINGLE_DOWN))->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_FindSlave)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_EDIT_SlaveNum)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_CBO_SingleNo)->ShowWindow(isShowSingleUpDown);

			GetDlgItem(IDC_CHK_SVON)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_Resetme)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_CONNECT)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_BTN_DISCONNECT)->ShowWindow(isShowSingleUpDown);

			GetDlgItem(IDC_BTN_InitialCard)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_STATIC)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_CBO_CardNo)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_STATIC_SHOW_INIT)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_EDIT_InitialStatus)->ShowWindow(isShowSingleUpDown);
			GetDlgItem(IDC_SHOW)->ShowWindow(!isShowSingleUpDown);
		}
		if (pMsg->wParam == 'X')
		{
			closeDataThread = true;
		}
		if (pMsg->wParam == 'Z' || pMsg->wParam == 'L')
		{
			OnBnClickedBtnPause();
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{

	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CECATSampleDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CDialog::PreCreateWindow(cs);
}

void CECATSampleDlg::InitOpenGlControl()
{
	CWnd *pWnd = GetDlgItem(IDC_SHOW);
	hrenderDC = ::GetDC(pWnd->GetSafeHwnd());
	if(SetWindowPixelFormat(hrenderDC)==FALSE) 
		return; 
	if(CreateViewGLContext(hrenderDC)==FALSE) 
		return; 
	OpenGlLightInit();
}

BOOL CECATSampleDlg::SetWindowPixelFormat(HDC hDC) 
{ 
	PIXELFORMATDESCRIPTOR pixelDesc; 
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
	pixelDesc.nVersion = 1; 
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE; 
	pixelDesc.iPixelType = PFD_TYPE_RGBA; 
	pixelDesc.cColorBits = 32; 
	pixelDesc.cRedBits = 8; 
	pixelDesc.cRedShift = 16; 
	pixelDesc.cGreenBits = 8; 
	pixelDesc.cGreenShift = 8; 
	pixelDesc.cBlueBits = 8; 
	pixelDesc.cBlueShift = 0; 
	pixelDesc.cAlphaBits = 0; 
	pixelDesc.cAlphaShift = 0; 
	pixelDesc.cAccumBits = 64; 
	pixelDesc.cAccumRedBits = 16; 
	pixelDesc.cAccumGreenBits = 16; 
	pixelDesc.cAccumBlueBits = 16; 
	pixelDesc.cAccumAlphaBits = 0; 
	pixelDesc.cDepthBits = 32; 
	pixelDesc.cStencilBits = 8; 
	pixelDesc.cAuxBuffers = 0; 
	pixelDesc.iLayerType = PFD_MAIN_PLANE; 
	pixelDesc.bReserved = 0; 
	pixelDesc.dwLayerMask = 0; 
	pixelDesc.dwVisibleMask = 0; 
	pixelDesc.dwDamageMask = 0; 
	PixelFormat = ChoosePixelFormat(hDC,&pixelDesc); 
	if(PixelFormat==0) // Choose default 
	{ 
		PixelFormat = 1; 
		if(DescribePixelFormat(hDC,PixelFormat, 
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0) 
		{ 
			return FALSE; 
		} 
	} 
	if(SetPixelFormat(hDC,PixelFormat,&pixelDesc)==FALSE) 
	{ 
		return FALSE; 
	} 
	return TRUE; 
} 

BOOL CECATSampleDlg::CreateViewGLContext(HDC hDC) 
{ 
	hrenderRC = wglCreateContext(hDC); 
	if(hrenderRC == NULL) 
		return FALSE; 
	if(wglMakeCurrent(hDC, hrenderRC) == FALSE) 
		return FALSE; 
	return TRUE; 
} 

void CECATSampleDlg::RenderScene()   
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OpenGL_SetData(data.X * XYZ_SCALE, data.Y * XYZ_SCALE, data.Z * XYZ_SCALE, 
		data.Roll * DEG_SCALE, data.Yaw * DEG_SCALE, data.Pitch * DEG_SCALE);
	RenderSixdofImage();
	SwapBuffers(hrenderDC); 
} 

void CECATSampleDlg::FillCtlColor(CWnd* cwnd, COLORREF color)
{
	CDC * pDC = cwnd->GetDC();
	CRect rct;
	cwnd->GetWindowRect(&rct);
	CBrush brs;
	brs.CreateSolidBrush(color);
	CRect picrct;
	picrct.top = 0;
	picrct.left = 0;
	picrct.bottom = rct.Height();
	picrct.right = rct.Width();
	pDC->FillRect(&picrct, &brs);
	pDC->ReleaseAttribDC();
	cwnd->ReleaseDC(pDC);
}

void CECATSampleDlg::ShowSingleInitImage(int ctlId)
{
	CRect rect; 
	GetDlgItem(ctlId)->GetClientRect(&rect);
	Gdiplus::Graphics g(GetDlgItem(ctlId)->GetDC()->m_hDC);   
	g.Clear(Gdiplus::Color::White);
	g.DrawImage(GetPumpImage(0, _T("mm")), 0, 0, rect.Width(), rect.Height());
}

void CECATSampleDlg::ShowSingleInitImage(CWnd* pic, float value)
{
	CRect rect; 
	pic->GetClientRect(&rect);
	Gdiplus::Graphics g(pic->GetDC()->m_hDC);   
	g.Clear(Gdiplus::Color::White);
	g.DrawImage(GetPumpImage(value, _T("mm")), 0, 0, rect.Width(), rect.Height());
}

void CECATSampleDlg::ShowInitImage()
{
	ShowSingleInitImage(IDC_STATIC_PIC_POLE1);
	ShowSingleInitImage(IDC_STATIC_PIC_POLE2);
	ShowSingleInitImage(IDC_STATIC_PIC_POLE3);
	ShowSingleInitImage(IDC_STATIC_PIC_POLE4);
	ShowSingleInitImage(IDC_STATIC_PIC_POLE5);
	ShowSingleInitImage(IDC_STATIC_PIC_POLE6);
}

CPen drawLinePen(PS_SOLID, 2, RGB(0, 0, 0));
CPen clearLinePen(PS_SOLID, 10, RGB(244, 244, 244));
CBrush clearBrush(RGB(244, 244, 244));
double ptCenterX = 52;
double ptCenterY = 52;
double f0;
double p0X;
double p0Y;
double p1X;
double p1Y;
//	张开的弧度
float radian=60;
//	同心圆半径
int radius[]=
{
	50,
	24,
	1
};

double fMax = 100;
double fMin = -100;

void CECATSampleDlg::ShowSingleImage(CWnd* pic, float value)
{
	value *= 2;
	CDC* dc = pic->GetDC();   
	f0 = ((180-radian)/2+radian/(fMax-fMin)*(-value + fMax)) / 180 * PI;
	dc->SelectObject(&clearBrush);
	dc->FillSolidRect(2,28,100,32, RGB(255, 255, 255));
	p0X = ptCenterX+(radius[2]*cos(f0));
	p0Y = ptCenterY-(radius[2]*sin(f0));
	p1X = ptCenterX+((radius[1])*cos(f0));
	p1Y = ptCenterY-((radius[1])*sin(f0));
	dc->SelectObject(&drawLinePen);
	dc->MoveTo(p0X, p0Y);
	dc->LineTo(p1X, p1Y);
	pic->ReleaseDC(dc);
}

void CECATSampleDlg::ShowImage()
{
	ShowSingleImage(GetDlgItem(IDC_STATIC_PIC_POLE1), poleLength[0]);
	ShowSingleImage(GetDlgItem(IDC_STATIC_PIC_POLE2), poleLength[1]);
	ShowSingleImage(GetDlgItem(IDC_STATIC_PIC_POLE3), poleLength[2]);
	ShowSingleImage(GetDlgItem(IDC_STATIC_PIC_POLE4), poleLength[3]);
	ShowSingleImage(GetDlgItem(IDC_STATIC_PIC_POLE5), poleLength[4]);
	ShowSingleImage(GetDlgItem(IDC_STATIC_PIC_POLE6), poleLength[5]);
}

void CECATSampleDlg::RenderSwitchStatus()
{
	FillCtlColor(GetDlgItem(IDC_STATIC_STATUS1), false ? COLOR_GREEN : COLOR_RED);
	FillCtlColor(GetDlgItem(IDC_STATIC_STATUS2), false ? COLOR_GREEN : COLOR_RED);
	FillCtlColor(GetDlgItem(IDC_STATIC_STATUS3), false ? COLOR_GREEN : COLOR_RED);
	FillCtlColor(GetDlgItem(IDC_STATIC_STATUS4), false ? COLOR_GREEN : COLOR_RED);
	FillCtlColor(GetDlgItem(IDC_STATIC_STATUS5), false ? COLOR_GREEN : COLOR_RED);
	FillCtlColor(GetDlgItem(IDC_STATIC_STATUS6), false ? COLOR_GREEN : COLOR_RED);
}

void CECATSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);		
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CECATSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CECATSampleDlg::OnBTNInitialCard() 
{
	CString xx;
	if (InitialFlag)
	{
		xx.Format(_T("%d"), 123);
		((CComboBox*)GetDlgItem(IDC_CBO_CardNo))->AddString(xx);
		((CComboBox*)GetDlgItem(IDC_CBO_CardNo))->SetCurSel(0);
		((CButton *)GetDlgItem(IDC_BTN_FindSlave))->EnableWindow(TRUE);
	}	
}

void CECATSampleDlg::OnBTNFindSlave() 
{

}

void CECATSampleDlg::EanbleButton(int isenable)
{	
	GetDlgItem(IDC_CHK_SVON)->EnableWindow(isenable);
	GetDlgItem(IDC_BTN_Rise)->EnableWindow(isenable);
	GetDlgItem(IDC_BTN_Middle)->EnableWindow(isenable);
	GetDlgItem(IDC_BTN_Start)->EnableWindow(isenable);
	GetDlgItem(IDC_BTN_StopMe)->EnableWindow(isenable);
	GetDlgItem(IDC_BTN_Down)->EnableWindow(isenable);
	GetDlgItem(IDC_BTN_Resetme)->EnableWindow(isenable);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(isenable);
	GetDlgItem(IDC_BUTTON_STOP_TEST)->EnableWindow(isenable);
}

void CECATSampleDlg::OnTimer(UINT nIDEvent) 
{
	CString statusStr;

	MoveValPoint();
	RenderScene();
	//RenderSwitchStatus();
	//ShowImage();
	statusStr.Format(_T("x:%d y:%d z:%d y:%d a:%d b:%d time:%d count:%d"), data.X, data.Y, data.Z,
		data.Yaw, data.Pitch, data.Roll, runTime, Counter);
	SetDlgItemText(IDC_EDIT_Pose, statusStr);

	statusStr.Format(_T("1:%.2f 2:%.2f 3:%.2f 4:%.2f 5:%.2f 6:%.2f"), 
		poleLength[0], poleLength[1], poleLength[2],
		poleLength[3], poleLength[4], poleLength[5]);
	SetDlgItemText(IDC_EDIT_Pulse, statusStr);

	statusStr.Format(_T("1:%.2f 2:%.2f 3:%.2f 4:%.2f 5:%.2f 6:%.2f"), 
		poleLength[0], poleLength[1], poleLength[2],
		poleLength[3], poleLength[4], poleLength[5]);
	SetDlgItemText(IDC_EDIT_Sensor, statusStr);
	
	CDialog::OnTimer(nIDEvent);
}

void CECATSampleDlg::OnChkSvon() 
{
	
}

void CECATSampleDlg::OnOK() 
{
	CDialog::OnOK();
}

void CECATSampleDlg::OnChkAbs() 
{
	
}

void CECATSampleDlg::OnBnClickedBtnRise()
{	
	if (status == SIXDOF_STATUS_READY)
	{
		MessageBox(_T(SIXDOF_NOT_BOTTOM_AND_RISE_MESSAGE));
	}
	status = SIXDOF_STATUS_ISRISING;	
	sendStatus = CONTROL_COMMAND_RISING;
	Sleep(50);
	status = SIXDOF_STATUS_READY;
}

void CECATSampleDlg::OnBnClickedBtnMiddle()
{
	if (stopAndMiddle == true)
	{
		if (status != SIXDOF_STATUS_READY)
		{
#if IS_USE_MESSAGEBOX
			MessageBox(_T(SIXDOF_NOT_MIDDLE_MESSAGE));
#endif
			return;
		}
	}
	else
	{
		if (status != SIXDOF_STATUS_MIDDLE)
		{
#if IS_USE_MESSAGEBOX
			MessageBox(_T(SIXDOF_NOT_MIDDLE_MESSAGE));
#endif
			return;
		}
	}
	status = SIXDOF_STATUS_READY;
	sendStatus = CONTROL_COMMAND_STOP;
}

void CECATSampleDlg::OnBnClickedBtnStart()
{
	if (status != SIXDOF_STATUS_READY)
	{
#if IS_USE_MESSAGEBOX
		MessageBox(_T(SIXDOF_NOT_BEGIN_MESSAGE));
#endif
		return;
	}
	status = SIXDOF_STATUS_RUN;
	sendStatus = CONTROL_COMMAND_START_SIGNAL;
	isTest = false;
	sin_time_pulse = 0;
	t = 0;
	dataChartTime = 0;
	closeDataThread = false;
	isStart = true;	
}

void CECATSampleDlg::OnCommandStopme()
{
	if (status != SIXDOF_STATUS_RUN)
	{
		return;
	}
	closeDataThread = true;
	status = SIXDOF_STATUS_READY;
	sendStatus = CONTROL_COMMAND_STOP;
	ResetDefaultData(&data);
}

void CECATSampleDlg::OnBnClickedBtnStopme()
{
	stopSCurve = true;
	closeDataThread = true;
	if (status == SIXDOF_STATUS_RUN)
	{
		status = SIXDOF_STATUS_READY;
	}
	sendStatus = CONTROL_COMMAND_STOP;
	ResetDefaultData(&data);
}

bool isPaused = false;
void CECATSampleDlg::OnBnClickedBtnPause()
{
#if _DEBUG
#else
	if (status == SIXDOF_STATUS_RUN)
	{
#endif
		if (isPaused == false)
		{
			closeDataThread = true;
			isPaused = true;
		}
		else
		{
			closeDataThread = false;
			isPaused = false;
		}
#if _DEBUG
#else
	}
#endif
}

void CECATSampleDlg::OnBnClickedBtnDown()
{	
	if(status == SIXDOF_STATUS_ISRISING)
	{
		return;
	}
	if (status != SIXDOF_STATUS_READY)
	{
#if IS_USE_MESSAGEBOX
		MessageBox(_T(SIXDOF_NOT_FALLING_MESSAGE));
#endif
		return;
	}
	status = SIXDOF_STATUS_ISFALLING;
	sendStatus = CONTROL_COMMAND_DOWN;
}

void CECATSampleDlg::OnBnClickedOk()
{
	CloseThread();
	CDialog::OnOK();
}

void CECATSampleDlg::OnBnClickedBtnConnect()
{

}

void CECATSampleDlg::OnBnClickedBtnDisconnect()
{

}

void CECATSampleDlg::OnBnClickedBtnResetme()
{

}

void CECATSampleDlg::OnBnClickedBtnSingleUp()
{
	auto index = ((CComboBox*)GetDlgItem(IDC_CBO_SingleNo))->GetCurSel();
}

void CECATSampleDlg::OnBnClickedBtnSingleDown()
{
	auto index = ((CComboBox*)GetDlgItem(IDC_CBO_SingleNo))->GetCurSel();
}

void CECATSampleDlg::RunTestMode()
{
	//位移单位mm 角度单位 度
	auto xval = RANGE(GetCEditNumber(IDC_EDIT_X_VAL), -MAX_XYZ_X, MAX_XYZ_X); 
	auto yval = RANGE(GetCEditNumber(IDC_EDIT_Y_VAL), -MAX_XYZ_Y, MAX_XYZ_Y);
	auto zval = RANGE(GetCEditNumber(IDC_EDIT_Z_VAL), -MAX_XYZ_Z, MAX_XYZ_Z);
	auto rollval = RANGE(GetCEditNumber(IDC_EDIT_ROLL_VAL), -MAX_DEG_ROLL, MAX_DEG_ROLL);
	auto pitchval = RANGE(GetCEditNumber(IDC_EDIT_PITCH_VAL), -MAX_DEG_PITCH, MAX_DEG_PITCH);
	auto yawval = RANGE(GetCEditNumber(IDC_EDIT_YAW_VAL), -MAX_DEG_YAW, MAX_DEG_YAW);
	//频率单位1hz
	auto xhz = RANGE(GetCEditNumber(IDC_EDIT_X_HZ), 0, MAX_HZ);
	auto yhz = RANGE(GetCEditNumber(IDC_EDIT_Y_HZ), 0, MAX_HZ);
	auto zhz = RANGE(GetCEditNumber(IDC_EDIT_Z_HZ), 0, MAX_HZ);
	auto rollhz = RANGE(GetCEditNumber(IDC_EDIT_ROLL_HZ), 0, MAX_HZ);
	auto pitchhz = RANGE(GetCEditNumber(IDC_EDIT_PITCH_HZ), 0, MAX_HZ);
	auto yawhz = RANGE(GetCEditNumber(IDC_EDIT_YAW_HZ), 0, MAX_HZ);

	auto xphase = RANGE(GetCEditNumber(IDC_EDIT_X_PHASE), 0, MAX_PHASE);
	auto yphase = RANGE(GetCEditNumber(IDC_EDIT_Y_PHASE), 0, MAX_PHASE);
	auto zphase = RANGE(GetCEditNumber(IDC_EDIT_Z_PHASE), 0, MAX_PHASE);
	auto rollphase = RANGE(GetCEditNumber(IDC_EDIT_ROLL_PHASE), 0, MAX_PHASE);
	auto pitchphase = RANGE(GetCEditNumber(IDC_EDIT_PITCH_PHASE), 0, MAX_PHASE);
	auto yawphase = RANGE(GetCEditNumber(IDC_EDIT_YAW_PHASE), 0, MAX_PHASE);

	auto xzeropos = RANGE(GetCEditNumber(IDC_EDIT_X_ZERO_POS), -MAX_XYZ_X, MAX_XYZ_X);
	auto yzeropos = RANGE(GetCEditNumber(IDC_EDIT_Y_ZERO_POS), -MAX_XYZ_Y, MAX_XYZ_Y);
	auto zzeropos = RANGE(GetCEditNumber(IDC_EDIT_Z_ZERO_POS), -MAX_XYZ_Z, MAX_XYZ_Z);
	auto rollzeropos = RANGE(GetCEditNumber(IDC_EDIT_ROLL_ZERO_POS), -MAX_DEG_ROLL, MAX_DEG_ROLL);
	auto pitchzeropos = RANGE(GetCEditNumber(IDC_EDIT_PITCH_ZERO_POS), -MAX_DEG_PITCH, MAX_DEG_PITCH);
	auto yawzeropos = RANGE(GetCEditNumber(IDC_EDIT_YAW_ZERO_POS), -MAX_DEG_YAW, MAX_DEG_YAW);

	testVal[0] = xval;
	testVal[1] = yval;
	testVal[2] = zval;
	testVal[3] = rollval;
	testVal[4] = pitchval;
	testVal[5] = yawval;

	testHz[0] = xhz;
	testHz[1] = yhz;
	testHz[2] = zhz;
	testHz[3] = rollhz;
	testHz[4] = pitchhz;
	testHz[5] = yawhz;

	testPhase[0] = xphase;
	testPhase[1] = yphase;
	testPhase[2] = zphase;
	testPhase[3] = rollphase;
	testPhase[4] = pitchphase;
	testPhase[5] = yawphase;

	testZeroPos[0] = xzeropos;
	testZeroPos[1] = yzeropos;
	testZeroPos[2] = zzeropos;
	testZeroPos[3] = rollzeropos;
	testZeroPos[4] = pitchzeropos;
	testZeroPos[5] = yawzeropos;

	if (xphase != 0 || yphase != 0 || zphase != 0 || 
		rollphase != 0 || pitchphase != 0 || yawphase != 0)
	{
		enableChirp = false;
		isCosMode = true;
	}
	else
	{
		enableChirp = ENABLE_CHIRP;
		isCosMode = false;
	}
	stopSCurve = false;
	isCsp = false;
	if (isCsp == false)
	{
		
	}
	isTest = true;
	t = 0;
	dataChartTime = 0;

	closeDataThread = false;
	isStart = true;
}

void CECATSampleDlg::RunJudgeRangeTestMode()
{
	//位移单位mm 角度单位 度
	auto xval = GetCEditNumber(IDC_EDIT_X_VAL); 
	auto yval = GetCEditNumber(IDC_EDIT_Y_VAL); 
	auto zval = GetCEditNumber(IDC_EDIT_Z_VAL); 
	auto rollval = GetCEditNumber(IDC_EDIT_ROLL_VAL); 
	auto pitchval = GetCEditNumber(IDC_EDIT_PITCH_VAL); 
	auto yawval = GetCEditNumber(IDC_EDIT_YAW_VAL);
	//频率单位1hz
	auto xhz = RANGE(GetCEditNumber(IDC_EDIT_X_HZ), 0, MAX_HZ);
	auto yhz = RANGE(GetCEditNumber(IDC_EDIT_Y_HZ), 0, MAX_HZ);
	auto zhz = RANGE(GetCEditNumber(IDC_EDIT_Z_HZ), 0, MAX_HZ);
	auto rollhz = RANGE(GetCEditNumber(IDC_EDIT_ROLL_HZ), 0, MAX_HZ);
	auto pitchhz = RANGE(GetCEditNumber(IDC_EDIT_PITCH_HZ), 0, MAX_HZ);
	auto yawhz = RANGE(GetCEditNumber(IDC_EDIT_YAW_HZ), 0, MAX_HZ);

	auto xphase = RANGE(GetCEditNumber(IDC_EDIT_X_PHASE), 0, MAX_PHASE);
	auto yphase = RANGE(GetCEditNumber(IDC_EDIT_Y_PHASE), 0, MAX_PHASE);
	auto zphase = RANGE(GetCEditNumber(IDC_EDIT_Z_PHASE), 0, MAX_PHASE);
	auto rollphase = RANGE(GetCEditNumber(IDC_EDIT_ROLL_PHASE), 0, MAX_PHASE);
	auto pitchphase = RANGE(GetCEditNumber(IDC_EDIT_PITCH_PHASE), 0, MAX_PHASE);
	auto yawphase = RANGE(GetCEditNumber(IDC_EDIT_YAW_PHASE), 0, MAX_PHASE);

	auto xzeropos = GetCEditNumber(IDC_EDIT_X_ZERO_POS); 
	auto yzeropos = GetCEditNumber(IDC_EDIT_Y_ZERO_POS); 
	auto zzeropos = GetCEditNumber(IDC_EDIT_Z_ZERO_POS); 
	auto rollzeropos = GetCEditNumber(IDC_EDIT_ROLL_ZERO_POS);
	auto pitchzeropos = GetCEditNumber(IDC_EDIT_PITCH_ZERO_POS); 
	auto yawzeropos = GetCEditNumber(IDC_EDIT_YAW_ZERO_POS); 

	if (abs(xval) + abs(xzeropos) > MAX_XYZ_X)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_X_OUT_OF_RANGE_MESSAGE));	
	}

	if (abs(yval) + abs(yzeropos) > MAX_XYZ_Y)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_Y_OUT_OF_RANGE_MESSAGE));
	}

	if (abs(zval) + abs(zzeropos) > MAX_XYZ_Z)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_Z_OUT_OF_RANGE_MESSAGE));	
	}

	if (abs(rollval) + abs(rollzeropos) > MAX_DEG_ROLL)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_ROLL_OUT_OF_RANGE_MESSAGE));	
	}

	if (abs(pitchval) + abs(pitchzeropos) > MAX_DEG_PITCH)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_PITCH_OUT_OF_RANGE_MESSAGE));	
	}

	if (abs(yawval) + abs(yawzeropos) > MAX_DEG_YAW)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_YAW_OUT_OF_RANGE_MESSAGE));
	}

	if (2 * PI * xhz * xval / 1000.0 > MAX_XYZ_SPEED ||
		2 * PI * yhz * yval / 1000.0 > MAX_XYZ_SPEED ||
		2 * PI * zhz * zval / 1000.0 > MAX_XYZ_SPEED)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_XYZ_SPEED_OUT_OF_RANGE_MESSAGE));
	}

	if (2 * PI * rollhz * rollval > MAX_DEG_SPEED ||
		2 * PI * pitchhz * pitchval > MAX_DEG_SPEED ||
		2 * PI * yawhz * yawval > MAX_DEG_SPEED)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_DEG_SPEED_OUT_OF_RANGE_MESSAGE));
	}
	
	if (4 * PI * PI * xhz * xhz * xval / 1000.0 > MAX_XYZ_ACC ||
		4 * PI * PI * yhz * yhz * yval / 1000.0 > MAX_XYZ_ACC ||
		4 * PI * PI * zhz * zhz * zval / 1000.0 > MAX_XYZ_ACC)
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_XYZ_ACC_OUT_OF_RANGE_MESSAGE));
	}

	if (4 * PI * PI * rollhz * rollhz * rollval > MAX_DEG_ACC ||
		4 * PI * PI * pitchhz * pitchhz * pitchval > MAX_DEG_ACC ||
		4 * PI * PI * yawhz * yawhz * yawval > MAX_DEG_ACC )
	{
		status = SIXDOF_STATUS_READY;
		MessageBox(_T(SIXDOF_NOT_BEGIN_DEG_ACC_OUT_OF_RANGE_MESSAGE));
	}

	if (status == SIXDOF_STATUS_READY)
	{
		return;
	}

	testVal[0] = xval;
	testVal[1] = yval;
	testVal[2] = zval;
	testVal[3] = rollval;
	testVal[4] = pitchval;
	testVal[5] = yawval;

	testHz[0] = xhz;
	testHz[1] = yhz;
	testHz[2] = zhz;
	testHz[3] = rollhz;
	testHz[4] = pitchhz;
	testHz[5] = yawhz;

	testPhase[0] = xphase;
	testPhase[1] = yphase;
	testPhase[2] = zphase;
	testPhase[3] = rollphase;
	testPhase[4] = pitchphase;
	testPhase[5] = yawphase;

	testZeroPos[0] = xzeropos;
	testZeroPos[1] = yzeropos;
	testZeroPos[2] = zzeropos;
	testZeroPos[3] = rollzeropos;
	testZeroPos[4] = pitchzeropos;
	testZeroPos[5] = yawzeropos;

	if (xphase != 0 || yphase != 0 || zphase != 0 || 
		rollphase != 0 || pitchphase != 0 || yawphase != 0)
	{
		enableChirp = false;
		isCosMode = true;
	}
	else
	{
		enableChirp = ENABLE_CHIRP;
		isCosMode = false;
	}
	stopSCurve = false;
	isCsp = false;
	if (isCsp == false)
	{

	}
	isTest = true;
	t = 0;
	dataChartTime = 0;

	closeDataThread = false;
	isStart = true;
}

void CECATSampleDlg::OnBnClickedButtonTest()
{
	if (status != SIXDOF_STATUS_READY)
	{
#if IS_USE_MESSAGEBOX
		MessageBox(_T(SIXDOF_NOT_BEGIN_MESSAGE));
#endif
		return;
	}
	isPlayData = false;
	status = SIXDOF_STATUS_RUN;
	sendStatus = CONTROL_COMMAND_START_SINE;
	RunJudgeRangeTestMode();
}

void CECATSampleDlg::OnBnClickedButtonExit()
{
	OnBnClickedOk();
}

void CECATSampleDlg::OnBnClickedButtonTest3()
{
#if _DEBUG
	status = SIXDOF_STATUS_RUN;
#endif
	RunJudgeRangeTestMode();
}

void CECATSampleDlg::OnBnClickedButtonStopTest()
{
	OnBnClickedBtnStopme();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//路谱生成
void CECATSampleDlg::OnBnClickedButtonGenerate()
{

	// TODO: 在此添加控件通知处理程序代码
	CDIALOGGenerate * dlg = new CDIALOGGenerate(this);
	dlg->DoModal();
	dlg->Create(IDD_GENERATE, this);
	dlg->ShowWindow(SW_SHOW);
	delete dlg;	
}

//路谱处理
void CECATSampleDlg::OnBnClickedButtonProcessing()
{
	// TODO: 在此添加控件通知处理程序代码
	CDIALOGProcessing * dlg = new CDIALOGProcessing(this);
	dlg->DoModal();
	dlg->Create(IDD_PROCESSING, this);
	dlg->ShowWindow(SW_SHOW);
	delete dlg;
}

double SourceBuf[DATA_COL_NUM] = {0};

void CECATSampleDlg::DataFromFile()
{
	CString targetHistoryPath = "";
	CString defaultDir = L"C:\\";  //默认打开的文件路径
	CString defaultFile = L"test.txt"; //默认打开的文件名
	CFileDialog dlg(TRUE, _T("txt"), defaultDir + "\\" + defaultFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("数据文件|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		targetHistoryPath = dlg.GetPathName();
		if (targetHistoryPath == "")
		{
			MessageBox(_T("未选择文件！"));
			return;
		}
	}

	ifstream fin(targetHistoryPath);		
	double *ptr = &SourceBuf[0]; 
	int readcount = 0;
	int arrcount = 0;
	if (!fin.is_open())
	{
		AfxMessageBox(_T("未找到文件!"));
		return;
	}
	roadSpectrum.DataBuffer.clear();
	while (!fin.eof() && readcount < MAX_REPRODUCE_LINE * DATA_COL_NUM)
	{
		fin >> *ptr; 
		ptr++;
		readcount++;
		arrcount++;
		if (arrcount >= DATA_COL_NUM)
		{
			arrcount = 0;
			roadSpectrum.DataBuffer.push_back(Signal::RoadSpectrumData::FromArray(SourceBuf));
			ptr = &SourceBuf[0];
		}
	}
	fin.close();
}

// 路谱复现
void CECATSampleDlg::OnBnClickedButtonReproduce()
{
#if _DEBUG
#else
	if (status != SIXDOF_STATUS_READY)
	{
#if IS_USE_MESSAGEBOX
		MessageBox(_T(SIXDOF_NOT_BEGIN_MESSAGE));
#endif
		return;
	}
#endif
	DataFromFile();
	Sleep(20);
	isPlayData = false;
#if _DEBUG
	status = SIXDOF_STATUS_READY;
#endif
	OnBnClickedBtnStart();
}

void CECATSampleDlg::OnBnClickedButtonData()
{
	DataFromFile();
	isTest = false;
	isPlayData = true;
	sin_time_pulse = 0;
	t = 0;
	dataChartTime = 0;
	closeDataThread = false;
	isStart = true;	
}


void CECATSampleDlg::OnBnClickedButtonVisibleTest()
{
	
}
