// DIALOGProcessing.cpp : ʵ���ļ�
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

// CDIALOGProcessing ��Ϣ�������

BOOL CDIALOGProcessing::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��Ͽ���ʾ����
	// Ϊ��Ͽ�ؼ����б������б��λ���ס�
	m_combo1.AddString(_T("λ����"));
	// Ϊ��Ͽ�ؼ����б������б��**��2��
	m_combo1.AddString(_T("**��2"));
	// ����Ͽ�ؼ����б��������Ϊ1��λ�ò����б��**��3��
	m_combo1.InsertString(1, _T("**��3"));
	// Ĭ��ѡ���һ��
	m_combo1.SetCurSel(2);

	m_combo2.InsertString(0, _T("һ����"));
	m_combo2.InsertString(1, _T("**��"));
	// Ĭ��ѡ���һ��
	m_combo2.SetCurSel(0);

	m_combo3.InsertString(0, _T("��ͨ"));
	m_combo3.InsertString(1, _T("����"));
	m_combo3.InsertString(2, _T("��ͨ"));
	m_combo3.InsertString(3, _T("��ͨ"));
	// Ĭ��ѡ���һ��
	m_combo3.SetCurSel(0);

	return TRUE; // return TRUE unless you set the focus to a control
				 // �쳣: OCX ����ҳӦ���� FALSE
}

void CDIALOGProcessing::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDIALOGProcessing::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData(TRUE))
	{
		m_dNum0 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
	{
		m_dNum0 = m_dNum0 + 1;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData(TRUE))
	{
		m_dNum1 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
	{
		m_dNum1 = m_dNum1 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData(TRUE))
	{
		m_dNum2 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
	{
		m_dNum2 = m_dNum2 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
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

//��Դ�ļ�ѡ��
void CDIALOGProcessing::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SourceHistoryPath = "";												//�ļ�ѡ�����
	CString defaultDir = _T("C:\\filetest"); //Ĭ�ϴ򿪵��ļ�·��
	CString defaultFile = L"test.txt";										//Ĭ�ϴ򿪵��ļ���
	CFileDialog dlg(TRUE, _T("txt"), defaultDir + "\\" + defaultFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("�����ļ�|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		m_SourceHistoryPath = dlg.GetPathName();
		if (m_SourceHistoryPath == "")
		{
			MessageBox(_T("δѡ���ļ���"));
			return;
		}
	}
	//((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_SourceHistoryPath);
	m_SourcePath = m_SourceHistoryPath;
	UpdateData(false);
}

//ѡ������Ŀ���׵�Ŀ¼
void CDIALOGProcessing::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi; //BROWSEINFO�ṹ��
	wchar_t Buffer[512] = _T("");
	wchar_t dirBuff[256];

	bi.hwndOwner = m_hWnd; //m_hWnd��ĳ���������
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;			   //����ѡ���Ŀ¼���Ļ�����
	bi.lpszTitle = _T("ѡ��Ҫ�����λ��"); //�����Ĵ��ڵ�������ʾ
	bi.ulFlags = BIF_NEWDIALOGSTYLE;	   //ֻ����Ŀ¼��������־��MSDN
	bi.lpfn = NULL;						   //�ص���������ʱ������
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST *pidl = ::SHBrowseForFolder(&bi); //��ʾ�������ڣ�ITEMIDLIST����Ҫ
	if (::SHGetPathFromIDList(pidl, dirBuff))	 //��ITEMIDLIST�еõ�Ŀ¼��������·��
	{
		//�ɹ�
		m_TargetDir = dirBuff;
		//SetCurrentDirectory(m_TargetDir); // ���õ�ѡ����Ŀ¼
		UpdateData(false);				  // д�ؼ�
	}
}

//·�״���
void CDIALOGProcessing::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ReadFile(); //��Դ�׵�SourceBuf
	//·�״���
	TestProcessing();

	WriteFile(); //��TargetBufд��Ŀ��·���ļ�target.data��
}

//��������Դ������/////////////////////////////////////////
void CDIALOGProcessing::ReadFile()
{

	ifstream fin(m_SourcePath);		//���ļ�������
	double *ptr = &SourceBuf[0][0]; //����
	int i = 0;
	if (!fin.is_open())
	{
		MessageBox(_T("δ�ҵ��ļ�!"));
		return;
	}
	while (!fin.eof() && i < MAX_PROCESS_LINE * ColNum)
	{
		fin >> *ptr; //����ǰ��ĵ����������Ӧ��ptrλ�õ���ֵ��
		ptr++;
		i++;
	}
	fin.close();
}

//д��Ŀ��·���ļ�target.data////////////////////////////////////////����???
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
		double *ptr = &TargetBuf[0][0]; //����
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
		AfxMessageBox(_T("Ŀ�������ɳɹ���") + TargetFileName);
	}
	else
	{
		AfxMessageBox(_T("Ŀ��������ʧ�ܣ�"));
	}
}
//·�״�����
void CDIALOGProcessing::Processing()
{
	//������ �����޸�Ϊmemcpy��ʱ�临�Ӷ�O(1)
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
	// ����
	ASSERT(MAX_PROCESS_LINE < RowNum); 
	// �ڴ濽��
	memmove(TargetBuf, SourceBuf, sizeof(double) * MAX_PROCESS_LINE * ColNum); 
}


