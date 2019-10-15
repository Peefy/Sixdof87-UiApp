
#ifndef __ROAD_SPECTRUM_H
#define __ROAD_SPECTRUM_H

#include <queue>
#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace Signal
{

struct SixdofData
{
	//��λ1mm
	long double X;
	//��λ1mm
	long double Y;
	//��λ1mm
	long double Z;
	//��λ1��
	long double Yaw;
	//��λ1��
	long double Roll;
	//��λ1��
	long double Pitch;

	string ToString()
	{
		return to_string(X) + " " + to_string(Y) + " " 
			+ to_string(Z) + " " + to_string(Yaw) + " "
			+ to_string(Roll) + " " + to_string(Pitch) + " ";
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
	SixdofData Position; //λ��
	SixdofData Speed;    //�ٶ�
	SixdofData Acc;      //���ٶ�

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

	string ToString() {
		return Position.ToString() + 
			Speed.ToString() + 
			Acc.ToString();
	}

	static RoadSpectrumData FromString()
	{
		return RoadSpectrumData();
	}

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
	queue<RoadSpectrumData> DataBuffer;
	int OperateMode;
private:
	
};

}

#endif
