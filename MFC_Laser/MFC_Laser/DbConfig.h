#pragma once


// DbConfig 对话框

class DbConfig : public CDialog
{
	DECLARE_DYNAMIC(DbConfig)

public:
	DbConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DbConfig();

// 对话框数据
	enum { IDD = IDD_DBCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_serverip;
	CString m_port;
	CString m_username;
	CString m_password;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnBnClickedConnectsaveButton();
};
