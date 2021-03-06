// CDZ_Laser.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_Laser.h"
#include "CDZ_Laser.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include <vector>
#include "MFC_LaserDlg.h"
#include "CSpreadSheet.h"
#include "Laser.h"
#include <vector>
#include "CUnlockDlg.h"
#include "ResizeCtrl.h"
#include "CLogInDlg.h"
#include <algorithm>
//#include "HansAdvDllDefine.h"
// CDZ_Laser 对话框
using  namespace std;
//全局
CString DZMachineName;
CMFC_LaserDlg autoLaserDlg;
CString DZzhidan;
vector<CString> RelativeDataVec;
BOOL CheckIMEIThreadFlag = TRUE;
CWindowSizeMange DzResizeCtrl;
CString IMEIStaStart, IMEIStaEnd;
BOOL CheckNoUseDataFlag = FALSE;
BOOL LdSysIsOpen = FALSE;
CString strFilePath;
CLaser LaserMachine;
BOOL waitStopFlag;
BOOL getTxtData = FALSE;
vector<CString>PcNumVec;
vector<CString>StartIMEIVec;
vector<CString>EndIMEIVec;
vector<CString>CurIMEIVec;
vector<CString>Ziduan;
vector<CString>ZiduanValue;
vector<CString>IMEI1_IMEI14;
BOOL IsListClick = FALSE;
HANDLE CheckIMEIThreadHandel = NULL;
int DataShowNo = 0;
CString DzSN1, DzSN2, DzSN3;//SN号段范围的变量
CString DzStrSn;
IMPLEMENT_DYNAMIC(CDZ_Laser, CDialogEx)

CDZ_Laser::CDZ_Laser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DZLASER, pParent)
	, m_TagsCount(_T(""))
	, m_NoUseDataCount(_T(""))
	, DzLaserFile(_T(""))
	, m_MarkType(FALSE)
	, FastCheck(FALSE)
	, EmptyDataCheck(FALSE)
	, DataRelativeChoose(FALSE)
	, RemarkCheck(FALSE)
	, DBIFilterCheck(FALSE)
	, m_ZiDuanVal(_T(""))
	, m_delayTime(_T(""))
	, m_ScanData(_T(""))
	, m_BitsCheck(_T(""))
	, m_ScanDataPre(_T(""))
	, CheckIMEIData(FALSE)
	, m_StartIMEI(_T(""))
	, m_EndIMEI(_T(""))
	, m_CheckCodeVal(FALSE)
	, m_IncreasingNumVal(_T(""))
	, m_PcNumVal(_T(""))
	, m_MultLDVal(FALSE)
	, m_CheckPrintVal(FALSE)
{

}

CDZ_Laser::~CDZ_Laser()
{
}

void CDZ_Laser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_DZZhiDanComBo);
	DDX_Control(pDX, IDC_CHECK1, m_DZHaveCheckCode);
	DDX_Control(pDX, IDC_COMBO2, m_IncreasingNum);
	DDX_Control(pDX, IDC_CHECK7, m_DZHaveCheckCode);
	DDX_Control(pDX, IDC_COMBO3, m_DZRelativeZiduan);
	DDX_Control(pDX, IDC_CHECK1, m_DZHaveCheckCode);
	DDX_Control(pDX, IDC_CHECK7, m_DZHaveRelativeZiDuan);
	DDX_Control(pDX, IDC_EDIT3, m_IMEISTAR);
	DDX_Control(pDX, IDC_EDIT4, m_IMEIEND);
	DDX_Control(pDX, IDC_LIST2, m_DataShow);
	DDX_Text(pDX, IDC_EDIT6, m_TagsCount);
	DDX_Control(pDX, IDC_LIST1, m_NoUseData);
	DDX_Text(pDX, IDC_EDIT11, m_NoUseDataCount);
	DDX_Control(pDX, IDC_RICHEDIT21, m_DzLaserLog);
	DDX_Text(pDX, IDC_EDIT1, DzLaserFile);
	DDX_Radio(pDX, IDC_RADIO1, m_MarkType);
	DDX_Control(pDX, IDC_LIST3, m_list2);
	DDX_Check(pDX, IDC_CHECK5, FastCheck);
	DDX_Check(pDX, IDC_CHECK4, EmptyDataCheck);
	DDX_Control(pDX, IDC_EDIT5, m_MarkData);
	DDX_Check(pDX, IDC_CHECK7, DataRelativeChoose);
	DDX_Check(pDX, IDC_CHECK3, RemarkCheck);
	DDX_Check(pDX, IDC_CHECK2, DBIFilterCheck);
	DDX_CBString(pDX, IDC_COMBO3, m_ZiDuanVal);
	DDX_Text(pDX, IDC_EDIT10, m_delayTime);
	DDX_Text(pDX, IDC_EDIT9, m_ScanData);
	DDX_Text(pDX, IDC_EDIT7, m_BitsCheck);
	DDX_Text(pDX, IDC_EDIT8, m_ScanDataPre);
	DDX_Check(pDX, IDC_CHECK6, CheckIMEIData);
	DDX_Text(pDX, IDC_EDIT3, m_StartIMEI);
	DDX_Text(pDX, IDC_EDIT4, m_EndIMEI);
	DDX_Control(pDX, IDC_EDIT9, m_ScanCtrl);
	DDX_Check(pDX, IDC_CHECK1, m_CheckCodeVal);
	DDX_CBString(pDX, IDC_COMBO2, m_IncreasingNumVal);
	DDX_Control(pDX, IDC_CHECK5, m_FastCheclCtrl);
	DDX_Control(pDX, IDC_CHECK6, m_IsIMEICtrl);
	DDX_Control(pDX, IDC_LIST4, m_HaoDuan);
	DDX_Control(pDX, IDC_CHECK8, m_MultLD);
	DDX_Control(pDX, IDC_COMBO4, m_PcNum);
	DDX_CBString(pDX, IDC_COMBO4, m_PcNumVal);
	DDX_Check(pDX, IDC_CHECK8, m_MultLDVal);
	DDX_Control(pDX, IDC_CHECK2, DBIFliterCheckCtrl);
	DDX_Control(pDX, IDC_CHECK9, m_CheckPrint);
	DDX_Check(pDX, IDC_CHECK9, m_CheckPrintVal);
}


BEGIN_MESSAGE_MAP(CDZ_Laser, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDZ_Laser::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDZ_Laser::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CDZ_Laser::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDZ_Laser::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDZ_Laser::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON4, &CDZ_Laser::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON9, &CDZ_Laser::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON3, &CDZ_Laser::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON8, &CDZ_Laser::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON10, &CDZ_Laser::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON1, &CDZ_Laser::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON11, &CDZ_Laser::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CDZ_Laser::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CDZ_Laser::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CDZ_Laser::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CDZ_Laser::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CDZ_Laser::OnBnClickedButton16)
	ON_EN_CHANGE(IDC_EDIT9, &CDZ_Laser::OnEnChangeEdit9)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON17, &CDZ_Laser::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_CHECK8, &CDZ_Laser::OnBnClickedCheck8)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST4, &CDZ_Laser::OnDblclkList4)
	ON_BN_CLICKED(IDC_RADIO2, &CDZ_Laser::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CDZ_Laser::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CDZ_Laser::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON18, &CDZ_Laser::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CDZ_Laser::OnBnClickedButton19)
END_MESSAGE_MAP()


// CDZ_Laser 消息处理程序

BOOL CDZ_Laser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_FastCheclCtrl.SetCheck(1);

	DZFillZhidanToComBo();

	DzResizeCtrl.Init(m_hWnd);

	UnLockSomething(FALSE);

	GetLocalHostIPName(DzName, DzIp);
	SetDlgItemText(IDC_EDIT12, DzIp);

	CFont* ffont;
	ffont = new CFont;
	ffont->CreateFont(36, 0, 0, 0, 700, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"arial");
	m_IMEISTAR.SetFont(ffont);
	m_IMEIEND.SetFont(ffont);

	CFont* ffont1;
	ffont1 = new CFont;
	ffont1->CreateFont(30, 0, 0, 0, 700, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"arial");
	GetDlgItem(IDC_EDIT5)->SetFont(ffont1);
	GetDlgItem(IDC_EDIT9)->SetFont(ffont1);

	CFont* ffont2;
	ffont2 = new CFont;
	ffont2->CreateFont(26, 0, 0, 0, 700, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"arial");
	GetDlgItem(IDC_EDIT12)->SetFont(ffont2);

	m_IncreasingNum.AddString(L"1");
	m_IncreasingNum.AddString(L"2");
	m_IncreasingNum.AddString(L"3");
	m_IncreasingNum.AddString(L"4");
	m_IncreasingNum.AddString(L"5");

	m_DZRelativeZiduan.AddString(L"0");
	m_DZRelativeZiduan.AddString(L"1");
	m_DZRelativeZiduan.AddString(L"2");
	m_DZRelativeZiduan.AddString(L"3");
	m_DZRelativeZiduan.AddString(L"4");
	m_DZRelativeZiduan.AddString(L"5");
	m_DZRelativeZiduan.AddString(L"6");
	m_DZRelativeZiduan.AddString(L"7");
	m_DZRelativeZiduan.AddString(L"8");
	m_DZRelativeZiduan.AddString(L"9");
	m_DZRelativeZiduan.AddString(L"10");
	m_DZRelativeZiduan.AddString(L"11");
	m_DZRelativeZiduan.AddString(L"12");
	m_DZRelativeZiduan.AddString(L"13");
	m_DZRelativeZiduan.SetCurSel(1);

	m_DataShow.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS);
	m_DataShow.SetBkColor(RGB(255, 255, 255));
	m_DataShow.SetTextColor(RGB(3, 146, 207));
	m_DataShow.SetTextBkColor(RGB(255, 255, 255));
	m_DataShow.DeleteAllItems();
	m_DataShow.InsertColumn(0, _T("No"), LVCFMT_CENTER, 50, 0);
	m_DataShow.InsertColumn(1, _T("IMEI/SN"), LVCFMT_CENTER, 180, 0);
	m_DataShow.InsertColumn(2, _T("数据库检查"), LVCFMT_CENTER, 110, 0);
	m_DataShow.InsertColumn(3, _T("操作状态"), LVCFMT_CENTER, 80, 0);

	m_NoUseData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS);
	m_NoUseData.SetBkColor(RGB(255, 255, 255));
	m_NoUseData.SetTextColor(RGB(3, 146, 207));
	m_NoUseData.SetTextBkColor(RGB(255, 255, 255));
	m_NoUseData.DeleteAllItems();
	m_NoUseData.InsertColumn(0, _T("No"), LVCFMT_CENTER, 80, 0);
	m_NoUseData.InsertColumn(1, _T("IMEI/SN"), LVCFMT_CENTER, 300, 0);

	m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS);
	m_list2.SetBkColor(RGB(255, 255, 255));
	m_list2.SetTextColor(RGB(3, 146, 207));
	m_list2.SetTextBkColor(RGB(255, 255, 255));
	m_list2.DeleteAllItems();
	//清空表头
	while (m_list2.DeleteColumn(0));
	//将字段显示出来
	m_list2.InsertColumn(0, L"No", LVCFMT_CENTER, 30);
	m_list2.InsertColumn(1, L"设置项目", LVCFMT_LEFT, 65);
	m_list2.InsertColumn(2, L"设置变量名", LVCFMT_LEFT, 100);
	m_list2.InsertColumn(3, L"设置变量", LVCFMT_LEFT, 230);
	m_list2.SetReadOnlyColumns(0);//read only
	CString strValidChars = L"";
	m_list2.SetColumnValidEditCtrlCharacters(strValidChars, 2);//none control edit 
	m_list2.EnableVScroll();
	m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CString str;
	for (int i = 0; i < 48; i++)
	{
		str.Format(L"%02d", i + 1);
		m_list2.InsertItem(LVIF_TEXT | LVIF_STATE, i,
			str, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);
		if (i == 2)
		{
			m_list2.SetItemText(i, 1, L"IMEI号段");
			m_list2.SetItemText(i, 2, L"IMEI");
			m_list2.SetItemText(i, 3, L"根据号段生成！！");
		}
		else if (i == 0)
		{
			m_list2.SetItemText(i, 1, L"机型");
			m_list2.SetItemText(i, 2, L"Model");
		}
		else if (i == 1)
		{
			m_list2.SetItemText(i, 1, L"软件版本");
			m_list2.SetItemText(i, 2, L"Version");
		}
		else
		{
			m_list2.SetItemText(i, 1, L"");
			m_list2.SetItemText(i, 2, L"Other" + str);
		}
	}
	m_DZRelativeZiduan.SetCurSel(-1);
	m_IncreasingNum.SetCurSel(0);
	m_DZHaveCheckCode.SetCheck(1);
	m_DZHaveRelativeZiDuan.SetCheck(0);
	m_IsIMEICtrl.SetCheck(1);
	SetDlgItemInt(IDC_EDIT7, 15);
	//SetDlgItemText(IDC_EDIT8, L"000000");

	m_PcNum.AddString(L"一");
	m_PcNum.AddString(L"二");
	m_PcNum.AddString(L"三");
	m_PcNum.AddString(L"四");
	m_PcNum.AddString(L"五");
	m_PcNum.AddString(L"六");
	m_PcNum.AddString(L"七");
	m_PcNum.AddString(L"八");
	m_PcNum.AddString(L"九");
	m_PcNum.AddString(L"十");
	m_PcNum.AddString(L"十一");
	m_PcNum.AddString(L"十二");
	m_PcNum.AddString(L"十三");
	m_PcNum.AddString(L"十四");
	m_PcNum.AddString(L"十五");
	m_PcNum.AddString(L"十六");

	m_HaoDuan.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS);
	m_HaoDuan.SetBkColor(RGB(255, 255, 255));
	m_HaoDuan.SetTextColor(RGB(3, 146, 207));
	m_HaoDuan.SetTextBkColor(RGB(255, 255, 255));
	m_HaoDuan.DeleteAllItems();
	m_HaoDuan.InsertColumn(0, _T("电脑序号"), LVCFMT_CENTER, 90, 0);
	m_HaoDuan.InsertColumn(1, _T("起始号"), LVCFMT_CENTER, 150, 0);
	m_HaoDuan.InsertColumn(2, _T("终止号"), LVCFMT_CENTER, 150, 0);
	m_HaoDuan.InsertColumn(3, _T("当前镭雕号"), LVCFMT_CENTER, 150, 0);

	DBIFliterCheckCtrl.SetCheck(1);
	m_CheckPrint.SetCheck(1);

	CString str1;
	for (int i = 1; i <= 14; i++)
	{
		str1.Format(L"IMEI%d", i);
		IMEI1_IMEI14.push_back(str1);
	}
	//m_DZHaveCheckCode.SetCheck(0);
	//m_DZRelativeZiduan.SetCurSel(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE4
}

