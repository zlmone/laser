#pragma once


// UnLock 对话框

class UnLock : public CDialogEx
{
	DECLARE_DYNAMIC(UnLock)

public:
	UnLock(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UnLock();

// 对话框数据
	enum { IDD = IDD_TOLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUlcancelButton();
	afx_msg void OnBnClickedUlsureButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
};
