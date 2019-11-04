
// MFC_LaserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "MFC_LaserDlg.h"
#include "afxdialogex.h"
#include "DbConfig.h"
#include "ADOManage.h"
#include "UnLock.h"
#include "ResizeCtrl.h"
#include "SetRelative.h"
#include <afxmt.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*全局变量*/
volatile BOOL m_LdPortRun = true;//读串口全局变量，控制旋转镭雕
volatile BOOL m_BurningConrtolFlag; //烧写主控变量
BOOL SendTestFlag = true;       //发送TEST指令控制变量
BOOL BurnLastSendTestFlag = true;       //烧写完发送TEST指令控制变量
BOOL CompareLastSendTestFlag = true;       //对比完发送TEST指令控制变量
BOOL HaveCheckCode = true;     //有无检验码标志位
CString CurrentImei;   //当前IMEI号
CString CurrentBurningImei;   //当前烧写IMEI号
CString CurrentLdImei; //当前镭雕IMEI号
CString FImeiStart;
CString FImeiEnd; //IMEI起始位 终止位
CString ComputerIp;    //本机信息
CString zhidan;        //将选择的订单名称放在这个全局变量里
CString IniTestCommand, IniTestRec, IniCheckIMEICommand, IniCheckIMEICommandRec, IniBurningCommand1;
CString IniBurningCommand1Rec, IniBurningCommand2, IniBurningCommand2Rec, IMEIIniDataDelimiter1, IMEIIniDataDelimiter2;
CString IMEIIniDataDelimiter1Length/*, IniDataDelimiter2Length*/, RIDIniDataDelimiter1, RIDIniDataDelimiter2, RIDIniDataDelimiter1Length;
CString iniCheckRIDCommand, iniCheckRIDCommandRec;
CWindowSizeMange myDlgManage;
//三个工位的工作状态，true为空闲
BOOL BurnFinishFlag = TRUE;
BOOL LdFinishFlag = TRUE;
BOOL CompareFinishFlag = TRUE;
BOOL EnterScanFlag = FALSE;
BOOL BurnIsOpenFlag = FALSE;
BOOL LDIsOpenFlag = FALSE;
int IncreasingnumberInt;
CString IncreasingNumberStr;
//硬件名称
CString MachineName;
CString Allversion;
CString LDversion, LDsoftmodel;

//监听标志
BOOL listenFlag = TRUE;
BOOL LD2ConnectFlag = FALSE;
CString readFromClient = L"";
BOOL readFromClientFlag = TRUE;
int iRet;
//ADOManage adomanageCompare;

//外部变量
extern BOOL RelativeIMEI2, RelativeIMEI3, RelativeIMEI4, RelativeIMEI5, RelativeIMEI6, RelativeIMEI7, RelativeIMEI8, RelativeIMEI9\
, RelativeIMEI10, RelativeIMEI11;
extern BOOL RelativeIMEI12, RelativeIMEI13;

//三个工位的IMEI传递变量
CString BurnToLd, LdToCompare;

//三个工位标识
unsigned long long Control[3][3] = { { 0, 0, 0 }, { 1, 1, 1 }, { 1, 1, 1 } };

//存贮txt中的IMEI
vector<CString> TxTResult;
vector<CString> AutoRelativeDataVec;
//存贮Station字段数据
vector<CString> StationValVector;

//对比串口线程控制变量
volatile BOOL m_CompareConrtolFlag; //烧写主控变量
BOOL CompareSendTestFlag = true;       //发送TEST指令控制变量

CMFC_LaserDlg * parent;

//锁
CRITICAL_SECTION cs;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_LaserDlg 对话框



CMFC_LaserDlg::CMFC_LaserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_LaserDlg::IDD, pParent)
	, m_pCLDStartDlg(NULL)
	, m_checkState(FALSE)
	, m_RelativeSN(FALSE)
	, m_ReLD(FALSE)
	, m_LdRandom(FALSE)
	, m_NocheckCode(FALSE)
	, m_AutoCheckPrintVal(FALSE)
	, m_BurningEditVal(_T(""))
	, m_EndIMEIVal(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_OWNUSE);

	//制单号全局变量
	ZhiDanNO = _T("");

	//IMEI全局变量
	IMEIStaStart = _T("");
	IMEIStaEnd = _T("");
}

void CMFC_LaserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZhiDan_COMBO, ZhiDanCombo);
	DDX_Control(pDX, IDC_CURRENTTCP_RICHEDIT, m_currentRichedit);
	DDX_Control(pDX, IDC_BurningLog_EDIT, m_BurningeditLog);
	DDX_Control(pDX, IDC_NoCheckCode_CHECK, m_NoCheckCodeCheckControl);
	DDX_Control(pDX, IDC_BurningImei_EDIT, m_BurningImeiEdit);
	DDX_Control(pDX, IDC_Compare_EDIT, m_CompareLog);
	DDX_Control(pDX, myport, myLdport);
	DDX_Control(pDX, IDC_CHECK1, m_Check);
	DDX_Check(pDX, IDC_CHECK1, m_checkState);
	DDX_Control(pDX, IDC_RELATESN, m_RelateSn);
	DDX_Check(pDX, IDC_RELATESN, m_RelativeSN);
	DDX_Check(pDX, IDC_RELD, m_ReLD);
	DDX_Check(pDX, IDC_CHECK2, m_LdRandom);
	DDX_Control(pDX, IDC_BURNLOG, m_BurnLog);
	DDX_Control(pDX, IDC_COMPARELOG, m_CpLog);
	DDX_Control(pDX, IDC_COMBO2, m_IncreasingNumber);
	DDX_Check(pDX, IDC_NoCheckCode_CHECK, m_NocheckCode);
	DDX_Control(pDX, IDC_CHECK3, m_AutoCheckPrint);
	DDX_Check(pDX, IDC_CHECK3, m_AutoCheckPrintVal);
	DDX_Control(pDX, IDC_BurningPort_COMBO, m_BurnPort);
	DDX_Text(pDX, IDC_BurningImei_EDIT, m_BurningEditVal);
	DDX_Text(pDX, IDC_ImeiEnd_EDIT, m_EndIMEIVal);
	DDX_Control(pDX, IDC_LIST1, m_AutoList);
}

BEGIN_MESSAGE_MAP(CMFC_LaserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DbConfig_BUTTON, &CMFC_LaserDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_RefreshZhidan_BUTTON, &CMFC_LaserDlg::OnBnClickedRefreshzhidanButton)
	ON_CBN_SELCHANGE(IDC_ZhiDan_COMBO, &CMFC_LaserDlg::OnSelchangeZhidanCombo)
	ON_BN_CLICKED(IDC_ChooseTemplate_BUTTON, &CMFC_LaserDlg::OnBnClickedChoosetemplateButton)
	ON_BN_CLICKED(IDC_OpenTemplate_BUTTON, &CMFC_LaserDlg::OnBnClickedOpentemplateButton)
	ON_BN_CLICKED(IDC_ToLock_BUTTON, &CMFC_LaserDlg::OnBnClickedTolockButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OpenLdSystem_BUTTON, &CMFC_LaserDlg::OnBnClickedOpenldsystemButton)
	ON_CBN_DROPDOWN(IDC_BurningPort_COMBO, &CMFC_LaserDlg::OnDropdownBurningportCombo)
	ON_BN_CLICKED(IDC_BurningConn_BUTTON, &CMFC_LaserDlg::OnBnClickedBurningconnButton)
	ON_BN_CLICKED(IDC_NoCheckCode_CHECK, &CMFC_LaserDlg::OnBnClickedNocheckcodeCheck)
	ON_BN_CLICKED(IDC_LdOnceTime_BUTTON, &CMFC_LaserDlg::OnBnClickedLdoncetimeButton)
	ON_CBN_DROPDOWN(IDC_ComparePort_COMBO, &CMFC_LaserDlg::OnDropdownCompareportCombo)
	ON_BN_CLICKED(IDC_CompareConn_BUTTON, &CMFC_LaserDlg::OnBnClickedCompareconnButton)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_StartLaser_BUTTON, &CMFC_LaserDlg::OnBnClickedStartlaserButton)
	ON_BN_CLICKED(IDC_ALLAUTO_BUTTON, &CMFC_LaserDlg::OnBnClickedAllautoButton)
	ON_BN_CLICKED(IDC_BurningScanningGun_Button, &CMFC_LaserDlg::OnBnClickedBurningscanninggunButton)
	ON_BN_CLICKED(IDC_NoNet_CHECK, &CMFC_LaserDlg::OnBnClickedNonetCheck)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_GETCOMMAND, &CMFC_LaserDlg::OnBnClickedGetcommand)
	ON_BN_CLICKED(IDC_OPENRELAY, &CMFC_LaserDlg::OnBnClickedOpenrelay)
	ON_BN_CLICKED(IDC_CHECK1, &CMFC_LaserDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_RELD, &CMFC_LaserDlg::OnBnClickedReld)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFC_LaserDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_LODATXT, &CMFC_LaserDlg::OnBnClickedLodatxt)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC_LaserDlg::OnBnClickedButton2)
	ON_CBN_DROPDOWN(IDC_JDQ_COMBO, &CMFC_LaserDlg::OnDropdownJdqCombo)
END_MESSAGE_MAP()


// CMFC_LaserDlg 消息处理程序

BOOL CMFC_LaserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//初始化本机信息
	GetLocalHostIPName(m_pcnameEdit, m_pcipEdit);//调用函数获得IP和计算机名称
	SetDlgItemText(IDC_LocalPcName_EDIT, m_pcipEdit);//将计算机的名称显示在名称控件中
	SetDlgItemText(IDC_LocalPcIP_EDIT, m_pcnameEdit);//将计算机的IP显示在IP控件中
	//读取制单号到下拉框
	FillZhidanToComBo();

	parent = this;

	//初始化串口列表
	FindCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);
	FindCommPort((CComboBox*)GetDlgItem(IDC_ComparePort_COMBO), Port2No);
	FindCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);

	//初始化字体大小
	fontinit();

	SetDlgItemText(IDC_SendCommand1_EDIT, L"AT*MNVMQ=\"1.0\"");
	SetDlgItemText(IDC_SendCommand2_EDIT, L"AT*MCGSN=1,\"imei\"");
	SetDlgItemText(IDC_ReciveCommand_EDIT, L"OK");

	/*获取控件位置*/
	myDlgManage.Init(m_hWnd);
	SetDlgItemText(ID_PORT, L"8090");
	//m_RelateSn.SetCheck(TRUE);
	m_IncreasingNumber.AddString(L"1");
	m_IncreasingNumber.AddString(L"2");
	m_IncreasingNumber.AddString(L"3");
	m_IncreasingNumber.AddString(L"4");
	m_IncreasingNumber.AddString(L"5");
	m_IncreasingNumber.AddString(L"6");
	m_IncreasingNumber.SetCurSel(0);

	m_AutoCheckPrint.SetCheck(1);

	InitializeCriticalSection(&cs);

	m_AutoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS);
	m_AutoList.SetBkColor(RGB(255, 255, 255));
	m_AutoList.SetTextColor(RGB(3, 146, 207));
	m_AutoList.SetTextBkColor(RGB(255, 255, 255));
	m_AutoList.DeleteAllItems();
	//清空表头
	while (m_AutoList.DeleteColumn(0));
	//将字段显示出来
	m_AutoList.InsertColumn(0, L"No", LVCFMT_CENTER, 30);
	m_AutoList.InsertColumn(1, L"设置项目", LVCFMT_LEFT, 65);
	m_AutoList.InsertColumn(2, L"设置变量名", LVCFMT_LEFT, 100);
	m_AutoList.InsertColumn(3, L"设置变量", LVCFMT_LEFT, 230);
	m_AutoList.SetReadOnlyColumns(0);//read only
	CString strValidChars = L"";
	m_AutoList.SetColumnValidEditCtrlCharacters(strValidChars, 2);//none control edit 
	m_AutoList.EnableVScroll();
	m_AutoList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CString str;
	for (int i = 0; i < 48; i++)
	{
		str.Format(L"%02d", i + 1);
		m_AutoList.InsertItem(LVIF_TEXT | LVIF_STATE, i,
			str, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);

		if (i == 0)
		{
			m_AutoList.SetItemText(i, 1, L"机型");
			m_AutoList.SetItemText(i, 2, L"Model");
		}
		else if (i == 1)
		{
			m_AutoList.SetItemText(i, 1, L"软件版本");
			m_AutoList.SetItemText(i, 2, L"Version");
		}
		else
		{
			m_AutoList.SetItemText(i, 1, L"");
			m_AutoList.SetItemText(i, 2, L"Other" + str);
		}
	}
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_LaserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_LaserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_LaserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//界面按下Enter键后不会退出
void CMFC_LaserDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

//关闭程序时
void CMFC_LaserDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	INT_PTR nRes;
	nRes = MessageBox(_T("您确定要退出吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDCANCEL == nRes)
		return;
	CDialogEx::OnClose();
}

/*初始化配置模块*/
//数据库配置按钮
void CMFC_LaserDlg::OnBnClickedDbconfigButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值 
	DbConfig dbconfigDlg;           // 构造对话框类CTipDlg的实例   
	nRes = dbconfigDlg.DoModal();  // 弹出对话框
	if (IDCANCEL == nRes)
		return;
}

