#pragma once


// CLogInDlg 对话框

class CLogInDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogInDlg)

public:
	CLogInDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLogInDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGINDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL CheckUser(CString Name, CString Password, CString * Limits);
	BOOL GetUserType(CString Name, CString Password, CString * Limits);
	CString m_s_name;
	CString m_s_pwd;
	virtual BOOL OnInitDialog();
	CEdit m_UserID;
};
