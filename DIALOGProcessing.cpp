// DIALOGProcessing.cpp : 实现文件
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGProcessing.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>

#include "signal/roadspectrum.h"

using namespace std;

#define MAX_PROCESS_LINE 1000

double SourceBuf[RowNum][ColNum] = {{0.0}};
double TargetBuf[RowNum][ColNum] = {{0.0}};

IMPLEMENT_DYNAMIC(CDIALOGProcessing, CDialogEx)

CDIALOGProcessing::CDIALOGProcessing(CWnd *pParent /*=NULL*/)
	: CDialogEx(CDIALOGProcessing::IDD, pParent), m_dNum0(1), m_dNum1(20.00), m_dNum2(0.001), m_TargetDir(_T("")), m_SourcePath(_T(""))
{
}

CDIALOGProcessing::~CDIALOGProcessing()
{
}

void CDIALOGProcessing::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	DDX_Control(pDX, IDC_COMBO2, m_combo2);
	DDX_Control(pDX, IDC_COMBO3, m_combo3);
	DDX_Text(pDX, IDC_EDIT5, m_dNum0);
	DDX_Text(pDX, IDC_EDIT6, m_dNum1);
	DDX_Text(pDX, IDC_EDIT7, m_dNum2);
	DDV_MinMaxInt(pDX, m_dNum0, dMin0, dMax0);
	DDV_MinMaxDouble(pDX, m_dNum1, dMin1, dMax1);
	DDV_MinMaxDouble(pDX, m_dNum2, dMin2, dMax2);
	DDX_Text(pDX, IDC_EDIT2, m_TargetDir);
	DDX_Text(pDX, IDC_EDIT1, m_SourcePath);
}

BEGIN_MESSAGE_MAP(CDIALOGProcessing, CDialogEx)
ON_CBN_SELCHANGE(IDC_COMBO1, &CDIALOGProcessing::OnCbnSelchangeCombo1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDIALOGProcessing::OnDeltaposSpin1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDIALOGProcessing::OnDeltaposSpin3)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CDIALOGProcessing::OnDeltaposSpin5)
ON_BN_CLICKED(IDC_BUTTON2, &CDIALOGProcessing::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON3, &CDIALOGProcessing::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON1, &CDIALOGProcessing::OnBnClickedButton1)
END_MESSAGE_MAP()

// CDIALOGProcessing 消息处理程序

BOOL CDIALOGProcessing::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//组合框显示内容
	// 为组合框控件的列表框添加列表项“位移谱”
	m_combo1.AddString(_T("位移谱"));
	// 为组合框控件的列表框添加列表项“**谱2”
	m_combo1.AddString(_T("**谱2"));
	// 在组合框控件的列表框中索引为1的位置插入列表项“**谱3”
	m_combo1.InsertString(1, _T("**谱3"));
	// 默认选择第一项
	m_combo1.SetCurSel(2);

	m_combo2.InsertString(0, _T("一步法"));
	m_combo2.InsertString(1, _T("**法"));
	// 默认选择第一项
	m_combo2.SetCurSel(0);

	m_combo3.InsertString(0, _T("带通"));
	m_combo3.InsertString(1, _T("带阻"));
	m_combo3.InsertString(2, _T("低通"));
	m_combo3.InsertString(3, _T("高通"));
	// 默认选择第一项
	m_combo3.SetCurSel(0);

	return TRUE; // return TRUE unless you set the focus to a control
				 // 异常: OCX 属性页应返回 FALSE
}

