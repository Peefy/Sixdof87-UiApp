﻿// ECATSampleDlg.h : header file
//

#if !defined(AFX_ECATSampleDLG_H__87C425E4_3161_4F6F_BAE6_700496040E60__INCLUDED_)
#define AFX_ECATSampleDLG_H__87C425E4_3161_4F6F_BAE6_700496040E60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CECATSampleDlg : public CDialog
{
// Construction
public:
	CECATSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CECATSampleDlg)
	enum { IDD = IDD_ECATSAMPLE_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECATSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CECATSampleDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void InitOpenGlControl();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBTNInitialCard();
	afx_msg void OnBTNFindSlave();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeCBONodeID();
	afx_msg void OnChkSvon();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnRalm();
	virtual void OnOK();
	afx_msg void OnBTNPMove();
	afx_msg void OnBTNMMove();
	afx_msg void OnChkAbs();
	afx_msg void OnSelchangeCBOCardNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL SetWindowPixelFormat(HDC hDC);   
	BOOL CreateViewGLContext(HDC hDC);     
	void RenderScene();                               
	void VisionDataDeal();
	void RenderSwitchStatus();
	void FillCtlColor(CWnd* cwnd, COLORREF color);
	void ShowSingleInitImage(int ctlId);
	void ShowSingleInitImage(CWnd* pic, float value);
	void ShowInitImage();
	void ShowImage();
	void ShowSingleImage(CWnd* cwnd, float value);
	void JudgeControlCommand();
	HDC hrenderDC;      //DC 
	HGLRC hrenderRC;  //RC  
	int PixelFormat; 
	afx_msg void OnBnClickedChkSvon2();
	afx_msg void OnBnClickedBtnMmove2();
	afx_msg void OnBnClickedBtnRise();
	afx_msg void OnBnClickedBtnMiddle();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStopme();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnDown();
	void OnCommandStopme();
	void EanbleButton(int isenable);
	void ChartInit();
	void KalmanFilterInit();
	void AppInit();
	void AppConfigInit();
	void RunTestMode();
	void RunJudgeRangeTestMode();
	void DataFromFile();
	double GetCEditNumber(int cEditId);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnResetme();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnSingleUp();
	afx_msg void OnBnClickedBtnSingleDown();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonTest3();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonProcessing();
	afx_msg void OnBnClickedButtonReproduce();
	afx_msg void OnBnClickedButtonData();
};


#endif