//填充订单号到下拉框
void CMFC_LaserDlg::FillZhidanToComBo() {
	ZhiDanCombo.ResetContent();
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetZhiDan();
	while (!adomanage.m_pRecordSet->adoEOF)
	{
		ZhiDanCombo.AddString(adomanage.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanage.m_pRecordSet->MoveNext();
	}
	adomanage.CloseAll();
}

//刷新制单
void CMFC_LaserDlg::OnBnClickedRefreshzhidanButton()
{
	// TODO:  在此添加控件通知处理程序代码
	SetDlgItemText(IDC_ImeiStart_EDIT, L"");
	SetDlgItemText(IDC_ImeiEnd_EDIT, L"");
	SetDlgItemText(IDC_ImeiCurrent_EDIT, L"");
	SetDlgItemText(IDC_LdTemplate_EDIT, L"");
	ZhiDanCombo.ResetContent();
	FillZhidanToComBo();
	SetRicheditText(L"刷新成功，请重新选择订单", 0);
}

//选择订单号触发事件
void CMFC_LaserDlg::OnSelchangeZhidanCombo()
{
	UpdateData(TRUE);
	try {
		// TODO:  在此添加控件通知处理程序代码
		_variant_t imeitemp;//用来放getcollect变量的
		int nSel;
		nSel = ZhiDanCombo.GetCurSel();
		ZhiDanCombo.GetLBText(nSel, ZhiDanNO);
		SetDlgItemText(IDC_ZhiDan_COMBO, ZhiDanNO);
		SetDlgItemText(IDC_ImeiStart_EDIT, L"");
		SetDlgItemText(IDC_ImeiEnd_EDIT, L"");
		SetDlgItemText(IDC_LdTemplate_EDIT, L"");
		ADOManage adomanage;
		adomanage.ConnSQL();
		adomanage.GetIMEIByZhiDan(ZhiDanNO);
		IMEIStaStart = adomanage.m_pRecordSet->GetCollect("IMEIStart").bstrVal;
		IMEIStaEnd = adomanage.m_pRecordSet->GetCollect("IMEIEnd").bstrVal;
		SN1 = adomanage.m_pRecordSet->GetCollect("SN1").bstrVal;
		SN2 = adomanage.m_pRecordSet->GetCollect("SN2").bstrVal;
		SN3 = adomanage.m_pRecordSet->GetCollect("SN3").bstrVal;
		SN1Len = SN1.GetLength();
		GetStation(ZhiDanNO);
		MachineName = getMachineName(ZhiDanNO);
		SetDlgItemText(IDC_ImeiStart_EDIT, IMEIStaStart);
		SetDlgItemText(IDC_ImeiEnd_EDIT, IMEIStaEnd);
		SetDlgItemText(IDC_SnStart_EDIT, SN1 + SN2);
		SetDlgItemText(IDC_SnEnd_EDIT, SN1 + SN3);
		UpdateWindow();
		adomanage.CloseAll();
		ADOManage adomanage1;
		adomanage1.ConnSQL();
		if (adomanage1.GetIMEICurrentAndTemplate(ZhiDanNO) == 1) {
			imeitemp = adomanage1.m_pRecordSet1->GetCollect("LdTemplate");
			SetDlgItemText(IDC_LdTemplate_EDIT, adomanage1.m_pRecordSet1->GetCollect("LdTemplate").bstrVal);
			SetDlgItemText(IDC_ImeiCurrent_EDIT, adomanage1.m_pRecordSet1->GetCollect("LdCurrentImei").bstrVal);
			SetDlgItemText(IDC_BurningImei_EDIT, adomanage1.m_pRecordSet1->GetCollect("LdCurrentImei").bstrVal);
			SetDlgItemText(IDC_LdImei_EDIT, adomanage1.m_pRecordSet1->GetCollect("LdImei").bstrVal);
			if (m_RelativeSN)
			{
				SetDlgItemText(IDC_SnCurrent_EDIT, adomanage1.m_pRecordSet1->GetCollect("ldSn").bstrVal);
				SetDlgItemText(IDC_LdSn_EDIT, adomanage1.m_pRecordSet1->GetCollect("ldSn").bstrVal);
			}
			else
			{
				CString myIMEI;
				GetDlgItemText(IDC_LdImei_EDIT, myIMEI);
				SetDlgItemText(IDC_SnCurrent_EDIT, adomanage1.GetRelateSN(myIMEI));
				SetDlgItemText(IDC_LdSn_EDIT, adomanage1.GetRelateSN(myIMEI));
			}
			int checkcodeflag;
			checkcodeflag = adomanage1.m_pRecordSet1->GetCollect("CheckCode").intVal;
			m_NoCheckCodeCheckControl.SetCheck(checkcodeflag);
		}
		else
		{
			SetDlgItemText(IDC_ImeiCurrent_EDIT, IMEIStaStart);
			SetDlgItemText(IDC_BurningImei_EDIT, IMEIStaStart);
			SetDlgItemText(IDC_LdImei_EDIT, IMEIStaStart);
			if (m_RelativeSN)
			{
				SetDlgItemText(IDC_SnCurrent_EDIT, SN1 + SN2);
				SetDlgItemText(IDC_LdSn_EDIT, SN1 + SN2);
			}
			else
			{
				CString getSN;
				GetDlgItemText(IDC_ImeiCurrent_EDIT, getSN);
				SetDlgItemText(IDC_SnCurrent_EDIT, adomanage1.GetRelateSN(getSN));
				SetDlgItemText(IDC_LdSn_EDIT, adomanage1.GetRelateSN(getSN));
			}
			m_NoCheckCodeCheckControl.SetCheck(0);
		}
		adomanage1.CloseAll1();
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}

//选择模板按钮
void CMFC_LaserDlg::OnBnClickedChoosetemplateButton()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("镭雕模版文件(*.ezd)|*.ezd|所有文件(*.*)|*.*||");
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T("ezd"), NULL, 0, szFilter, this);

	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_LdTemplate_EDIT, strFilePath);
	}
}

//打开模板函数
void CMFC_LaserDlg::OnBnClickedOpentemplateButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;

	GetDlgItemText(IDC_LdTemplate_EDIT, strFilePath);
	if (strFilePath == "") {
		MessageBox(_T("请先选择模板"), _T("提示"), NULL);
		return;
	}
	ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOWNORMAL);
}

//获取本机IP和地址
int CMFC_LaserDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
{
	char szLocalName[256];//定义
	WSADATA wsaData;//定义套接字存储变量
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)//<span class="con">windows初始化socket网络库，申请2，0的版本，windows socket编程必<img class="selectsearch-hide" id="selectsearch-icon" alt="搜索" src="http://img.baidu.com/img/iknow/qb/select-search.png" />须先初始化。如果出错,则返回0</span>
	{
		if (gethostname(szLocalName, sizeof(szLocalName)) != 0)//产生错误
		{
			sLocalName = _T("没有取得");
			WSACleanup();
			return GetLastError();
		}
		else
		{
			sLocalName = szLocalName;//用sLocalName变量存储获得的计算机名称
			struct hostent FAR * lpHostEnt = gethostbyname(CStringA(sLocalName));
			if (lpHostEnt == NULL)//错误
			{
				sIpAddress = _T("");
				WSACleanup();
				return GetLastError();
			}
			LPSTR lpAddr = lpHostEnt->h_addr_list[0];//获取IP地址
			if (lpAddr)
			{
				struct in_addr inAddr;
				memmove(&inAddr, lpAddr, 4);
				sIpAddress = inet_ntoa(inAddr);//转换为标准格式
				if (sIpAddress.IsEmpty())
				{
					sIpAddress = _T("没有取得");
					WSACleanup();
					return 1;
				}
			}
		}
	}
	WSACleanup();//清空套接字初始化变量
	return 0;
}

//锁定按钮
void CMFC_LaserDlg::OnBnClickedTolockButton()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateWindow();
	CString LockText;
	CString ZhuanHuanChar;
	GetDlgItem(IDC_ToLock_BUTTON)->GetWindowText(LockText);
	if (LockText == "锁定") {
		//判断制单选择了没有
		GetDlgItemText(IDC_ZhiDan_COMBO, zhidan);
		if (zhidan == "") {
			MessageBox(_T("请先选择订单"), _T("提示"), NULL);
			return;
		}
		//判断选择模板了没
		CString strFilePath;
		GetDlgItemText(IDC_LdTemplate_EDIT, strFilePath);
		if (strFilePath == "") {
			MessageBox(_T("请先选择模板"), _T("提示"), NULL);
			return;
		}
		//记录当前SN的变量
		CString SnCurrent;
		GetDlgItemText(IDC_SnCurrent_EDIT, SnCurrent);
		//判断当前位在不在范围内
		CString  ImeiCurrent;
		GetDlgItemText(IDC_ImeiStart_EDIT, FImeiStart);
		GetDlgItemText(IDC_ImeiEnd_EDIT, FImeiEnd);
		GetDlgItemText(IDC_ImeiCurrent_EDIT, ImeiCurrent);
		//判断IMEI位数
		/*if (HaveCheckCode){
			if (ImeiCurrent.GetLength() != 14){
				MessageBox(_T("IMEI号必须为14位"), _T("提示"), NULL);
				return;
			}
		}
		else
		{
			if (ImeiCurrent.GetLength() != 15){
				MessageBox(_T("IMEI号必须为15位"), _T("提示"), NULL);
				return;
			}
		}*/
		/*	if (ImeiCurrent.GetLength() != FImeiStart.GetLength()){
				MessageBox(_T("IMEI当前位不在范围内"), _T("提示"), NULL);
				return;
			}
			if (ImeiCurrent<FImeiStart || ImeiCurrent>FImeiEnd){
				MessageBox(_T("IMEI当前位不在范围内"), _T("提示"), NULL);
				return;
			}*/
			//将本机信息放在全局变量里
		int NumSel;
		NumSel = m_IncreasingNumber.GetCurSel();
		m_IncreasingNumber.GetLBText(NumSel, IncreasingNumberStr);
		IncreasingnumberInt = _ttoi(IncreasingNumberStr);
		CString localIP, localname;
		GetDlgItemText(IDC_LocalPcIP_EDIT, localIP);
		GetDlgItemText(IDC_LocalPcName_EDIT, localname);
		ComputerIp = localIP + "," + localname;
		//将烧写、镭雕IMEI放入全局变量
		GetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
		GetDlgItemText(IDC_BurningImei_EDIT, CurrentBurningImei);
		int checkcodeflagnum = m_NoCheckCodeCheckControl.GetCheck();
		CString checkcodeflag;
		checkcodeflag.Format(_T("%d"), checkcodeflagnum);
		CString LdImei;
		GetDlgItemText(IDC_LdImei_EDIT, LdImei);
		GetDlgItem(IDC_ZhiDan_COMBO)->EnableWindow(false);
		GetDlgItem(IDC_ImeiCurrent_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_ChooseTemplate_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_DbConfig_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_RefreshZhidan_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_OpenTemplate_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_ComparePort_COMBO)->EnableWindow(true);
		GetDlgItem(IDC_CompareConn_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_CompareIMEI2_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_OpenLdSystem_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_NoCheckCode_CHECK)->EnableWindow(false);
		GetDlgItem(IDC_GETCOMMAND)->EnableWindow(false);
		GetDlgItem(IDC_COMBO2)->EnableWindow(false);
		GetDlgItem(IDC_CHECK3)->EnableWindow(false);
		//GetDlgItem(IDC_GETCOMMAND)->EnableWindow(false);
		//GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(true);
		GetDlgItem(IDC_BurningScanningGun_Button)->EnableWindow(true);
		GetDlgItem(IDC_ALLAUTO_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(true);

		GetDlgItem(IDC_LODATXT)->EnableWindow(false);
		GetDlgItem(IDC_RELATESN)->EnableWindow(false);
		GetDlgItem(IDC_RELD)->EnableWindow(false);
		GetDlgItem(IDC_CHECK1)->EnableWindow(false);
		GetDlgItem(IDC_CHECK2)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);

		GetDlgItem(IDC_ImeiStart_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_ImeiEnd_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_SnStart_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_SnEnd_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_LIST1)->EnableWindow(false);
		GetDlgItem(IDC_OPENRELAY)->EnableWindow(true);

		SetDlgItemText(IDC_ToLock_BUTTON, TEXT("解锁"));
		ADOManage adomanage;
		adomanage.ConnSQL();
		adomanage.InsetrLdRecordParam(zhidan, strFilePath, ImeiCurrent, checkcodeflag, SnCurrent, LdImei);
		adomanage.GetSoftModelAndVersion(LDsoftmodel, LDversion, zhidan);
		adomanage.CloseAll();
	}
	else
	{
		// TODO:  在此添加控件通知处理程序代码
		CString BurningButton, LdButton, CompareButton, ScanningGunButton;
		GetDlgItemText(IDC_BurningConn_BUTTON, BurningButton);
		GetDlgItemText(IDC_OpenLdSystem_BUTTON, LdButton);
		GetDlgItemText(IDC_CompareConn_BUTTON, CompareButton);
		GetDlgItemText(IDC_BurningScanningGun_Button, ScanningGunButton);
		if (BurningButton == "一键关闭") {
			MessageBox(_T("请先关闭烧写串口"), _T("提示"), NULL);
			return;
		}
		if (LdButton == "关闭镭雕系统") {
			MessageBox(_T("请先关闭镭雕系统"), _T("提示"), NULL);
			return;
		}
		if (CompareButton == "一键关闭") {
			MessageBox(_T("请先关闭对比串口"), _T("提示"), NULL);
			return;
		}
		if (ScanningGunButton == "关闭扫描枪模式") {
			MessageBox(_T("请先关闭扫描枪模式"), _T("提示"), NULL);
			return;
		}
		INT_PTR nRes;             // 用于保存DoModal函数的返回值 
		UnLock unlockDlg;           // 构造对话框类CTipDlg的实例   
		nRes = unlockDlg.DoModal();  // 弹出对话框
		if (IDCANCEL == nRes)
			return;
	}
}



//解锁的内容
void CMFC_LaserDlg::UnlockContent()
{
	GetDlgItem(IDC_ZhiDan_COMBO)->EnableWindow(true);
	GetDlgItem(IDC_ImeiCurrent_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_ChooseTemplate_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_DbConfig_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_RefreshZhidan_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_OpenTemplate_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_ComparePort_COMBO)->EnableWindow(false);
	GetDlgItem(IDC_CompareConn_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_CompareIMEI2_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_OpenLdSystem_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_NoCheckCode_CHECK)->EnableWindow(true);
	GetDlgItem(IDC_GETCOMMAND)->EnableWindow(true);
	GetDlgItem(IDC_LODATXT)->EnableWindow(true);
	GetDlgItem(IDC_RELATESN)->EnableWindow(true);
	GetDlgItem(IDC_RELD)->EnableWindow(true);
	GetDlgItem(IDC_CHECK1)->EnableWindow(true);
	GetDlgItem(IDC_CHECK2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
	//GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(false);
	GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(false);
	GetDlgItem(IDC_BurningScanningGun_Button)->EnableWindow(false);
	GetDlgItem(IDC_ALLAUTO_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(false);
	GetDlgItem(IDC_ImeiStart_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_ImeiEnd_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_SnStart_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_SnEnd_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_COMBO2)->EnableWindow(true);
	GetDlgItem(IDC_CHECK3)->EnableWindow(true);
	GetDlgItem(IDC_LIST1)->EnableWindow(false);

	SetDlgItemText(IDC_ToLock_BUTTON, TEXT("锁定"));
}

//点击无校验位复选框
void CMFC_LaserDlg::OnBnClickedNocheckcodeCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!((CButton *)GetDlgItem(IDC_NoCheckCode_CHECK))->GetCheck())
	{
		HaveCheckCode = true;
	}
	else
	{
		HaveCheckCode = false;
	}
}


/*信息日志模块*/
//根据值来在信息日志中显示不同的信息
void CMFC_LaserDlg::SetRicheditText(CString strMsg, int No)
{
	m_currentRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (No)
	{
	case 0:
		m_currentRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentRichedit.LineScroll(m_currentRichedit.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_currentRichedit.SetSelectionCharFormat(cf);
		m_currentRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentRichedit.LineScroll(m_currentRichedit.GetLineCount() - 1);
		break;
	default:
		break;
	}

	/*
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	if (strMsg == "127.0.0.1>>测试信息")
	{
	cf.crTextColor = RGB(255, 0, 0);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf); //这个是richedit的专用方法，richedit常用来作为日志输出
	m_currenttcp1RichEdit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("测试\r\n"));
	}
	cf.crTextColor = RGB(255, 200, 100);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf);*/
}

//获取系统时间
CString CMFC_LaserDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}


/*串口初始化*/
//初始化串口
HANDLE CMFC_LaserDlg::InitCom(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"串口初始化失败！", 1);
		//MessageBox(L"串口初始化失败！", L"提示信息", NULL);
		return NULL;
	}
	SetupComm(hCom, 100, 100); //输入缓冲区和输出缓冲区的大小都是100
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 115200; //波特率
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

//初始化串口
HANDLE CMFC_LaserDlg::InitCom1(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"串口初始化失败！", 1);
		//MessageBox(L"串口初始化失败！", L"提示信息", NULL);
		return NULL;
	}
	SetupComm(hCom, 100, 100); //输入缓冲区和输出缓冲区的大小都是100
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

