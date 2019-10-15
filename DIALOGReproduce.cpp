// DIALOGReproduce.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGReproduce.h"
#include "afxdialogex.h"

#include <fstream>

#define DefaultPath "C:\\"

// CDIALOGReproduce �Ի���

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

// CDIALOGReproduce ��Ϣ�������

void CDIALOGReproduce::OnBnClickedButton2() //"ѡ���ļ�"
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TargetHistoryPath = "";		   //�ļ�ѡ�����
	CString defaultDir = DefaultPath;  //Ĭ�ϴ򿪵��ļ�·��
	CString defaultFile = L"test.txt"; //Ĭ�ϴ򿪵��ļ���
	CFileDialog dlg(TRUE, _T("txt"), defaultDir + "\\" + defaultFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("�����ļ�|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		m_TargetHistoryPath = dlg.GetPathName();
		if (m_TargetHistoryPath == "")
		{
			MessageBox(_T("δѡ���ļ���"));
			return;
		}
	}
	//((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_SourceHistoryPath);
	m_TargetPath = m_TargetHistoryPath;
	UpdateData(false);
}

void CDIALOGReproduce::OnBnClickedButton1() //��ģ����ԡ�
{
	//DataFromFile();
}

void CDIALOGReproduce::OnBnClickedButton3() //���������С�
{
	//DataFromFile();
}

static double sourceBuf[DATA_COL_NUM] = {0};

void CDIALOGReproduce::DataFromFile()
{
	if (m_TargetPath == "") {
		AfxMessageBox(_T("δѡ���ļ�"));
		return;
	}

	ifstream fin(m_TargetPath);		//���ļ�������
	double *ptr = &sourceBuf[0]; //����
	int readcount = 0;
	int arrcount = 0;
	if (!fin.is_open())
	{
		AfxMessageBox(_T("δ�ҵ��ļ�!"));
		return;
	}
	while (!fin.eof() && readcount < MAX_REPRODUCE_LINE * DATA_COL_NUM)
	{
		fin >> *ptr; //����ǰ��ĵ����������Ӧ��ptrλ�õ���ֵ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CDIALOGReproduce::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDIALOGReproduce::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
