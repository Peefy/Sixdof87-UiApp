#pragma once
#include "afxwin.h"


// CDIALOGProcessing 对话框

class CDIALOGProcessing : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGProcessing)

public:
	CDIALOGProcessing(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDIALOGProcessing();

// 对话框数据
	enum { IDD = IDD_PROCESSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CString m_SourceHistoryPath;//谱源文件路径
	afx_msg void OnBnClickedButton3();
	CString m_TargetDir;
	afx_msg void OnBnClickedButton1();
	CString m_SourcePath;
	void CDIALOGProcessing::ReadFile();
	void CDIALOGProcessing::WriteFile();
   void CDIALOGProcessing::Processing();

	#define dMax0 10
	#define dMin0 1
	#define dMax1 20
	#define dMin1 0
	#define dMax2 20
	#define dMin2 0
	#define RowNum 2
	#define ColNum 3
	#define TargetNum 100
	double SourceBuf[RowNum][ColNum];//如果数据量过大 则需要把 array 定义成static类型
	 //因为默认的堆栈大小容量不够，可以放到静态存储区  
	double TargetBuf[RowNum][ColNum];
};
