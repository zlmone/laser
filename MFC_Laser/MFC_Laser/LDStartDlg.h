#pragma once


// LDStartDlg �Ի���

class LDStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LDStartDlg)

public:
	LDStartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LDStartDlg();

// �Ի�������
	enum { IDD = IDD_LDSTART_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
