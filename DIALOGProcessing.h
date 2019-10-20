#pragma once
#include "afxwin.h"

// CDIALOGProcessing �Ի���

#define dMax0 10
#define dMin0 1
#define dMax1 20
#define dMin1 0
#define dMax2 20
#define dMin2 0
#define RowNum 1000
#define ColNum 18
#define TargetNum 100

class CDIALOGProcessing : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGProcessing)

public:
	CDIALOGProcessing(CWnd *pParent = NULL); // ��׼���캯��
	virtual ~CDIALOGProcessing();

	// �Ի�������
	enum
	{
		IDD = IDD_PROCESSING
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_combo1;
	CComboBox m_combo2;
	CComboBox m_combo3;
	afx_msg void OnCbnSelchangeCombo1();
	int m_dNum0;
	double m_dNum1;
	double m_dNum2;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	CString m_SourceHistoryPath; //��Դ�ļ�·��
	afx_msg void OnBnClickedButton3();
	CString m_TargetDir;
	afx_msg void OnBnClickedButton1();
	CString m_SourcePath;
	void ReadFile();
	void WriteFile();
	void Processing();
	void TestProcessing();
	void DataFromFile();
};
