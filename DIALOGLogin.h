#pragma once

// CDIALOGLogin �Ի���

class CDIALOGLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGLogin)

public:
	CDIALOGLogin(CWnd *pParent = NULL); // ��׼���캯��
	virtual ~CDIALOGLogin();

	// �Ի�������
	enum
	{
		IDD = IDD_LOGIN
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_EditUsername;
	CString m_EditPassword;
	afx_msg void OnBnClickedOk();
	static int user_FLAG; //0������¼ʧ��, 1��������Աadmin��2��������Աtest
	afx_msg void OnBnClickedCancel();
};
