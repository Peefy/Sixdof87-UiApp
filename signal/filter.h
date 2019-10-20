
#ifndef __FILTER_H
#define __FILTER_H

#include <queue>
#include <vector>
#include <deque>

#include "signal.h"

using namespace std;

// ��ʶ��������
#define PARA_IN
// ��ʶ�������
#define PARA_OUT
// �˲���������
#define MAX_ORDER 10

namespace Signal
{

	// z�任���ݺ���
	class Ztrans
	{
	public:
		Ztrans(double dT, int order, double* nums, double* dens);
		Ztrans(double dT, int order);
		~Ztrans() {};
		int Order;
		// ���ò�������
		void SetSampleTime(double sampleTime);
		// ���뾭���ݺ����õ����
		double Update(double now);
		// ����z���ݺ����ķ��ӷ�ĸϵ��
		void SetNumsAndDensZtrans(double* nums, double* dens);
		// ����s���ݺ����ķ��ӷ�ĸϵ��
		void SetNumsAndDensLaplace(double* nums, double* dens, double fs);
		// s��z���˫���Ա任
		void Bilinear(PARA_IN double* b, PARA_IN double* a, double fs, int dimensions, 
			PARA_OUT double* bprimes, PARA_OUT double* aprimes);
	private:
		double dT;
		double fs;
		deque<double> input;
		deque<double> output;
		double inner_nums[MAX_ORDER];
		double inner_dens[MAX_ORDER];
	};

	// �źŵ�ͨ�˲���
	class SignalLpf : public Ztrans
	{
	public:
		SignalLpf(double dT = DEFAULT_DT); 
		~SignalLpf() {};

	private:
		double lowFreq;
		double highFreq;
		int order;
	};

	class ButterworthFilter
	{
	public:
		ButterworthFilter();
		~ButterworthFilter();

	private:

	};

	ButterworthFilter::ButterworthFilter()
	{
	}

	ButterworthFilter::~ButterworthFilter()
	{
	}

}

#endif
