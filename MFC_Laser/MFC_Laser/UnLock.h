#pragma once


// UnLock �Ի���

class UnLock : public CDialogEx
{
	DECLARE_DYNAMIC(UnLock)

public:
	UnLock(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UnLock();

// �Ի�������
	enum { IDD = IDD_TOLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUlcancelButton();
	afx_msg void OnBnClickedUlsureButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
};
