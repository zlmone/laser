// DbConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "DbConfig.h"
#include "afxdialogex.h"
#include "ADOManage.h"


// DbConfig 对话框

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


// DbConfig 消息处理程序


BOOL DbConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常:  OCX 属性页应返回 FALSE
}


//取消按钮
void DbConfig::OnBnClickedCancelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void DbConfig::OnBnClickedConnectsaveButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	ADOManage adoManage;
	CFileFind finder;
	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	//获取IP地址
	GetDlgItemText(IDC_IPAddress, m_serverip);
	if (m_serverip == "" || m_serverip=="0.0.0.0"){
		MessageBox(L"IP地址不能为空", L"提示信息", NULL);	
		return;
	}
	else
	{
		adoManage.m_SourceIp = m_serverip;
	}
	//获取端口号
	GetDlgItemText(IDC_Port_EDIT, m_port);
	if (m_port == ""){
		MessageBox(L"端口号不能为空", L"提示信息", NULL);
		return;
	}
	else
	{
		adoManage.m_Port = m_port;
	}
	//获取用户名
	GetDlgItemText(IDC_UserName_EDIT, m_username);
	if (m_username == ""){
		MessageBox(L"用户名不能为空", L"提示信息", NULL);
		return;
	}
	else
	{
		adoManage.m_UserName = m_username;
	}
	//获取密码
	GetDlgItemText(IDC_Password_EDIT, m_password);
	if (m_password == ""){
		MessageBox(L"密码不能为空", L"提示信息", NULL);
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
		MessageBox(L"数据库连接成功", L"提示信息", NULL);
		adoManage.CloseDB();
		CDialog::OnCancel();
	}
	GetDlgItem(IDC_ConnectSave_BUTTON)->EnableWindow(true);
}
