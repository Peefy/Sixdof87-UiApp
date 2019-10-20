// DIALOGGenerate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGGenerate.h"
#include "afxdialogex.h"
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

#define pi 3.1415926

#define fMax 20.00
#define fMin 0.00
#define DefaultFileName "Generate_RoadSpectrum.txt"

#include "signal/wavegenerator.h"

Signal::WaveGenerator waveGenerator;

IMPLEMENT_DYNAMIC(CDIALOGGenerate, CDialogEx)

CDIALOGGenerate::CDIALOGGenerate(CWnd *pParent /*=NULL*/)
	: CDialogEx(CDIALOGGenerate::IDD, pParent), m_dNum0(0), m_dNum1(0), m_dNum2(0), m_dNum3(0), m_dNum4(0), m_dNum5(0)

	  ,
	  m_chSaveDir(_T("")), m_FileName(_T(DefaultFileName)), m_Radio1(FALSE), m_Radio4(FALSE), m_Radio7(FALSE), m_Radio10(FALSE), m_Radio8(FALSE), m_Radio17(FALSE)
{
	m_dNum0 = 10.00;
	m_dNum1 = 10.00;
	m_dNum2 = 10.00;
	m_dNum3 = 10.00;
	m_dNum4 = 10.00;
	m_dNum5 = 10.00;
}

CDIALOGGenerate::~CDIALOGGenerate()
{
}

void CDIALOGGenerate::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_dNum0);
	DDX_Text(pDX, IDC_EDIT6, m_dNum1);
	DDX_Text(pDX, IDC_EDIT8, m_dNum2);
	DDX_Text(pDX, IDC_EDIT10, m_dNum3);
	DDX_Text(pDX, IDC_EDIT12, m_dNum4);
	DDX_Text(pDX, IDC_EDIT15, m_dNum5);
	DDV_MinMaxDouble(pDX, m_dNum0, fMin, fMax);
	DDV_MinMaxDouble(pDX, m_dNum1, fMin, fMax);
	DDV_MinMaxDouble(pDX, m_dNum2, fMin, fMax);
	DDV_MinMaxDouble(pDX, m_dNum3, fMin, fMax);
	DDV_MinMaxDouble(pDX, m_dNum4, fMin, fMax);
	DDV_MinMaxDouble(pDX, m_dNum5, fMin, fMax);
	DDX_Text(pDX, IDC_EDIT2, m_chSaveDir);
	DDX_Text(pDX, IDC_EDIT3, m_FileName);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio1);
	DDX_Radio(pDX, IDC_RADIO4, m_Radio4);
	DDX_Radio(pDX, IDC_RADIO7, m_Radio7);
	DDX_Radio(pDX, IDC_RADIO10, m_Radio10);
	DDX_Radio(pDX, IDC_RADIO8, m_Radio8);
	DDX_Radio(pDX, IDC_RADIO17, m_Radio17);
}

BEGIN_MESSAGE_MAP(CDIALOGGenerate, CDialogEx)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDIALOGGenerate::OnDeltaposSpin1)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDIALOGGenerate::OnDeltaposSpin3)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CDIALOGGenerate::OnDeltaposSpin4)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CDIALOGGenerate::OnDeltaposSpin5)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CDIALOGGenerate::OnDeltaposSpin6)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CDIALOGGenerate::OnDeltaposSpin7)
ON_BN_CLICKED(IDC_BUTTON5, &CDIALOGGenerate::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON3, &CDIALOGGenerate::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON1, &CDIALOGGenerate::OnBnClickedButton1)
END_MESSAGE_MAP()

// CDIALOGGenerate ��Ϣ�������

void CDIALOGGenerate::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_dNum0 = m_dNum0 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
	{
		m_dNum0 = m_dNum0 - 0.01;
	}
	if (m_dNum0 > fMax || m_dNum0 < fMin)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);

	*pResult = 0;
}

