// LDStartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "LDStartDlg.h"
#include "afxdialogex.h"


// LDStartDlg 对话框

IMPLEMENT_DYNAMIC(LDStartDlg, CDialogEx)

LDStartDlg::LDStartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LDStartDlg::IDD, pParent)
{

}

LDStartDlg::~LDStartDlg()
{
}

void LDStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LDStartDlg, CDialogEx)
END_MESSAGE_MAP()


// LDStartDlg 消息处理程序
