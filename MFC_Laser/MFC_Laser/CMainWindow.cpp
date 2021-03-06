// CMainWindow.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "CMainWindow.h"
#include "afxdialogex.h"
#include "MFC_LaserDlg.h"
#include "CDZ_Laser.h"
#include "ResizeCtrl.h"
#include "CUnlockDlg.h"
// CMainWindow 对话框

//全局变量
CMFC_LaserDlg AutoLaserDlg;
CDZ_Laser DzLaserDlg;
CWindowSizeMange ResizeCtrl;
extern CLaser LaserMachine;
IMPLEMENT_DYNAMIC(CMainWindow, CDialogEx)

CMainWindow::CMainWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAINWINDOWDLG, pParent)
{

}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CMainWindow, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainWindow::OnTcnSelchangeTab1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainWindow 消息处理程序


BOOL CMainWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HICON myicon = AfxGetApp()->LoadIcon(IDR_OWNUSE);
	SetIcon(myicon, TRUE);
	SetIcon(myicon, FALSE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_APPWINDOW);

	// TODO:  在此添加额外的初始化
	ResizeCtrl.Init(m_hWnd);
	m_Tab.InsertItem(0,L"自动化镭雕");
	m_Tab.InsertItem(1, L"大族镭雕");
	AutoLaserDlg.Create(IDD_MFC_LASER_DIALOG, GetDlgItem(IDC_TAB1));
	DzLaserDlg.Create(IDD_DZLASER , GetDlgItem(IDC_TAB1));
	CRect tabRect;
	m_Tab.GetClientRect(tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 22;
	//tabRect.bottom -= 1;
	AutoLaserDlg.MoveWindow(&tabRect);
	DzLaserDlg.MoveWindow(&tabRect);
	AutoLaserDlg.ShowWindow(SW_SHOW);
	DzLaserDlg.ShowWindow(SW_HIDE);

	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMainWindow::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int TabSel;
	TabSel = m_Tab.GetCurSel();
	switch (TabSel)
	{
	case 0:
		AutoLaserDlg.ShowWindow(SW_SHOW);
		DzLaserDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		DzLaserDlg.ShowWindow(SW_SHOW);
		AutoLaserDlg.ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
}


void CMainWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ResizeCtrl.ResizeWindow();
		CRect TabCtrlRect;
		CWnd *TabPtr;
		TabPtr = GetDlgItem(IDC_TAB1);
		if (TabPtr == NULL)
		{
			return;
		}
		TabPtr->GetClientRect(&TabCtrlRect);
		TabCtrlRect.left += 1;
		TabCtrlRect.right -= 1;
		TabCtrlRect.top += 22;
		AutoLaserDlg.MoveWindow(&TabCtrlRect, TRUE);
		DzLaserDlg.MoveWindow(&TabCtrlRect, TRUE);
	}
	// TODO: 在此处添加消息处理程序代码
}

//退出前先关闭镭雕系统
void CMainWindow::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CUnlockDlg unlock;
	if (unlock.DoModal()==IDCANCEL)
	{
		return;
	}
	LaserMachine.SysClose();
	CDialogEx::OnCancel();
}


void CMainWindow::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
