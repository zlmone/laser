#pragma once


// SetRelative 对话框

class SetRelative : public CDialogEx
{
	DECLARE_DYNAMIC(SetRelative)

public:
	SetRelative(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetRelative();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETRELATIVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedYes();
	virtual void OnOK();
	BOOL m_IMEI2;
	BOOL m_IMEI3;
	BOOL m_IMEI4;
	BOOL m_IMEI5;
	BOOL m_IMEI6;
	BOOL m_IMEI7;
	BOOL m_IMEI8;
	BOOL m_IMEI9;
	CButton m_IMEI10;
	BOOL m_IMEI11;
	BOOL m_IMEI12;
	BOOL m_IMEI13;
	BOOL m_VAL10;
	afx_msg void OnBnClickedNo();
	CButton m_check2;
	CButton m_check3;
	CButton m_check4;
	CButton m_check5;
	CButton m_check6;
	CButton m_check7;
	CButton m_check8;
	CButton m_check9;
	CButton m_check11;
	CButton m_check12;
	CButton m_check13;
	virtual BOOL OnInitDialog();
};