void CDZ_Laser::DZFillZhidanToComBo()
{
	m_DZZhiDanComBo.ResetContent();
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetZhiDan();
	while (!adomanage.m_pRecordSet->adoEOF)
	{
		m_DZZhiDanComBo.AddString(adomanage.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanage.m_pRecordSet->MoveNext();
	}
	adomanage.CloseAll();
}

CString CDZ_Laser::getMachineName(CString zhidan)
{
	ADOManage ado;
	ado.ConnSQL();
	CString subName;
	subName = ado.adoGetMachineName(zhidan);
	ado.CloseAll();
	return subName;
}

void CDZ_Laser::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	FirstData = 0;
	FirstScan = 1;
	try {
		// TODO:  在此添加控件通知处理程序代码
		SetDlgItemText(IDC_EDIT6, L"");
		CheckDBResult.clear(); //清上个订单的缓存
		RelativeDataVec.clear();
		m_DataShow.DeleteAllItems();

		//CheckIMEIThreadFlag = FALSE;
		//CheckIMEIThreadHandel = NULL;//结束查重线程

		_variant_t imeitemp;//用来放getcollect变量的
		int nSel;
		CString DZZhidanNo, SubValStr;
		nSel = m_DZZhiDanComBo.GetCurSel();
		m_DZZhiDanComBo.GetLBText(nSel, DZZhidanNo);
		DZzhidan = DZZhidanNo;
		SetDlgItemText(IDC_COMBO1, DZZhidanNo);
		SetDlgItemText(IDC_EDIT5, L"");
		ADOManage adomanage;
		adomanage.ConnSQL();
		adomanage.GetIMEIByZhiDan(DZZhidanNo);
		IMEIStaStart = adomanage.m_pRecordSet->GetCollect("IMEIStart").bstrVal;
		IMEIStaEnd = adomanage.m_pRecordSet->GetCollect("IMEIEnd").bstrVal;
		DzSN1 = adomanage.m_pRecordSet->GetCollect("SN1").bstrVal;
		DzSN2 = adomanage.m_pRecordSet->GetCollect("SN2").bstrVal;
		DzSN3 = adomanage.m_pRecordSet->GetCollect("SN3").bstrVal;
		DZMachineName = getMachineName(DZZhidanNo);
		if (DZzhidan.Find(L"-") != -1)
		{
			DZzhidan = DZzhidan.Left(DZzhidan.Find(L"-"));
		}
		SubValStr = adomanage.GetSubSegment(DZzhidan);
		if (SubValStr == L"")
		{
			m_MultLD.SetCheck(0);
			m_PcNum.SetWindowTextW(L"");
			m_HaoDuan.DeleteAllItems();
			SetDlgItemText(IDC_EDIT3, IMEIStaStart);
			SetDlgItemText(IDC_EDIT4, IMEIStaEnd);
			GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
		}
		else
		{
			SetDlgItemText(IDC_EDIT3, L"");
			SetDlgItemText(IDC_EDIT4, L"");
			m_HaoDuan.DeleteAllItems();
			PcNumVec.clear();
			StartIMEIVec.clear();
			EndIMEIVec.clear();
			CurIMEIVec.clear();
			CutSegment(SubValStr);
			m_MultLD.SetCheck(1);
			//AfxMessageBox(SubValStr);
			GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
			MessageBox(L"请从子号段列表中选择号段", L"提示");
			//ShowDZLaserMsg(L"请从子号段列表中选择号段",1);
		}
		UpdateWindow();
		adomanage.CloseAll();
		ADOManage adomanage1;
		adomanage1.ConnSQL();
		if (adomanage1.GetIMEICurrentAndTemplate(DZZhidanNo) == 1)  //获取返工单的模板
		{
			SetDlgItemText(IDC_EDIT1, adomanage1.m_pRecordSet1->GetCollect("LdTemplate").bstrVal);
		}
		else
		{
			SetDlgItemText(IDC_EDIT1, L"");
		}
		if (adomanage1.GetIMEICurrentAndTemplate(DZzhidan) == 1) //获取主订单的前缀，位数等
		{
			if (adomanage1.m_pRecordSet1->GetCollect("IncreasingNum").vt != VT_NULL)
				m_IncreasingNum.SetCurSel(adomanage1.m_pRecordSet1->GetCollect("IncreasingNum").intVal - 1);
			else
				m_IncreasingNum.SetCurSel(0);
			if (adomanage1.m_pRecordSet1->GetCollect("DZhaHavecheckCode").vt != VT_NULL)
				m_DZHaveCheckCode.SetCheck(adomanage1.m_pRecordSet1->GetCollect("DZhaHavecheckCode").intVal);
			else
				m_DZHaveCheckCode.SetCheck(1);
			if (adomanage1.m_pRecordSet1->GetCollect("HaveRelativeZiduan").vt != VT_NULL)
				m_DZHaveRelativeZiDuan.SetCheck(adomanage1.m_pRecordSet1->GetCollect("HaveRelativeZiduan").intVal);
			else
				m_DZHaveRelativeZiDuan.SetCheck(0);
			//m_DZRelativeZiduan.SetCurSel(adomanage1.m_pRecordSet1->GetCollect("RelativeZiduan").intVal);
			if (adomanage1.m_pRecordSet1->GetCollect("Bits").vt != VT_NULL)
				SetDlgItemInt(IDC_EDIT7, adomanage1.m_pRecordSet1->GetCollect("Bits").intVal);
			else
				SetDlgItemInt(IDC_EDIT7, 15);
			if (adomanage1.m_pRecordSet1->GetCollect("IMEIPre").vt != VT_NULL)
				SetDlgItemText(IDC_EDIT8, (LPCTSTR)adomanage1.m_pRecordSet1->GetCollect("IMEIPre").bstrVal);
			else
				SetDlgItemText(IDC_EDIT8, IMEIStaStart.Left(6));
			if (adomanage1.m_pRecordSet1->GetCollect("ldSn").vt != VT_NULL)
				DzStrSn = adomanage1.m_pRecordSet1->GetCollect("ldSn").bstrVal;
			else
				DzStrSn = DzSN1 + DzSN2;
		}
		else
		{
			m_DZRelativeZiduan.SetCurSel(-1);
			m_IncreasingNum.SetCurSel(0);
			m_DZHaveCheckCode.SetCheck(1);
			m_DZHaveRelativeZiDuan.SetCheck(0);
			SetDlgItemInt(IDC_EDIT7, 15);
			SetDlgItemText(IDC_EDIT8, IMEIStaStart.Left(6));
		}
		adomanage1.CloseAll1();
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}

void CDZ_Laser::CutSegment(CString val)
{
	CString ResidueStr, SubStr;
	while (val.Find(L"-") != -1)
	{
		SubStr = val.Mid(1, 1);
		PcNumVec.push_back(SubStr);
		val = val.Right(val.GetLength() - val.Find(L",") - 1);
		SubStr = val.Left(val.Find(L"-"));
		StartIMEIVec.push_back(SubStr);
		val = val.Right(val.GetLength() - val.Find(L"-") - 1);
		SubStr = val.Left(val.Find(L","));
		EndIMEIVec.push_back(SubStr);
		val = val.Right(val.GetLength() - val.Find(L",") - 1);
		SubStr = val.Left(val.Find(L")"));
		CurIMEIVec.push_back(SubStr);
		val = val.Right(val.GetLength() - val.Find(L",") - 1);
	}
	for (int i = 0; i < PcNumVec.size(); i++)
	{
		m_HaoDuan.InsertItem(i, L"1", 0);//插入行
		m_HaoDuan.SetItemText(i, 0, PcNumVec[i]);
		m_HaoDuan.SetItemText(i, 1, StartIMEIVec[i]);
		m_HaoDuan.SetItemText(i, 2, EndIMEIVec[i]);
		m_HaoDuan.SetItemText(i, 3, CurIMEIVec[i]);
	}
}

void CDZ_Laser::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT3, L"");
	SetDlgItemText(IDC_EDIT4, L"");
	SetDlgItemText(IDC_EDIT1, L"");
	m_DZZhiDanComBo.ResetContent();
	DZFillZhidanToComBo();
	MessageBox(L"刷新成功，请重新选择订单", L"提示");

}

void CDZ_Laser::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString   FilePathName, txtValue;//文件名参数定义
	CString ValueStr;
	CStdioFile file;
	wchar_t CharValue[200];
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
	int i = 0;
	m_DataShow.DeleteAllItems();
	while (file.ReadString(ValueStr))
	{
		m_DataShow.InsertItem(i, L"1", 0);//插入行
		CString Count_PID;
		Count_PID.Format(L"%d", i + 1);
		m_DataShow.SetItemText(i, 0, Count_PID);
		m_DataShow.SetItemText(i, 1, ValueStr);
		i++;
	}
	SetDlgItemInt(IDC_EDIT2, m_DataShow.GetItemCount());
	SetDlgItemInt(IDC_EDIT6, m_DataShow.GetItemCount());
	MessageBox(L"导入成功", L"提示");
	getTxtData = TRUE;
	file.Close();
}

