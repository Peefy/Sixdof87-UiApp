// DIALOGReproduce.cpp : 实现文件
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGReproduce.h"
#include "afxdialogex.h"

#include <fstream>

#define DefaultPath "C:\\"

// CDIALOGReproduce 对话框

#define MAX_REPRODUCE_LINE 10
#define DATA_COL_NUM       18

IMPLEMENT_DYNAMIC(CDIALOGReproduce, CDialogEx)

CDIALOGReproduce::CDIALOGReproduce(CWnd* pParent) : 
	CDialogEx(CDIALOGReproduce::IDD, pParent), m_TargetPath(_T(""))
{

}

CDIALOGReproduce::~CDIALOGReproduce()
{
}

void CDIALOGReproduce::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_TargetPath);
	DDX_Control(pDX, IDC_EDIT1, m_Time);
}

BEGIN_MESSAGE_MAP(CDIALOGReproduce, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON2, &CDIALOGReproduce::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON1, &CDIALOGReproduce::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON3, &CDIALOGReproduce::OnBnClickedButton3)
ON_BN_CLICKED(IDOK, &CDIALOGReproduce::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CDIALOGReproduce::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON4, &CDIALOGReproduce::OnBnClickedButton4)
END_MESSAGE_MAP()

// CDIALOGReproduce 消息处理程序

void CDIALOGReproduce::OnBnClickedButton2() //"选择文件"
{
	// TODO: 在此添加控件通知处理程序代码
	m_TargetHistoryPath = "";		   //文件选择清空
	CString defaultDir = DefaultPath;  //默认打开的文件路径
	CString defaultFile = L"test.txt"; //默认打开的文件名
	CFileDialog dlg(TRUE, _T("txt"), defaultDir + "\\" + defaultFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("数据文件|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		m_TargetHistoryPath = dlg.GetPathName();
		if (m_TargetHistoryPath == "")
		{
			MessageBox(_T("未选择文件！"));
			return;
		}
	}
	//((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_SourceHistoryPath);
	m_TargetPath = m_TargetHistoryPath;
	UpdateData(false);
}

void CDIALOGReproduce::OnBnClickedButton1() //“模拟测试”
{
	//DataFromFile();
}

void CDIALOGReproduce::OnBnClickedButton3() //“复现运行”
{
	//DataFromFile();
}

static double sourceBuf[DATA_COL_NUM] = {0};

void CDIALOGReproduce::DataFromFile()
{
	if (m_TargetPath == "") {
		AfxMessageBox(_T("未选择文件"));
		return;
	}

	ifstream fin(m_TargetPath);		//打开文件流对象
	double *ptr = &sourceBuf[0]; //定义
	int readcount = 0;
	int arrcount = 0;
	if (!fin.is_open())
	{
		AfxMessageBox(_T("未找到文件!"));
		return;
	}
	while (!fin.eof() && readcount < MAX_REPRODUCE_LINE * DATA_COL_NUM)
	{
		fin >> *ptr; //这个是把文档里面的数对应在ptr位置的数值上
		ptr++;
		readcount++;
		arrcount++;
		if (arrcount >= DATA_COL_NUM)
		{
			arrcount = 0;
			//RoadSpectrum->DataBuffer.push(Signal::RoadSpectrumData::FromArray(SourceBuf));
			ptr = &sourceBuf[0];
		}
	}
	fin.close();
}


void CDIALOGReproduce::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDIALOGReproduce::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDIALOGReproduce::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
}
