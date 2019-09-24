// DIALOGGenerate.cpp : 实现文件
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
//#define DefaultFilePath "C:\\Users\\w\\Desktop\\六自由度台\\filetest"

// CDIALOGGenerate 对话框

IMPLEMENT_DYNAMIC(CDIALOGGenerate, CDialogEx)

CDIALOGGenerate::CDIALOGGenerate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDIALOGGenerate::IDD, pParent)
	, m_dNum0(0)
	, m_dNum1(0)
	, m_dNum2(0)
	, m_dNum3(0)
	, m_dNum4(0)
	, m_dNum5(0)
	
	, m_chSaveDir(_T(""))
	, m_FileName(_T(DefaultFileName))
	, m_test(_T(""))
	, m_Radio1(FALSE)
	, m_Radio4(FALSE)
	, m_Radio7(FALSE)
	, m_Radio10(FALSE)
	, m_Radio8(FALSE)
	, m_Radio17(FALSE)
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

void CDIALOGGenerate::DoDataExchange(CDataExchange* pDX)
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
//	DDX_Text(pDX, IDC_EDIT13, m_test);
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


// CDIALOGGenerate 消息处理程序


void CDIALOGGenerate::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(!UpdateData(TRUE))
     {
         m_dNum0 = 0;
         UpdateData(FALSE);
         return;
     }
     if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
     {
         m_dNum0 = m_dNum0 + 0.01;
     }
     else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
     {
         m_dNum0 = m_dNum0 - 0.01;
     }
     if(m_dNum0 > fMax || m_dNum0 < fMin)
     {
         UpdateData(TRUE);
     }
     UpdateData(FALSE);

	*pResult = 0;
}


void CDIALOGGenerate::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(!UpdateData(TRUE))
     {
         m_dNum1 = 0;
         UpdateData(FALSE);
         return;
     }
     if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
     {
         m_dNum1 = m_dNum1 + 0.01;
     }
     else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
     {
         m_dNum1 = m_dNum1 - 0.01;
     }
     if(m_dNum1 > fMax || m_dNum1 < fMin)
     {
         UpdateData(TRUE);
     }
     UpdateData(FALSE);
	*pResult = 0;
}


void CDIALOGGenerate::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(!UpdateData(TRUE))
     {
         m_dNum2 = 0;
         UpdateData(FALSE);
         return;
     }
     if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
     {
         m_dNum2 = m_dNum2 + 0.01;
     }
     else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
     {
         m_dNum2 = m_dNum2 - 0.01;
     }
     if(m_dNum2> fMax || m_dNum2 < fMin)
     {
         UpdateData(TRUE);
     }
     UpdateData(FALSE);
	*pResult = 0;
}


void CDIALOGGenerate::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(!UpdateData(TRUE))
     {
         m_dNum3 = 0;
         UpdateData(FALSE);
         return;
     }
     if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
     {
         m_dNum3 = m_dNum3 + 0.01;
     }
     else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
     {
         m_dNum3 = m_dNum3 - 0.01;
     }
     if(m_dNum3 > fMax || m_dNum3 < fMin)
     {
         UpdateData(TRUE);
     }
     UpdateData(FALSE);
	*pResult = 0;
}


void CDIALOGGenerate::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(!UpdateData(TRUE))
     {
         m_dNum4 = 0;
         UpdateData(FALSE);
         return;
     }
     if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
     {
         m_dNum4 = m_dNum4 + 0.01;
     }
     else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
     {
         m_dNum4 = m_dNum4 - 0.01;
     }
     if(m_dNum4 > fMax || m_dNum4 < fMin)
     {
         UpdateData(TRUE);
     }
     UpdateData(FALSE);
	*pResult = 0;
}


void CDIALOGGenerate::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(!UpdateData(TRUE))
     {
         m_dNum5 = 0;
         UpdateData(FALSE);
         return;
     }
     if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
     {
         m_dNum5 = m_dNum5 + 0.01;
     }
     else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
     {
         m_dNum5 = m_dNum5 - 0.01;
     }
     if(m_dNum5 > fMax || m_dNum5 < fMin)
     {
         UpdateData(TRUE);
     }
     UpdateData(FALSE);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDIALOGGenerate::OnBnClickedButton5()//“选择目录”按钮
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;                                                    //BROWSEINFO结构体   
    wchar_t Buffer[512]=_T("");   
    wchar_t dirBuff[256];
 
    bi.hwndOwner=m_hWnd;                                            //m_hWnd你的程序主窗口
    bi.pidlRoot=NULL;   
    bi.pszDisplayName=Buffer;                                        //返回选择的目录名的缓冲区   
    bi.lpszTitle=_T("选择要保存的位置");                                //弹出的窗口的文字提示   
    bi.ulFlags= BIF_NEWDIALOGSTYLE;                                    //只返回目录。其他标志看MSDN   
    bi.lpfn=NULL;                                                   //回调函数，有时很有用   
    bi.lParam=0;   
	bi.iImage=0;   
    ITEMIDLIST* pidl=::SHBrowseForFolder(&bi);                        //显示弹出窗口，ITEMIDLIST很重要   
    if(::SHGetPathFromIDList(pidl,dirBuff))                            //在ITEMIDLIST中得到目录名的整个路径   
    {            
         //成功
        m_chSaveDir=dirBuff;
        SetCurrentDirectory(m_chSaveDir);                                // 设置到选定的目录
        //theApp.WriteProfileStringW(_T(WORKER_PARAM),_T(TENEMENT_DIR),m_chSaveDir);        // 写注册表？？？
        UpdateData(false);                // 写控件
	}   
    //FineNodeList();                                                // 刷新文件浏览？？？

