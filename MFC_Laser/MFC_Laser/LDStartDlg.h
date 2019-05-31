#pragma once


// LDStartDlg 对话框

class LDStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LDStartDlg)

public:
	LDStartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LDStartDlg();

// 对话框数据
	enum { IDD = IDD_LDSTART_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
