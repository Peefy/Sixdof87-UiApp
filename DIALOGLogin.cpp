// DIALOGLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGLogin.h"
#include "afxdialogex.h"

int CDIALOGLogin:: user_FLAG = 0;//1——管理员admin，2——试验员test
// CDIALOGLogin 对话框

IMPLEMENT_DYNAMIC(CDIALOGLogin, CDialogEx)

CDIALOGLogin::CDIALOGLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDIALOGLogin::IDD, pParent)
	, m_EditUsername(_T(""))
	, m_EditPassword(_T(""))
{

}

CDIALOGLogin::~CDIALOGLogin()
{
}

void CDIALOGLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_username, m_EditUsername);
	DDX_Text(pDX, IDC_EDIT_password, m_EditPassword);
}


BEGIN_MESSAGE_MAP(CDIALOGLogin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDIALOGLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDIALOGLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDIALOGLogin 消息处理程序


void CDIALOGLogin::OnBnClickedOk()
{    
	//读回编辑框数据并判断注册状态////////////////////////////////////
	UpdateData(TRUE);
	if ((m_EditUsername=="admin") && (m_EditPassword == "000"))
	{
		user_FLAG = 1;//管理员登陆
		CDialogEx::OnOK();	
	}
	else if(m_EditUsername=="test" && m_EditPassword=="001")
     {
		 user_FLAG = 2;//试验员登陆
		 CDialogEx::OnOK();
	}
	else
	{
		MessageBox(_T("账户或密码错误！"));
	}

	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CDIALOGLogin::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
