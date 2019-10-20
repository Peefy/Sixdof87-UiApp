
#include "stdafx.h"
#include "noise.h"

namespace Signal
{
	NoiseGenerator::NoiseGenerator() :
		type(NoiseTypeRandom), 
		dBval(50.0)
	{

	}

	NoiseGenerator::~NoiseGenerator()
	{

	}

	SixdofData NoiseGenerator::GenerateNoise(SixdofData val)
	{
		srand((unsigned)time(NULL));
		auto xrand = (rand() / (double)RAND_MAX) - 0.5;
		auto yrand = (rand() / (double)RAND_MAX) - 0.5;
		auto zrand = (rand() / (double)RAND_MAX) - 0.5;
		auto yawrand = (rand() / (double)RAND_MAX) - 0.5;
		auto rollrand = (rand() / (double)RAND_MAX) - 0.5;
		auto pitchrand = (rand() / (double)RAND_MAX) - 0.5;
		return val / dBval + SixdofData(xrand, yrand, zrand, yawrand, rollrand, pitchrand);
	}

	SixdofData NoiseGenerator::GenerateNoise(double val)
	{
		srand((unsigned)time(NULL));
		auto xrand = (rand() / (double)RAND_MAX) - 0.5;
		auto yrand = (rand() / (double)RAND_MAX) - 0.5;
		auto zrand = (rand() / (double)RAND_MAX) - 0.5;
		auto yawrand = (rand() / (double)RAND_MAX) - 0.5;
		auto rollrand = (rand() / (double)RAND_MAX) - 0.5;
		auto pitchrand = (rand() / (double)RAND_MAX) - 0.5;
		return SixdofData(xrand, yrand, zrand, yawrand, rollrand, pitchrand) * val;
	}

}