//初始化与PLC通信的串口
HANDLE CMFC_LaserDlg::InitCom2(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"串口初始化失败！", 1);
		//MessageBox(L"串口初始化失败！", L"提示信息", NULL);
		return NULL;
	}
	SetupComm(hCom, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是100
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600; //波特率为9600
	dcb.ByteSize = 7; //每个字节有8位
	dcb.Parity = EVENPARITY; //偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

//关闭串口
int CMFC_LaserDlg::CloseCom(HANDLE handler)
{
	return CloseHandle(handler);
}



/************************************镭雕系统模块*********************************/
//打开镭雕系统
void CMFC_LaserDlg::OnBnClickedOpenldsystemButton()
{
	CString LaserSystemTest;
	GetDlgItem(IDC_OpenLdSystem_BUTTON)->GetWindowText(LaserSystemTest);
	if (LaserSystemTest == "打开镭雕系统") {
		// TODO:  在此添加控件通知处理程序代码
		if (NULL == m_pCLDStartDlg)
		{
			m_pCLDStartDlg = new LDStartDlg();
			m_pCLDStartDlg->Create(IDD_LDSTART_DIALOG, this);
		}
		m_pCLDStartDlg->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_OpenLdSystem_BUTTON)->EnableWindow(false);
		//WindowTimeClose(L"镭雕系统启动中请稍等...", 100);

		//加密狗
		//DllFlag = markezd.lmc1_Initial(L"F:\\工作资料\\李维韩\\EzCad", TRUE, AfxGetMainWnd()->m_hWnd);
		DllFlag = markezd.lmc1_Initial(L"", FALSE, AfxGetMainWnd()->m_hWnd);
		//启动完毕后关闭掉上面那个模态对话框
		CWnd* hWnd = FindWindow(NULL, _T("提示信息"));
		if (hWnd)
		{
			hWnd->PostMessage(WM_CLOSE, NULL, NULL);
		}
		DllStr.Format(_T("%d"), DllFlag);

		if (DllFlag == 0)
		{
			SetRicheditText(L"镭雕系统启动成功", 0);
			WritetoTxt(L"Laser_" + zhidan + _T("_"), L"镭雕系统启动成功\r\n");
		}
		else
		{
			SetRicheditText(L"镭雕系统启动失败，错误代码" + DllStr, 1);
			WritetoTxt(L"Laser_" + zhidan + _T("_"), L"镭雕系统启动失败，错误代码" + DllStr + L"\r\n");
			return;
		}
		//读镭雕模版
		CString Templatepath;
		GetDlgItemText(IDC_LdTemplate_EDIT, Templatepath);
		DllFlag = markezd.lmc1_LoadEzdFile((LPTSTR)(LPCTSTR)(Templatepath));//从CString转换成TCHAR*类型，要转两次
		DllStr.Format(_T("%d"), DllFlag);
		if (DllFlag == 0)
		{
			SetRicheditText(L"镭雕模版读取成功", 0);
			WritetoTxt(L"Laser_" + zhidan + _T("_"), L"镭雕模版读取成功\r\n");
		}
		else
		{
			SetRicheditText(L"镭雕模版读取失败,请检查模版路径是否有误，错误代码" + DllStr, 1);
			CloseLaserSystem();
			return;
		}
		//检查镭雕模版中是否存在指定对象名称，此函数找得到对象名称会返回0，找不到就返回1并直接关闭镭雕系统
		if (FindLdTemplateObject())
		{
			CloseLaserSystem();
			return;
		}
		SetDlgItemText(IDC_OpenLdSystem_BUTTON, TEXT("关闭镭雕系统"));
		GetDlgItem(IDC_LdOnceTime_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(true);
	}
	else
	{
		CloseLaserSystem();
	}
}

//关闭镭雕系统
void CMFC_LaserDlg::CloseLaserSystem() {
	DllFlag = markezd.lmc1_Close();

	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"关闭镭雕系统成功", 0);
	}
	else
	{
		SetRicheditText(L"关闭镭雕系统失败，错误代码" + DllStr, 1);
		return;
	}
	SetDlgItemText(IDC_OpenLdSystem_BUTTON, TEXT("打开镭雕系统"));
	GetDlgItem(IDC_LdOnceTime_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(false);
}

//寻找镭雕模板对象名称
int CMFC_LaserDlg::FindLdTemplateObject()
{
	double a, b, c, d, e;
	CString ObIMEI = _T("IMEI");
	CString ObSN = _T("SN");
	//一个一个去找是否存在对象名称，找不到就返回1，找得到就返回0
	DllFlag = markezd.lmc1_GetEntSize((LPTSTR)(LPCTSTR)ObIMEI, a, b, c, d, e);
	if (DllFlag != 0)
	{
		DllStr.Format(_T("%d"), DllFlag);
		SetRicheditText(L"镭雕系统启动失败，错误代码" + DllStr, 1);
		MessageBox(L"找不到IMEI对象", _T("错误信息"), NULL);
		return 1;
	}
	/*DllFlag = markezd.lmc1_GetEntSize((LPTSTR)(LPCTSTR)ObSN, a, b, c, d, e);
	if (DllFlag != 0)
	{
		DllStr.Format(_T("%d"), DllFlag);
		SetRicheditText(L"镭雕系统启动失败，错误代码" + DllStr, 1);
		MessageBox(L"找不到SN对象", _T("错误信息"), NULL);
		return 1;
	}*/
	return 0;
}

//生成IMEI校验位
CString CMFC_LaserDlg::CreateIMEI15(CString imei)
{
	long long a = 0;
	int j = 0;
	char imeiChar[14];
	strncpy(imeiChar, CStringA(imei), sizeof(imeiChar));
	//	std::string sz2 = CT2A(imei.GetBuffer());

	int resultInt = 0;
	for (int i = 0; i < 14; i++)
	{
		int a = (int)imeiChar[i] - '0';
		i++;
		int temp = ((int)imeiChar[i] - '0') * 2;
		int b = temp < 10 ? temp : temp - 9;
		resultInt += a + b;
	}
	resultInt %= 10;
	GetLastError();
	resultInt = (resultInt == 0 ? 0 : 10 - resultInt);
	CString resultStr, abc;
	resultStr.Format(_T("%d"), resultInt);
	abc = imei + resultStr;
	imei.ReleaseBuffer();
	return abc;
}

//窗口在N毫秒后自动关闭
void CMFC_LaserDlg::WindowTimeClose(CString str, int strtime)
{
	SetRicheditText(str, 1);
	SetTimer(1, strtime, NULL);//启动定时器1,定时时间是1.5秒
	MessageBox(str + L"(此弹出框会自动关闭)", L"提示信息", NULL);
}

//定时器函数
void CMFC_LaserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CWnd* hWnd = FindWindow(NULL, _T("提示信息"));
	if (hWnd)
	{
		hWnd->PostMessage(WM_CLOSE, NULL, NULL);
	}
	KillTimer(1);   //关闭定时器
	CDialogEx::OnTimer(nIDEvent);
}

//getrelate
BOOL CMFC_LaserDlg::AutoGetAllRelativeData(CString IMEI)
{
	UpdateData(TRUE);
	AutoRelativeDataVec.clear();
	CString RetData, ziduan;
	_variant_t val;
	RetData = L"NULL";
	ADOManage ado;
	ado.ConnSQL();
	ado.GetRela1_13(IMEI);
	if (ado.m_pRecordSet->adoEOF)
	{
		ado.CloseAll();
		//ShowDZLaserMsg(L"数据库无关联数据",1);
		return FALSE;
	}
	for (int j = 0; j <= 13; j++)
	{
		if (j == 0)
		{
			val = ado.m_pRecordSet->GetCollect(L"SN");
			if (val.vt != VT_NULL)
			{
				AutoRelativeDataVec.push_back(val.bstrVal);
			}
			else
			{
				AutoRelativeDataVec.push_back(RetData);
			}
		}
		else
		{
			ziduan.Format(L"IMEI%d", j);
			val = ado.m_pRecordSet->GetCollect(_variant_t(ziduan));
			if (val.vt != VT_NULL)
			{
				AutoRelativeDataVec.push_back(val.bstrVal);
			}
			else
			{
				AutoRelativeDataVec.push_back(RetData);
			}
		}

	}
	ado.CloseAll();
	return TRUE;
}

