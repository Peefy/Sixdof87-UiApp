#pragma once

#include "resource.h"
// CDialogSignalRun �Ի���

class CDialogSignalRun : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSignalRun)

public:
	CDialogSignalRun(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogSignalRun();

// �Ի�������
	enum { IDD = IDD_SIGNAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
};