void CDIALOGProcessing::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDIALOGProcessing::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData(TRUE))
	{
		m_dNum0 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
	{
		m_dNum0 = m_dNum0 + 1;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为, 说明点击了Spin的往下箭头
	{
		m_dNum0 = m_dNum0 - 1;
	}
	if (m_dNum0 > dMax0 || m_dNum0 < dMin0)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDIALOGProcessing::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData(TRUE))
	{
		m_dNum1 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
	{
		m_dNum1 = m_dNum1 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为, 说明点击了Spin的往下箭头
	{
		m_dNum1 = m_dNum1 - 0.01;
	}
	if (m_dNum1 > dMax1 || m_dNum1 < dMin1)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDIALOGProcessing::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData(TRUE))
	{
		m_dNum2 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
	{
		m_dNum2 = m_dNum2 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为, 说明点击了Spin的往下箭头
	{
		m_dNum2 = m_dNum2 - 0.01;
	}
	if (m_dNum2 > dMax2 || m_dNum2 < dMin2)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

//谱源文件选择
void CDIALOGProcessing::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SourceHistoryPath = "";												//文件选择清空
	CString defaultDir = _T("C:\\filetest"); //默认打开的文件路径
	CString defaultFile = L"test.txt";										//默认打开的文件名
	CFileDialog dlg(TRUE, _T("txt"), defaultDir + "\\" + defaultFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("数据文件|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		m_SourceHistoryPath = dlg.GetPathName();
		if (m_SourceHistoryPath == "")
		{
			MessageBox(_T("未选择文件！"));
			return;
		}
	}
	//((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_SourceHistoryPath);
	m_SourcePath = m_SourceHistoryPath;
	UpdateData(false);
}

//选择生成目标谱的目录
void CDIALOGProcessing::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi; //BROWSEINFO结构体
	wchar_t Buffer[512] = _T("");
	wchar_t dirBuff[256];

	bi.hwndOwner = m_hWnd; //m_hWnd你的程序主窗口
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;			   //返回选择的目录名的缓冲区
	bi.lpszTitle = _T("选择要保存的位置"); //弹出的窗口的文字提示
	bi.ulFlags = BIF_NEWDIALOGSTYLE;	   //只返回目录。其他标志看MSDN
	bi.lpfn = NULL;						   //回调函数，有时很有用
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST *pidl = ::SHBrowseForFolder(&bi); //显示弹出窗口，ITEMIDLIST很重要
	if (::SHGetPathFromIDList(pidl, dirBuff))	 //在ITEMIDLIST中得到目录名的整个路径
	{
		//成功
		m_TargetDir = dirBuff;
		//SetCurrentDirectory(m_TargetDir); // 设置到选定的目录
		UpdateData(false);				  // 写控件
	}
}

//路谱处理
void CDIALOGProcessing::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadFile(); //读源谱到SourceBuf
	//路谱处理
	TestProcessing();

	WriteFile(); //将TargetBuf写到目标路谱文件target.data中
}

//读二进制源谱数据/////////////////////////////////////////
void CDIALOGProcessing::ReadFile()
{

	ifstream fin(m_SourcePath);		//打开文件流对象
	double *ptr = &SourceBuf[0][0]; //定义
	int i = 0;
	if (!fin.is_open())
	{
		MessageBox(_T("未找到文件!"));
		return;
	}
	while (!fin.eof() && i < MAX_PROCESS_LINE * ColNum)
	{
		fin >> *ptr; //这个是把文档里面的数对应在ptr位置的数值上
		ptr++;
		i++;
	}
	fin.close();
}

//写入目标路谱文件target.data////////////////////////////////////////待改???
void CDIALOGProcessing::WriteFile()
{
	CTime tm = CTime::GetCurrentTime();
	CString TargetFileName;
	TargetFileName.Format(_T("RoadSpectrumDeal%d_%d_%d_%d_%d_%d.txt"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	//CString TargetFileName=_T("\\target.txt");
	ofstream fout(m_TargetDir + "\\" + TargetFileName);

	ASSERT(MAX_PROCESS_LINE < RowNum);
	if (fout.is_open())
	{
		double *ptr = &TargetBuf[0][0]; //定义
		for (int i = 0; i < MAX_PROCESS_LINE; i++)
		{
			for (int j = 0; j < ColNum - 1; j++)
			{
				fout << *ptr << " ";
				ptr++;
			}
			fout << *ptr;
			fout << endl;
			ptr++;
		}
		fout.flush();
		fout.close();
		AfxMessageBox(_T("目标谱生成成功！") + TargetFileName);
	}
	else
	{
		AfxMessageBox(_T("目标谱生成失败！"));
	}
}
//路谱处理函数
void CDIALOGProcessing::Processing()
{
	//不处理 建议修改为memcpy，时间复杂度O(1)
	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{
			TargetBuf[i][j] = SourceBuf[i][j];
		}
	}
}

void CDIALOGProcessing::TestProcessing()
{
	// 断言
	ASSERT(MAX_PROCESS_LINE < RowNum); 
	// 内存拷贝
	memmove(TargetBuf, SourceBuf, sizeof(double) * MAX_PROCESS_LINE * ColNum); 
}