//开始镭雕函数
int CMFC_LaserDlg::LdCore()
{
	UpdateData(TRUE);
	int sendSucceedFlag;
	CString StrImeiLd, StrSnLd; //镭雕用的变量
	ADOManage adomanage;
	adomanage.ConnSQL();
	LdFinishFlag = FALSE;
	CString midChange;
	if (BurnIsOpenFlag)
	{
		//GetDlgItemText(IDC_NEXTLDIMEI, midChange);
		if (BurnVec.size() == 0 || LdIndex >= BurnVec.size())
		{
			CString SizeStr, LdIndexStr;
			SizeStr.Format(L"%d", BurnVec.size());
			LdIndexStr.Format(L"%d", LdIndex);
			SetRicheditText(_T("镭雕索引超出[ size:") + SizeStr + _T(" , LdIndex:") + LdIndexStr + _T("]"), 1);
			SendToPlc(8);
			SendToPlc(80);
			return 0;
		}
		SetDlgItemText(IDC_LdImei_EDIT, BurnVec[LdIndex]);
		UpdateWindow();
	}
	//先判断IMEI号段是否已经全部镭雕完
	if (m_LdRandom)
	{
		if (TxTResult.empty())
		{
			SetRicheditText(_T("本TXT文件中的IMEI已全部镭雕完毕"), 0);
			MessageBox(L"本TXT文件中的IMEI已全部镭雕完毕！", L"提示信息", NULL);
			LdFinishFlag = TRUE;
			SendToPlc(8);
			SendToPlc(80);
			LdIndex++;
			return 0;
		}
		int length = TxTResult.size();
		CurrentLdImei = TxTResult[length - 1];
		SetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
		UpdateWindow();
	}
	GetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
	if (CurrentLdImei > FImeiEnd)
	{
		SetRicheditText(_T("IMEI：") + CurrentLdImei + _T("在号段外，请检查是否已镭雕完此号段"), 1);
		MessageBox(L"本次IMEI号段已全部镭雕完毕！", L"提示信息", NULL);
		LdFinishFlag = TRUE;
		SendToPlc(8);
		SendToPlc(80);
		return 0;
	}
	if (m_RelativeSN)//镭雕递增Sn时，判断是否超出号段
	{
		CString SnPre, Snlaf, endSN, justSN;
		GetDlgItemText(IDC_LdSn_EDIT, SnPre);
		justSN = SnPre.Right(SN2.GetLength());
		if (justSN > SN3 || justSN < SN2)
		{
			SetRicheditText(_T("此SN号在号段外，请检查是否已镭雕完本号段"), 1);
			MessageBox(L"本次SN号段已全部镭雕完毕！", L"提示信息", NULL);
			LdFinishFlag = TRUE;
			SendToPlc(8);
			SendToPlc(80);
			return 0;
		}
	}
	//有无校验位要区分开
	if (!m_NoCheckCodeCheckControl.GetCheck())
	{
		StrImeiLd = CreateIMEI15(CurrentLdImei);
	}
	else
	{
		StrImeiLd = CurrentLdImei;
	}
	if (m_ReLD)  //是重打
	{
		goto ISRELD;
	}
	if (BurnIsOpenFlag)
	{
		goto ISRELD;
	}
	BOOL IMEIRepairFlag = TRUE;
	do
	{
		if (m_AutoCheckPrintVal)
		{
			if (adomanage.CheckLdIMEIExitLM(StrImeiLd) == 1 || adomanage.CheckLdIMEIExitPrint(StrImeiLd) == 1)
			{
				SetRicheditText(L"该IMEI：" + StrImeiLd + L"已镭雕过，自动过滤", 1);
				UpdateWindow();
				WritetoTxt(L"Laser_" + zhidan + _T("_"), L"该IMEI：" + StrImeiLd + L"已镭雕过\r\n");
				if (m_LdRandom && !TxTResult.empty())
				{
					TxTResult.pop_back();
					int length = TxTResult.size();
					CurrentLdImei = TxTResult[length - 1];
				}
				else
				{
					unsigned long long imeiint = 0;
					imeiint = _atoi64(CStringA(CurrentLdImei)) + IncreasingnumberInt;
					CurrentLdImei = _ui64toa(imeiint, CT2A(CurrentLdImei), 10);
				}
				if (!m_NoCheckCodeCheckControl.GetCheck())
				{
					StrImeiLd = CreateIMEI15(CurrentLdImei);
				}
				else
				{
					StrImeiLd = CurrentLdImei;
				}
			}
			else
			{
				SetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
				UpdateWindow();
				//GetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
				if (!m_NoCheckCodeCheckControl.GetCheck())
				{
					StrImeiLd = CreateIMEI15(CurrentLdImei);
				}
				else
				{
					StrImeiLd = CurrentLdImei;
				}
				IMEIRepairFlag = FALSE;
			}
		}
		else
		{
			if (adomanage.CheckLdIMEIExitLM(StrImeiLd) == 1)
			{
				SetRicheditText(L"该IMEI：" + StrImeiLd + L"已镭雕过，自动过滤", 1);
				UpdateWindow();
				WritetoTxt(L"Laser_" + zhidan + _T("_"), L"该IMEI：" + StrImeiLd + L"已镭雕过\r\n");
				if (m_LdRandom && !TxTResult.empty())
				{
					TxTResult.pop_back();
					int length = TxTResult.size();
					CurrentLdImei = TxTResult[length - 1];
				}
				else
				{
					unsigned long long imeiint = 0;
					imeiint = _atoi64(CStringA(CurrentLdImei)) + IncreasingnumberInt;
					CurrentLdImei = _ui64toa(imeiint, CT2A(CurrentLdImei), 10);
				}
				if (!m_NoCheckCodeCheckControl.GetCheck())
				{
					StrImeiLd = CreateIMEI15(CurrentLdImei);
				}
				else
				{
					StrImeiLd = CurrentLdImei;
				}
			}
			else
			{
				SetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
				UpdateWindow();
				//GetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
				if (!m_NoCheckCodeCheckControl.GetCheck())
				{
					StrImeiLd = CreateIMEI15(CurrentLdImei);
				}
				else
				{
					StrImeiLd = CurrentLdImei;
				}
				IMEIRepairFlag = FALSE;
			}
		}

	} while (IMEIRepairFlag);
ISRELD:
	if (ChangeLdName(L"IMEI", StrImeiLd) == 0)
	{
		SetRicheditText(L"镭雕失败", 1);
		LdFinishFlag = TRUE;
		SendToPlc(8);
		SendToPlc(80);
		LdIndex++;
		return 0;
	}
	if (m_RelativeSN)
	{
		GetDlgItemText(IDC_LdSn_EDIT, StrSnLd);
		if (ChangeLdName(L"SN", StrSnLd) == 0)
		{
			SetRicheditText(L"镭雕失败", 1);
			LdFinishFlag = TRUE;
			SendToPlc(8);
			SendToPlc(80);
			LdIndex++;
			return 0;
		}
	}
	if (RelativeIMEI2 || RelativeIMEI3 || RelativeIMEI4 || RelativeIMEI5 || RelativeIMEI6 || RelativeIMEI7 || RelativeIMEI8 || RelativeIMEI9\
		|| RelativeIMEI10 || RelativeIMEI11 || RelativeIMEI12 || RelativeIMEI13)
	{
		if (!AutoGetAllRelativeData(StrImeiLd))
		{
			SetRicheditText(L"无关联数据！！", 1);
			LdFinishFlag = TRUE;
			SendToPlc(8);
			SendToPlc(80);
			LdIndex++;
			return 0;
			return FALSE;
		}
		adomanage.GetRelativeIMEI2_IMEI13(StrImeiLd);
		if (RelativeIMEI2)
		{
			if (ChangeLdName(L"IMEI2", adomanage.IMEI2Str) == 0) {
				SetRicheditText(L"替换对象IMEI2时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI3)
		{
			if (ChangeLdName(L"IMEI3", adomanage.IMEI3Str) == 0) {
				SetRicheditText(L"替换对象IMEI3时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI4)
		{
			if (ChangeLdName(L"IMEI4", adomanage.IMEI4Str) == 0) {
				SetRicheditText(L"替换对象IMEI4时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI5)
		{
			if (ChangeLdName(L"IMEI5", adomanage.IMEI5Str) == 0) {
				SetRicheditText(L"替换对象IMEI5时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI6)
		{
			if (ChangeLdName(L"IMEI6", adomanage.IMEI6Str) == 0) {
				SetRicheditText(L"替换对象IMEI6时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI7)
		{
			if (ChangeLdName(L"IMEI7", adomanage.IMEI7Str) == 0) {
				SetRicheditText(L"替换对象IMEI7时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI8)
		{
			if (ChangeLdName(L"IMEI8", adomanage.IMEI8Str) == 0) {
				SetRicheditText(L"替换对象IMEI8时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI9)
		{
			if (ChangeLdName(L"IMEI9", adomanage.IMEI9Str) == 0) {
				SetRicheditText(L"替换对象IMEI9时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI10)
		{
			if (ChangeLdName(L"IMEI10", adomanage.IMEI10Str) == 0) {
				SetRicheditText(L"替换对象IMEI10时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI11)
		{
			if (ChangeLdName(L"IMEI11", adomanage.IMEI11Str) == 0) {
				SetRicheditText(L"替换对象IMEI11时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI12)
		{
			if (ChangeLdName(L"IMEI12", adomanage.IMEI12Str) == 0) {
				SetRicheditText(L"替换对象IMEI12时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
		if (RelativeIMEI13)
		{
			if (ChangeLdName(L"IMEI13", adomanage.IMEI13Str) == 0) {
				SetRicheditText(L"替换对象IMEI13时出现错误，镭雕失败", 1);
				LdFinishFlag = TRUE;
				SendToPlc(8);
				SendToPlc(80);
				LdIndex++;
				return 0;
			}
		}
	}
	CString ttCS, List2Val, List3Val, ConnVal, ConnChar;
	int ConnNum;
	for (int tt = 2; tt < 48; tt++)
	{
		ConnVal = L"";
		ConnChar = L"";
		if (tt == 0 || tt == 1)
		{

			if ((m_AutoList.GetItemText(tt, 1) != "") && (m_AutoList.GetItemText(tt, 2) != "") && (m_AutoList.GetItemText(tt, 3) != ""))
				if (ChangeLdName((LPTSTR)(LPCTSTR)m_AutoList.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)m_AutoList.GetItemText(tt, 3)) == 0)
				{
					SetRicheditText(L"镭雕失败", 1);
					LdFinishFlag = TRUE;
					SendToPlc(8);
					SendToPlc(80);
					LdIndex++;
					return 0;
				}
		}
		else
		{
			List2Val = m_AutoList.GetItemText(tt, 2);
			if (List2Val.Find(L"Other") == -1)
			{

				ConnChar = List3Val.Left(1);
				List3Val = List3Val.Mid(1);
				while (true)
				{
					if (List3Val.Find(ConnChar) != -1)
					{
						ConnNum = _ttoi(List3Val.Mid(0, List3Val.Find(ConnChar)));
						if (ConnNum == 1)
						{
							ConnVal += StrImeiLd + ConnChar;
						}
						else
						{
							ConnVal += AutoRelativeDataVec.at(ConnNum) + ConnChar;
						}
					}
					else
					{
						ConnNum = _ttoi(List3Val);
						if (ConnNum == 1)
						{
							ConnVal += StrImeiLd;
						}
						else
						{
							ConnVal += AutoRelativeDataVec.at(ConnNum);
						}
						break;
					}
					List3Val = List3Val.Mid(List3Val.Find(ConnChar) + 1);
				}
				if (ChangeLdName((LPTSTR)(LPCTSTR)m_AutoList.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)ConnVal) == 0)
				{
					SetRicheditText(L"镭雕失败", 1);
					LdFinishFlag = TRUE;
					SendToPlc(8);
					SendToPlc(80);
					LdIndex++;
					return 0;
				}
			}
			else
			{
				break;
			}
		}
	}
	SetRicheditText(L"镭雕中。。。", 0);
	char szIpAdd[256];
	USES_CONVERSION; //定义后才能使用T2A
	sprintf_s(szIpAdd, 256, "%s", T2A(StrImeiLd));
	send(clientSock, szIpAdd, 256, 0);
	DllFlag = markezd.lmc1_Mark(TRUE);
	DllStr.Format(_T("%d"), DllFlag);
	if (DllFlag == 0)
	{
		SetRicheditText(L"IMEI号:" + StrImeiLd + L"已镭雕完成", 0);
		WritetoTxt(L"Laser_" + zhidan + _T("_"), L"IMEI号:" + StrImeiLd + L"已镭雕完成\r\n");
		//adomanage.UpdateLdData(StrImeiLd, StrSnLd);三合一用
		if (m_ReLD)
		{
			adomanage.UpdateReLdData(StrImeiLd);
		}
		else
		{
			adomanage.InsertLdData(StrImeiLd, ComputerIp, zhidan, LDsoftmodel, LDversion);
		}
		LdToCompare = StrImeiLd;
		if (m_LdRandom && !TxTResult.empty())
		{
			if (adomanage.CheckLdzhidanExit(zhidan) == 0)
			{
				adomanage.insertLdSnAndImei(StrSnLd, CurrentLdImei, zhidan);
			}
			else
			{
				adomanage.UpdateLdSnAndImei(StrSnLd, CurrentLdImei, zhidan);
			}
			TxTResult.pop_back();
		}
		else
		{
			//						//镭雕完记录数据后IMEI号+1
			unsigned long long imeiint = 0;
			imeiint = _atoi64(CStringA(CurrentLdImei)) + IncreasingnumberInt;
			CurrentLdImei = _ui64toa(imeiint, CT2A(CurrentLdImei), 10);
			SetDlgItemText(IDC_LdImei_EDIT, CurrentLdImei);
			if (adomanage.CheckLdzhidanExit(zhidan) == 0)
			{
				adomanage.insertLdSnAndImei(StrSnLd, CurrentLdImei, zhidan);
			}
			else
			{
				adomanage.UpdateLdSnAndImei(StrSnLd, CurrentLdImei, zhidan);
			}
		}
		if (m_RelativeSN)//镭雕完后SN号+1
		{
			unsigned long long snint = 0;
			CString SnPre, Snlaf, lengthstr;
			int length = SN2.GetLength();
			lengthstr.Format(_T("%d"), length);
			SnPre = StrSnLd.Left(StrSnLd.GetLength() - SN2.GetLength());
			Snlaf = StrSnLd.Right(SN2.GetLength());
			snint = _ttoi(Snlaf) + IncreasingnumberInt;
			Snlaf.Format(_T("%0") + lengthstr + _T("d"), snint);
			StrSnLd = SnPre + Snlaf;
			SetDlgItemText(IDC_LdSn_EDIT, StrSnLd);
			adomanage.UpdateLdSn(StrSnLd, zhidan);
		}
	}
	else
	{
		SetRicheditText(L"镭雕失败，错误代码" + DllStr, 1);
		WritetoTxt(L"Laser_" + zhidan + _T("_"), L"镭雕失败，错误代码" + DllStr + L"\r\n");
		/*DllFlag = markezd.lmc1_Mark(FALSE);
		CString stt;
		stt.Format(L"%d", DllFlag);
		SetRicheditText(stt,1);*/
		adomanage.CloseAll();
		LdFinishFlag = TRUE;
		SendToPlc(8);
		SendToPlc(80);
		LdIndex++;
		return 0;
	}
	adomanage.CloseAll();
	//SetDlgItemText(IDC_NEXTCOMPAREIMEI, LdToCompare);
	LdVec.push_back(LdToCompare);
	UpdateWindow();
	LdFinishFlag = TRUE;
	SendToPlc(7);
	SendToPlc(70);
	LdIndex++;
	return 1;
}

//替换指定对象名称的值
int CMFC_LaserDlg::ChangeLdName(CString LdVariable, CString strld)
{
	DllFlag = markezd.lmc1_ChangeTextByName((LPTSTR)(LPCTSTR)LdVariable, (LPTSTR)(LPCTSTR)strld);
	DllStr.Format(_T("%d"), DllFlag);
	if (DllFlag == 0)
	{

	}
	else
	{
		SetRicheditText(L"替换对象" + LdVariable + L"的值失败，错误代码" + DllStr, 1);
		WritetoTxt(L"Laser_" + zhidan + _T("_"), L"替换对象的值失败，错误代码" + DllStr + L"\r\n");
		return 0;
	}
	return 1;
}

//镭雕一次
void CMFC_LaserDlg::OnBnClickedLdoncetimeButton()
{

	LdCore();
	// TODO:  在此添加控件通知处理程序代码
}

//旋转镭雕模式
void CMFC_LaserDlg::OnBnClickedStartlaserButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString buttontext;
	GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(FALSE);
	Sleep(200);
	GetDlgItemText(IDC_StartLaser_BUTTON, buttontext);
	if (buttontext == "开启旋转镭雕模式")
	{
		GetDlgItem(IDC_LdOnceTime_BUTTON)->EnableWindow(FALSE);
		SetDlgItemText(IDC_StartLaser_BUTTON, _T("关闭旋转镭雕模式"));
		//开启子线程
		if (ThreadReadLdPort == NULL)
			ThreadReadLdPort = AfxBeginThread(ReadLdPortThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_LdPortRun = true;
		LDIsOpenFlag = TRUE;

		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPENRELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(TRUE);
		return;
	}
	else
	{
		SetDlgItemText(IDC_StartLaser_BUTTON, _T("开启旋转镭雕模式"));
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPENRELAY)->EnableWindow(TRUE);
		m_LdPortRun = false;
		LDIsOpenFlag = FALSE;
		ThreadReadLdPort = NULL;
		return;
	}
}

//旋转镭雕线程
UINT ReadLdPortThread(LPVOID pParam)
{
	CMFC_LaserDlg* dlg;
	dlg = (CMFC_LaserDlg*)pParam;
	dlg->UpdateData(TRUE);
	CString destStr, destBit;
	if (!dlg->m_checkState)
	{
		while (m_LdPortRun)
		{
			dlg->SendToPlc(11);
			destStr = dlg->ReadFromPLCPort();
			destStr = dlg->HexStr2BinStr(destStr);
			destBit = destStr.Mid(15, 1);
			dlg->myLdport.SetWindowTextW(destBit);
			if (destBit == L"1")
			{
				dlg->LdCore();
				destBit = L"0";
				destStr = L"";
				dlg->myLdport.SetWindowTextW(destBit);
			}
			//Sleep(200);//停止0.5秒然后接着跑
		}
	}
	else
	{
		while (m_LdPortRun)
		{
			if (readFromClient.Find(L"TCP") != -1)
			{
				dlg->myLdport.SetWindowTextW(L"1");
				dlg->LdCore();
				readFromClient = L"";
				dlg->myLdport.SetWindowTextW(L"0");
			}
			Sleep(100);
		}
	}

	return 0;
}

//PLC交互
CString CMFC_LaserDlg::ReadFromPLCPort()
{
	//串口变量
	char str[256] = { 0 };
	char destStr[256] = { 0 };
	memset(str, 0, sizeof(str) / sizeof(str[0]));
	int length;
	DWORD readreal = 0;
	BOOL bReadStat;
	CString strread;
	char szMsg[255] = { 0 };
	char p_StrBcd[255] = { 0 };
	bReadStat = ReadFile(Port3handler, str, 199, &readreal, 0);
	if (bReadStat)
	{
		Sdk_Str2BcdStr(str, 6, destStr);
		strread = destStr;
	}
	else
	{
		strread = "";
	}
	return strread;
}
void Sdk_Int2Char(int p_nNum, char *p_Ch)
{
	if (p_nNum <= 9)
	{
		*p_Ch = p_nNum + '0';
	}
	else
	{
		/*0 - 9 是十个数*/
		*p_Ch = (p_nNum - 10) + 'A';
	}
}
void Sdk_Str2BcdStr(char *p_Str, int p_nLen, char *p_StrBcd)
{
	int i = 0, j = 0;

	for (i = 0; i < p_nLen; ++i)
	{
		Sdk_Int2Char((p_Str[i] >> 4) & 0x0F, &p_StrBcd[j++]);
		Sdk_Int2Char(p_Str[i] & 0x0F, &p_StrBcd[j++]);
	}
	return;
}

/*打开继电器串口*/
void CMFC_LaserDlg::OnBnClickedOpenrelay()
{
	// TODO: 在此添加控件通知处理程序代码
	CString MyConnButtonText;
	GetCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);
	GetDlgItem(IDC_OPENRELAY)->GetWindowText(MyConnButtonText);
	if (MyConnButtonText == L"打开PLC串口")
	{

		Port3handler = InitCom2(Port3No);
		if (Port3handler == NULL)
		{
			SetRicheditText(L"打开PLC串口失败", 1);
			return;
		}
		SetRicheditText(L"PLC串口初始化成功", 0);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(false);
		GetDlgItem(IDC_OPENRELAY)->SetWindowText(L"关闭PLC串口");
	}
	else
	{
		if (!CloseCom(Port3handler))
		{
			SetRicheditText(L"关闭继PLC串口失败", 1);
			MessageBox(L"关闭PLC串口失败", L"提示信息", NULL);
			return;
		}
		SetRicheditText(L"关闭PLC串口成功", 0);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(true);
		GetDlgItem(IDC_OPENRELAY)->SetWindowText(L"打开PLC串口");
	}

	//ShowBurningMsg(L"继电器串口初始化成功!");
}

/*是否双面镭雕*/
void CMFC_LaserDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_checkState)
	{
		listenFlag = TRUE;
		readFromClientFlag = TRUE;
		AfxBeginThread(listenThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		AfxBeginThread(readFromLd2, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else
	{
		closesocket(m_SockListen);
		//closesocket(m_SockListen);
		LD2ConnectFlag = FALSE;
		listenFlag = FALSE;
		readFromClientFlag = FALSE;
	}
}

/*tcpServer监听线程*/
UINT listenThread(LPVOID lparam)
{
	CMFC_LaserDlg * pServer = (CMFC_LaserDlg *)lparam;;
	CString ipstr;
	int port;
	if (INVALID_SOCKET == (pServer->m_SockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox(_T("建立socket失败"));
		return 0;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	pServer->GetDlgItemText(IDC_LocalPcName_EDIT, ipstr);
	port = pServer->GetDlgItemInt(ID_PORT);
	char szIpAdd[32];
	USES_CONVERSION; //定义后才能使用T2A
	sprintf_s(szIpAdd, 32, "%s", T2A(ipstr));
	service.sin_addr.S_un.S_addr = inet_addr(szIpAdd);
	//service.sin_addr.s_addr = inet_addr(cstr);
	//inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);
	service.sin_port = htons(port);
	if (0 != bind(pServer->m_SockListen, (sockaddr *)&service, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("绑定端口失败"));
		return 0;
	}
	if (0 != listen(pServer->m_SockListen, 5))
	{
		AfxMessageBox(_T("监听端口失败"));
		return 0;
	}

	while (listenFlag)
	{
		sockaddr_in clientAddr;
		int iLen = sizeof(sockaddr_in);
		pServer->clientSock = accept(pServer->m_SockListen, (struct sockaddr *)&clientAddr, &iLen);
		if (pServer->clientSock == INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			LD2ConnectFlag = TRUE;
			//pServer->MessageBox(L"镭雕机二连接成功");
		}
		Sleep(100);
	}
	return 0;
}
UINT readFromLd2(LPVOID lparam)
{
	USES_CONVERSION;
	CMFC_LaserDlg *dlg = (CMFC_LaserDlg*)lparam;
	while (readFromClientFlag)
	{

		char szRev[256] = { 0 };
		iRet = recv(dlg->clientSock, szRev, sizeof(szRev), 0);
		if (iRet > 0)
		{
			readFromClient = A2T(szRev); //中文出现乱码，英文正常
			dlg->SetRicheditText(readFromClient, 0);
			//dlg->SetDlgItemTextW(IDC_LocalPcName_EDIT, readFromClient);

		}
	}
	return 0;
}

//重新镭雕
void CMFC_LaserDlg::OnBnClickedReld()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_ReLD)
	{
		GetDlgItem(IDC_LdImei_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LdSn_EDIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_LdImei_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LdSn_EDIT)->EnableWindow(FALSE);
	}
}

//设置关联
void CMFC_LaserDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	SetRelative SetRelativeDlg;
	INT_PTR res = SetRelativeDlg.DoModal();
}

//导入文档
void CMFC_LaserDlg::OnBnClickedLodatxt()
{
	// TODO: 在此添加控件通知处理程序代码
	CString   FilePathName, txtValue;//文件名参数定义
	CStdioFile file;
	CFileDialog  Dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"TXT Files(*.txt)|*.txt|All Files(*.*)|*.*", this, NULL, TRUE);
	//打开文件
	if (Dlg.DoModal() == IDOK)//是否打开成功
	{
		FilePathName = Dlg.GetPathName();//取得文件路径及文件名
	}
	else
	{
		return;
	}
	if (!file.Open(FilePathName, CFile::modeRead))
	{
		return;
	}
	while (file.ReadString(txtValue))
	{
		TxTResult.push_back(txtValue);
	}
	MessageBox(L"导入成功", L"提示");
	file.Close();
}


//镭雕串口
void CMFC_LaserDlg::OnDropdownJdqCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);
}



/*****************************烧写IMEI模块******************************/
//从注册表中寻找已经注册串口号然后展示出来
void CMFC_LaserDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo)
{
	//先获取当前串口号
	int cur = 0;
	int nSel;
	BOOL curflag = TRUE;
	nSel = pComboBox->GetCurSel();
	if (nSel >= 0)
	{
		pComboBox->GetLBText(nSel, ComNo);
	}

	//先清空一下列表
	pComboBox->ResetContent();

	HKEY hKey;
#ifdef _DEBUG
	ASSERT(pComboBox != NULL);
	pComboBox->AssertValid();
#endif
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // 打开串口注册表
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPWSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // 枚举串口
			{
				break;
			}
			//if (nSel >= 0 && ComNo == LPWSTR(commName))//如果跟上次选择的相等那就不让当前选择变动
			//{
			//	/*cur = i - PortNO+1;*/
			//	cur = i;
			//	curflag = FALSE;
			//}
			////如果不相等那就按顺序自动分配一下，结合一个标志位使得各个串口不会对cur重复赋值
			//else
			//{
			//	if (PortNO == 1 && i == 0 && curflag == TRUE)
			//	{
			//		cur = i;
			//	}
			//	else if (PortNO == 2 && i == 1 && curflag == TRUE)
			//	{
			//		cur = i;
			//	}
			//	else if (PortNO == 3 && i == 2 && curflag == TRUE)
			//	{
			//		cur = i;
			//	}
			//	else if (PortNO == 4 && i == 3 && curflag == TRUE)
			//	{
			//		cur = i;
			//	}
			//}
			pComboBox->AddString(LPWSTR(commName));
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			//SetRicheditText(L"找不到串口！请检测串口设备是否存在问题！", 1);
			//MessageBox(L"找不到串口！请检测串口设备是否存在问题！", L"提示信息", NULL);
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//点击串口下拉框时刷新注册表串口
void CMFC_LaserDlg::OnDropdownBurningportCombo()
{
	// TODO:  在此添加控件通知处理程序代码

	FindCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);
}

//获取当前串口号
void CMFC_LaserDlg::GetCommPort(CComboBox *pComboBox, CString &ComNo)
{
	int nSel;
	nSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel, ComNo);

	if (ComNo == "")
	{
		MessageBox(L"请先选择串口号！", L"提示信息", NULL);
	}
}

//一键连接按钮
void CMFC_LaserDlg::OnBnClickedBurningconnButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//if (IniTestCommand.IsEmpty())
	//{
	//	MessageBox(L"请先获取测试指令",L"提示");
	//	return;
	//}
	UpdateData(TRUE);
	GetCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);//先获取当前串口号
	//GetCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);//先获取当前串口号
	//if (Port1No == Port3No){
	//	MessageBox(L"请选择正确的烧写和继电器串口", L"提示信息", NULL);
	//	return;
	//}
	CString ConnButtonText;
	GetDlgItem(IDC_BurningConn_BUTTON)->GetWindowText(ConnButtonText);
	if (ConnButtonText == "一键开始")
	{
		//判断串口有没有开启成功
		if (StationValVector.size() == 0)
		{
			MessageBox(_T("获取指令失败，请检查是否已上传指令至服务器"), _T("提示"), NULL);
			return;
		}
		Port1handler = InitCom(Port1No);
		if (Port1handler == NULL)
		{
			ShowBurningMsg(L"打开串口失败", 1);
			return;
		}
		ShowBurningMsg(L"烧写串口初始化成功!", 0);
		BurnIsOpenFlag = TRUE;
		WritetoTxt(L"Burn_" + zhidan + _T("_"), L"打开烧写串口成功\r\n");
		//打开继电器串口
		/*Port3handler = InitCom1(Port3No);
		if (Port3handler == NULL)
		{
			ShowBurningMsg(L"打开继电器串口失败");
			return;
		}
		ShowBurningMsg(L"继电器串口初始化成功!");	*/
		int MyDataCount = 0;
		MyDataCount = _ttoi64(m_EndIMEIVal) - _ttoi64(m_BurningEditVal);
		WriteThread = AfxBeginThread(SendDBCommand, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_BurningConrtolFlag = true;
		BurnLastSendTestFlag = true;
		SetDlgItemText(IDC_BurningConn_BUTTON, TEXT("一键关闭"));
		SetDlgItemText(IDC_BurningRemind_STATIC, L"就绪");
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(false);

		//GetDlgItem(IDC_BurningScanningGun_Button)->EnableWindow(false);
		GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(false);
		//GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(false);
	}
	else
	{
		if (!CloseCom(Port1handler))
		{
			ShowBurningMsg(L"关闭串口失败", 1);
			MessageBox(L"关闭串口1失败", L"提示信息", NULL);
			return;
		}
		ShowBurningMsg(L"关闭烧写串口成功", 0);
		BurnIsOpenFlag = FALSE;
		WritetoTxt(L"Burn_" + zhidan + _T("_"), L"关闭烧写串口成功\r\n");
		//关闭继电器串口
		/*if (!CloseCom(Port3handler))
		{
			ShowBurningMsg(L"关闭继电器串口失败");
			MessageBox(L"关闭继电器串口失败", L"提示信息", NULL);
			return;
		}
		ShowBurningMsg(L"关闭继电器串口成功");*/
		m_BurningConrtolFlag = false; //停止线程
		BurnLastSendTestFlag = false;
		//SendTestFlag = false;
		Sleep(2000);
		SetDlgItemText(IDC_BurningConn_BUTTON, TEXT("一键开始"));
		SetDlgItemText(IDC_BurningRemind_STATIC, L"未连接");
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(true);
		//GetDlgItem(IDC_BurningScanningGun_Button)->EnableWindow(true);
		GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(true);
		//GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(true);
	}
}

//显示烧写日志
int CMFC_LaserDlg::ShowBurningMsg(CString strMsg, int MsgNum)
{
	CString str = GetTime() + _T("\r\n ");
	str += strMsg + _T("\r\n");
	m_BurnLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (MsgNum)
	{
	case 0:
		m_BurnLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		break;
	case 1:
		m_BurnLog.SetSelectionCharFormat(cf);
		m_BurnLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		break;
	default:
		break;
	}
	m_BurnLog.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return 0;
}

//读取串口返回值
CString CMFC_LaserDlg::ReadFromBurningPort()
{

	//串口变量
	char str[200];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;
	CString strread;
	char szMsg[255] = { 0 };
	char p_StrBcd[255] = { 0 };
	bReadStat = ReadFile(Port1handler, str, 199, &readreal, 0);
	if (bReadStat) {
		strread = str;
	}
	else
	{
		strread = "";
	}
	return strread;
}

//判断IMEI是否存在
int CMFC_LaserDlg::CheckIMEI(CString IMEI)
{
	ADOManage adomanageJudge;
	int judgeflag;
	adomanageJudge.ConnSQL();
	judgeflag = adomanageJudge.CheckIMEIExit(IMEI);
	adomanageJudge.CloseAll();
	return judgeflag;
}

//查chipid是否与其他imei绑定
CString CMFC_LaserDlg::getBandIMEIByChipID(CString chipid)
{
	ADOManage adomanageJudge;
	CString judgeflag;
	adomanageJudge.ConnSQL();
	judgeflag = adomanageJudge.checkAndGetIMEIbyID(chipid);
	adomanageJudge.CloseAll();
	return judgeflag;
}

//获取与RID绑定得IMEI号
CString CMFC_LaserDlg::CheckRID(CString IMEI)
{
	ADOManage adomanageJudge;
	CString IMEIStr;
	adomanageJudge.ConnSQL();
	IMEIStr = adomanageJudge.CheckRIDExit(IMEI);
	adomanageJudge.CloseAll();
	return IMEIStr;
}
int CMFC_LaserDlg::checkCompareResult(CString IMEI)
{
	ADOManage adomanageJudge;
	int checkResult;
	adomanageJudge.ConnSQL();
	checkResult = adomanageJudge.CheckCompareIMEIExit(IMEI);
	adomanageJudge.CloseAll();
	return checkResult;
}

//插入烧写信息
BOOL CMFC_LaserDlg::InsertBurningMes(CString zhidan, CString Imei, CString ComputerIp, CString burningresult, CString Rid)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.InsertBurningData(zhidan, Imei, ComputerIp, burningresult, Rid);
	adomanageInsert.CloseAll();
	return TRUE;
}

//更新烧写信息
BOOL CMFC_LaserDlg::UpdateBurningMes(CString Imei, CString Rid, CString computerIp, CString BurnResult)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.UpdateBurningData(Imei, Rid, BurnResult, computerIp);
	adomanageInsert.CloseAll();
	return TRUE;
}

/*更新对比信息*/
BOOL CMFC_LaserDlg::UpdateCompareMes(CString Imei, CString BurnResult)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.UpdateCompareData(Imei, BurnResult);
	adomanageInsert.CloseAll();
	return TRUE;
}

//更新IMEI当前位
void CMFC_LaserDlg::UpdataBurningMes(CString zhidan, CString Imei)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.UpdateCurrnetImei(Imei, zhidan);
	adomanageInsert.CloseAll();
}

//进入扫描枪模式按钮
void CMFC_LaserDlg::OnBnClickedBurningscanninggunButton()
{
	// TODO:  在此添加控件通知处理程序代码

	CString scanninggunButtonText;
	GetDlgItemText(IDC_BurningScanningGun_Button, scanninggunButtonText);
	//GetCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);//先获取当前串口号
	if (scanninggunButtonText == "进入扫描枪模式") {
		//判断串口有没有开启成功
		/*Port1handler = InitCom(Port1No);
		if (Port1handler == NULL)
		{
			ShowBurningMsg(L"打开串口失败", 1);
			return;
		}
		ShowBurningMsg(L"串口初始化成功!", 0);*/
		EnterScanFlag = TRUE;
		/*	GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(false);
			GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(false);
			GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(true);
			GetDlgItem(IDC_SendCommand1_EDIT)->EnableWindow(false);
			GetDlgItem(IDC_SendCommand2_EDIT)->EnableWindow(false);
			GetDlgItem(IDC_ReciveCommand_EDIT)->EnableWindow(false);*/
			//GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(false);
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(true);
		m_BurningImeiEdit.SetFocus();
		SetDlgItemText(IDC_BurningScanningGun_Button, L"关闭扫描枪模式");
		SetDlgItemText(IDC_BurningRemind_STATIC, L"就绪");
		SetDlgItemText(IDC_BurningImei_EDIT, L"");
	}
	else {
		/*if (!CloseCom(Port1handler))
		{
			ShowBurningMsg(L"关闭串口失败", 1);
			MessageBox(L"关闭串口1失败", L"提示信息", NULL);
			return;
		}
		ShowBurningMsg(L"关闭串口成功", 0);*/
		EnterScanFlag = FALSE;
		/*GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(true);
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_SendCommand1_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_SendCommand2_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_ReciveCommand_EDIT)->EnableWindow(true);*/
		//GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(false);
		SetDlgItemText(IDC_BurningScanningGun_Button, L"进入扫描枪模式");
		SetDlgItemText(IDC_BurningRemind_STATIC, L"未连接");
		CString CurrentImei;
		GetDlgItemText(IDC_ImeiCurrent_EDIT, scanninggunButtonText);
		SetDlgItemText(IDC_BurningImei_EDIT, scanninggunButtonText);
	}
}

// 获取MachineName字段数据
CString CMFC_LaserDlg::getMachineName(CString zhidan)
{
	ADOManage ado;
	ado.ConnSQL();
	CString subName;
	subName = ado.adoGetMachineName(zhidan);
	ado.CloseAll();
	return subName;
}

//根据id查数据库，是否已烧写IMEI
BOOL CMFC_LaserDlg::checkIMEIbyID(CString chipID)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	if (myadomanage.checkIMEIbyID(chipID))
	{
		myadomanage.CloseAll();
		return TRUE;
	}
	else
	{
		myadomanage.CloseAll();
		return FALSE;
	}
}

//获取关联写号数据
CString CMFC_LaserDlg::getRelativeFata(CString param, CString paramIMEI)
{
	ADOManage myadomanage;
	CString relativeData;
	myadomanage.ConnSQL();
	relativeData = myadomanage.getRelativeIMEIX(param, paramIMEI);
	myadomanage.CloseAll();
	return relativeData;
}

//保存获取的数据到数据库
void CMFC_LaserDlg::saveDataToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.saveData(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//获取新表的数据
CString CMFC_LaserDlg::getNewSheetData(CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	CString dbdata = L"";
	myadomanage.ConnSQL();
	dbdata = myadomanage.getNewSheetDataDB(IMEI, ziduan);
	myadomanage.CloseAll();
	return dbdata;
}

//把获取到的新表数据更新到旧表
void CMFC_LaserDlg::saveNewDataToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.saveData(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//把获取到的数据insert进旧表
void CMFC_LaserDlg::insertNewDataToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.insertToRelative(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//查旧关联表是否存在IMEI
BOOL CMFC_LaserDlg::checkIMEIInRelative(CString strIMEI)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	if (myadomanage.CheckIMEIInRelative(strIMEI))
	{
		myadomanage.CloseAll();
		return TRUE;               //返回true，说明已有IMEI，则把读到的数据更新到关联表
	}
	myadomanage.CloseAll();
	return FALSE;                  //返回false，说明没有IMEI，则把读到的数据和当前烧写的IMEI插入到关联表
}

//查新关联表是否存在IMEI
BOOL CMFC_LaserDlg::checkIMEIInNewRelative(CString strIMEI)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	if (myadomanage.CheckIMEIInRelative(strIMEI))
	{
		myadomanage.CloseAll();
		return TRUE;               //返回true，说明已有IMEI，则把读到的数据更新到关联表
	}
	myadomanage.CloseAll();
	return FALSE;                  //返回false，说明没有IMEI，则把读到的数据和当前烧写的IMEI插入到关联表
}

//更新IMEI到新关联表
void CMFC_LaserDlg::UpdateIMEIToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.UpdateIMEIToRelative(data, IMEI, ziduan);
	myadomanage.CloseAll();
}
//更新ICCID、SIM、MAC到新关联表
void CMFC_LaserDlg::UpdateICCIDToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.UpdateICCIDToRelative(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//更新IMEI到耦合表
void  CMFC_LaserDlg::updateToCoupleTest(CString IMEI, CString chipid)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.updateDatatoCoupleDB(IMEI, chipid);
	myadomanage.CloseAll();
}

//插入成功信息到总表
void CMFC_LaserDlg::insertToMainSheet(CString IMEI, CString result, CString chipid, CString version, CString machinename)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.insertToMainSheetDB(IMEI, result, chipid, version, machinename);
	myadomanage.CloseAll();
}

//如果芯片ID已与IMEI绑定，则更新
void CMFC_LaserDlg::UpdateToMainSheet(CString IMEI, CString result, CString chipid, CString version, CString machinename)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.UpdateToMainSheetDB(IMEI, result, chipid, version, machinename);
	myadomanage.CloseAll();
}

//烧写指令发送线程（发送数据库中的指令）
UINT SendDBCommand(LPVOID lparam)
{
	CMFC_LaserDlg *dlg = (CMFC_LaserDlg*)lparam;
	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//PurgeComm(dlg->Port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	BOOL pcEnterTestFlag = TRUE;
	BOOL bWriteStat;
	BOOL DBLastSendTestFlag = TRUE;
	int tryTimeInt;
	BOOL FailTestFlag;
	DWORD dwBytesWrite;
	BOOL haveBandIMEI = FALSE;
	CString recChipID;
	CString JudgeIMEI;
	CString justIMEI, BurnSendToLd;
	int j;
	CString commandName, commandVal, changeParam, commandReceive, tryTime, readPort, DbTestcommand;
	CString chipID;
	while (m_BurningConrtolFlag)
	{
		dlg->GetDlgItemText(IDC_BurningImei_EDIT, JudgeIMEI);
		if (JudgeIMEI.GetLength() != FImeiStart.GetLength() || JudgeIMEI<FImeiStart || JudgeIMEI.GetLength() != FImeiEnd.GetLength() || JudgeIMEI>FImeiEnd)
		{
			dlg->ShowBurningMsg(L"IMEI:" + JudgeIMEI + L"在当前号段外，请检查此订单号段是否已烧写完成", 1);
			dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI：" + JudgeIMEI + L"在当前号段外\r\n");
			goto FALILDONE;
		}
		dlg->GetCommandAndRec(0, commandName, commandReceive, tryTime, commandVal);
		tryTimeInt = _ttoi(tryTime);
		tryTimeInt = tryTimeInt * 3;
		do
		{
			if (EnterScanFlag)
			{
				BOOL EditHaveVal = TRUE;
				CString EditVal = L"";
				do
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					dlg->GetDlgItemTextW(IDC_BurningImei_EDIT, EditVal);
					if (EditVal.Find(L"\r\n") != -1)
					{
						EditHaveVal = FALSE;
					}
					Sleep(200);
				} while (EditHaveVal);
			}
			dlg->GetCommandAndRec(1, commandName, commandReceive, tryTime, commandVal);
			commandVal.Format(L"%s\r\n", commandVal);
			DbTestcommand = commandVal;
			if (m_BurningConrtolFlag == false)
			{
				goto CLOSETHREAD;
			}
			//dlg->BatOnAndOff(1);
			bWriteStat = WriteFile(dlg->Port1handler, CT2A(DbTestcommand), DbTestcommand.GetLength(), &dwBytesWrite, NULL);
			dlg->ShowBurningMsg(L"发:" + DbTestcommand, 0);
			Sleep(400);
			readPort = dlg->ReadFromBurningPort();
			if (readPort.Find(commandReceive) >= 0)
			{
				//dlg->BatOnAndOff(10);
				dlg->ShowBurningMsg(L"收:" + readPort, 0);
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + DbTestcommand + L"\r\n");
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
				dlg->SendToPlc(3);  //给继电器发送信息，给plc烧写中的信号
				BurnFinishFlag = FALSE;
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
				dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("烧写中"));
				pcEnterTestFlag = false;
				break;
			}
		} while (pcEnterTestFlag);
		for (j = 3; j <= StationValVector.size() - 1; j++)
		{
			dlg->GetCommandAndRec(j, commandName, commandReceive, tryTime, commandVal);
			if (commandName.Find(L"芯片ID") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int k = 0; k <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"收:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");
					if (readPort.Find(commandReceive) != -1)
					{
						chipID = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						chipID = chipID.Left(32);
						dlg->ShowBurningMsg(L"读取芯片ID成功:" + chipID, 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"读取芯片成功：" + chipID + L"\r\n");
						k = tryTimeInt + 1;
					}
					else
					{
						k++;
						if (k > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"读取芯片ID失败", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"读取芯片失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"软件版本") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int p = 0; p <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"发:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						Allversion = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						Allversion = Allversion.Left(Allversion.Find(L"\r\n"));
						dlg->ShowBurningMsg(L"读取软件版本成功：" + Allversion, 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"读取软件版本成功：" + Allversion + L"\r\n");
						p = tryTimeInt + 1;
					}
					else
					{
						p++;
						if (p > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"读取软件版本失败", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"读取软件版本失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"关联写号") != -1)
			{
				CString lastParam;
				lastParam = L"IMEI" + commandName.Right(1);
				CString relativeData = dlg->getRelativeFata(lastParam, CurrentBurningImei);
				commandVal = commandVal + relativeData + L"\r\n";
				for (int L = 0; L <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"发:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						dlg->ShowBurningMsg(commandName + L"成功", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"成功\r\n");
						L = tryTimeInt + 1;
					}
					else
					{
						L++;
						if (L > tryTimeInt)
						{
							dlg->ShowBurningMsg(commandName + L"失败", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"写IMEI1") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int R = 0; R <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"发:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						Allversion = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						dlg->ShowBurningMsg(L"第一条烧写指令发送成功", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"第一条烧写指令发送成功\r\n");
						R = tryTimeInt + 1;
					}
					else
					{
						R++;
						if (R > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"第一条烧写指令发送失败", 0);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"第一条烧写指令发送失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"写IMEI") != -1)
			{
				CString NoCheckCodeIMEI;
				CString chipBandIMEI;
				CString haveCheckCodeIMEI;
				chipBandIMEI = dlg->getBandIMEIByChipID(chipID);
				if (chipBandIMEI != L"") //已有绑定IMEI
				{
					if (chipBandIMEI.GetLength() == 15)
					{
						haveCheckCodeIMEI = chipBandIMEI;
						chipBandIMEI = chipBandIMEI.Left(chipBandIMEI.GetLength() - 1);
					}
					dlg->ShowBurningMsg(L"芯片ID：" + chipID + L"已与IMEI：" + haveCheckCodeIMEI + L"绑定", 1);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"芯片ID：" + chipID + L"已与IMEI：" + haveCheckCodeIMEI + L"绑定\r\n");
					goto FALILDONE;
					/*if (chipBandIMEI>=FImeiStart&&chipBandIMEI<=FImeiEnd)
					{
						CurrentBurningImei = haveCheckCodeIMEI;
						haveBandIMEI = TRUE;
						goto HAVEBANDIMEI;
					}
					else
					{
						dlg->GetDlgItemText(IDC_BurningImei_EDIT, CurrentBurningImei);
					}*/
				}
				else
				{
					dlg->GetDlgItemText(IDC_BurningImei_EDIT, CurrentBurningImei);
				}
				if (!dlg->m_NoCheckCodeCheckControl.GetCheck())
				{
					CurrentBurningImei = dlg->CreateIMEI15(CurrentBurningImei);
				}
				else
				{
					CurrentBurningImei = CurrentBurningImei;
				}
				BOOL checkRidFlag = TRUE;
				do
				{
					if (m_BurningConrtolFlag == false)
					{
						break;
					}
					if (!dlg->m_NoCheckCodeCheckControl.GetCheck())
					{
						NoCheckCodeIMEI = CurrentBurningImei.Left(14);
					}
					else
					{
						NoCheckCodeIMEI = CurrentBurningImei;
					}
					if (dlg->CheckIMEI(CurrentBurningImei) == 1) //检查此IMEI是否已被用过，==1代表用过，则IMEI+1后再进行判断
					{
						dlg->ShowBurningMsg(L"IMEI：" + CurrentBurningImei + L"重号，自动过滤", 1);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", CurrentBurningImei + L"重号\r\n");
						if (EnterScanFlag)    //扫描模式，如果重号则直接判断位失败
						{
							checkRidFlag = FALSE;
							goto FALILDONE;
						}
						unsigned long long Ridimeiint = 0;
						Ridimeiint = _atoi64(CStringA(NoCheckCodeIMEI)) + IncreasingnumberInt;
						NoCheckCodeIMEI = _ui64toa(Ridimeiint, CT2A(NoCheckCodeIMEI), 10);
						if (NoCheckCodeIMEI.GetLength() != FImeiStart.GetLength() || NoCheckCodeIMEI<FImeiStart || NoCheckCodeIMEI.GetLength() != FImeiEnd.GetLength() || NoCheckCodeIMEI>FImeiEnd)
						{
							dlg->ShowBurningMsg(L"IMEI:" + CurrentBurningImei + L"在当前号段外，请检查此订单号段是否已烧写完成", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI：" + CurrentBurningImei + L"在当前号段外\r\n");
							goto FALILDONE;
						}
						if (!dlg->m_NoCheckCodeCheckControl.GetCheck())
						{
							CurrentBurningImei = dlg->CreateIMEI15(NoCheckCodeIMEI);
						}
						else
						{
							CurrentBurningImei = NoCheckCodeIMEI;
						}
					}
					else
					{
						dlg->SetDlgItemText(IDC_BurningImei_EDIT, NoCheckCodeIMEI);
						dlg->UpdateWindow();
						checkRidFlag = FALSE;
					}
				} while (checkRidFlag);
			HAVEBANDIMEI:
				commandVal = commandVal + CurrentBurningImei + L"\r\n";
				for (int o = 0; o <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"收:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						unsigned long long imeiint;
						CString imeistr;
						dlg->ShowBurningMsg(L"IMEI:" + CurrentBurningImei + L"烧写成功", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI：" + CurrentBurningImei + L"烧写成功\r\n");
						dlg->updateToCoupleTest(CurrentBurningImei, chipID); //更新IMEI号到耦合表
						if (haveBandIMEI)
						{
							dlg->UpdateToMainSheet(CurrentBurningImei, L"1", chipID, Allversion, MachineName);
						}
						else
						{
							dlg->insertToMainSheet(CurrentBurningImei, L"1", chipID, Allversion, MachineName);
						}
						if (!haveBandIMEI && !EnterScanFlag)
						{
							if (!dlg->m_NoCheckCodeCheckControl.GetCheck())
							{
								justIMEI = CurrentBurningImei.Left(14);
							}
							else
							{
								justIMEI = CurrentBurningImei;
							}
							imeiint = _atoi64(CStringA(justIMEI)) + IncreasingnumberInt;
							imeistr = _ui64toa(imeiint, CT2A(justIMEI), 10);
							dlg->SetDlgItemText(IDC_ImeiCurrent_EDIT, imeistr);
							dlg->SetDlgItemText(IDC_BurningImei_EDIT, imeistr);
							dlg->UpdateWindow();
							dlg->UpdataBurningMes(zhidan, imeistr);
						}
						haveBandIMEI = FALSE;
						o = tryTimeInt + 1;
					}
					else
					{
						o++;
						if (o > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"IMEI:" + CurrentBurningImei + L"烧写失败", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI：" + CurrentBurningImei + L"烧写失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"读取保存") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int M = 0; M <= tryTimeInt; )
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					CString NumberStr, lastChar, DBziduan;
					int pos;
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burning_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"收:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					lastChar = commandName.Right(1);
					if (readPort.Find(commandReceive) != -1)
					{
						NumberStr = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						NumberStr = NumberStr.Left(NumberStr.Find(L"\r\n"));
						pos = NumberStr.Find('"');
						if (pos >= 0)
						{
							NumberStr = NumberStr.Mid(pos + 1);
							NumberStr = NumberStr.Left(NumberStr.Find('"'));
						}
						else
						{
						}

						pos = NumberStr.Find('<');
						if (pos >= 0)
						{
							NumberStr = NumberStr.Mid(pos + 1);
							NumberStr = NumberStr.Left(NumberStr.Find('>'));
						}
						else
						{
						}

						//判断前缀
						if (commandName.Find(L"(PRE)") != -1)
						{
							CString PreStr = commandName.Left(commandName.Find(L"(PRE)"));

							if (NumberStr.Find(PreStr) != 0)//如果查找出来不等于0，那就代表前缀肯定不对
							{
								dlg->ShowBurningMsg(L"字段前缀错误", 1);
								dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"字段前缀错误\r\n");
								goto FALILDONE;
							}

							commandName = commandName.Right(commandName.GetLength() - commandName.Find(L"(PRE)") - 5);//截取掉PRE部分
						}

						//判断长度
						if (commandName.Find(L"(LENGTH)") != -1)
						{
							CString LengthStr = commandName.Left(commandName.Find(L"(LENGTH)"));

							if (NumberStr.GetLength() != _ttoi(LengthStr))//长度不对就返回错误
							{
								dlg->ShowBurningMsg(L"字段长度错误", 1);
								dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"字段长度错误\r\n");
								goto FALILDONE;
							}


							commandName = commandName.Right(commandName.GetLength() - commandName.Find(L"(LENGTH)") - 8);//截取掉LENGTH部分
						}

						//判断是否在范围内
						if (commandName.Find(L"(SectionNumber)") != -1)
						{
							CString SectionNumberStr = commandName.Left(commandName.Find(L"(SectionNumber)"));

							CString SectionNumberHighStr = SectionNumberStr.Left(SectionNumberStr.Find(L"-"));

							CString SectionNumberLowStr = SectionNumberStr.Right(SectionNumberStr.GetLength() - SectionNumberStr.Find(L"-") - 1);

							if (NumberStr.GetLength() != SectionNumberHighStr.GetLength() || NumberStr < SectionNumberHighStr || NumberStr.GetLength() != SectionNumberLowStr.GetLength() || NumberStr >SectionNumberLowStr)//大于最大值或者小于最小值就返回错误
							{
								dlg->ShowBurningMsg(L"字段不在号段内", 1);
								dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"字段不在号段内\r\n");
								goto FALILDONE;
							}
						}
						//huhu
						if (lastChar == L"0")
						{
							DBziduan = L"SN";
						}
						else
						{
							DBziduan = L"IMEI" + lastChar;
						}
						if (dlg->checkIMEIInRelative(CurrentBurningImei))  //有IMEI更新
						{
							dlg->saveDataToDB(NumberStr, CurrentBurningImei, DBziduan);
						}
						else
						{
							dlg->insertNewDataToDB(NumberStr, CurrentBurningImei, DBziduan);   //没有则插入
						}
						dlg->ShowBurningMsg(commandName + L"成功", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"成功\r\n");
						M = tryTimeInt + 1;
					}
					else
					{
						M++;
						if (M > tryTimeInt)
						{
							dlg->ShowBurningMsg(commandName + L"失败", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"关联更新"))
			{
				CString firstChar, lastChar, NumberStr, ziduan, newSheetData;
				int pos;
				//firstChar = commandName.Left(1);
				lastChar = commandName.Right(1);
				commandVal = commandVal + L"\r\n";
				for (int N = 0; N <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"发:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"收:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						NumberStr = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						NumberStr = NumberStr.Left(NumberStr.Find(L"\r\n"));
						pos = NumberStr.Find('"');
						if (pos >= 0)
						{
							NumberStr = NumberStr.Mid(pos + 1);
							NumberStr = NumberStr.Left(NumberStr.Find('"'));
						}
						else
						{
						}
						pos = NumberStr.Find('<');
						if (pos >= 0)
						{
							NumberStr = NumberStr.Mid(pos + 1);
							NumberStr = NumberStr.Left(NumberStr.Find('>'));
						}
						else
						{
						}
						if (lastChar == L"0")
						{
							ziduan = L"SN";
						}
						else
						{
							ziduan = L"IMEI" + lastChar;
						}
						newSheetData = dlg->getNewSheetData(NumberStr, ziduan);//获取新关联表的关联数据
						if (dlg->checkIMEIInRelative(CurrentBurningImei))   //关联表有IMEI，则更新
						{
							dlg->saveNewDataToDB(newSheetData, CurrentBurningImei, ziduan);
						}
						else
						{
							dlg->insertNewDataToDB(newSheetData, CurrentBurningImei, ziduan);  //否则插入
						}
						dlg->ShowBurningMsg(commandName + L"成功", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"成功\r\n");
						N = tryTimeInt + 1;
					}
					else
					{
						N++;
						if (N > tryTimeInt)
						{
							dlg->ShowBurningMsg(commandName + L"失败", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"失败\r\n");
							goto FALILDONE;
						}
					}
				}
			}
		}
		dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("成功"));
		if (dlg->m_NoCheckCodeCheckControl.GetCheck())
		{
			BurnSendToLd = CurrentBurningImei;
		}
		else
		{
			BurnSendToLd = CurrentBurningImei.Left(14);
		}
		//dlg->SetDlgItemTextW(IDC_NEXTLDIMEI, BurnSendToLd);
		dlg->BurnVec.push_back(BurnSendToLd);
		dlg->UpdateWindow();
		j = 3;
		dlg->SendToPlc(1);
		dlg->SendToPlc(10);
		goto WAITBREAKCONNECT;
	FALILDONE:
		dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"烧写失败\r\n");
		dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("失败"));
		j = 3;
		dlg->SendToPlc(2);
		dlg->SendToPlc(20);
		goto WAITBREAKCONNECT;
	WAITBREAKCONNECT:
		dlg->SendToPlc(30);
		do
		{
			if (m_BurningConrtolFlag == false)
			{
				break;
			}
			WriteFile(dlg->Port1handler, CT2A(DbTestcommand), DbTestcommand.GetLength(), &dwBytesWrite, NULL);
			//dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + DbTestcommand + L"\r\n");
			Sleep(500);
			readPort = dlg->ReadFromBurningPort();
			//dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
			if (readPort == "")
			{
				WriteFile(dlg->Port1handler, CT2A(DbTestcommand), DbTestcommand.GetLength(), &dwBytesWrite, NULL);
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"发送：" + DbTestcommand + L"\r\n");
				Sleep(500);
				readPort = dlg->ReadFromBurningPort();
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"接收：" + readPort + L"\r\n");
				if (readPort == "")
				{
					if (EnterScanFlag)
					{
						dlg->SetDlgItemTextW(IDC_BurningImei_EDIT, L"");
					}
					dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("就绪"));
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"就绪\r\n");
					j = 3;
					DBLastSendTestFlag = false;
				}
			}
			//Sleep(500);
		} while (DBLastSendTestFlag);
		pcEnterTestFlag = true;
		DBLastSendTestFlag = true;
	CLOSETHREAD:
		int twodog = 2;
	}
	return 0;
}


/**************************************对比IMEI模块**************************************/
//点击串口下拉框时刷新注册表串口
void CMFC_LaserDlg::OnDropdownCompareportCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_ComparePort_COMBO), Port2No);
}

//显示对比信息
int CMFC_LaserDlg::ShowCompareMsg(CString strMsg, int MsgNum)
{
	CString str = GetTime() + _T("\r\n ");
	str += strMsg + _T("\r\n");
	m_CpLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (MsgNum)
	{
	case 0:
		m_CpLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		break;
	case 1:
		m_CpLog.SetSelectionCharFormat(cf);
		m_CpLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		break;
	default:
		break;
	}
	m_CpLog.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return 0;
}

//一键开始
void CMFC_LaserDlg::OnBnClickedCompareconnButton()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (IniTestCommand.IsEmpty())
	{
		MessageBox(L"请先获取测试指令", L"提示");
		return;
	}*/
	GetCommPort((CComboBox*)GetDlgItem(IDC_ComparePort_COMBO), Port2No);//先获取当前串口号
	CString CompareConnButtonText;
	GetDlgItem(IDC_CompareConn_BUTTON)->GetWindowText(CompareConnButtonText);
	if (CompareConnButtonText == "一键开始") {
		//判断串口有没有开启成功
		if (StationValVector.size() == 0)
		{
			MessageBox(_T("获取指令失败，请检查是否已上传指令至服务器"), _T("提示"), NULL);
			return;
		}
		Port2handler = InitCom(Port2No);
		if (Port2handler == NULL)
		{
			SetDlgItemText(IDC_CompareRemind_STATIC, L"打开串口失败");
			return;
		}
		ShowCompareMsg(L"对比串口初始化成功", 0);
		WriteComThread = AfxBeginThread(SendComparePortThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_CompareConrtolFlag = true;
		SetDlgItemText(IDC_CompareRemind_STATIC, L"就绪");
		WritetoTxt(L"Compare_", L"打开对比串口成功\r\n");
		SetDlgItemText(IDC_CompareConn_BUTTON, TEXT("一键关闭"));
		GetDlgItem(IDC_ComparePort_COMBO)->EnableWindow(false);
	}
	else
	{
		if (!CloseCom(Port2handler))
		{
			MessageBox(L"关闭对比串口失败", L"提示信息", NULL);
			return;
		}
		ShowCompareMsg(L"关闭对比串口成功", 0);
		WritetoTxt(L"Compare_", L"关闭对比串口成功\r\n");
		CompareLastSendTestFlag = false;
		m_CompareConrtolFlag = false; //停止线程
		Sleep(500);
		SetDlgItemText(IDC_CompareConn_BUTTON, TEXT("一键开始"));
		SetDlgItemText(IDC_CompareRemind_STATIC, L"未连接");
		GetDlgItem(IDC_ComparePort_COMBO)->EnableWindow(true);
	}
}

//对比串口线程
UINT SendComparePortThread(LPVOID lpParam)
{
	CMFC_LaserDlg* dlg;
	dlg = (CMFC_LaserDlg*)lpParam;
	PurgeComm(dlg->Port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//串口变量
	DWORD dwBytesWrite;
	BOOL bWriteStat;
	CString CIMEI1;
	CString CIMEI2;
	int ComparetryTimeInt;
	BOOL FuncCompareSendTestFlag = TRUE;
	BOOL FuncFuncCompareLastSendTestFlag = TRUE;
	BOOL checkBurnandLd = TRUE;
	CString strcommand; //放指令用变量
	CString StrRead; //放串口返回值用
	CString IMEIStr; //存放读出来的IMEI用
	CString midChangeCpIMEI;
	CString compareName, compareCommand, compareRev, compareTryTime, Teststrcommand;
	dlg->GetCommandAndRec(0, compareName, compareRev, compareTryTime, compareCommand);
	ComparetryTimeInt = _ttoi(compareTryTime);
	ComparetryTimeInt = ComparetryTimeInt * 3;
	while (m_CompareConrtolFlag)
	{
		do
		{
			if (m_CompareConrtolFlag == false)
			{
				break;
			}
			dlg->GetCommandAndRec(1, compareName, compareRev, compareTryTime, compareCommand);
			Teststrcommand.Format(L"%s\r\n", compareCommand);
			bWriteStat = WriteFile(dlg->Port2handler, CT2A(Teststrcommand), Teststrcommand.GetLength(), &dwBytesWrite, NULL);
			dlg->ShowCompareMsg(L"发:" + Teststrcommand, 0);
			//dlg->](1);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"发送：" + Teststrcommand + L"\r\n");
			Sleep(400);
			StrRead = dlg->ReadFromComparePort();
			if (StrRead.Find(compareRev) >= 0)
			{
				//dlg->BatOnAndOff(10);
				dlg->ShowCompareMsg(L"收:" + StrRead, 0);
				dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"接收：" + StrRead + L"\r\n");
				dlg->SendToPlc(6);
				dlg->SetDlgItemText(IDC_CompareRemind_STATIC, _T("对比中"));
				if (LDIsOpenFlag)
				{
					/*dlg->GetDlgItemText(IDC_NEXTCOMPAREIMEI, midChangeCpIMEI);
					CIMEI2 = midChangeCpIMEI;*/
					if (dlg->LdVec.size() == 0 || dlg->CompareIndex >= dlg->LdVec.size())
					{
						CString SizeStr, LdIndexStr;
						SizeStr.Format(L"%d", dlg->LdVec.size());
						LdIndexStr.Format(L"%d", dlg->CompareIndex);
						dlg->SetRicheditText(_T("对比索引超出[ size:") + SizeStr + _T(" , LdIndex:") + LdIndexStr + _T("]"), 1);
						dlg->SendToPlc(5);
						dlg->SendToPlc(50);
						goto WAITCONNECTBREAK;
					}
					CString SubIMEI2;
					SubIMEI2  =  dlg->LdVec[dlg->CompareIndex];
					if (SubIMEI2== CIMEI2)
					{
						if (dlg->CompareIndex<dlg->LdVec.size())
						{
							dlg->CompareIndex += 1;
							SubIMEI2 = dlg->LdVec[dlg->CompareIndex];
						}
						else
						{
							CString SizeStr, LdIndexStr;
							SizeStr.Format(L"%d", dlg->LdVec.size());
							LdIndexStr.Format(L"%d", dlg->CompareIndex);
							dlg->SetRicheditText(_T("对比索引超出[ size:") + SizeStr + _T(" , LdIndex:") + LdIndexStr + _T("]"), 1);
							goto WAITCONNECTBREAK;
						}
					}
					CIMEI2 = SubIMEI2;
					dlg->SetDlgItemText(IDC_CompareIMEI2_EDIT, CIMEI2);
					dlg->UpdateWindow();
				}
				CompareFinishFlag = FALSE;
				FuncCompareSendTestFlag = false;
			}
		} while (FuncCompareSendTestFlag);
		if (m_CompareConrtolFlag == false)
		{
			break;
		}
		dlg->GetCommandAndRec(2, compareName, compareRev, compareTryTime, compareCommand);
		compareCommand.Format(L"%s\r\n", compareCommand);
		for (int i = 0; i <= ComparetryTimeInt;)
		{
			bWriteStat = WriteFile(dlg->Port2handler, CT2A(compareCommand), compareCommand.GetLength(), &dwBytesWrite, NULL);
			dlg->ShowCompareMsg(L"发:" + compareCommand, 0);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"发送：" + compareCommand + L"\r\n");
			Sleep(400);
			StrRead = dlg->ReadFromComparePort();
			dlg->ShowCompareMsg(L"收:" + StrRead, 0);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"接收：" + StrRead + L"\r\n");
			compareRev.Replace(L" ", L"");
			StrRead.Replace(L" ", L"");
			if (StrRead.Find(compareRev) != -1)
			{
				IMEIStr = StrRead.Mid(StrRead.Find(compareRev) + compareRev.GetLength());
				IMEIStr = IMEIStr.Left(IMEIStr.Find(L"\r\n"));
				if (IMEIStr != "")
				{
					dlg->SetDlgItemText(IDC_CompareIMEI1_EDIT, IMEIStr);
					dlg->UpdateWindow();
				}
				dlg->GetDlgItemText(IDC_CompareIMEI1_EDIT, CIMEI1);
				if (CIMEI1 == CIMEI2)
				{
					dlg->ShowCompareMsg(L"机内IMEI：" + CIMEI1 + L" 与机身IMEI：" + CIMEI2 + L"对比通过", 0);
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"对比通过");
					dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"IMEI：" + CIMEI1 + L"对比通过\r\n");
					dlg->UpdateCompareMes(CIMEI2, L"1");
					goto SUCCEED;
				}
				else
				{
					dlg->ShowCompareMsg(L"机内IMEI：" + CIMEI1 + L" 与机身IMEI：" + CIMEI2 + L"对比通过", 1);
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"对比失败");
					dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"IMEI：" + CIMEI1 + L"对比失败\r\n");
					dlg->UpdateCompareMes(CIMEI2, L"0");
					dlg->SetDlgItemText(IDC_CompareIMEI1_EDIT, L"");
					goto FAIL;
				}
			}
			else
			{
				i++;
				if (i > ComparetryTimeInt)
				{
					dlg->ShowCompareMsg(L"指令回复失败", 1);
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"对比失败");
					dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"指令回复失败\r\n");
					goto FAIL;
				}
			}
		}
	SUCCEED:
		dlg->SendToPlc(4);
		dlg->SendToPlc(40);
		dlg->CompareIndex++;
		goto WAITCONNECTBREAK;
	FAIL:
		dlg->SendToPlc(5);
		dlg->SendToPlc(50);
		dlg->CompareIndex++;
		goto WAITCONNECTBREAK;
	WAITCONNECTBREAK:
		dlg->SendToPlc(60);
		//对比IMEI完成后检测该机子是否已断开
		FuncFuncCompareLastSendTestFlag = true;
		do
		{
			if (m_CompareConrtolFlag == false)
			{
				break;
			}
			dlg->GetCommandAndRec(1, compareName, compareRev, compareTryTime, compareCommand);
			Teststrcommand.Format(L"%s\r\n", compareCommand);
			WriteFile(dlg->Port2handler, CT2A(Teststrcommand), Teststrcommand.GetLength(), &dwBytesWrite, NULL);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"发送：" + Teststrcommand + L"\r\n");
			Sleep(500);
			StrRead = dlg->ReadFromComparePort();
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"接收：" + StrRead + L"\r\n");
			if (StrRead == "")
			{
				WriteFile(dlg->Port2handler, CT2A(Teststrcommand), Teststrcommand.GetLength(), &dwBytesWrite, NULL);
				dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"发送：" + Teststrcommand + L"\r\n");
				Sleep(500);
				StrRead = dlg->ReadFromComparePort();
				dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"接收：" + StrRead + L"\r\n");
				if (StrRead == "") {
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"就绪");
					dlg->SetDlgItemText(IDC_CompareIMEI2_EDIT, L"");
					FuncFuncCompareLastSendTestFlag = false;
				}
			}
			Sleep(500);
		} while (FuncFuncCompareLastSendTestFlag);
		FuncFuncCompareLastSendTestFlag = true;
		FuncCompareSendTestFlag = true;
	}
	return 0;
}

