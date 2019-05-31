// UnLock.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "UnLock.h"
#include "afxdialogex.h"
#include "MFC_LaserDlg.h"


// UnLock 对话框
extern CMFC_LaserDlg * parent;


IMPLEMENT_DYNAMIC(UnLock, CDialogEx)

UnLock::UnLock(CWnd* pParent /*=NULL*/)
	: CDialogEx(UnLock::IDD, pParent)
{

}

UnLock::~UnLock()
{
}

void UnLock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UnLock, CDialogEx)
	ON_BN_CLICKED(IDC_ULCancel_Button, &UnLock::OnBnClickedUlcancelButton)
	ON_BN_CLICKED(IDC_ULSure_BUTTON, &UnLock::OnBnClickedUlsureButton)
END_MESSAGE_MAP()


// UnLock 消息处理程序

//取消按钮
void UnLock::OnBnClickedUlcancelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void UnLock::OnBnClickedUlsureButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString inputpassword;
	GetDlgItemText(IDC_ULPassword_EDIT, inputpassword);
	if (inputpassword == "jimi"){
		parent->UnlockContent();
		CDialog::OnCancel();
	}
	else
	{
		MessageBox(_T("密码错误"), _T("提示"), NULL);
		SetDlgItemText(IDC_ULPassword_EDIT, _T(""));
		GetDlgItem(IDC_ULPassword_EDIT)->SetFocus();
		return;
	}
}


BOOL UnLock::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN){

		int nVirtKey = (int)pMsg->wParam;

		if (nVirtKey == VK_RETURN)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_ULPassword_EDIT)->GetSafeHwnd()){
				UnLock::OnBnClickedUlsureButton();
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void UnLock::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
