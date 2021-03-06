#pragma once


// CMainWindow 对话框

class CMainWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CMainWindow)

public:
	CMainWindow(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainWindow();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual void OnOK();
	CMenu menu;
};
