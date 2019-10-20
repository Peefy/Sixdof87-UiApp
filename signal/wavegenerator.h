
#ifndef __WAVE_GENERATOR_H
#define __WAVE_GENERATOR_H

#include <math.h>

#include "roadspectrum.h"

#define DEFAULT_DT 0.02

namespace Signal
{
	class WaveGenerator
	{
	public:
		WaveGenerator();
		~WaveGenerator();
		SixdofData Amp;
		SixdofData Freq;
		void SetSampleTime(double time);
		double GetSampleTime();
		void ClearCountTime();
		RoadSpectrumData GetNext();
	private:
		RoadSpectrumData data;
		RoadSpectrumData lastdata;
		double deltat;
		double t;
	};

}

#endif