void CDZ_Laser::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (DZzhidan == L"")
	{
		MessageBox(L"请先选择订单", L"提示");
		return;
	}
	if (_ttoi64(m_StartIMEI) < _ttoi64(IMEIStaStart) || _ttoi64(m_StartIMEI) > _ttoi64(IMEIStaEnd) || _ttoi64(m_EndIMEI) > _ttoi64(IMEIStaEnd) || _ttoi64(m_EndIMEI) < _ttoi64(IMEIStaStart))
	{
		MessageBox(L"请设置正确的号段!!", L"提示");
		ShowDZLaserMsg(L"订单" + DZzhidan + L"的号段范围是：" + IMEIStaStart + L"--" + IMEIStaEnd, 1);
		return;
	}
	if (m_StartIMEI == L"" || m_EndIMEI == L"")
	{
		MessageBox(L"请输入号段!!", L"提示");
		return;
	}
	int IncreasingSel = m_IncreasingNum.GetCurSel();
	int CountNum = 0;
	CString IncreasingStr, IMEIStar, IMEIEnd, IMEICur;
	int IncreasingInt, HaveCheckCode, TagsCount;
	unsigned long long imeiint = 0;
	m_IncreasingNum.GetLBText(IncreasingSel, IncreasingStr);
	IncreasingInt = _ttoi(IncreasingStr);
	HaveCheckCode = m_DZHaveCheckCode.GetCheck();
	GetDlgItemText(IDC_EDIT3, IMEIStar);
	GetDlgItemText(IDC_EDIT4, IMEIEnd);
	m_DataShow.DeleteAllItems();
	SetDlgItemText(IDC_EDIT6, L"");
	ADOManage ado;
	ado.ConnSQL();
	if (!m_MultLDVal)
	{
		if (!ado.GetIMEICurrentAndTemplate(DZzhidan))
		{
			TagsCount = _atoi64(CStringA(IMEIEnd)) - _atoi64(CStringA(IMEIStar));
			TagsCount = TagsCount / IncreasingInt;
			LDIMEI = IMEIStar;
			ado.CloseAll1();
		}
		else
		{
			_variant_t IMEIVal;
			IMEIVal = ado.m_pRecordSet1->GetCollect("LdImei");
			LDIMEI = (LPCTSTR)IMEIVal.bstrVal;
			if (HaveCheckCode)
			{
				LDIMEI = LDIMEI.Left(14);
			}
			long long m_int64SNextTags;
			TCHAR LdTemp[50] = { 0 };
			//_tcscpy(LdTemp, LDIMEI);
			m_int64SNextTags = _ttoi64(LDIMEI);
			TagsCount = _atoi64(CStringA(IMEIEnd)) - m_int64SNextTags;
			TagsCount = TagsCount / IncreasingInt;
			ado.CloseAll1();
		}
	}
	else
	{
		if (m_HaoDuan.GetItemCount() == 0)
		{
			TagsCount = _atoi64(CStringA(m_EndIMEI)) - _atoi64(CStringA(m_StartIMEI));
			TagsCount = TagsCount / IncreasingInt;
			LDIMEI = m_StartIMEI;
		}
		else
		{
			if (!IsListClick)
			{
				LDIMEI = m_StartIMEI;
			}
			else
			{
				if (m_StartIMEI != m_HaoDuan.GetItemText(ListCurItem, 1) || m_EndIMEI != m_HaoDuan.GetItemText(ListCurItem, 2))
				{
					LDIMEI = m_StartIMEI;
				}
				IsListClick = FALSE;
			}
			if (HaveCheckCode)
			{
				LDIMEI = LDIMEI.Left(14);
			}
			TCHAR LdTemp[50] = { 0 };
			//_tcscpy(LdTemp, LDIMEI);
			long long IMEIInt64 = _ttoi64(LDIMEI);
			TagsCount = _atoi64(CStringA(IMEIEnd)) - IMEIInt64;
			TagsCount = TagsCount / IncreasingInt;
		}
	}
	//CountNum += IncreasingInt;
	for (int i = 0; i <= TagsCount; i++)
	{
		imeiint = _atoi64(CStringA(LDIMEI)) + CountNum;
		IMEICur = _ui64toa(imeiint, CT2A(IMEIStar), 10);
		if (IMEICur > IMEIEnd)
		{
			break;
		}
		if (HaveCheckCode)
		{
			IMEICur = autoLaserDlg.CreateIMEI15(IMEICur);
		}
		m_DataShow.InsertItem(i, L"1", 0);//插入行
		CString Count_PID;
		Count_PID.Format(L"%d", i + 1);
		m_DataShow.SetItemText(i, 0, Count_PID);
		m_DataShow.SetItemText(i, 1, IMEICur);
		CountNum = CountNum + IncreasingInt;
	}
	SetDlgItemInt(IDC_EDIT6, m_DataShow.GetItemCount());
	MessageBox(L"生成成功", L"提示");
	getTxtData = FALSE;
}

UINT CheckIMEIThread(LPVOID lparam)
{
	CDZ_Laser *dlg = (CDZ_Laser*)lparam;
	dlg->UpdateData(TRUE);
	CString RelativeData, RelativeZiDuan;
	if (_ttoi(dlg->m_ZiDuanVal) != 0)
	{
		RelativeZiDuan.Format(L"IMEI%s", dlg->m_ZiDuanVal);
	}
	else
	{
		RelativeZiDuan = L"SN";
	}
	while (CheckIMEIThreadFlag)
	{
		for (int i = 0; i < _ttoi(dlg->m_TagsCount); i++)
		{
			if (!CheckIMEIThreadFlag)
			{
				break;
			}
			if (dlg->m_CheckPrintVal)
			{
				if (!dlg->CheckNoUseDataInDB(dlg->m_DataShow.GetItemText(i, 1)) && !dlg->CheckNoUseDataInPrint(dlg->m_DataShow.GetItemText(i, 1))) //不存在
				{
					dlg->CheckDBResult.push_back(TRUE);
					dlg->m_DataShow.SetItemText(i, 2, L"OK");
				}
				else
				{
					dlg->CheckDBResult.push_back(FALSE);
					dlg->m_DataShow.SetItemText(i, 2, L"重复");
				}
			}
			else
			{
				if (!dlg->CheckNoUseDataInDB(dlg->m_DataShow.GetItemText(i, 1))) //不存在
				{
					dlg->CheckDBResult.push_back(TRUE);
					dlg->m_DataShow.SetItemText(i, 2, L"OK");
				}
				else
				{
					dlg->CheckDBResult.push_back(FALSE);
					dlg->m_DataShow.SetItemText(i, 2, L"重复");
				}
			}

		}
		//CheckIMEIThreadHandel = NULL;
		CheckIMEIThreadFlag = FALSE;
	}
	return 0;
}

BOOL CDZ_Laser::GetAllRelativeData(CString IMEI)
{
	UpdateData(TRUE);
	RelativeDataVec.clear();
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
	for (int j = 0; j <= 14; j++)
	{
		if (j == 0)
		{
			val = ado.m_pRecordSet->GetCollect(L"SN");
			if (val.vt != VT_NULL)
			{
				RelativeDataVec.push_back(val.bstrVal);
			}
			else
			{
				RelativeDataVec.push_back(RetData);
			}
		}
		else
		{
			ziduan.Format(L"IMEI%d", j);
			val = ado.m_pRecordSet->GetCollect(_variant_t(ziduan));
			if (val.vt != VT_NULL)
			{
				RelativeDataVec.push_back(val.bstrVal);
			}
			else
			{
				RelativeDataVec.push_back(RetData);
			}
		}

	}
	ado.CloseAll();
	return TRUE;
}

CString CDZ_Laser::GetRelativeData(CString IMEI, CString ziduan)
{
	CString RetData;
	ADOManage ado;
	ado.ConnSQL();
	RetData = ado.DZGetRelateData(IMEI, ziduan);
	ado.CloseAll();
	return RetData;
}

void CDZ_Laser::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (_ttoi(m_TagsCount) == 0)
	{
		AfxMessageBox(L"没有选择要导出的数据！");
		return;
	}
	CString buttonText;
	GetDlgItemText(IDC_BUTTON7, buttonText);
	if (buttonText.Find(L"导出中") != -1)
	{
		return;
	}
	GetDlgItem(IDC_BUTTON7)->SetWindowText(L"导出中...");
	AfxBeginThread(OutDataThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

UINT OutDataThread(LPVOID lparam)
{
	CDZ_Laser *dlg = (CDZ_Laser*)lparam;
	dlg->UpdateData(TRUE);
	CString LOG_Time;
	CTime t = CTime::GetCurrentTime();
	LOG_Time.Format(L"%02d-%02d-%02d", t.GetYear(), t.GetMonth(), t.GetDay());

	//system("mkdir \"foldername\"");
	// 打开的路径为本程序所在的路径
	char sFileName[256] = { 0 };
	CString ProjectPath = _T("");
	GetModuleFileName(AfxGetInstanceHandle(), LPWSTR(sFileName), 255);
	ProjectPath.Format(L"%s", sFileName);
	int pos = ProjectPath.ReverseFind('\\');
	if (pos != -1)
		ProjectPath = ProjectPath.Left(pos);

	//AfxMessageBox(L"导出路径-->" + ProjectPath + L"\\TagsEXCEL\\" + LOG_Time + L"_" + DZzhidan + L"_Out.xls");
	//CSpreadSheet SS(ProjectPath + L"\\TagsEXCEL\\" + LOG_Time + L"_" + DZzhidan + L"_Out.xls", L"导出数据");//*(strContents+i)
	CString TxtPath = ProjectPath + L"\\TagsTXT\\" + LOG_Time + L"_" + DZzhidan + L"_Out.txt";
	AfxMessageBox(TxtPath);
	//autoLaserDlg.WritetoTxt(L"OutData_", L"TagsTXT\\");


	//CStringArray *sampleArray;
	//SS.BeginTransaction();

	//// 加入标题
	////获取字段名strName==================================
	////清空表头
	////将字段显示出来
	//sampleArray = new CStringArray[_ttoi(dlg->m_TagsCount) + 8];
	//sampleArray->RemoveAll();
	//sampleArray->Add(L"EXPORT-DATA");
	//SS.AddHeaders(*sampleArray);
	//////////////////////////////

	//int tt = 0;
	//for (tt = 0; tt < _ttoi(dlg->m_TagsCount); tt++)
	//{
	//	(sampleArray + tt)->RemoveAll();
	//	(sampleArray + tt)->Add(dlg->m_DataShow.GetItemText(tt, 1));
	//}
	CFile file;
	char szANSIString[MAX_PATH];
	int length;
	try
	{
		file.Open(TxtPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
		for (int tt = 0; tt < _ttoi(dlg->m_TagsCount); tt++)
		{
			//SS.AddRow(*(sampleArray + tt));			//写EXCEL----<*(sampleArray+tt)>此处是C++的引用
			file.SeekToEnd(); // 移动文件指针到末尾
			/*BYTE byUnicode[] = { 0xFF, 0xFE };
			file.Write(byUnicode, sizeof(byUnicode));*/
			length = dlg->m_DataShow.GetItemText(tt, 1).GetLength() + 2;
			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, dlg->m_DataShow.GetItemText(tt, 1) + L"\r\n", -1, szANSIString, sizeof(szANSIString), NULL, NULL);
			file.Write(szANSIString, length);
			//file.Write(dlg->m_DataShow.GetItemText(tt, 1) + L"\r\n", wcslen(dlg->m_DataShow.GetItemText(tt, 1) + L"\r\n") * sizeof(WCHAR));
			//AfxMessageBox(L"1");
			//file.Write(m_DataShow.GetItemText(tt, 1) + L"\r\n", m_DataShow.GetItemText(tt, 1).GetLength()+19);
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		//SS.Commit();
		//delete[] sampleArray;
		file.Close();
		return 0;
	}
	//SS.Commit();
	//delete[] sampleArray;
	file.Close();
	AfxMessageBox(L"导出数据完毕！");
	dlg->GetDlgItem(IDC_BUTTON7)->SetWindowText(L"导出");
	return 1;
}

void CDZ_Laser::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString buttonText, checkCode, haveRelativeCode;
	GetDlgItemText(IDC_BUTTON4, buttonText);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	CLogInDlg lonin;
	if (buttonText == L"锁定")
	{
		if (DZzhidan == L"")
		{
			MessageBox(L"请先选择订单", L"提示");
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}
		if (DzLaserFile == L"")
		{
			MessageBox(L"请先选择模板", L"提示");
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}
		if (m_StartIMEI.GetLength() != IMEIStaStart.GetLength() || m_StartIMEI< IMEIStaStart || \
			m_StartIMEI > IMEIStaEnd || \
			m_EndIMEI.GetLength() != IMEIStaEnd.GetLength() || m_EndIMEI > IMEIStaEnd || \
			m_EndIMEI < m_StartIMEI
			)
		{
			MessageBox(L"请设置正确的号段!!", L"提示");
			ShowDZLaserMsg(L"订单" + DZzhidan + L"的号段范围是：" + IMEIStaStart + L"--" + IMEIStaEnd, 1);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}
		if (m_MultLDVal)
		{
			if (m_PcNumVal == L"")
			{
				MessageBox(L"请先选择PC序号", L"提示");
				GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
				return;
			}
			for (int i = 0; i < m_HaoDuan.GetItemCount(); i++)
			{
				if (m_PcNumVal == m_HaoDuan.GetItemText(i, 0))
				{
					if (m_StartIMEI != m_HaoDuan.GetItemText(i, 1) || m_EndIMEI != m_HaoDuan.GetItemText(i, 2))
					{
						MessageBox(L"请不要设置相同PC序号，已有PC序号请参照子号段列表", L"提示");
						GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
						return;
					}
				}
			}
		}
		if (lonin.DoModal() == IDCANCEL)
		{
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}
		checkCode.Format(L"%d", m_CheckCodeVal);
		haveRelativeCode.Format(L"%d", DataRelativeChoose);
		ADOManage ado;
		ado.ConnSQL();
		ado.InsetrDZLdRecordParam(DZzhidan, DzLaserFile, checkCode, m_StartIMEI, m_IncreasingNumVal, haveRelativeCode\
			, L"", m_BitsCheck, m_ScanDataPre);
		//if (m_MultLD)
		//{
		//	//上传到新字段
		//	if (m_PcNum.GetCurSel()==-1)
		//	{
		//		MessageBox(L"请先选择电脑序号", L"提示");
		//		return;
		//	}
		//	CString ValStr, PcNumVal, SubValStr;
		//	int pos, pos1;
		//	m_PcNum.GetLBText(m_PcNum.GetCurSel(), PcNumVal);
		//	ValStr = GetSubIMEISegmentVal(DZzhidan);
		//	pos = ValStr.Find(PcNumVal);
		//	if (pos != -1)
		//	{
		//		pos = ValStr.Find(L",", pos + 2);
		//		pos1 = ValStr.Find(L")", pos + 2);
		//		SubValStr = ValStr.Mid(pos + 1, pos1 - pos - 1);
		//		ValStr.Replace(SubValStr, m_DataShow.GetItemText(0, 1));
		//		//ValStr.Replace(ValStr[pos], ValStr[pos+15], dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
		//	}
		//	else
		//	{
		//		if (ValStr == L"")
		//		{
		//			ValStr = ValStr + L"(" + PcNumVal + L"," + m_StartIMEI + L"-" + m_EndIMEI + L"," + m_DataShow.GetItemText(0, 1) + L")";
		//		}
		//		else
		//		{
		//			ValStr = ValStr + L",(" + PcNumVal + L"," + m_StartIMEI + L"-" + m_EndIMEI + L"," + m_DataShow.GetItemText(0, 1) + L")";
		//		}
		//	}
		//	UpdateIMEISegment(DZzhidan, ValStr);
		//}
		ado.CloseAll();
		LockSomething(FALSE);
		UnLockSomething(TRUE);
		if (m_MarkType == 1)
		{
			GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT9)->SetFocus();
		}
		GetDlgItem(IDC_BUTTON4)->SetWindowText(L"解锁");
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	}
	else
	{
		if (LdSysIsOpen == TRUE)
		{
			MessageBox(L"请先关闭镭雕系统", L"提示");
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}
		if (lonin.DoModal() == IDCANCEL)
		{
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}
		/*CUnlockDlg unlockdlg;
		INT_PTR ret;
		ret = unlockdlg.DoModal();
		if (ret== IDCANCEL)
		{
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			return;
		}*/
		GetDlgItem(IDC_BUTTON4)->SetWindowText(L"锁定");
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
		UnLockSomething(FALSE);
		LockSomething(TRUE);
	}
	//StartmarkFunc(0,L"123",0,0,0,0,1);
}

