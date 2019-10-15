// DialogSignalRun.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogSignalRun.h"
#include "afxdialogex.h"


// CDialogSignalRun 对话框

IMPLEMENT_DYNAMIC(CDialogSignalRun, CDialogEx)

CDialogSignalRun::CDialogSignalRun(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSignalRun::IDD, pParent)
{

}

CDialogSignalRun::~CDialogSignalRun()
{
}

void CDialogSignalRun::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogSignalRun, CDialogEx)
END_MESSAGE_MAP()


// CDialogSignalRun 消息处理程序
