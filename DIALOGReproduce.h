#pragma once



using namespace std;

// CDIALOGReproduce �Ի���
class CDIALOGReproduce : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOGReproduce)

public:
	CDIALOGReproduce(CWnd* pParent = NULL); 
	virtual ~CDIALOGReproduce();

// �Ի�������
	enum { IDD = IDD_REPRODUCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();

	CString m_TargetHistoryPath;

	CString m_TargetPath;
	CEdit m_Time;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton4();
public:
	//Signal::RoadSpectrum* RoadSpectrum;
	void DataFromFile();

};
