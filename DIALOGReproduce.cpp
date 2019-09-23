// DIALOGReproduce.cpp : 实现文件
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGReproduce.h"
#include "afxdialogex.h"
#define DefaultPath "C:\\Users\\w\\Desktop\\六自由度台\\filetest"

// CDIALOGReproduce 对话框

IMPLEMENT_DYNAMIC(CDIALOGReproduce, CDialogEx)

CDIALOGReproduce::CDIALOGReproduce(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDIALOGReproduce::IDD, pParent)
	, m_TargetPath(_T(""))
{

}

CDIALOGReproduce::~CDIALOGReproduce()
{
}

void CDIALOGReproduce::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_TargetPath);
	DDX_Control(pDX, IDC_EDIT1, m_Time);
}


BEGIN_MESSAGE_MAP(CDIALOGReproduce, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDIALOGReproduce::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDIALOGReproduce::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDIALOGReproduce::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDIALOGReproduce 消息处理程序


void CDIALOGReproduce::OnBnClickedButton2()//"选择文件"
{
	// TODO: 在此添加控件通知处理程序代码
	m_TargetHistoryPath = "";//文件选择清空
	CString defaultDir = DefaultPath;   //默认打开的文件路径    
	CString defaultFile = L"test.txt";         //默认打开的文件名    
    CFileDialog dlg(TRUE,_T("txt"),defaultDir + "\\" + defaultFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("数据文件|*.txt||"));
    if (dlg.DoModal()==IDOK)
	{  
         m_TargetHistoryPath = dlg.GetPathName();
          if (m_TargetHistoryPath == "")
         {
             MessageBox(_T("未选择文件！"));
             return;
         }
     }
     //((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_SourceHistoryPath);
	 m_TargetPath=m_TargetHistoryPath;
	 UpdateData(false);
}


void CDIALOGReproduce::OnBnClickedButton1()//“模拟测试”
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDIALOGReproduce::OnBnClickedButton3()//“复现运行”
{
	// TODO: 在此添加控件通知处理程序代码
}