//读串口函数
CString CMFC_LaserDlg::ReadFromComparePort()
{

	//串口变量
	char str[200];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;
	CString strread;
	bReadStat = ReadFile(Port2handler, str, 199, &readreal, 0);
	if (bReadStat) {
		strread = str;
	}
	else
	{
		strread = "";
	}
	return strread;
}

//按键函数重写
BOOL CMFC_LaserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类


	//if (pMsg->hwnd == GetDlgItem(IDC_CompareConn_BUTTON)->GetSafeHwnd() && pMsg->message == WM_LBUTTONUP)
	//{
	//	OnBnClickedCompareconnButton();
	//	return CDialogEx::PreTranslateMessage(pMsg);
	//}

	//if (pMsg->hwnd == GetDlgItem(IDC_ALLAUTO_BUTTON)->GetSafeHwnd() && pMsg->message == WM_LBUTTONUP)
	//{
	//	OnBnClickedAllautoButton();
	//	return CDialogEx::PreTranslateMessage(pMsg);
	//}

	//CString tempStr;
	//GetDlgItem(IDC_CompareConn_BUTTON)->GetWindowTextW(tempStr);

	////if (tempStr == "一键关闭"){
	////	GetDlgItem(IDC_CompareIMEI2_EDIT)->SetFocus();
	////}
	//
	//
	//if (pMsg->message == WM_KEYDOWN){

	//	int nVirtKey = (int)pMsg->wParam;

	//	if (nVirtKey == VK_RETURN)
	//	{
	//		if (pMsg->hwnd == GetDlgItem(IDC_BurningImei_EDIT)->GetSafeHwnd())
	//		{
	//			ImeiBurning();
	//		}
	//		else if (pMsg->hwnd == GetDlgItem(IDC_CompareIMEI2_EDIT)->GetSafeHwnd())
	//		{
	//			ImeiComparing();
	//		}
	//	}
	//	pMsg->wParam = VK_TAB;
	//}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//更改字体大小颜色
