// SetRelative.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "SetRelative.h"
#include "afxdialogex.h"

//全局变量
BOOL RelativeIMEI2, RelativeIMEI3, RelativeIMEI4, RelativeIMEI5, RelativeIMEI6, RelativeIMEI7, RelativeIMEI8, RelativeIMEI9\
, RelativeIMEI10, RelativeIMEI11;
BOOL RelativeIMEI12, RelativeIMEI13;

// SetRelative 对话框

IMPLEMENT_DYNAMIC(SetRelative, CDialogEx)

SetRelative::SetRelative(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETRELATIVE, pParent)
	, m_IMEI2(FALSE)
	, m_IMEI3(FALSE)
	, m_IMEI4(FALSE)
	, m_IMEI5(FALSE)
	, m_IMEI6(FALSE)
	, m_IMEI7(FALSE)
	, m_IMEI8(FALSE)
	, m_IMEI9(FALSE)
	, m_IMEI11(FALSE)
	, m_IMEI12(FALSE)
	, m_IMEI13(FALSE)
	, m_VAL10(FALSE)
{

}

SetRelative::~SetRelative()
{
}

void SetRelative::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IMEI2, m_IMEI2);
	DDX_Check(pDX, IMEI3, m_IMEI3);
	DDX_Check(pDX, IDC_IMEI4, m_IMEI4);
	DDX_Check(pDX, IDC_IMEI5, m_IMEI5);
	DDX_Check(pDX, IDC_IMEI6, m_IMEI6);
	DDX_Check(pDX, IDC_IMEI7, m_IMEI7);
	DDX_Check(pDX, IDC_IMEI8, m_IMEI8);
	DDX_Check(pDX, IDC_IMEI9, m_IMEI9);
	DDX_Control(pDX, IDC_IMEI10, m_IMEI10);
	DDX_Check(pDX, IDC_IMEI11, m_IMEI11);
	DDX_Check(pDX, IDC_IMEI12, m_IMEI12);
	DDX_Check(pDX, IDC_IMEI13, m_IMEI13);
	DDX_Check(pDX, IDC_IMEI10, m_VAL10);
	DDX_Control(pDX, IMEI2, m_check2);
	DDX_Control(pDX, IMEI3, m_check3);
	DDX_Control(pDX, IDC_IMEI4, m_check4);
	DDX_Control(pDX, IDC_IMEI5, m_check5);
	DDX_Control(pDX, IDC_IMEI6, m_check6);
	DDX_Control(pDX, IDC_IMEI7, m_check7);
	DDX_Control(pDX, IDC_IMEI8, m_check8);
	DDX_Control(pDX, IDC_IMEI9, m_check9);
	DDX_Control(pDX, IDC_IMEI11, m_check11);
	DDX_Control(pDX, IDC_IMEI12, m_check12);
	DDX_Control(pDX, IDC_IMEI13, m_check13);
}


BEGIN_MESSAGE_MAP(SetRelative, CDialogEx)
	ON_BN_CLICKED(IDC_YES, &SetRelative::OnBnClickedYes)
	ON_BN_CLICKED(IDC_NO, &SetRelative::OnBnClickedNo)
END_MESSAGE_MAP()


// SetRelative 消息处理程序


void SetRelative::OnBnClickedYes()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	RelativeIMEI2 = m_check2.GetCheck();
	RelativeIMEI3 = m_check3.GetCheck();
	RelativeIMEI4 = m_check4.GetCheck();;
	RelativeIMEI5 = m_check5.GetCheck();;
	RelativeIMEI6 = m_check6.GetCheck();;
	RelativeIMEI7 = m_check7.GetCheck();;
	RelativeIMEI8 = m_check8.GetCheck();;
	RelativeIMEI9 = m_check9.GetCheck();;
	RelativeIMEI10 = m_IMEI10.GetCheck();
	RelativeIMEI11 = m_check11.GetCheck();;
	RelativeIMEI12 = m_check12.GetCheck();;
	RelativeIMEI13 = m_check13.GetCheck();;
	OnOK();
}


void SetRelative::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void SetRelative::OnBnClickedNo()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL SetRelative::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (RelativeIMEI2)
	{
		m_check2.SetCheck(1);
	}
	if (RelativeIMEI3)
	{
		m_check3.SetCheck(1);
	}
	if (RelativeIMEI4)
	{
		m_check4.SetCheck(1);
	}
	if (RelativeIMEI5)
	{
		m_check5.SetCheck(1);
	}
	if (RelativeIMEI6)
	{
		m_check6.SetCheck(1);
	}
	if (RelativeIMEI7)
	{
		m_check7.SetCheck(1);
	}
	if (RelativeIMEI8)
	{
		m_check8.SetCheck(1);
	}
	if (RelativeIMEI9)
	{
		m_check9.SetCheck(1);
	}
	if (RelativeIMEI10)
	{
		m_IMEI10.SetCheck(1);
	}
	if (RelativeIMEI11)
	{
		m_check11.SetCheck(1);
	}
	if (RelativeIMEI12)
	{
		m_check12.SetCheck(1);
	}
	if (RelativeIMEI13)
	{
		m_check13.SetCheck(1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
