// DIALOGLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGLogin.h"
#include "afxdialogex.h"

#define DUGU_USERNAME "123"
#define DUGU_PASSWORD "123"

#define USERFLAG_ADMIN 1
#define USERFLAG_TEST 2

int CDIALOGLogin::user_FLAG = 0; //1��������Աadmin��2��������Աtest
// CDIALOGLogin �Ի���

IMPLEMENT_DYNAMIC(CDIALOGLogin, CDialogEx)

CDIALOGLogin::CDIALOGLogin(CWnd *pParent /*=NULL*/)
	: CDialogEx(CDIALOGLogin::IDD, pParent), m_EditUsername(_T("")), m_EditPassword(_T(""))
{
}

CDIALOGLogin::~CDIALOGLogin()
{
}

void CDIALOGLogin::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_username, m_EditUsername);
	DDX_Text(pDX, IDC_EDIT_password, m_EditPassword);
}

BEGIN_MESSAGE_MAP(CDIALOGLogin, CDialogEx)
ON_BN_CLICKED(IDOK, &CDIALOGLogin::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CDIALOGLogin::OnBnClickedCancel)
END_MESSAGE_MAP()

// CDIALOGLogin ��Ϣ�������

void CDIALOGLogin::OnBnClickedOk()
{
	//���ر༭�����ݲ��ж�ע��״̬////////////////////////////////////
	UpdateData(TRUE);
	if (m_EditUsername == _T("admin") && m_EditPassword == _T("000"))
	{
		user_FLAG = USERFLAG_ADMIN; //����Ա��½
		CDialogEx::OnOK();
	}
	else if (m_EditUsername == _T("test") && m_EditPassword == _T("001"))
	{
		user_FLAG = USERFLAG_TEST; //����Ա��½
		CDialogEx::OnOK();
	}
	else if (m_EditUsername == _T(DUGU_USERNAME) && m_EditPassword == _T(DUGU_PASSWORD))
	{
		user_FLAG = USERFLAG_ADMIN;
		CDialogEx::OnOK();
	}
	else
	{
		MessageBox(_T("�˻����������"));
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

void CDIALOGLogin::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
