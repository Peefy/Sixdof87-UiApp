
#ifndef __ROAD_SPECTRUM_H
#define __ROAD_SPECTRUM_H

#include <queue>
#include <deque>
#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace Signal
{

struct SixdofData
{
	//单位1mm
	long double X;
	//单位1mm
	long double Y;
	//单位1mm
	long double Z;
	//单位1度
	long double Yaw;
	//单位1度
	long double Roll;
	//单位1度
	long double Pitch;

	SixdofData()
	{
		X = 0;
		Y = 0;
		Z = 0;
		Yaw = 0;
		Roll = 0;
		Pitch = 0;
	}

	string ToString()
	{
		return to_string(X) + " " + to_string(Y) + " " 
			+ to_string(Z) + " " + to_string(Yaw) + " "
			+ to_string(Roll) + " " + to_string(Pitch) + " ";
	}

	void SetData(long double x, long double y, long double z, 
		long double yaw, long double roll, long double pitch)
	{
		X = x;
		Y = y;
		Z = z;
		Yaw = yaw;
		Roll = roll;
		Pitch = pitch;
	}

	void GetDataFromArray(double* data)
	{
		X = data[0];
		Y = data[1];
		Z = data[2];
		Yaw = data[3];
		Roll = data[4];
		Pitch = data[5];
	}
};

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

	// ser sixdof length = 6
	static RoadSpectrumData FromArray(double* arr)
	{
		return RoadSpectrumData(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
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
