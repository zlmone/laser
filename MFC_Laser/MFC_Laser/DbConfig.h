#pragma once


// DbConfig �Ի���

class DbConfig : public CDialog
{
	DECLARE_DYNAMIC(DbConfig)

public:
	DbConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DbConfig();

// �Ի�������
	enum { IDD = IDD_DBCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