void CDZ_Laser::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
	if (DZzhidan == L"")
	{
		MessageBox(L"请先选择订单", L"提示");
		GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		return;
	}
	CString CheckButtonText;
	GetDlgItem(IDC_BUTTON9)->GetWindowText(CheckButtonText);
	if (CheckButtonText.Find(L"停止") != -1)
	{
		CheckNoUseDataFlag = FALSE;
		SetDlgItemTextW(IDC_BUTTON9, L"查询");
		GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		return;
	}
	CheckNoUseDataFlag = TRUE;
	CheckThread = AfxBeginThread(CheckNoUseData, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	SetDlgItemTextW(IDC_BUTTON9, L"停止（查询中..）");
	SetDlgItemText(IDC_EDIT11, L"");
	GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
}

UINT CheckNoUseData(LPVOID lparam)
{
	CDZ_Laser *dlg;
	dlg = (CDZ_Laser*)lparam;
	int IncreasingSel = dlg->m_IncreasingNum.GetCurSel();
	CString IncreasingStr, IMEIStar, IMEIEnd, IMEICur, NoCheckCodeIMEI;
	int IncreasingInt, HaveCheckCode, TagsCount;
	unsigned long long imeiint = 0;
	dlg->m_IncreasingNum.GetLBText(IncreasingSel, IncreasingStr);
	IncreasingInt = _ttoi(IncreasingStr);
	HaveCheckCode = dlg->m_DZHaveCheckCode.GetCheck();
	dlg->GetDlgItemText(IDC_EDIT3, IMEIStar);
	dlg->GetDlgItemText(IDC_EDIT4, IMEIEnd);
	TagsCount = _atoi64(CStringA(IMEIEnd)) - _atoi64(CStringA(IMEIStar));
	TagsCount = TagsCount / IncreasingInt;
	dlg->m_NoUseData.DeleteAllItems();
	int FirstRow = 0;
	while (CheckNoUseDataFlag)
	{
		for (int i = 0; i <= TagsCount; i++)
		{
			if (!CheckNoUseDataFlag)
			{
				break;
			}
			imeiint = _atoi64(CStringA(IMEIStar)) + IncreasingInt * i;
			IMEICur = _ui64toa(imeiint, CT2A(IMEIStar), 10);
			NoCheckCodeIMEI = IMEICur;
			if (HaveCheckCode)
			{
				IMEICur = autoLaserDlg.CreateIMEI15(IMEICur);
			}
			if (!dlg->CheckNoUseDataInDB(IMEICur)) //为真时，表示存在
			{
				dlg->m_NoUseData.InsertItem(FirstRow, L"1", 0);//插入行
				CString Count_PID;
				Count_PID.Format(L"%d", FirstRow + 1);
				dlg->m_NoUseData.SetItemText(FirstRow, 0, Count_PID);
				dlg->m_NoUseData.SetItemText(FirstRow, 1, IMEICur);
				FirstRow++;
			}
		}
		CheckNoUseDataFlag = FALSE;
	}
	dlg->SetDlgItemInt(IDC_EDIT11, dlg->m_NoUseData.GetItemCount());
	dlg->SetDlgItemTextW(IDC_BUTTON9, L"查询");
	return 0;
}

int CDZ_Laser::CheckNoUseDataInDB(CString IMEI)
{
	ADOManage ado;
	ado.ConnSQL();
	if (ado.CheckLdIMEIExitLM(IMEI))
	{
		ado.CloseAll();
		return 1;  //代表存在
	}
	ado.CloseAll();
	return 0;
}

int CDZ_Laser::CheckScanDataInRela(CString ziduan, CString value)
{
	ADOManage ado;
	ado.ConnSQL();
	if (ado.CheckScanDataExit(ziduan, value))
	{
		ado.CloseAll();
		return 1;  //代表存在
	}
	ado.CloseAll();
	return 0;
}

int CDZ_Laser::CheckNoUseDataInPrint(CString IMEI)
{
	ADOManage ado;
	ado.ConnSQL();
	if (ado.CheckLdIMEIExitPrint(IMEI))
	{
		ado.CloseAll();
		return 1;  //代表存在
	}
	ado.CloseAll();
	return 0;
}

void CDZ_Laser::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString CheckButtonText, OutButtonText;
	GetDlgItem(IDC_BUTTON9)->GetWindowText(CheckButtonText);
	GetDlgItem(IDC_BUTTON3)->GetWindowText(OutButtonText);
	if (CheckButtonText.Find(L"查询中") != -1)
	{
		MessageBox(L"请查询完毕再进行导出！", L"提示");
		return;
	}
	if (OutButtonText.Find(L"导出中") != -1)
	{
		return;
	}
	UpdateData(TRUE);
	if (_ttoi(m_NoUseDataCount) == 0)
	{
		AfxMessageBox(L"没有选择要导出的数据！");
		return;
	}
	SetDlgItemText(IDC_BUTTON3, L"导出中...");
	CString LOG_Time;
	CTime t = CTime::GetCurrentTime();
	LOG_Time.Format(L"%02d-%02d-%02d", t.GetDay(), t.GetHour(), t.GetMinute());

	//system("mkdir \"foldername\"");
	// 打开的路径为本程序所在的路径
	char sFileName[256] = { 0 };
	CString ProjectPath = _T("");
	GetModuleFileName(AfxGetInstanceHandle(), LPWSTR(sFileName), 255);
	ProjectPath.Format(L"%s", sFileName);
	int pos = ProjectPath.ReverseFind('\\');
	if (pos != -1)
		ProjectPath = ProjectPath.Left(pos);

	//AfxMessageBox(L"导出路径-->" + ProjectPath + L"\\NoUseDataExcel\\" + LOG_Time + L"_" + DZzhidan + L"_NoUseDataOut.xls");
	//CSpreadSheet SS(ProjectPath + L"\\NoUseDataExcel\\" + LOG_Time + L"_" + DZzhidan + L"_NoUseDataOut.xls", L"导出数据");//*(strContents+i)
	CString TxtPath = ProjectPath + L"\\NoUseDataTxt\\" + LOG_Time + L"_" + DZzhidan + L"_NoUseDataOut.txt";
	AfxMessageBox(TxtPath);


	//CStringArray *sampleArray;
	//SS.BeginTransaction();

	//// 加入标题
	////获取字段名strName==================================
	////清空表头
	////将字段显示出来
	//sampleArray = new CStringArray[_ttoi(m_NoUseDataCount) + 8];
	//sampleArray->RemoveAll();
	//sampleArray->Add(L"NOUSEDATA");
	//SS.AddHeaders(*sampleArray);
	////////////////////////////

	/*int tt = 0;
	for (tt = 0; tt < _ttoi(m_NoUseDataCount); tt++)
	{
		(sampleArray + tt)->RemoveAll();
		(sampleArray + tt)->Add(m_NoUseData.GetItemText(tt, 1));
	}*/
	CFile file;
	char szANSIString[MAX_PATH];
	int length;
	try
	{
		file.Open(TxtPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::typeUnicode);
		for (int tt = 0; tt < _ttoi(m_NoUseDataCount); tt++)
		{
			//SS.AddRow(*(sampleArray + tt));			//写EXCEL----<*(sampleArray+tt)>此处是C++的引用
			file.SeekToEnd(); // 移动文件指针到末尾
			/*BYTE byUnicode[] = { 0xFF, 0xFE };
			file.Write(byUnicode, sizeof(byUnicode));*/
			length = m_NoUseData.GetItemText(tt, 1).GetLength() + 2;
			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_NoUseData.GetItemText(tt, 1) + L"\r\n", -1, szANSIString, sizeof(szANSIString), NULL, NULL);
			file.Write(szANSIString, length);

		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		SetDlgItemText(IDC_BUTTON3, L"导出未镭雕数据");
		//SS.Commit();
		//delete[] sampleArray;
		file.Close();
		return;
	}
	//SS.Commit();
	//delete[] sampleArray;
	file.Close();
	AfxMessageBox(L"导出数据完毕！");
	SetDlgItemText(IDC_BUTTON3, L"导出未镭雕数据");
	return;
}

void CDZ_Laser::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
}

int CDZ_Laser::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
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

void CDZ_Laser::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (LaserMachine.SysIni() == TRUE)
	{
		ShowDZLaserMsg(L"初始化镭雕系统成功,DLL版本:" + LaserMachine.GetDLLVer(), 0);
		Sleep(500);
		if (LaserMachine.LoadMarkFile(DzLaserFile) == TRUE)
		{
			ShowDZLaserMsg(L"加载模板成功", 0);
			HS_SelectObjects(NULL, TRUE);	//选择所有对象
		}
		else
		{
			ShowDZLaserMsg(L"加载模板失败", 1);
			return;
		}
		//if (!CheckIMEIThreadHandel)//查重线程不开启的前提下才去开启线程
		//{
		//	//AfxMessageBox(L"1");
		//	CheckIMEIThreadFlag = TRUE;
		//	CheckIMEIThreadHandel = AfxBeginThread(CheckIMEIThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		//}
		LdSysIsOpen = TRUE;
		GetDlgItem(IDC_BUTTON10)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON11)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON12)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON13)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
	}
	else
	{
		ShowDZLaserMsg(L"初始化镭雕系统失败,DLL版本:" + LaserMachine.GetDLLVer(), 1);
	}
}

void CDZ_Laser::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, _T("*.HS"), NULL, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, _T("HS File|*.HS|"), NULL);
	if (IDCANCEL == dlg.DoModal())
		return;
	strFilePath = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1, strFilePath);
}

int CDZ_Laser::ShowDZLaserMsg(CString strMsg, int MsgNum)
{

	CString str = autoLaserDlg.GetTime() + _T("\r\n ");
	str += strMsg + _T("\r\n");
	m_DzLaserLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (MsgNum)
	{
	case 0:
		m_DzLaserLog.ReplaceSel(autoLaserDlg.GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		break;
	case 1:
		m_DzLaserLog.SetSelectionCharFormat(cf);
		m_DzLaserLog.ReplaceSel(autoLaserDlg.GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		break;
	default:
		break;
	}
	m_DzLaserLog.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return 0;
}

void CDZ_Laser::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buttonText, buttonText1;
	GetDlgItemText(IDC_BUTTON14, buttonText);
	GetDlgItemText(IDC_BUTTON16, buttonText1);
	if (buttonText.Find(L"停止") != -1 || buttonText1.Find(L"停止") != -1)
	{
		MessageBox(L"打标正在进行，请先停止", L"提示");
		return;
	}
	if (LaserMachine.SysClose() == TRUE)
	{
		ShowDZLaserMsg(L"关闭镭雕系统成功", 0);
		LdSysIsOpen = FALSE;
		GetDlgItem(IDC_BUTTON10)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON11)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON12)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON14)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
	}
	else
	{
		ShowDZLaserMsg(L"关闭镭雕系统失败！", 1);
	}
}

void CDZ_Laser::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowDZLaserMsg(L"阻塞红光预览开始..", 0);
	LaserMachine.PreviewMark(FALSE, TRUE, 0);//INFINITE
}

void CDZ_Laser::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowDZLaserMsg(L"阻塞打标开始..", 0);
	LaserMachine.StartMark(FALSE, TRUE, 0);//StartmarkFunc
}

void CDZ_Laser::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON14)->EnableWindow(FALSE);
	Sleep(200);
	CString buttonText;
	GetDlgItemText(IDC_BUTTON14, buttonText);
	if (buttonText.Find(L"开始") != -1)
	{
		if (m_MarkType == 0)
		{
			if (!EmptyDataCheck&&_ttoi(m_TagsCount) == 0)
			{
				MessageBox(L"数据为空", L"提示");
				GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
				return;
			}
			ShowDZLaserMsg(L"成功获取批量数据", 0);
			StartMark = TRUE;
			MarkThread = AfxBeginThread(StartMarkThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
			GetDlgItem(IDC_BUTTON14)->SetWindowText(L"停止");
			GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON15)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON12)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
			MessageBox(L"请选择正确的打标方式--脚踏打标");
			return;
		}
	}
	else
	{
		StartMark = FALSE;
		if (waitStopFlag)
		{
			AfxBeginThread(waitStopThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
			ShowDZLaserMsg(L"请再按一次脚踏，完成镭雕", 1);
			return;
		}
		GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON15)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON12)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON13)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON14)->SetWindowText(L"开始");
	}
}

