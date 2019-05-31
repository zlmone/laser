// UnLock.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "UnLock.h"
#include "afxdialogex.h"
#include "MFC_LaserDlg.h"


// UnLock �Ի���
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


// UnLock ��Ϣ�������

//ȡ����ť
void UnLock::OnBnClickedUlcancelButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void UnLock::OnBnClickedUlsureButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString inputpassword;
	GetDlgItemText(IDC_ULPassword_EDIT, inputpassword);
	if (inputpassword == "jimi"){
		parent->UnlockContent();
		CDialog::OnCancel();
	}
	else
	{
		MessageBox(_T("�������"), _T("��ʾ"), NULL);
		SetDlgItemText(IDC_ULPassword_EDIT, _T(""));
		GetDlgItem(IDC_ULPassword_EDIT)->SetFocus();
		return;
	}
}


BOOL UnLock::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
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
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}
