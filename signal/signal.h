
#ifndef __SIGNAL_H
#define __SIGNAL_H

#include <queue>
#include <deque>
#include <vector>
#include <memory>
#include <string>

using namespace std;

#define DEFAULT_DT 0.02

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

		SixdofData()
		{
			X = 0;
			Y = 0;
			Z = 0;
			Yaw = 0;
			Roll = 0;
			Pitch = 0;
		}

		SixdofData(long double x, long double y, long double z, 
			long double yaw, long double roll, long double pitch)
		{
			X = x;
			Y = y;
			Z = z;
			Yaw = yaw;
			Roll = roll;
			Pitch = pitch;
		}

		SixdofData(double* vals)
		{
			X = vals[0];
			Y = vals[1];
			Z = vals[2];
			Yaw = vals[3];
			Roll = vals[4];
			Pitch = vals[5];
		}

		SixdofData operator+ (SixdofData& right)
		{
			return SixdofData(X + right.X,  
				Y + right.Y,
				Z + right.Z,
				Yaw + right.Yaw,
				Roll + right.Roll,
				Pitch + right.Pitch);
		}

		SixdofData operator- (SixdofData& right)
		{
			return SixdofData(X - right.X,  
				Y - right.Y,
				Z - right.Z,
				Yaw - right.Yaw,
				Roll - right.Roll,
				Pitch - right.Pitch);
		}

		SixdofData operator* (double& val)
		{
			return SixdofData(X * val,  
				Y * val,
				Z * val,
				Yaw * val,
				Roll * val,
				Pitch * val);
		}

		SixdofData operator/ (double& val)
		{
			return SixdofData(X / val,  
				Y / val,
				Z / val,
				Yaw / val,
				Roll / val,
				Pitch / val);
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

}

#endif