void CDIALOGGenerate::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
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
	if (m_dNum1 > fMax || m_dNum1 < fMin)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDIALOGGenerate::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
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
	if (m_dNum2 > fMax || m_dNum2 < fMin)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDIALOGGenerate::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData(TRUE))
	{
		m_dNum3 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
	{
		m_dNum3 = m_dNum3 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
	{
		m_dNum3 = m_dNum3 - 0.01;
	}
	if (m_dNum3 > fMax || m_dNum3 < fMin)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDIALOGGenerate::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData(TRUE))
	{
		m_dNum4 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
	{
		m_dNum4 = m_dNum4 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
	{
		m_dNum4 = m_dNum4 - 0.01;
	}
	if (m_dNum4 > fMax || m_dNum4 < fMin)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDIALOGGenerate::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData(TRUE))
	{
		m_dNum5 = 0;
		UpdateData(FALSE);
		return;
	}
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
	{
		m_dNum5 = m_dNum5 + 0.01;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ, ˵�������Spin�����¼�ͷ
	{
		m_dNum5 = m_dNum5 - 0.01;
	}
	if (m_dNum5 > fMax || m_dNum5 < fMin)
	{
		UpdateData(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDIALOGGenerate::OnBnClickedButton5() //��ѡ��Ŀ¼����ť
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
	if (::SHGetPathFromIDList(pidl, dirBuff))	//��ITEMIDLIST�еõ�Ŀ¼��������·��
	{
		m_chSaveDir = dirBuff;
		UpdateData(false); 
	}
}

void CDIALOGGenerate::OnBnClickedButton3() //����������ļ�����ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(false);
	if (m_chSaveDir == _T(""))
	{
		MessageBox(_T("����·������Ϊ�գ�"));
		return;
	}

	GetDlgItemText(IDC_EDIT3, m_FileName);
	double Time = 5;
	double fx = 0, fy = 0, fz = 0, fyaw = 0.2, froll = 0.3, fpitch = 0.5;
	double xval = 0;
	double yval = 0;
	double zval = 0;
	double rollval = 5;
	double yawval = 3;
	double pitchval = 4;
	double delta_t = waveGenerator.GetSampleTime();
	waveGenerator.Amp.SetData(xval, yval, zval, yawval, rollval, pitchval);
	waveGenerator.Freq.SetData(fx, fy, fz, fyaw, froll, fpitch);
	waveGenerator.ClearCountTime();

	UpdateData(TRUE);


	ofstream fout(m_chSaveDir + "\\" + m_FileName);
	if (fout.is_open())
	{
		for (int i = 0; i < Time / delta_t; i++)
		{
			fout << waveGenerator.GetNext().ToString().c_str() << "\n";
		}
		fout.flush();
		fout.close();
		AfxMessageBox(_T("����ļ����ɳɹ���"));
	}
	else
	{
		AfxMessageBox(_T("�ļ���ʧ�ܣ�"));
	}
}
void CDIALOGGenerate::OnBnClickedButton1() //������Ĭ��Ŀ¼����ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi; //BROWSEINFO�ṹ��
	wchar_t Buffer[512] = _T("");
	wchar_t dirBuff[256];

	bi.hwndOwner = m_hWnd; //m_hWnd��ĳ���������
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;					 //����ѡ���Ŀ¼���Ļ�����
	bi.lpszTitle = _T("ѡ���ļ������Ĭ��Ŀ¼"); //�����Ĵ��ڵ�������ʾ
	bi.ulFlags = BIF_NEWDIALOGSTYLE;			 //ֻ����Ŀ¼��������־��MSDN
	bi.lpfn = NULL;								 //�ص���������ʱ������
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST *pidl = ::SHBrowseForFolder(&bi); //��ʾ�������ڣ�ITEMIDLIST����Ҫ
	if (::SHGetPathFromIDList(pidl, dirBuff))	//��ITEMIDLIST�еõ�Ŀ¼��������·��
	{
		//�ɹ�
		m_chSaveDir = dirBuff;
		//theApp.WriteProfileStringW(_T(WORKER_PARAM),_T(TENEMENT_DIR),m_chSaveDir);        // дע�����
		UpdateData(false); // д�ؼ�
	}
}