UINT waitStopThread(LPVOID lparam)
{
	CDZ_Laser * dlg = (CDZ_Laser*)lparam;
	while (waitStopFlag)
	{

	}
	dlg->OnBnClickedButton14();
	return 0;
}

UINT StartMarkThread(LPVOID lparam)
{
	CDZ_Laser * dlg = (CDZ_Laser*)lparam;
	dlg->UpdateData(TRUE);
	CString MarkData;
	if (dlg->m_MarkType == 0)
	{
		while (dlg->StartMark)
		{
			/*if (dlg->FirstData + 1 >= _ttoi(dlg->m_TagsCount))
			{
				dlg->MessageBox(L"当前号段已镭雕完成", L"提示");
				return 1;
			}*/
		PAUSE:		while (dlg->PauseMark == TRUE)
		{
			dlg->ShowDZLaserMsg(L"暂停中..", 1);
			Sleep(500);
			if (dlg->StartMark == FALSE)
			{
				dlg->ShowDZLaserMsg(L"已停止", 1);
				return FALSE;
			}
		}
					BOOL signal = FALSE;
					do
					{
						if (dlg->PauseMark == TRUE)
						{
							goto PAUSE;
						}
						dlg->ShowDZLaserMsg(L"等待触发", 0);
						if (dlg->StartMark == FALSE)
						{
							dlg->ShowDZLaserMsg(L"已停止", 1);
							return FALSE;
						}
						if (dlg->FastCheck == TRUE)
						{
							signal = LaserMachine.QuerytouchActive(dlg->GetSafeHwnd());
						}
						else
						{
							signal = LaserMachine.Querytouch();
						}
						if (signal == TRUE)
						{
							dlg->ShowDZLaserMsg(L"已触发", 0);
						}
						Sleep(200);
					} while (signal == FALSE);
					//AfxMessageBox(dlg->m_DataShow.GetItemText(dlg->FirstData, 1));
					if (dlg->StartmarkFunc((dlg->FirstData != 0), dlg->m_DataShow.GetItemText(dlg->FirstData, 1), TRUE, FALSE, INFINITE, 0, dlg->FirstData) == TRUE)
					{
						if (dlg->EmptyDataCheck == FALSE)
						{
							CString showStr;
							showStr.Format(L"第%d次 ", dlg->FirstData + 1);
							dlg->ShowDZLaserMsg(showStr + L"IMEI:" + dlg->m_DataShow.GetItemText(dlg->FirstData, 1) + L"镭雕完成", 0);
							//dlg->m_DataShow.SetItemText(dlg->FirstData, 3, L"已镭雕");
							if (dlg->m_MultLDVal)
							{
								//上传到新字段
								CString ValStr, PcNumVal, SubValStr;
								int pos, pos1;
								//dlg->m_PcNum.GetLBText(dlg->m_PcNum.GetCurSel(), PcNumVal);
								PcNumVal = dlg->m_PcNumVal;
								ValStr = dlg->GetSubIMEISegmentVal(DZzhidan);
								pos = ValStr.Find(PcNumVal);
								//AfxMessageBox(PcNumVal);
								if (pos != -1)
								{
									pos = ValStr.Find(L",", pos + 2);
									pos1 = ValStr.Find(L")", pos + 2);
									SubValStr = ValStr.Mid(pos + 1, pos1 - pos - 1);
									ValStr.Replace(SubValStr, dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
									//ValStr.Replace(ValStr[pos], ValStr[pos+15], dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
								}
								else
								{
									if (ValStr == L"")
									{
										ValStr = ValStr + L"(" + PcNumVal + L"," + dlg->m_StartIMEI + L"-" + dlg->m_EndIMEI + L"," + dlg->m_DataShow.GetItemText(1 + 1, 1) + L")";
									}
									else
									{
										ValStr = ValStr + L",(" + PcNumVal + L"," + dlg->m_StartIMEI + L"-" + dlg->m_EndIMEI + L"," + dlg->m_DataShow.GetItemText(1 + 1, 1) + L")";
									}
								}
								//AfxMessageBox(ValStr);
								dlg->UpdateIMEISegment(DZzhidan, ValStr);
							}
							else
							{
								if (dlg->FirstData + 1 < _ttoi(dlg->m_TagsCount))
								{
									dlg->UpdateDzLDNextIMEI(dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
								}
								else
								{
									dlg->UpdateDzLDNextIMEI(dlg->m_DataShow.GetItemText(dlg->FirstData, 1));
								}
							}
							dlg->FirstData++;
							if (dlg->FirstData >= _ttoi(dlg->m_TagsCount))
							{
								dlg->MessageBox(L"当前号段已镭雕完成", L"提示");
								return 1;
							}
						}
						else
						{
							dlg->ShowDZLaserMsg(L"镭雕完成", 0);
						}
					}
					else
					{
						CString showStr;
						showStr.Format(L"第%d次 ", dlg->FirstData + 1);
						dlg->ShowDZLaserMsg(showStr + L"IMEI:" + dlg->m_DataShow.GetItemText(dlg->FirstData, 1) + L"镭雕失败", 1);
					}
					Sleep(300);
		}
	}
	else if (dlg->m_MarkType == 2)  //定时打标
	{
		while (dlg->StartMark)
		{
			if (dlg->FirstData + 1 >= _ttoi(dlg->m_TagsCount))
			{
				dlg->MessageBox(L"当前号段已镭雕完成", L"提示");
				return 1;
			}
			while (dlg->PauseMark == TRUE)
			{
				if (dlg->StartMark == FALSE)
				{
					dlg->ShowDZLaserMsg(L"已停止", 1);
					return FALSE;
				}
				dlg->ShowDZLaserMsg(L"暂停中..", 1);
				Sleep(300);
			}
			if (dlg->StartmarkFunc((dlg->FirstData != 0), dlg->m_DataShow.GetItemText(dlg->FirstData, 1), FALSE, TRUE, INFINITE, 0, dlg->FirstData) == TRUE)
			{
				if (dlg->EmptyDataCheck == FALSE)
				{
					dlg->ShowDZLaserMsg(L"IMEI:" + dlg->m_DataShow.GetItemText(dlg->FirstData, 1) + L"镭雕完成", 0);
					dlg->m_DataShow.SetItemText(dlg->FirstData, 3, L"已镭雕");
					if (dlg->m_MultLDVal)
					{
						//上传到新字段
						CString ValStr, PcNumVal, SubValStr;
						int pos, pos1;
						//dlg->m_PcNum.GetLBText(dlg->m_PcNum.GetCurSel(), PcNumVal);
						PcNumVal = dlg->m_PcNumVal;
						ValStr = dlg->GetSubIMEISegmentVal(DZzhidan);
						pos = ValStr.Find(PcNumVal);
						if (pos != -1)
						{
							pos = ValStr.Find(L",", pos + 2);
							pos1 = ValStr.Find(L")", pos + 2);
							SubValStr = ValStr.Mid(pos + 1, pos1 - pos - 1);
							ValStr.Replace(SubValStr, dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
							//ValStr.Replace(ValStr[pos], ValStr[pos+15], dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
						}
						else
						{
							if (ValStr == L"")
							{
								ValStr = ValStr + L"(" + PcNumVal + L"," + dlg->m_StartIMEI + L"-" + dlg->m_EndIMEI + L"," + dlg->m_DataShow.GetItemText(1 + 1, 1) + L")";
							}
							else
							{
								ValStr = ValStr + L",(" + PcNumVal + L"," + dlg->m_StartIMEI + L"-" + dlg->m_EndIMEI + L"," + dlg->m_DataShow.GetItemText(1 + 1, 1) + L")";
							}
						}
						dlg->UpdateIMEISegment(DZzhidan, ValStr);
					}
					else
					{
						if (dlg->FirstData + 1 < _ttoi(dlg->m_TagsCount))
						{
							dlg->UpdateDzLDNextIMEI(dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
						}
						else
						{
							dlg->UpdateDzLDNextIMEI(dlg->m_DataShow.GetItemText(dlg->FirstData, 1));
							dlg->MessageBox(L"当前号段已镭雕完成", L"提示");
							return 1;
						}
					}
					dlg->FirstData++;
					if (dlg->FirstData >= _ttoi(dlg->m_TagsCount))
					{
						dlg->MessageBox(L"当前号段已镭雕完成", L"提示");
						return 1;
					}
				}
				else
				{
					dlg->ShowDZLaserMsg(L"镭雕完成", 0);
				}
			}
			else
			{
				dlg->ShowDZLaserMsg(L"IMEI:" + dlg->m_DataShow.GetItemText(dlg->FirstData, 1) + L"镭雕失败", 1);
			}
			Sleep(_ttoi(dlg->m_delayTime) * 1000);
		}
	}
	return 1;
}

BOOL CDZ_Laser::UpdateNextDate(CString date, CString zhidan)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.UpdateLdSnAndImei(L" ", date, zhidan);
	ado.CloseAll();
	return TRUE;
}

CString CDZ_Laser::GetSubIMEISegmentVal(CString zhidan)
{
	CString ValStr;
	ADOManage ado;
	ado.ConnSQL();
	ValStr = ado.GetSubSegment(zhidan);
	ado.CloseAll();
	return ValStr;
}

BOOL CDZ_Laser::UpdateIMEISegment(CString zhidan, CString val)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.UpdateSubIMEISegment(zhidan, val);
	ado.CloseAll();
	return TRUE;
}

BOOL CDZ_Laser::StartmarkFunc(BOOL OnceDone, CString InputData, BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime, int DataCount, int DataNo)
{
	BOOL Ret = TRUE;
	UpdateData(TRUE);
	int nFlag = 0;
	do
	{
		//AfxMessageBox(L"1");
		int nRet = HS_IsMarkEnd(&nFlag);//0---正在打标；1---正常结束；2---打标中止结束；3---设备异常打标异常
		if (0 == nRet)
		{
			//AfxMessageBox(L"2");
			if (nFlag == 0)
			{
				Sleep(200);
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	} while (nFlag == 0);
	//////////////////////////////////////////////////////////////////////

	if (!EmptyDataCheck)
	{
		//AfxMessageBox(L"1");
		if (m_MarkType != 1)
		{
			//AfxMessageBox(L"2");
			if (getTxtData)
			{
				CString compareInput;
				compareInput = InputData;
				if (m_CheckCodeVal)
				{
					compareInput = compareInput.Left(compareInput.GetLength() - 1);
				}
				if (compareInput.GetLength() != m_StartIMEI.GetLength() || compareInput< m_StartIMEI || \
					compareInput.GetLength() != m_EndIMEI.GetLength() || compareInput>m_EndIMEI)
				{
					MessageBox(L"数据：" + InputData + L" 在号段外", L"提示");
					ShowDZLaserMsg(L"数据：" + InputData + L" 在号段外", 1);
					return FALSE;
				}
			}
			if (!RemarkCheck)
			{
				BOOL IsRepairFlag = FALSE;
				if (m_CheckPrintVal)
				{
					if (!CheckNoUseDataInDB(InputData) && !CheckNoUseDataInPrint(InputData)) //不存在
					{
						m_DataShow.SetItemText(DataNo, 2, L"OK");
					}
					else
					{
						IsRepairFlag = TRUE;
					}
				}
				else
				{
					if (!CheckNoUseDataInDB(InputData)) //不存在
					{
						m_DataShow.SetItemText(DataNo, 2, L"OK");
					}
					else
					{
						IsRepairFlag = TRUE;
					}
				}
				if (IsRepairFlag == TRUE)
				{
					//AfxMessageBox(L"3");
					if (DBIFilterCheck)
					{
						//AfxMessageBox(L"4");
						ShowDZLaserMsg(L"数据:" + InputData + L"重号，自动过滤!!", 1);
						m_DataShow.SetItemText(DataNo, 3, L"重号，自动过滤");
						return TRUE;
					}
					else
					{
						//AfxMessageBox(L"5");
						ShowDZLaserMsg(L"数据:" + InputData + L"重号!!", 1);
						m_DataShow.SetItemText(DataNo, 3, L"重号");
						return FALSE;
					}
				}
			}
		}
		if (DataRelativeChoose == TRUE)//数据关联则还有值要赋予
		{
			if (!GetAllRelativeData(InputData))
			{
				ShowDZLaserMsg(L"无关联数据！！", 1);
				//AfxMessageBox(L"8");
				return FALSE;
			}
			else
			{
				for (int i = 0; i <= 13; i++)
				{
					if (RelativeDataVec.at(i) == L"NULL")
					{
						continue;
					}
					CString DataName;
					DataName.Format(L"RelaData%d", i);
					Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)DataName, (LPTSTR)(LPCTSTR)RelativeDataVec.at(i));
					if (Ret != TRUE)
					{
						ShowDZLaserMsg(DataName + L"赋值失败！！", 1);
						//AfxMessageBox(L"8");
						return FALSE;
					}
				}
			}
		}
		CString ttCS, List2Val, List3Val, ConnVal, ConnChar;
		int ConnNum;
		for (int tt = 0; tt < 48; tt++)
		{
			ConnVal = L"";
			ConnChar = L"";
			if (tt == 0 || tt == 1)
			{
				if (OnceDone == FALSE)
				{
					if ((m_list2.GetItemText(tt, 1) != "") && (m_list2.GetItemText(tt, 2) != "") && (m_list2.GetItemText(tt, 3) != ""))
						Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 3));
					if (Ret != TRUE)
					{
						ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + m_list2.GetItemText(tt, 3), 1);
						//AfxMessageBox(L"7");
						return FALSE;
					}
				}
			}
			else
			{
				List2Val = m_list2.GetItemText(tt, 2);
				if (List2Val.Find(L"Other") == -1)
				{
					List3Val = m_list2.GetItemText(tt, 3);
					if (List3Val.Find(L"号段生成") != -1)
					{
						Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)InputData);
						if (Ret != TRUE)
						{
							ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + InputData, 1);
							//AfxMessageBox(L"6");
							return FALSE;
						}
					}
					else
					{
						ConnChar = List3Val.Left(1);
						List3Val = List3Val.Mid(1);
						while (true)
						{
							if (List3Val.Find(ConnChar) != -1)
							{
								ConnNum = _ttoi(List3Val.Mid(0, List3Val.Find(ConnChar)));
								//ConnNum = _ttoi(List3Val.Mid(List3Val.Find(ConnChar) - 1, 1));
								if (ConnNum == 1)
								{
									ConnVal += InputData + ConnChar;
								}
								else
								{
									ConnVal += RelativeDataVec.at(ConnNum) + ConnChar;
								}
							}
							else
							{
								ConnNum = _ttoi(List3Val);
								//ConnNum = _ttoi(List3Val.Right(1));
								if (ConnNum == 1)
								{
									ConnVal += InputData;
								}
								else
								{
									ConnVal += RelativeDataVec.at(ConnNum);
								}
								break;
							}
							List3Val = List3Val.Mid(List3Val.Find(ConnChar) + 1);
						}
						Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)ConnVal);
						if (Ret != TRUE)
						{
							ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + ConnVal, 1);
							//AfxMessageBox(L"6");
							return FALSE;
						}
					}
				}
				else
				{
					break;
				}
			}
			//ttCS.Format(L"第%d次", tt + 1);
			//if (tt == 0)
			//{
			//	Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)InputData);
			//	if (Ret != TRUE)
			//	{
			//		ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + InputData, 1);
			//		//AfxMessageBox(L"6");
			//		return FALSE;
			//	}
			//}
			//else if ((m_list2.GetItemText(tt, 1) == "") || (m_list2.GetItemText(tt, 2) == "") || (m_list2.GetItemText(tt, 3) == ""))
			//{
			//	break;
			//}
			//else if (OnceDone == FALSE)
			//{
			//	if ((m_list2.GetItemText(tt, 1) != "") && (m_list2.GetItemText(tt, 2) != "") && (m_list2.GetItemText(tt, 3) != ""))
			//		Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 3));
			//	if (Ret != TRUE)
			//	{
			//		ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + m_list2.GetItemText(tt, 3), 1);
			//		//AfxMessageBox(L"7");
			//		return FALSE;
			//	}
			//}
			//else
			//	break;
		}
		if (Ret != TRUE)
		{
			//AfxMessageBox(L"9");
			return FALSE;
		}
		m_MarkData.SetWindowText(InputData);
	}
	ShowDZLaserMsg(L"开始调用StartMark..", 0);
	Ret = LaserMachine.StartMark(bWaitTouch, bWaitEnd, nOverTime);
	if (Ret == TRUE)
	{
		if (EmptyDataCheck == FALSE)
		{
			if (RemarkCheck)
			{
				UpdateDzLdData(InputData);
			}
			else
			{
				InsertDzLdData(InputData);
			}
		}
		//ShowDZLaserMsg(L"镭雕成功",0);
		m_DataShow.SetItemText(DataNo, 3, L"已镭雕");
		writeDZTxt(L"IMEI:" + InputData + L" 镭雕成功");
	}
	else
	{
		//ShowDZLaserMsg(L" 镭雕失败", 1);
		writeDZTxt(L"IMEI:" + InputData + L" 镭雕失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CDZ_Laser::ScanStartmarkFunc(BOOL OnceDone, CString InputData, CString relative, BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime, int DataCount, int DataNo)
{
	BOOL Ret = TRUE;
	UpdateData(TRUE);
	//AfxMessageBox(L"5");
	int nFlag = 0;
	do
	{
		int nRet = HS_IsMarkEnd(&nFlag);//0---正在打标；1---正常结束；2---打标中止结束；3---设备异常打标异常
		if (0 == nRet)
		{
			//AfxMessageBox(L"6");
			if (nFlag == 0)
			{
				Sleep(200);
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	} while (nFlag == 0);
	//////////////////////////////////////////////////////////////////////
	if (!EmptyDataCheck)
	{
		/*if (m_MarkType != 1)
		{
			if (CheckDBResult[DataNo] == FALSE)
			{
				if (DBIFilterCheck)
				{
					ShowDZLaserMsg(L"数据:" + InputData + L"重号，自动过滤!!", 1);
					m_DataShow.SetItemText(DataNo, 3, L"重号，自动过滤");
					return TRUE;
				}
				else
				{
					ShowDZLaserMsg(L"数据:" + InputData + L"重号!!", 1);
					m_DataShow.SetItemText(DataNo, 3, L"重号");
					return FALSE;
				}
			}
		}*/
		//AfxMessageBox(L"7");
		//CString ttCS, List2Val, List3Val, ConnVal, ConnChar;
		//int ConnNum;
		//for (int tt = 0; tt < 48; tt++)
		//{
		//	ConnVal = L"";
		//	ConnChar = L"";
		//	if (tt == 0 || tt == 1)
		//	{
		//		if (OnceDone == FALSE)
		//		{
		//			if ((m_list2.GetItemText(tt, 1) != "") && (m_list2.GetItemText(tt, 2) != "") && (m_list2.GetItemText(tt, 3) != ""))
		//				Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 3));
		//			if (Ret != TRUE)
		//			{
		//				ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + m_list2.GetItemText(tt, 3), 1);
		//				//AfxMessageBox(L"7");
		//				return FALSE;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		List2Val = m_list2.GetItemText(tt, 2);
		//		if (List2Val.Find(L"Other") == -1)
		//		{
		//			List3Val = m_list2.GetItemText(tt, 3);
		//			if (List3Val.Find(L"号段生成") != -1)
		//			{
		//				Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)InputData);
		//				if (Ret != TRUE)
		//				{
		//					ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + InputData, 1);
		//					//AfxMessageBox(L"6");
		//					return FALSE;
		//				}
		//			}
		//			else
		//			{
		//				ConnChar = List3Val.Left(1);
		//				List3Val = List3Val.Mid(1);
		//				while (true)
		//				{
		//					if (List3Val.Find(ConnChar) != -1)
		//					{
		//						ConnNum = _ttoi(List3Val.Mid(0, List3Val.Find(ConnChar)));
		//						//ConnNum = _ttoi(List3Val.Mid(List3Val.Find(ConnChar) - 1, 1));
		//						if (ConnNum == 1)
		//						{
		//							ConnVal += InputData + ConnChar;
		//						}
		//						else
		//						{
		//							ConnVal += RelativeDataVec.at(ConnNum) + ConnChar;
		//						}
		//					}
		//					else
		//					{
		//						ConnNum = _ttoi(List3Val);
		//						//ConnNum = _ttoi(List3Val.Right(1));
		//						if (ConnNum == 1)
		//						{
		//							ConnVal += InputData;
		//						}
		//						else
		//						{
		//							ConnVal += RelativeDataVec.at(ConnNum);
		//						}
		//						break;
		//					}
		//					List3Val = List3Val.Mid(List3Val.Find(ConnChar) + 1);
		//				}
		//				Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)m_list2.GetItemText(tt, 2), (LPTSTR)(LPCTSTR)ConnVal);
		//				if (Ret != TRUE)
		//				{
		//					ShowDZLaserMsg(ttCS + L"赋值失败!!" + m_list2.GetItemText(tt, 2) + ConnVal, 1);
		//					//AfxMessageBox(L"6");
		//					return FALSE;
		//				}
		//			}
		//		}
		//		else
		//		{
		//			break;
		//		}
		//	}
		//}
		CString ttCS, List2Val, List3Val, ConnVal, ConnChar,IMEIBuff;
		CString buffer[15][2];
		int ZiduanNum = 0;
		BOOL IsUpdateFlag = FALSE;
		int ConnNum;
		for (int tt = 0; tt < 48; tt++)
		{
			ConnVal = L"";
			ConnChar = L"";
			List2Val = m_list2.GetItemText(tt, 2);
			if (tt == 0 || tt == 1)
			{

			}
			else if (tt == 2)
			{
				if (List2Val != "IMEI1")//不是IMEI，需要上传关联和查重关联表
				{
					IsUpdateFlag = TRUE;
				}
				else //是IMEI，不需要上传关联和查重关联表
				{
					IsUpdateFlag = FALSE;
					IMEIBuff = InputData;
				}
				ZiduanNum = _ttoi(List2Val.Mid(4));
				buffer[ZiduanNum][0] = List2Val;
				buffer[ZiduanNum][1] = InputData;
				Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)List2Val, (LPTSTR)(LPCTSTR)InputData);
				if (Ret != TRUE)
				{
					ShowDZLaserMsg(List2Val + L"赋值失败！！", 1);
					//AfxMessageBox(L"8");
					return FALSE;
				}
			}
			else
			{
				if (List2Val == "IMEI1") //IMEI
				{
					ZiduanNum = _ttoi(List2Val.Mid(4));
					buffer[ZiduanNum][0] = List2Val;
					buffer[ZiduanNum][1] = m_DataShow.GetItemText(DataNo,1);
					IMEIBuff = m_DataShow.GetItemText(DataNo, 1);
					Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)List2Val, (LPTSTR)(LPCTSTR)m_DataShow.GetItemText(DataNo, 1));
					if (Ret != TRUE)
					{
						ShowDZLaserMsg(List2Val + L"赋值失败！！", 1);
						//AfxMessageBox(L"8");
						return FALSE;
					}
				}
				else if (List2Val == "IMEI2")  //SN
				{
					do
					{
						if (CheckScanDataInRela(List2Val, L"SN号"))
						{
							unsigned long long snint = 0;
							CString SnPre, Snlaf, lengthstr, num;
							int length = DzSN2.GetLength();
							m_IncreasingNum.GetLBText(m_IncreasingNum.GetCurSel(), num);
							lengthstr.Format(_T("%d"), length);
							SnPre = DzStrSn.Left(DzStrSn.GetLength() - DzSN2.GetLength());
							Snlaf = DzStrSn.Right(DzSN2.GetLength());
							snint = _ttoi(Snlaf) + _ttoi(num);
							Snlaf.Format(_T("%0") + lengthstr + _T("d"), snint);
							DzStrSn = SnPre + Snlaf;
						}
						else
						{
							break;
						}

					} while (true);

					ZiduanNum = _ttoi(List2Val.Mid(4));
					buffer[ZiduanNum][0] = List2Val;
					buffer[ZiduanNum][1] = DzStrSn;
					//在这里获取SN号，并替换到模板，对象名是List2Val
					Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)List2Val, (LPTSTR)(LPCTSTR)DzStrSn);
					if (Ret != TRUE)
					{
						ShowDZLaserMsg(List2Val + L"赋值失败！！", 1);
						//AfxMessageBox(L"8");
						return FALSE;
					}
					unsigned long long snint = 0;
					CString SnPre, Snlaf, lengthstr, num;
					int length = DzSN2.GetLength();
					m_IncreasingNum.GetLBText(m_IncreasingNum.GetCurSel(), num);
					lengthstr.Format(_T("%d"), length);
					SnPre = DzStrSn.Left(DzStrSn.GetLength() - DzSN2.GetLength());
					Snlaf = DzStrSn.Right(DzSN2.GetLength());
					snint = _ttoi(Snlaf) + _ttoi(num);
					Snlaf.Format(_T("%0") + lengthstr + _T("d"), snint);
					DzStrSn = SnPre + Snlaf;
					UpdateDzLDNextLdsn(DzStrSn);  //sn+1,update
				}
				else if (List2Val.GetLength() > 6&& List2Val.Find(L"Other")==-1) //拼接的数据
				{
					if (IsUpdateFlag)
					{
						InsertRelaData(buffer);
					}
					if (!GetAllRelativeData(IMEIBuff))
					{
						ShowDZLaserMsg(L"无关联数据！！", 1);
						//AfxMessageBox(L"8");
						return FALSE;
					}
					List3Val = m_list2.GetItemText(tt, 3);
					ConnChar = List3Val.Left(1);
					List3Val = List3Val.Mid(1);
					while (true)
					{
						if (List3Val.Find(ConnChar) != -1)
						{
							ConnNum = _ttoi(List3Val.Mid(0, List3Val.Find(ConnChar)));
							if (ConnNum == 1)
							{
								ConnVal += L"IMEI" + ConnChar;
							}
							else
							{
								ConnVal += RelativeDataVec.at(ConnNum) + ConnChar;
							}
						}
						else
						{
							ConnNum = _ttoi(List3Val);
							if (ConnNum == 1)
							{
								ConnVal += L"IMEI";
							}
							else
							{
								ConnVal += RelativeDataVec.at(ConnNum);
							}
							break;
						}
						List3Val = List3Val.Mid(List3Val.Find(ConnChar) + 1);
					}
					Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)List2Val, (LPTSTR)(LPCTSTR)ConnVal);
					if (Ret != TRUE)
					{
						ShowDZLaserMsg(List2Val + L"赋值失败！！", 1);
						//AfxMessageBox(L"8");
						return FALSE;
					}
				}
				else
				{
					if (List2Val.Find(L"Other") != -1)
						break;
				}

			}
		}

		if (DataRelativeChoose == TRUE)//数据关联则还有值要赋予
		{
			if (!GetAllRelativeData(InputData))
			{
				ShowDZLaserMsg(L"无关联数据！！", 1);
				//AfxMessageBox(L"8");
				return FALSE;
			}
			else
			{
				for (int i = 0; i <= 13; i++)
				{
					if (RelativeDataVec.at(i) == L"NULL")
					{
						continue;
					}
					CString DataName;
					DataName.Format(L"RelaData%d", i);
					Ret = LaserMachine.ChangeTextByName((LPTSTR)(LPCTSTR)DataName, (LPTSTR)(LPCTSTR)RelativeDataVec.at(i));
					if (Ret != TRUE)
					{
						ShowDZLaserMsg(DataName + L"赋值失败！！", 1);
						//AfxMessageBox(L"8");
						return FALSE;
					}
				}
			}
		}

		if (Ret != TRUE)
		{
			return FALSE;
		}
	}
	//AfxMessageBox(L"8");
	CString showStr;
	ShowDZLaserMsg(L"开始调用StartMark..", 0);
	Ret &= LaserMachine.StartMark(bWaitTouch, bWaitEnd, nOverTime);
	if (Ret == TRUE)
	{
		if (EmptyDataCheck == FALSE)
		{
			if (RemarkCheck)
			{
				UpdateDzLdData(InputData);
			}
			else
			{
				InsertDzLdData(InputData);
			}
		}
		showStr.Format(L"第%d次 ", FirstScan);
		ShowDZLaserMsg(showStr + L"IMEI:" + InputData + L" 镭雕成功", 0);
		writeDZTxt(L"IMEI:" + InputData + L" 镭雕成功");
		DataBuffer = InputData;
		FirstScan++;
	}
	else
	{
		showStr.Format(L"第%d次 ", FirstScan);
		ShowDZLaserMsg(showStr + L"IMEI:" + InputData + L" 镭雕失败", 1);
		writeDZTxt(L"IMEI:" + InputData + L" 镭雕失败");
		FirstScan++;
		return FALSE;
	}
	return TRUE;
}

