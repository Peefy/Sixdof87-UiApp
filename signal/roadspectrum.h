
#ifndef __ROAD_SPECTRUM_H
#define __ROAD_SPECTRUM_H

#include <queue>
#include <deque>
#include <vector>
#include <memory>
#include <string>

#include "signal.h"

using namespace std;

namespace Signal
{


struct RoadSpectrumData
{
	SixdofData Position; //位移
	SixdofData Speed;    //速度
	SixdofData Acc;      //加速度

	RoadSpectrumData(double x, double y, double z, 
		double yaw, double roll, double pitch)  
	{
		Position.X = x;
		Position.Y = y;
		Position.Z = z;
		Position.Yaw = yaw;
		Position.Roll = roll;
		Position.Pitch = pitch;	
	}

	RoadSpectrumData() {}

	virtual ~RoadSpectrumData() {}

	void SetPositions(double x, double y, double z, 
		double yaw, double roll, double pitch)  
	{
		Position.X = x;
		Position.Y = y;
		Position.Z = z;
		Position.Yaw = yaw;
		Position.Roll = roll;
		Position.Pitch = pitch;	
	}

	void SetSpeeds(double x, double y, double z, 
		double yaw, double roll, double pitch)  
	{
		Speed.X = x;
		Speed.Y = y;
		Speed.Z = z;
		Speed.Yaw = yaw;
		Speed.Roll = roll;
		Speed.Pitch = pitch;	
	}

	void SetAccs(double x, double y, double z, 
		double yaw, double roll, double pitch)  
	{
		Acc.X = x;
		Acc.Y = y;
		Acc.Z = z;
		Acc.Yaw = yaw;
		Acc.Roll = roll;
		Acc.Pitch = pitch;	
	}

	void SetPositions(double* datas)  
	{
		Position.X = datas[0];
		Position.Y = datas[1];
		Position.Z = datas[2];
		Position.Yaw = datas[3];
		Position.Roll = datas[4];
		Position.Pitch = datas[5];
	}

	void SetSpeeds(double* datas)  
	{
		Speed.X = datas[0];
		Speed.Y = datas[1];
		Speed.Z = datas[2];
		Speed.Yaw = datas[3];
		Speed.Roll = datas[4];
		Speed.Pitch = datas[5];	
	}

	void SetAccs(double* datas)   
	{
		Acc.X = datas[0];
		Acc.Y = datas[1];
		Acc.Z = datas[2];
		Acc.Yaw = datas[3];
		Acc.Roll = datas[4];
		Acc.Pitch = datas[5];	
	}

	string ToString() 
	{
		return Position.ToString() + 
			Speed.ToString() + 
			Acc.ToString();
	}

	static RoadSpectrumData FromString()
	{
		return RoadSpectrumData();
	}

	// ser sixdof length = 18
	static RoadSpectrumData FromArray(double* arr)
	{
		auto data = RoadSpectrumData();
		data.SetPositions(arr);
		data.SetSpeeds(arr + 6);
		data.SetAccs(arr + 12);
		return data;
	}

};

class RoadSpectrum
{
public:
	RoadSpectrum();
	virtual ~RoadSpectrum();
	deque<RoadSpectrumData> DataBuffer;
	int OperateMode;
private:
	
};

}

#endif
