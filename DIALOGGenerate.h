#pragma once


// CDIALOGGenerate 对话框

class CDIALOGGenerate : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGGenerate)

public:
	CDIALOGGenerate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDIALOGGenerate();

// 对话框数据
	enum { IDD = IDD_GENERATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	double m_dNum0;
	double m_dNum1;
	double m_dNum2;
	double m_dNum3;
	double m_dNum4;
	double m_dNum5;	
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButton5();
	CString m_chSaveDir;
	afx_msg void OnBnClickedButton3();
	CString m_FileName;
	afx_msg void OnBnClickedButton1();
	BOOL m_Radio1;
	BOOL m_Radio4;
	BOOL m_Radio7;
	BOOL m_Radio10;
	BOOL m_Radio8;
	BOOL m_Radio17;
};