/*	AfxGetApp()->WriteProfileString(_T("GenerateDlg"),_T("OpenDir"),m_chSaveDir);
    AfxGetApp()->GetProfileString(_T("GenerateDlg"),_T("OpenDir"),m_chSaveDir );
	*/
}


void CDIALOGGenerate::OnBnClickedButton3()//“生成随机文件”按钮
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(false);
	SetCurrentDirectory(m_chSaveDir); //设置到默认的目录m_chSaveDir
	GetDlgItemText(IDC_EDIT3,m_FileName);
	double Time=2;
	double t=0;
	double delta_t=0.001;
	double fx=1,fy=2,fz=3,fyaw=1,frow=1,fpitch=1;
	double x=0,y=0,z=0,yaw=0,row=0,pitch=0,dx,dy,dz,dyaw,drow,dpitch,ddx,ddy,ddz,ddyaw,ddrow,ddpitch;


	//复选框radio button：“路谱定义”——4组：m_Radio1,4,7,10（选中状态用0，1，2，3表示）；“强度”——m_Radio8，17
	//UpdateData(TRUE);
	//CString m_R1;
    //m_R1.Format(_T("the value of m_Radio1 is %d"),m_Radio1);
	//AfxMessageBox(m_R1);
	UpdateData(TRUE);


	//周期框读回double m_dNum0,1,2,3,4,5





	ofstream fout(m_FileName);
	if (fout.is_open()) {
		for (int i=0;i<Time/delta_t;i++){
		
		x=sin(2*pi*fx*t);
		y=sin(2*pi*fy*t);
		z=sin(2*pi*fz*t);
		yaw=sin(2*pi*fyaw*t);
		row=sin(2*pi*frow*t);
		pitch=sin(2*pi*fpitch*t);
		t+=delta_t;

		dx=(sin(2*pi*fx*t)-x)/delta_t;
		dy=(sin(2*pi*fy*t)-y)/delta_t;
		dz=(sin(2*pi*fz*t)-z)/delta_t;
		dyaw=(sin(2*pi*fyaw*t)-yaw)/delta_t;
		drow=(sin(2*pi*frow*t)-row)/delta_t;
		dpitch=(sin(2*pi*fpitch*t)-pitch)/delta_t;

		ddx=sin(2*pi*fx*(t+delta_t))-sin(2*pi*fx*t)-dx;
		ddy=sin(2*pi*fy*(t+delta_t))-sin(2*pi*fy*t)-dy;
		ddz=sin(2*pi*fz*(t+delta_t))-sin(2*pi*fz*t)-dz;
		ddyaw=sin(2*pi*fyaw*(t+delta_t))-sin(2*pi*fyaw*t)-dyaw;
		ddrow=sin(2*pi*frow*(t+delta_t))-sin(2*pi*frow*t)-drow;
		ddpitch=sin(2*pi*fpitch*(t+delta_t))-sin(2*pi*fpitch*t)-dpitch;

		fout<<x<<" "<<y<<" "<<z<<" "<<yaw<<" "<<row<<" "<<pitch<<" "<<dx<<" "<<dy<<" "<<dz<<" "<<dyaw<<" "<<drow<<" "<<dpitch<<" "<<ddx<<" "<<ddy<<" "<<ddz<<" "<<ddyaw<<" "<<ddrow<<" "<<ddpitch<<endl;
	}
	fout.flush();
	fout.close();
    AfxMessageBox(_T("随机文件生成成功！"));
	}
	else{
	AfxMessageBox(_T("文件打开失败！"));
	}

}
void CDIALOGGenerate::OnBnClickedButton1()//“设置默认目录”按钮
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;                                                    //BROWSEINFO结构体   
    wchar_t Buffer[512]=_T("");   
    wchar_t dirBuff[256];
 
    bi.hwndOwner=m_hWnd;                                            //m_hWnd你的程序主窗口
    bi.pidlRoot=NULL;   
    bi.pszDisplayName=Buffer;                                        //返回选择的目录名的缓冲区   
    bi.lpszTitle=_T("选择文件保存的默认目录");                                //弹出的窗口的文字提示   
    bi.ulFlags= BIF_NEWDIALOGSTYLE;                                    //只返回目录。其他标志看MSDN   
    bi.lpfn=NULL;                                                   //回调函数，有时很有用   
    bi.lParam=0;   
	bi.iImage=0;   
    ITEMIDLIST* pidl=::SHBrowseForFolder(&bi);                        //显示弹出窗口，ITEMIDLIST很重要   
    if(::SHGetPathFromIDList(pidl,dirBuff))                            //在ITEMIDLIST中得到目录名的整个路径   
    {            
         //成功
        m_chSaveDir=dirBuff;
        //theApp.WriteProfileStringW(_T(WORKER_PARAM),_T(TENEMENT_DIR),m_chSaveDir);        // 写注册表？？		
        UpdateData(false);                // 写控件
	}   
}

