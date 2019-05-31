// CUnlockDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "CUnlockDlg.h"
#include "afxdialogex.h"


// CUnlockDlg 对话框

IMPLEMENT_DYNAMIC(CUnlockDlg, CDialogEx)

CUnlockDlg::CUnlockDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CUnlockDlg::~CUnlockDlg()
{
}

void CUnlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnlockDlg, CDialogEx)
END_MESSAGE_MAP()


// CUnlockDlg 消息处理程序


BOOL CUnlockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFont* ffont1;
	ffont1 = new CFont;
	ffont1->CreateFont(30, 0, 0, 0, 700, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"arial");
	GetDlgItem(IDC_SUREUNLOCK)->SetFont(ffont1);
	SetDlgItemText(IDC_SUREUNLOCK, L"确定解锁？？");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
