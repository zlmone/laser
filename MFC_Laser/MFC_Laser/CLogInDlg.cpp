// CLogInDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "CLogInDlg.h"
#include "afxdialogex.h"
#include "ADOManage.h"

// CLogInDlg 对话框

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGINDLG, pParent)
	, m_s_name(_T(""))
	, m_s_pwd(_T(""))
{

}

CLogInDlg::~CLogInDlg()
{
}

void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_name);
	DDX_Text(pDX, IDC_EDIT2, m_s_pwd);
	DDX_Control(pDX, IDC_EDIT1, m_UserID);
}


BEGIN_MESSAGE_MAP(CLogInDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLogInDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogInDlg 消息处理程序


void CLogInDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	CString LimitsTemp = L"";

	if (m_s_name.IsEmpty() || m_s_pwd.IsEmpty())
	{
		MessageBox(L"用户名或密码不能为空", L"用户登录信息", MB_ICONWARNING);
		SetDlgItemText(IDC_EDIT2, L"");
		m_UserID.SetFocus();
	}
	if ((m_s_name != "") && (CheckUser(m_s_name, m_s_pwd, &LimitsTemp) == TRUE))//账号1不为空
	{
		if (LimitsTemp == "")
		{
			AfxMessageBox(L"账号/密码不正确！");
			SetDlgItemText(IDC_EDIT2, L"");
			m_UserID.SetFocus();
			return;
		}
		else
		{
			TesterID = m_s_name;
			OnOK();
		}
	}
	else
	{
		// 不匹配
		MessageBox(L"用户名或密码不正确", L"提示", MB_ICONWARNING);
		SetDlgItemText(IDC_EDIT2,L"");
		m_UserID.SetFocus();
	}
}

BOOL CLogInDlg::CheckUser(CString Name, CString Password, CString* Limits)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline, val;
	CString strSql, FieldValue;
	strSql = _T("select * FROM [GPSTest].[dbo].[LUserAccount] WHERE Name='") + Name + L"' AND Password='" + Password + L"'";
	try
	{
		ado.m_pRecordSet = ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		if (!ado.m_pRecordSet->adoEOF)
		{
			if (GetUserType(Name, Password, Limits) == TRUE)
			{

			}
			else
			{
				return FALSE;
			}
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		ado.CloseAll();
		return FALSE;
	}
	ado.CloseAll();
	return TRUE;
}


BOOL CLogInDlg::GetUserType(CString Name, CString Password, CString *Limits)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline, val;
	CString strSql, FieldValue;

	//将数据插入表中
	strSql = _T("select * FROM [GPSTest].[dbo].[LUserAccount] WHERE Name='") + Name + L"' AND Password='" + Password + L"'";
	try
	{
		ado.m_pRecordSet = ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		ado.m_pRecordSet->MoveFirst();
		val = ado.m_pRecordSet->GetCollect(L"UserType");
		if (val.vt != VT_NULL)
		{
			FieldValue.Format(L"%s", (LPCSTR)(_bstr_t)val);
			*Limits = FieldValue;
		}
		else
		{
			*Limits = "";
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		*Limits = "";
		ado.CloseAll();
		return FALSE;
	}
	ado.CloseAll();
	return TRUE;
}

BOOL CLogInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_UserID.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