BOOL CDZ_Laser::UpdateDzLDNextIMEI(CString IMEI)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET LdImei = '") + IMEI + L"' where Zhidan = '" + DZzhidan + L"'";
	try
	{
		ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
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

BOOL CDZ_Laser::UpdateDzLDNextLdsn(CString SN)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET ldSn = '") + SN + L"' where Zhidan = '" + DZzhidan + L"'";
	try
	{
		ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
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

BOOL CDZ_Laser::InsertRelaData(CString buffer[15][2])
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString ZiduanStrSql, ValueStr, strSql;
	CString dot = _T(",");
	CString dot1 = _T("'");
	ZiduanStrSql = _T("insert into [GPSTest].[dbo].[DataRelativeSheet] (");

	//将数据插入表中	
	for (int i = 1; i < 15; i++)
	{
		if (buffer[i][0] =="")
		{
			continue;
		}
		ZiduanStrSql += buffer[i][0] + dot;
		ValueStr += dot1 + buffer[i][1] + dot1 + dot;
	}
	ValueStr = ValueStr.Mid(0, ValueStr.GetLength() - 1);
	ZiduanStrSql = ZiduanStrSql.Mid(0, ZiduanStrSql.GetLength() - 1) + _T(") ");
	strSql = ZiduanStrSql + _T("values(") + ValueStr + _T(")");
	try
	{
		ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
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

BOOL CDZ_Laser::UpdateToSubIMEISegment(CString IMEI)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET SubIMEISegment = '") + IMEI + L"' where Zhidan = '" + DZzhidan + L"'";
	try
	{
		ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
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

BOOL CDZ_Laser::InsertDzLdData(CString IMEI)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	/*strSql = _T("insert into [GPSTest].[dbo].[LPrintMarkData] (IMEI,SoftModel,Version,ZhiDan,Computer,TestTime,TesterId,ReMarkTimes) values('") + IMEI + L"','" + m_list2.GetItemText(1, 3) + "','" + m_list2.GetItemText(2, 3) + "','"\
		+ DZzhidan + L"','" + DzIp + L"','" + autoLaserDlg.GetTime() + L"','" + TesterID +L"','"+L"1"+ L"')";*/

	strSql = _T("insert into [GPSTest].[dbo].[LPrintMarkData] (IMEI,SoftModel,Version,ZhiDan,Computer,TestTime,TesterId) values('") + IMEI + L"','" + m_list2.GetItemText(1, 3) + "','" + m_list2.GetItemText(2, 3) + "','"\
		+ DZzhidan + L"','" + DzIp + L"','" + autoLaserDlg.GetTime() + L"','" + TesterID + L"')";
	//如果执行错误就会报异常原因
	try
	{
		ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
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

BOOL CDZ_Laser::UpdateDzLdData(CString IMEI)
{
	ADOManage ado;
	ado.ConnSQL();
	ado.m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString strSql, ReMarkTimesValStr;
	int ReMarkTimesVal;
	strSql = _T("select ReMarkTimes from [GPSTest].[dbo].[LPrintMarkData] where IMEI = '") + IMEI + L"'";
	//AfxMessageBox(strSql);
	try
	{
		ado.m_pRecordSet = ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		//AfxMessageBox(L"0");
		if (!ado.m_pRecordSet->adoEOF)
		{
			//AfxMessageBox(L"1");
			if (ado.m_pRecordSet->GetCollect(L"ReMarkTimes").vt != VT_NULL)
			{
				//AfxMessageBox(L"2");
				ReMarkTimesVal = ado.m_pRecordSet->GetCollect(L"ReMarkTimes").intVal;
				ReMarkTimesValStr.Format(L"%d", ReMarkTimesVal + 1);
			}
			else
			{
				ReMarkTimesValStr = L"1";
			}
		}
	}
	catch (_com_error &e)
	{
		//AfxMessageBox(L"3");
		AfxMessageBox(e.Description());/*打印出异常原因*/
		ado.CloseAll();
		return FALSE;
	}
	//AfxMessageBox(ReMarkTimesValStr);
	strSql = _T("UPDATE [GPSTest].[dbo].[LPrintMarkData] SET TestTime = '") + autoLaserDlg.GetTime() + L"',TesterId = '" + TesterID + L"',ReMarkTimes = '" + ReMarkTimesValStr + L"' where IMEI = '" + IMEI + L"'";
	//将数据插入表中
	//strSql = _T("UPDATE [GPSTest].[dbo].[LPrintMarkData] SET TestTime = '") + autoLaserDlg.GetTime() + L"',TesterId = '" + TesterID + L"' where IMEI = '" + IMEI + L"'";
	//如果执行错误就会报异常原因
	try
	{
		ado.m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
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

void CDZ_Laser::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buttonText;
	GetDlgItemText(IDC_BUTTON15, buttonText);
	if (buttonText.Find(L"暂停") != -1)
	{
		PauseMark = TRUE;
		ShowDZLaserMsg(L"暂停中...", 1);
		GetDlgItem(IDC_BUTTON15)->SetWindowText(L"继续");
	}
	else
	{
		PauseMark = FALSE;
		GetDlgItem(IDC_BUTTON15)->SetWindowText(L"暂停");
	}
}

LRESULT CDZ_Laser::PopulateComboList2(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);
	//pInPlaceCombo->ShowDropDown();

	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft());

	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_list2.GetWindowRect(&obWindowRect);

	int iColIndex = (int)wParam;

	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll();

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft))
	{
		if (iColIndex == 1)
		{
			pComboList->AddTail(L"软件版本");
			pComboList->AddTail(L"终端机型");
			pComboList->AddTail(L"共有指令");
			pComboList->AddTail(L"白卡测试");
			pComboList->AddTail(L"IMEI域名");
		}
		//pComboList->IsEmpty();
		//pComboList->GetAt()
	}
	return true;
}

BOOL CDZ_Laser::writeDZTxt(CString value)
{
	char sFileName[256] = { 0 };
	CString ProjectPath = _T("");
	GetModuleFileName(AfxGetInstanceHandle(), LPWSTR(sFileName), 255);
	ProjectPath.Format(L"%s", sFileName);
	int pos = ProjectPath.ReverseFind('\\');
	if (pos != -1)
		ProjectPath = ProjectPath.Left(pos);
	CString TxtPath = ProjectPath + L"\\HansLaserLog\\" + L"_" + DZzhidan + L"_LaserLog.txt";
	CFile file;
	try
	{
		file.Open(TxtPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
		file.SeekToEnd(); // 移动文件指针到末尾
		BYTE byUnicode[] = { 0xFF, 0xFE };
		file.Write(byUnicode, sizeof(byUnicode));
		file.Write(value + L"\r\n", wcslen(value + L"\r\n") * sizeof(WCHAR) + 2);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
	return TRUE;
}

void CDZ_Laser::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
	Sleep(200);
	CString buttonText;
	GetDlgItemText(IDC_BUTTON16, buttonText);
	if (buttonText.Find(L"开始") != -1)
	{
		if (m_MarkType == 2)
		{
			if (!EmptyDataCheck&&_ttoi(m_TagsCount) == 0)
			{
				MessageBox(L"数据为空", L"提示");
				GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
				return;
			}
			if (m_delayTime == L"")
			{
				MessageBox(L"时间不能为空", L"提示");
				GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
				return;
			}
			ShowDZLaserMsg(L"成功获取批量数据", 0);
			StartMark = TRUE;
			MarkThread = AfxBeginThread(StartMarkThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
			GetDlgItem(IDC_BUTTON16)->SetWindowText(L"停止");
			GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON12)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON15)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON14)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
			MessageBox(L"请选择正确的打标方式--定时打标");
			return;
		}
	}
	else
	{
		StartMark = FALSE;
		GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON15)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON12)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON13)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON16)->SetWindowText(L"开始");
	}
}

