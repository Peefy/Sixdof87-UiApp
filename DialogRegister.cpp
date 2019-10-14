// DialogRegister.cpp : ??????
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DialogRegister.h"
#include "afxdialogex.h"

#include "register\register.h"

void DialogRegister::AppInit()
{
	string mc = GetHardwareId();
	CString str = mc.c_str();
	SetDlgItemText(IDC_EDIT_MC, str);
}

bool DialogRegister::IsRegister = false;
// DialogRegister ?????

IMPLEMENT_DYNAMIC(DialogRegister, CDialogEx)

DialogRegister::DialogRegister(CWnd *pParent /*=NULL*/)
	: CDialogEx(DialogRegister::IDD, pParent)
{
}

DialogRegister::~DialogRegister()
{
}

BOOL DialogRegister::OnInitDialog()
{
	CDialog::OnInitDialog();
	AppInit();
	return TRUE;
}
void DialogRegister::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DialogRegister, CDialogEx)
ON_BN_CLICKED(IDOK, &DialogRegister::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &DialogRegister::OnBnClickedCancel)
END_MESSAGE_MAP()

// DialogRegister ???????????

void DialogRegister::OnBnClickedOk()
{
	CString str = "";
	GetDlgItemText(IDC_EDIT_RC, str);
	std::string STDStr(CW2A(str.GetString()));
	if (RegisterApp(STDStr) == false)
	{
		MessageBox(_T("????????"));
	}
	else
	{
		DialogRegister::IsRegister = true;
		CDialogEx::OnOK();
	}
}

void DialogRegister::OnBnClickedCancel()
{
	// TODO:  ??????????????????????
	CDialogEx::OnCancel();
}
