// DIALOGReproduce.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ECATSample.h"
#include "DIALOGReproduce.h"
#include "afxdialogex.h"
#define DefaultPath "C:\\Users\\w\\Desktop\\�����ɶ�̨\\filetest"

// CDIALOGReproduce �Ի���

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


// CDIALOGReproduce ��Ϣ�������


void CDIALOGReproduce::OnBnClickedButton2()//"ѡ���ļ�"
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TargetHistoryPath = "";//�ļ�ѡ�����
	CString defaultDir = DefaultPath;   //Ĭ�ϴ򿪵��ļ�·��    
	CString defaultFile = L"test.txt";         //Ĭ�ϴ򿪵��ļ���    
    CFileDialog dlg(TRUE,_T("txt"),defaultDir + "\\" + defaultFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("�����ļ�|*.txt||"));
    if (dlg.DoModal()==IDOK)
	{  
         m_TargetHistoryPath = dlg.GetPathName();
          if (m_TargetHistoryPath == "")
         {
             MessageBox(_T("δѡ���ļ���"));
             return;
         }
     }
     //((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_SourceHistoryPath);
	 m_TargetPath=m_TargetHistoryPath;
	 UpdateData(false);
}


void CDIALOGReproduce::OnBnClickedButton1()//��ģ����ԡ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDIALOGReproduce::OnBnClickedButton3()//���������С�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