void CDZ_Laser::OnEnChangeEdit9()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

}

void CDZ_Laser::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	/*CDialogEx::OnOK();*/
}

BOOL CDZ_Laser::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->GetDlgCtrlID() == IDC_EDIT9)
		{
			//AfxMessageBox(L"2");
			UpdateData(TRUE);
			if (m_MarkType == 1)
			{
				//AfxMessageBox(L"3");
				if (m_ScanData == "")
				{
					ShowDZLaserMsg(L"数据为空", 1);
					goto ERRORSCAN;
				}
				int IMEI_length = m_ScanData.GetLength();
				if (_ttoi(m_BitsCheck) != IMEI_length)
				{
					ShowDZLaserMsg(L"输入位数错误", 1);
					goto ERRORSCAN;
				}
				if (m_ScanData.Left(m_ScanDataPre.GetLength()) != m_ScanDataPre)
				{
					ShowDZLaserMsg(L"输入前缀错误", 1);
					goto ERRORSCAN;
				}
				if (m_ScanData.Left(m_StartIMEI.GetLength()) < m_StartIMEI || m_ScanData.Left(m_StartIMEI.GetLength()) > m_EndIMEI)//比较是否在号段内
				{
					ShowDZLaserMsg(L"数据在号段外", 1);
					goto ERRORSCAN;
				}
				//if (CheckIMEIData == TRUE)//校验IMEI
				//{
					/*for (int i = 0; i < IMEI_length; i++)
					{
						if (m_ScanData.GetAt(i) < '0' || m_ScanData.GetAt(i) > '9')
						{
							ShowDZLaserMsg(L"只能输入阿拉伯数字", 1);
							goto ERRORSCAN;
						}
					}*/
				if (!RemarkCheck)
				{
					if (CheckIMEIData == TRUE)
					{
						if (CheckNoUseDataInDB(m_ScanData) == 1)
						{
							ShowDZLaserMsg(L"数据重复", 1);
							goto ERRORSCAN;
						}
					}
					else
					{
						if (CheckScanDataInRela(m_list2.GetItemText(2, 2), m_ScanData) == 1)
						{
							ShowDZLaserMsg(L"数据重复", 1);
							goto ERRORSCAN;
						}
						do
						{
							if (CheckNoUseDataInDB(m_DataShow.GetItemText(DataShowNo, 1)) == 1)  //先查镭雕表
							{
								ShowDZLaserMsg(L"LPrintMark  IMEI重号，自动过滤", 1);
								DataShowNo++;
							}
							else
							{
								if (CheckScanDataInRela(L"IMEI1", m_DataShow.GetItemText(DataShowNo, 1)) == 1)//再查关联表
								{
									ShowDZLaserMsg(L"DataRelativeSheet  IMEI重号，自动过滤", 1);
									DataShowNo++;
								}
								else
								{
									break;
								}
							}
						} while (true);
						if (DataShowNo >= _ttoi(m_TagsCount))
						{
							ShowDZLaserMsg(L"IMEI超出范围", 1);
							goto ERRORSCAN;
						}
					}

				}
				else
				{
					if (DataBuffer.Find(m_ScanData) != -1)
					{
						MessageBox(L"此数据已重新镭雕过一次，想继续镭雕请点击更新缓存!", L"提示");
						goto ERRORSCAN;
					}
				}
				/*if (_ttoi64(m_ScanData.Left(m_StartIMEI.GetLength())) <  _ttoi64(m_StartIMEI) || _ttoi64(m_ScanData.Left(m_EndIMEI.GetLength())) > _ttoi64(m_EndIMEI))
				{
					ShowDZLaserMsg(L"数据不在号段内", 1);
					goto ERRORSCAN;
				}
			}	*/
				ScanRelativeData = L"y";
				if (ScanStartmarkFunc(FALSE, m_ScanData, ScanRelativeData, FALSE, TRUE, INFINITE, 0, DataShowNo))
				{
					if (!CheckIMEIData)
					{
						if (DataShowNo + 1 < _ttoi(m_TagsCount))
						{
							UpdateDzLDNextIMEI(m_DataShow.GetItemText(DataShowNo + 1, 1));
						}
						else
						{
							UpdateDzLDNextIMEI(m_DataShow.GetItemText(DataShowNo, 1));
						}
						DataShowNo++;						
					}
				}
				GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
			}
			else
			{
				AfxMessageBox(L"请先正确选择--扫描打标");
				m_ScanData = "";
				UpdateData(FALSE);
			}
		ERRORSCAN:
			m_ScanCtrl.EnableWindow(TRUE);
			m_ScanCtrl.SetWindowText(L"");
			m_ScanCtrl.SetFocus();
			m_ScanCtrl.SetSel(0, -1);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDZ_Laser::UnLockSomething(BOOL flag)
{
	GetDlgItem(IDC_BUTTON10)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK5)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO1)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK6)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO2)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT10)->EnableWindow(flag);
	GetDlgItem(IDC_RADIO3)->EnableWindow(flag);
	//GetDlgItem(IDC_LIST3)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK4)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK3)->EnableWindow(flag);//待会删除
	GetDlgItem(IDC_CHECK2)->EnableWindow(flag);
	//GetDlgItem(IDC_EDIT9)->EnableWindow(flag);
}