HBRUSH CMFC_LaserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CString Fontstr1 = L"";
	CString Fontstr2 = L"";
	if (pWnd->GetDlgCtrlID() == IDC_BurningRemind_STATIC)
	{
		GetDlgItemText(IDC_BurningRemind_STATIC, Fontstr1);
		if (Fontstr1 == "未连接" || Fontstr1 == "就绪" || Fontstr1 == "烧写中")
		{
			m_brush.CreateSolidBrush(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
		else if (Fontstr1 == "IMEI重号" || Fontstr1 == "失败") {
			m_brush.CreateSolidBrush(RGB(255, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
		else if (Fontstr1 == "成功")
		{
			m_brush.CreateSolidBrush(RGB(0, 128, 0));
			pDC->SetBkColor(RGB(0, 128, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_CompareRemind_STATIC)
	{
		GetDlgItemText(IDC_CompareRemind_STATIC, Fontstr2);
		if (Fontstr2 == "未连接" || Fontstr2 == "就绪" || Fontstr2 == "对比中")
		{
			m_brush.CreateSolidBrush(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
		else if (Fontstr2 == "IMEI已对比过" || Fontstr2 == "对比失败") {
			m_brush.CreateSolidBrush(RGB(255, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
		else if (Fontstr2 == "对比通过")
		{
			m_brush.CreateSolidBrush(RGB(0, 128, 0));
			pDC->SetBkColor(RGB(0, 128, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//初始化字体
void CMFC_LaserDlg::fontinit()
{
	staticHint1font.CreatePointFont(500, L"黑体");
	editfont.CreatePointFont(300, L"黑体");

	GetDlgItem(IDC_BurningRemind_STATIC)->SetFont(&editfont);
	GetDlgItem(IDC_CompareRemind_STATIC)->SetFont(&editfont);
}


/***************************************本地日志记录***********************************/
/*获取当前exe路径*/
CString CMFC_LaserDlg::GetExePath()
{
	char sFileName[256] = { 0 };
	CString ProjectPath = _T("");
	GetModuleFileName(AfxGetInstanceHandle(), LPWSTR(sFileName), 255);
	ProjectPath.Format(L"%s", sFileName);
	int pos = ProjectPath.ReverseFind('\\');
	if (pos != -1)
		ProjectPath = ProjectPath.Left(pos);
	else
		ProjectPath = _T("");
	return ProjectPath;
}

/*写数据到txt文件*/
BOOL CMFC_LaserDlg::WritetoTxt(CString module, CString sValue)
{
	try {
		sValue = GetTime() + L" " + sValue;
		CString SubPath;
		SubPath = module + GetDate();
		SubPath = SubPath;
		CString sFile = GetExePath() + L"\\AutomateLaserLog\\" + SubPath + L".txt";
		CFile file;
		if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{

			file.SeekToEnd(); // 移动文件指针到末尾
			BYTE byUnicode[] = { 0xFF, 0xFE };
			file.Write(byUnicode, sizeof(byUnicode));
			file.Write(sValue, wcslen(sValue) * sizeof(WCHAR));
			file.Close();
		}
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
	return FALSE;
}

/*获取日期*/
CString  CMFC_LaserDlg::GetDate()
{
	CTime time = CTime::GetCurrentTime();
	CString DateStr, DayStr, MonthStr, YearStr;
	int day, year, month;
	day = time.GetDay();
	month = time.GetMonth();
	year = time.GetYear();
	DayStr.Format(_T("%d"), day);
	MonthStr.Format(_T("%d"), month);
	YearStr.Format(_T("%d"), year);
	DateStr = YearStr + "-" + MonthStr + "-" + DayStr;
	return DateStr;
}


/*************************************其它***************************************/
//一键全启动
void CMFC_LaserDlg::OnBnClickedAllautoButton()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (IniTestCommand.IsEmpty())
	{
		MessageBox(L"请先获取测试指令", L"提示");
		return;
	}*/
	CString autobuttontext;
	GetDlgItemText(IDC_ALLAUTO_BUTTON, autobuttontext);
	if (autobuttontext == "一键全启动")
	{
		CString burnbuttontext, laserbuttontext, comparebuttontext, JDQButtinText;
		GetDlgItemText(IDC_BurningConn_BUTTON, burnbuttontext);
		GetDlgItemText(IDC_OpenLdSystem_BUTTON, laserbuttontext);
		GetDlgItemText(IDC_CompareConn_BUTTON, comparebuttontext);
		GetDlgItemText(IDC_OPENRELAY, JDQButtinText);
		if (burnbuttontext == "一键关闭" || laserbuttontext == "关闭镭雕系统" || comparebuttontext == "一键关闭" || JDQButtinText == L"关闭继电器串口")
		{
			MessageBox(_T("启动失败！请先关闭已启动的模块"), _T("提示"), NULL);
			return;
		}
		CString porttext1, porttext2, porttext3;
		GetDlgItemText(IDC_BurningPort_COMBO, porttext1);
		GetDlgItemText(IDC_ComparePort_COMBO, porttext2);
		GetDlgItemText(IDC_JDQ_COMBO, porttext3);
		if (porttext1 == "" || porttext2 == "" || porttext3 == "" || porttext1 == porttext2 || porttext1 == porttext3 || porttext2 == porttext3)
		{
			MessageBox(_T("启动失败！请检查烧写、对比、继电器串口"), _T("提示"), NULL);
			return;
		}
		//判断串口能不能开启成功
		if (StationValVector.size() == 0)
		{
			MessageBox(_T("获取指令失败，请检查是否已上传指令至服务器"), _T("提示"), NULL);
			return;
		}
		Port1handler = InitCom(porttext1);
		if (Port1handler == NULL)
		{
			MessageBox(_T("打开烧写串口失败"), _T("提示"), NULL);
			return;
		}
		Port2handler = InitCom(porttext2);
		if (Port2handler == NULL)
		{
			MessageBox(_T("打开对比串口失败"), _T("提示"), NULL);
			return;
		}
		Port3handler = InitCom1(porttext3);
		if (Port3handler == NULL)
		{
			MessageBox(_T("打开继电器串口失败"), _T("提示"), NULL);
			return;
		}
		CloseCom(Port1handler);
		CloseCom(Port2handler);
		CloseCom(Port3handler);
		GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_OpenLdSystem_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_CompareConn_BUTTON)->EnableWindow(false);
		GetDlgItem(IDC_SendCommand1_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_SendCommand2_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_ReciveCommand_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_OPENRELAY)->EnableWindow(false);
		OnBnClickedOpenldsystemButton();
		Sleep(1000);
		OnBnClickedBurningconnButton();
		Sleep(500);
		OnBnClickedStartlaserButton();
		Sleep(500);
		OnBnClickedCompareconnButton();
		Sleep(500);
		OnBnClickedOpenrelay();
		SetDlgItemText(IDC_ALLAUTO_BUTTON, L"一键全关闭");
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_OpenLdSystem_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_CompareConn_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_SendCommand1_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_SendCommand2_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_ReciveCommand_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_OPENRELAY)->EnableWindow(TRUE);
		OnBnClickedStartlaserButton();
		OnBnClickedOpenldsystemButton();
		OnBnClickedBurningconnButton();
		OnBnClickedCompareconnButton();
		OnBnClickedOpenrelay();
		SetDlgItemText(IDC_ALLAUTO_BUTTON, L"一键全启动");
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(true);
	}
}

//单机版按钮
void CMFC_LaserDlg::OnBnClickedNonetCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	GetCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);//先获取当前串口号
	//判断串口有没有开启成功
	Port1handler = InitCom(Port1No);
	if (Port1handler == NULL)
	{
		ShowBurningMsg(L"打开串口失败", 1);
		return;
	}
	ShowBurningMsg(L"串口初始化成功!", 0);
	if (((CButton *)GetDlgItem(IDC_NoNet_CHECK))->GetCheck()) {
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(true);
		m_BurningImeiEdit.SetFocus();
	}
	else {
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(false);
	}
}

//转16进制
BYTE * CMFC_LaserDlg::CStrToByte(int len, CString str)
{
	BYTE *Buf = new BYTE[len];
	CString StrSub;
	for (int iSrc = 0, iDst = 0; iDst < len; iSrc += 2, iDst++)
	{
		StrSub = str.Mid(iSrc, 2);
		Buf[iDst] = (BYTE)_tcstoul(StrSub, NULL, 16);//十六进制
	}
	return Buf;
}

//16进制字符串转2进制字符串
CString CMFC_LaserDlg::HexStr2BinStr(CString HexStr)
{
	int StrLength = HexStr.GetLength();
	CString BinStr;
	for (int i = 0; i < StrLength; i++)
	{
		if (HexStr[i] >= 'A'&&HexStr[i] <= 'F')
		{
			int a = static_cast<int>(HexStr[i] - 'A' + 10);
			switch (a)
			{
			case 10:BinStr += "1010"; break;
			case 11:BinStr += "1011"; break;
			case 12:BinStr += "1100"; break;
			case 13:BinStr += "1101"; break;
			case 14:BinStr += "1110"; break;
			case 15:BinStr += "1111"; break;
			}
		}
		else if (isdigit(HexStr[i]))
		{
			int b = static_cast<int>(HexStr[i] - '0');
			switch (b)
			{
			case 0:BinStr += "0000"; break;
			case 1:BinStr += "0001"; break;
			case 2:BinStr += "0010"; break;
			case 3:BinStr += "0011"; break;
			case 4:BinStr += "0100"; break;
			case 5:BinStr += "0101"; break;
			case 6:BinStr += "0110"; break;
			case 7:BinStr += "0111"; break;
			case 8:BinStr += "1000"; break;
			case 9:BinStr += "1001"; break;
			}
		}

	}
	return BinStr;
}

/*窗口最大化*/
void CMFC_LaserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		myDlgManage.ResizeWindow();
	}
}

/*获取测试指令*/
void CMFC_LaserDlg::OnBnClickedGetcommand()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetPrivateProfileString(_T("TestCommandMode"), _T("TestCommand"), _T(""), IniTestCommand.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("TestRec"), _T(""), IniTestRec.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("CheckIMEICommand "), _T(""), IniCheckIMEICommand.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("CheckIMEICommandRec"), _T(""), IniCheckIMEICommandRec.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("BurningCommand1"), _T(""), IniBurningCommand1.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("BurningCommand1Rec"), _T(""), IniBurningCommand1Rec.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("BurningCommand2"), _T(""), IniBurningCommand2.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("BurningCommand2Rec"), _T(""), IniBurningCommand2Rec.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("IMEIDataDelimiter1"), _T(""), IMEIIniDataDelimiter1.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("IMEIDataDelimiter2"), _T(""), IMEIIniDataDelimiter2.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("IMEIDataDelimiter1Length"), _T(""), IMEIIniDataDelimiter1Length.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("RIDDataDelimiter1 "), _T(""), RIDIniDataDelimiter1.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("RIDDataDelimiter2"), _T(""), RIDIniDataDelimiter2.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("RIDDataDelimiter1Length"), _T(""), RIDIniDataDelimiter1Length.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("CheckRIDCommand"), _T(""), iniCheckRIDCommand.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//GetPrivateProfileString(_T("TestCommandMode"), _T("CheckRIDCommandRec"), _T(""), iniCheckRIDCommandRec.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	////GetPrivateProfileString(_T("TestCommandMode"), _T("DataDelimiter2Length"), _T(""), IniDataDelimiter2Length.GetBuffer(50), 50, _T(".\\TestCommand.ini"));
	//IMEIIniDataDelimiter1.ReleaseBuffer();
	//IMEIIniDataDelimiter1.Replace(L"\\r", L"\r");
	//IMEIIniDataDelimiter1.Replace(L"\\n", L"\n");
	//IMEIIniDataDelimiter2.ReleaseBuffer();
	//IMEIIniDataDelimiter2.Replace(L"\\r", L"\r");
	//IMEIIniDataDelimiter2.Replace(L"\\n", L"\n");
	//RIDIniDataDelimiter1.ReleaseBuffer();
	//RIDIniDataDelimiter1.Replace(L"\\r", L"\r");
	//RIDIniDataDelimiter1.Replace(L"\\n", L"\n");
	//RIDIniDataDelimiter2.ReleaseBuffer();
	//RIDIniDataDelimiter2.Replace(L"\\r", L"\r");
	//RIDIniDataDelimiter2.Replace(L"\\n", L"\n");
	//send(clientSock, "123456789", 256, 0);
	GetStation(zhidan);
	MachineName = getMachineName(ZhiDanNO);
	MessageBox(L"获取成功", L"提示");
}

//以@@分割指令数据
void CMFC_LaserDlg::GetCommandAndRec(int num, CString &str1, CString &str2, CString &str3, CString &str4)
{
	CString vectorCommand;
	vectorCommand = StationValVector.at(num);
	int ValLength;
	ValLength = vectorCommand.GetLength();
	str1 = vectorCommand.Left(vectorCommand.Find(L"@@"));
	vectorCommand = vectorCommand.Mid(vectorCommand.Find(L"@@") + 2, ValLength);
	ValLength = vectorCommand.GetLength();
	str2 = vectorCommand.Left(vectorCommand.Find(L"@@"));
	vectorCommand = vectorCommand.Mid(vectorCommand.Find(L"@@") + 2, ValLength);
	ValLength = vectorCommand.GetLength();
	str3 = vectorCommand.Left(vectorCommand.Find(L"@@"));
	vectorCommand = vectorCommand.Mid(vectorCommand.Find(L"@@") + 2, ValLength);
	str4 = vectorCommand;
}

//获取Stationz字段指令
void CMFC_LaserDlg::GetStation(CString softWare)
{
	ADOManage ado;
	ado.ConnSQL();
	CString StationVal, SubStationVal;
	int ValLength;
	StationVal = ado.adoGetStation(softWare);
	while (true)
	{
		if (StationVal.Find(L"}}") != -1)
		{
			ValLength = StationVal.GetLength();
			StationVal = StationVal.Mid(StationVal.Find(L"}}") + 1, ValLength);
			if (StationVal.Find(L"}}") != -1)
			{
				SubStationVal = StationVal.Left(StationVal.Find(L"}}"));
				StationVal = StationVal.Mid(StationVal.Find(L"}}"), StationVal.GetLength());
				StationValVector.push_back(SubStationVal);
			}
		}
		else
		{
			break;
		}
	}
	ado.CloseAll();
}

//测试
void CMFC_LaserDlg::OnBnClickedButton2()
{
	/*int i = GetDlgItemInt(IDC_EDIT1);
	LdCylinderPress(i);
	Sleep(500);
	CString str;
	str = ReadFromPLCPort();
	str = HexStr2BinStr(str);
	str = str.Mid(21,1);
	SetDlgItemText(myport,str);*/
	//CString strcommand;
	//BYTE * strcommand1;
	//BOOL bWriteStat;
	//DWORD dwBytesWrite;
	//GetDlgItemText(IDC_EDIT1, strcommand);
	//strcommand1 = CStrToByte(16, strcommand);
	//bWriteStat = WriteFile(Port5handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
	/*LdCylinderPress(1);
	Sleep(400);
	LdCylinderPress(10);*/
	int i = 0;
	i = GetDlgItemInt(IDC_EDIT1);
}

//与PLC的数据交互
void CMFC_LaserDlg::SendToPlc(int MsgNum)
{
	EnterCriticalSection(&cs);
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	CString ErrorNumStr;
	CString strcommand; //放指令用变量
	BYTE * strcommand1;
	switch (MsgNum)
	{
		//烧写
	case 1://ok
		strcommand = "023735393038033130";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"发送指令" + strcommand, 0);
		break;
	case 2:
		strcommand = "023735413038033138";//no
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"发送指令" + strcommand, 0);
		break;
	case 3:
		strcommand = "023735423038033139";//testing
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"发送指令" + strcommand, 0);
		break;
	case 10:
		strcommand = "023835393038033131";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"发送指令" + strcommand, 0);
		break;
	case 20:
		strcommand = "023835413038033139";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"发送指令" + strcommand, 0);
		break;
	case 30:
		strcommand = "023835423038033141";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"发送指令" + strcommand, 0);
		break;

		//对比
	case 4://ok
		strcommand = "023736313038033039";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"发送指令" + strcommand, 0);
		break;
	case 5://no
		strcommand = "023736323038033041";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"发送指令" + strcommand, 0);
		break;
	case 6:
		strcommand = "023736333038033042";//testing
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"发送指令" + strcommand, 0);
		break;
	case 40:
		strcommand = "023836313038033041";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"发送指令" + strcommand, 0);
		break;
	case 50:
		strcommand = "023836323038033042";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"发送指令" + strcommand, 0);
		break;
	case 60:
		strcommand = "023836333038033043";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"发送指令" + strcommand, 0);
		break;

		//镭雕
	case 7:
		strcommand = "023735443038033142";//ok
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		break;
	case 8:
		strcommand = "023735453038033143";//no
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		break;
	case 9:
		strcommand = "023735463038033144";//testing
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		SetRicheditText(L"发送指令" + strcommand, 0);
		break;
	case 70:
		strcommand = "023835443038033143";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		break;
	case 80:
		strcommand = "023835453038033144";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		break;
	case 90:
		strcommand = "023835463038033145";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		SetRicheditText(L"发送指令" + strcommand, 0);
		break;
	case 11://读PLC	
		strcommand = "0230303130423031033637";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		//SetRicheditText(L"发送指令" + strcommand, 0);
		break;
	}
	if (bWriteStat)
	{
		SetRicheditText(L"发送指令成功" + strcommand, 0);
	}
	else
	{
		SetRicheditText(L"发送指令失败" + strcommand, 1);
		DWORD ErrorNum = GetLastError();
		ErrorNumStr.Format(L"%d", ErrorNum);
		SetRicheditText(L"错误码：" + ErrorNumStr, 1);
	}
	Sleep(400);
	LeaveCriticalSection(&cs);
}

