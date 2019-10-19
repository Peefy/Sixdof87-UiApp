
#ifndef __WAVE_GENERATOR_H
#define __WAVE_GENERATOR_H

#include "roadspectrum.h"

namespace Signal
{
	class WaveGenerator
	{
	public:
		WaveGenerator();
		~WaveGenerator();

	private:
		RoadSpectrumData data;
	};

}

#endif