void CDZ_Laser::LockSomething(BOOL flag)
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT3)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT4)->EnableWindow(flag);
	GetDlgItem(IDC_COMBO2)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK1)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK7)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT7)->EnableWindow(flag);
	GetDlgItem(IDC_COMBO3)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT8)->EnableWindow(flag);
	GetDlgItem(IDC_COMBO1)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT1)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON9)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(flag);
	GetDlgItem(IDC_CHECK8)->EnableWindow(flag);
	GetDlgItem(IDC_COMBO4)->EnableWindow(flag);
	GetDlgItem(IDC_LIST3)->EnableWindow(flag);
}

void CDZ_Laser::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		DzResizeCtrl.ResizeWindow();
	}

	// TODO: 在此处添加消息处理程序代码
}

void CDZ_Laser::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//if (m_MultLD)
	//{
	//	//上传到新字段
	//	CString ValStr, PcNumVal, SubValStr;
	//	int pos, pos1;
	//	m_PcNum.GetLBText(m_PcNum.GetCurSel(), PcNumVal);
	//	ValStr = GetSubIMEISegmentVal(DZzhidan);
	//	//CutSegment(ValStr);
	//	pos = ValStr.Find(PcNumVal);
	//	if (pos != -1)
	//	{
	//		pos = ValStr.Find(L",", pos + 2);
	//		pos1 = ValStr.Find(L")", pos + 2);
	//		SubValStr = ValStr.Mid(pos + 1, pos1 - pos-1);
	//		ValStr.Replace(SubValStr, m_DataShow.GetItemText(1 + 1, 1));
	//	}
	//	else
	//	{
	//		if (ValStr==L"")
	//		{
	//			ValStr = ValStr + L"(" + PcNumVal + L"," + m_StartIMEI + L"-" + m_EndIMEI + L"," + m_DataShow.GetItemText(1 + 1, 1) + L")";
	//		}
	//		else
	//		{
	//			ValStr = ValStr + L",(" + PcNumVal + L"," + m_StartIMEI + L"-" + m_EndIMEI + L"," + m_DataShow.GetItemText(1 + 1, 1) + L")";
	//		}		
	//	}
	//	UpdateIMEISegment(DZzhidan, ValStr);
	//}
	//GetAllRelativeData(L"100");
	//AfxMessageBox(RelativeDataVec.at(1));

	//if (m_MultLDVal)
	//{
	//	//上传到新字段
	//	CString ValStr, PcNumVal, SubValStr;
	//	int pos, pos1;
	//	//dlg->m_PcNum.GetLBText(dlg->m_PcNum.GetCurSel(), PcNumVal);
	//	PcNumVal = m_PcNumVal;
	//	ValStr = GetSubIMEISegmentVal(DZzhidan);
	//	AfxMessageBox(ValStr);
	//	pos = ValStr.Find(PcNumVal);
	//	AfxMessageBox(PcNumVal);
	//	if (pos != -1)
	//	{
	//		pos = ValStr.Find(L",", pos + 2);
	//		pos1 = ValStr.Find(L")", pos + 2);
	//		SubValStr = ValStr.Mid(pos + 1, pos1 - pos - 1);
	//		ValStr.Replace(SubValStr, m_DataShow.GetItemText(FirstData + 1, 1));
	//		//ValStr.Replace(ValStr[pos], ValStr[pos+15], dlg->m_DataShow.GetItemText(dlg->FirstData + 1, 1));
	//	}
	//	else
	//	{
	//		if (ValStr == L"")
	//		{
	//			ValStr = ValStr + L"(" + PcNumVal + L"," + m_StartIMEI + L"-" +m_EndIMEI + L"," + m_DataShow.GetItemText(1 + 1, 1) + L")";
	//		}
	//		else
	//		{
	//			ValStr = ValStr + L",(" + PcNumVal + L"," + m_StartIMEI + L"-" + m_EndIMEI + L"," + m_DataShow.GetItemText(1 + 1, 1) + L")";
	//		}
	//	}
	//	AfxMessageBox(ValStr);
	//	UpdateIMEISegment(DZzhidan, ValStr);
	//}
	/*if (!CheckIMEIThreadHandel)
	{
		CheckIMEIThreadFlag = TRUE;
		CheckIMEIThreadHandel = AfxBeginThread(CheckIMEIThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}*/

}

void CDZ_Laser::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL CheckVal;
	CheckVal = m_MultLD.GetCheck();
	if (CheckVal)
	{
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	}
}

void CDZ_Laser::OnDblclkList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate != NULL)
	{
		if (pNMItemActivate->iItem != -1)
		{
			if (m_HaoDuan.GetItemText(pNMItemActivate->iItem, 0) == L"")
			{
				return;
			}
			ListCurItem = pNMItemActivate->iItem;
			LDIMEI = m_HaoDuan.GetItemText(pNMItemActivate->iItem, 3);
			m_PcNum.SetWindowTextW(m_HaoDuan.GetItemText(pNMItemActivate->iItem, 0));
			SetDlgItemText(IDC_EDIT3, m_HaoDuan.GetItemText(pNMItemActivate->iItem, 1));
			SetDlgItemText(IDC_EDIT4, m_HaoDuan.GetItemText(pNMItemActivate->iItem, 2));
			IsListClick = TRUE;
		}
		else
			return;
	}

	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDZ_Laser::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT9)->SetFocus();
}

void CDZ_Laser::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
}

void CDZ_Laser::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
}

void CDZ_Laser::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	//CheckIMEIThreadHandel = NULL;//结束线程
	//CheckIMEIThreadFlag = FALSE;
	DataBuffer = L"";
}


void CDZ_Laser::OnBnClickedButton19()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetAllRelativeData(L"006");
	CString ttCS, List2Val, List3Val, ConnVal, ConnChar;
	CString buffer[15][2];
	int ZiduanNum = 0;
	BOOL IsUpdateFlag = FALSE;
	int ConnNum;
	for (int tt = 0; tt < 48; tt++)
	{
		ConnVal = L"";
		ConnChar = L"";
		List2Val = m_list2.GetItemText(tt, 2);
		if (tt == 0 || tt == 1)
		{

		}
		else if (tt == 2)
		{
			if (List2Val != "IMEI1")//不是IMEI，需要上传关联和查重关联表
			{
				IsUpdateFlag = TRUE;
			}
			else //是IMEI，不需要上传关联和查重关联表
			{
				IsUpdateFlag = FALSE;
			}
			ZiduanNum = _ttoi(List2Val.Mid(4));
			buffer[ZiduanNum][0] = List2Val;
			buffer[ZiduanNum][1] = "扫描得到的数据";
			//将扫描得到的数据替换到镭雕模板，对象名是List2Val
		}
		else
		{
			if (List2Val == "IMEI1") //IMEI
			{
				ZiduanNum = _ttoi(List2Val.Mid(4));
				buffer[ZiduanNum][0] = List2Val;
				buffer[ZiduanNum][1] = "生成的IMEI号";
				//在此获取IMEI号，并替换到模板，对象名是List2Val
			}
			else if (List2Val == "IMEI2")  //SN
			{
				do
				{
					if (CheckScanDataInRela(List2Val, L"SN号"))
					{
						unsigned long long snint = 0;
						CString SnPre, Snlaf, lengthstr, num;
						int length = DzSN2.GetLength();
						m_IncreasingNum.GetLBText(m_IncreasingNum.GetCurSel(), num);
						lengthstr.Format(_T("%d"), length);
						SnPre = DzStrSn.Left(DzStrSn.GetLength() - DzSN2.GetLength());
						Snlaf = DzStrSn.Right(DzSN2.GetLength());
						snint = _ttoi(Snlaf) + _ttoi(num);
						Snlaf.Format(_T("%0") + lengthstr + _T("d"), snint);
						DzStrSn = SnPre + Snlaf;						
					}
					else
					{
						break;
					}

				} while (true);
				
				ZiduanNum = _ttoi(List2Val.Mid(4));
				buffer[ZiduanNum][0] = List2Val;
				buffer[ZiduanNum][1] = DzStrSn;
				//在这里获取SN号，并替换到模板，对象名是List2Val
				unsigned long long snint = 0;
				CString SnPre, Snlaf, lengthstr, num;
				int length = DzSN2.GetLength();
				m_IncreasingNum.GetLBText(m_IncreasingNum.GetCurSel(), num);
				lengthstr.Format(_T("%d"), length);
				SnPre = DzStrSn.Left(DzStrSn.GetLength() - DzSN2.GetLength());
				Snlaf = DzStrSn.Right(DzSN2.GetLength());
				snint = _ttoi(Snlaf) + _ttoi(num);
				Snlaf.Format(_T("%0") + lengthstr + _T("d"), snint);
				DzStrSn = SnPre + Snlaf;
				UpdateDzLDNextLdsn(DzStrSn);  //sn+1,update
			}
			else if (List2Val.GetLength() > 6) //拼接的数据
			{
				if (IsUpdateFlag)
				{
					InsertRelaData(buffer);
				}
				GetAllRelativeData(buffer[1][1]);
				List3Val = m_list2.GetItemText(tt, 3);
				ConnChar = List3Val.Left(1);
				List3Val = List3Val.Mid(1);
				while (true)
				{
					if (List3Val.Find(ConnChar) != -1)
					{
						ConnNum = _ttoi(List3Val.Mid(0, List3Val.Find(ConnChar)));
						if (ConnNum == 1)
						{
							ConnVal += L"IMEI" + ConnChar;
						}
						else
						{
							ConnVal += RelativeDataVec.at(ConnNum) + ConnChar;
						}
					}
					else
					{
						ConnNum = _ttoi(List3Val);
						if (ConnNum == 1)
						{
							ConnVal += L"IMEI";
						}
						else
						{
							ConnVal += RelativeDataVec.at(ConnNum);
						}
						break;
					}
					List3Val = List3Val.Mid(List3Val.Find(ConnChar) + 1);
				}
			}
			else
			{
				if (List2Val.Find(L"Other") != -1)
					break;
			}

		}
	}
}