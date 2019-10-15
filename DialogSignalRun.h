#pragma once

#include "resource.h"
// CDialogSignalRun 对话框

class CDialogSignalRun : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSignalRun)

public:
	CDialogSignalRun(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogSignalRun();

// 对话框数据
	enum { IDD = IDD_SIGNAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
};
