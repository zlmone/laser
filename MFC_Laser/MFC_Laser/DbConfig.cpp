// DbConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "DbConfig.h"
#include "afxdialogex.h"
#include "ADOManage.h"


// DbConfig �Ի���

IMPLEMENT_DYNAMIC(DbConfig, CDialog)

DbConfig::DbConfig(CWnd* pParent /*=NULL*/)
	: CDialog(DbConfig::IDD, pParent)
{

}

DbConfig::~DbConfig()
{
}

void DbConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DbConfig, CDialog)
	ON_BN_CLICKED(IDC_Cancel_BUTTON, &DbConfig::OnBnClickedCancelButton)
	ON_BN_CLICKED(IDC_ConnectSave_BUTTON, &DbConfig::OnBnClickedConnectsaveButton)
END_MESSAGE_MAP()


// DbConfig ��Ϣ�������


BOOL DbConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CFileFind finder;
	CString str;
	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	if (ifFind)
	{
		GetPrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		SetDlgItemText(IDC_IPAddress, str);
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Port"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		SetDlgItemText(IDC_Port_EDIT, str);
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("UserName"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		SetDlgItemText(IDC_UserName_EDIT, str);
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		SetDlgItemText(IDC_Password_EDIT, str);
		str.ReleaseBuffer();
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


//ȡ����ť
void DbConfig::OnBnClickedCancelButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


void DbConfig::OnBnClickedConnectsaveButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	ADOManage adoManage;
	CFileFind finder;
	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	//��ȡIP��ַ
	GetDlgItemText(IDC_IPAddress, m_serverip);
	if (m_serverip == "" || m_serverip=="0.0.0.0"){
		MessageBox(L"IP��ַ����Ϊ��", L"��ʾ��Ϣ", NULL);	
		return;
	}
	else
	{
		adoManage.m_SourceIp = m_serverip;
	}
	//��ȡ�˿ں�
	GetDlgItemText(IDC_Port_EDIT, m_port);
	if (m_port == ""){
		MessageBox(L"�˿ںŲ���Ϊ��", L"��ʾ��Ϣ", NULL);
		return;
	}
	else
	{
		adoManage.m_Port = m_port;
	}
	//��ȡ�û���
	GetDlgItemText(IDC_UserName_EDIT, m_username);
	if (m_username == ""){
		MessageBox(L"�û�������Ϊ��", L"��ʾ��Ϣ", NULL);
		return;
	}
	else
	{
		adoManage.m_UserName = m_username;
	}
	//��ȡ����
	GetDlgItemText(IDC_Password_EDIT, m_password);
	if (m_password == ""){
		MessageBox(L"���벻��Ϊ��", L"��ʾ��Ϣ", NULL);
		return;
	}
	else
	{
		adoManage.m_Password = m_password;
	}
	if (!ifFind)
	{
		GetDlgItemText(IDC_IPAddress, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_Port_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Port"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_UserName_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("UserName"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_Password_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Password"), str, _T(".\\SystemInfo.ini"));
	}
	GetDlgItem(IDC_ConnectSave_BUTTON)->EnableWindow(false);
	if (adoManage.ConntestdbSQL()){
		GetDlgItemText(IDC_IPAddress, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_Port_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Port"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_UserName_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("UserName"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_Password_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Password"), str, _T(".\\SystemInfo.ini"));
		MessageBox(L"���ݿ����ӳɹ�", L"��ʾ��Ϣ", NULL);
		adoManage.CloseDB();
		CDialog::OnCancel();
	}
	GetDlgItem(IDC_ConnectSave_BUTTON)->EnableWindow(true);
}
