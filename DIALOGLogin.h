#pragma once


// CDIALOGLogin 对话框

class CDIALOGLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGLogin)

public:
	CDIALOGLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDIALOGLogin();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_EditUsername;
	CString m_EditPassword;
	afx_msg void OnBnClickedOk();
	static int user_FLAG;//0——登录失败, 1——管理员admin，2——试验员test
	afx_msg void OnBnClickedCancel();
};
