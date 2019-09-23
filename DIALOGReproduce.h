#pragma once


// CDIALOGReproduce 对话框

class CDIALOGReproduce : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGReproduce)

public:
	CDIALOGReproduce(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDIALOGReproduce();

// 对话框数据
	enum { IDD = IDD_REPRODUCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();

	CString m_TargetHistoryPath;

	CString m_TargetPath;
	CEdit m_Time;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
};
