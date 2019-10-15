#pragma once

#include <queue>

#include "communication/sixdof.h"
#include "signal/roadspectrum.h"

using namespace std;

typedef enum 
{
	None = 0,
	SimulationTest = 1,
	RecurringRun = 2
}ReproduceMode;

// CDIALOGReproduce �Ի���
class CDIALOGReproduce : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGReproduce)

public:
	CDIALOGReproduce(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDIALOGReproduce();

// �Ի�������
	enum { IDD = IDD_REPRODUCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();

	CString m_TargetHistoryPath;

	CString m_TargetPath;
	CEdit m_Time;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();

public:
	ReproduceMode Mode;
	queue<SixdofPackage> DataBuffer;
	void DataFromFile();
};
