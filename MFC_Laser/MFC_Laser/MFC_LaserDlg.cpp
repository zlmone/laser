
// MFC_LaserDlg.cpp : ʵ���ļ�
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

/*ȫ�ֱ���*/
volatile BOOL m_LdPortRun = true;//������ȫ�ֱ�����������ת�ص�
volatile BOOL m_BurningConrtolFlag; //��д���ر���
BOOL SendTestFlag = true;       //����TESTָ����Ʊ���
BOOL BurnLastSendTestFlag = true;       //��д�귢��TESTָ����Ʊ���
BOOL CompareLastSendTestFlag = true;       //�Ա��귢��TESTָ����Ʊ���
BOOL HaveCheckCode = true;     //���޼������־λ
CString CurrentImei;   //��ǰIMEI��
CString CurrentBurningImei;   //��ǰ��дIMEI��
CString CurrentLdImei; //��ǰ�ص�IMEI��
CString FImeiStart;
CString FImeiEnd; //IMEI��ʼλ ��ֹλ
CString ComputerIp;    //������Ϣ
CString zhidan;        //��ѡ��Ķ������Ʒ������ȫ�ֱ�����
CString IniTestCommand, IniTestRec, IniCheckIMEICommand, IniCheckIMEICommandRec, IniBurningCommand1;
CString IniBurningCommand1Rec, IniBurningCommand2, IniBurningCommand2Rec, IMEIIniDataDelimiter1, IMEIIniDataDelimiter2;
CString IMEIIniDataDelimiter1Length/*, IniDataDelimiter2Length*/, RIDIniDataDelimiter1, RIDIniDataDelimiter2, RIDIniDataDelimiter1Length;
CString iniCheckRIDCommand, iniCheckRIDCommandRec;
CWindowSizeMange myDlgManage;
//������λ�Ĺ���״̬��trueΪ����
BOOL BurnFinishFlag = TRUE;
BOOL LdFinishFlag = TRUE;
BOOL CompareFinishFlag = TRUE;
BOOL EnterScanFlag = FALSE;
BOOL BurnIsOpenFlag = FALSE;
BOOL LDIsOpenFlag = FALSE;
int IncreasingnumberInt;
CString IncreasingNumberStr;
//Ӳ������
CString MachineName;
CString Allversion;
CString LDversion, LDsoftmodel;

//������־
BOOL listenFlag = TRUE;
BOOL LD2ConnectFlag = FALSE;
CString readFromClient = L"";
BOOL readFromClientFlag = TRUE;
int iRet;
//ADOManage adomanageCompare;

//�ⲿ����
extern BOOL RelativeIMEI2, RelativeIMEI3, RelativeIMEI4, RelativeIMEI5, RelativeIMEI6, RelativeIMEI7, RelativeIMEI8, RelativeIMEI9\
, RelativeIMEI10, RelativeIMEI11;
extern BOOL RelativeIMEI12, RelativeIMEI13;

//������λ��IMEI���ݱ���
CString BurnToLd, LdToCompare;

//������λ��ʶ
unsigned long long Control[3][3] = { { 0, 0, 0 }, { 1, 1, 1 }, { 1, 1, 1 } };

//����txt�е�IMEI
vector<CString> TxTResult;
vector<CString> AutoRelativeDataVec;
//����Station�ֶ�����
vector<CString> StationValVector;

//�Աȴ����߳̿��Ʊ���
volatile BOOL m_CompareConrtolFlag; //��д���ر���
BOOL CompareSendTestFlag = true;       //����TESTָ����Ʊ���

CMFC_LaserDlg * parent;

//��
CRITICAL_SECTION cs;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFC_LaserDlg �Ի���



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

	//�Ƶ���ȫ�ֱ���
	ZhiDanNO = _T("");

	//IMEIȫ�ֱ���
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


// CMFC_LaserDlg ��Ϣ�������

BOOL CMFC_LaserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ��������Ϣ
	GetLocalHostIPName(m_pcnameEdit, m_pcipEdit);//���ú������IP�ͼ��������
	SetDlgItemText(IDC_LocalPcName_EDIT, m_pcipEdit);//���������������ʾ�����ƿؼ���
	SetDlgItemText(IDC_LocalPcIP_EDIT, m_pcnameEdit);//���������IP��ʾ��IP�ؼ���
	//��ȡ�Ƶ��ŵ�������
	FillZhidanToComBo();

	parent = this;

	//��ʼ�������б�
	FindCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);
	FindCommPort((CComboBox*)GetDlgItem(IDC_ComparePort_COMBO), Port2No);
	FindCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);

	//��ʼ�������С
	fontinit();

	SetDlgItemText(IDC_SendCommand1_EDIT, L"AT*MNVMQ=\"1.0\"");
	SetDlgItemText(IDC_SendCommand2_EDIT, L"AT*MCGSN=1,\"imei\"");
	SetDlgItemText(IDC_ReciveCommand_EDIT, L"OK");

	/*��ȡ�ؼ�λ��*/
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
	//��ձ�ͷ
	while (m_AutoList.DeleteColumn(0));
	//���ֶ���ʾ����
	m_AutoList.InsertColumn(0, L"No", LVCFMT_CENTER, 30);
	m_AutoList.InsertColumn(1, L"������Ŀ", LVCFMT_LEFT, 65);
	m_AutoList.InsertColumn(2, L"���ñ�����", LVCFMT_LEFT, 100);
	m_AutoList.InsertColumn(3, L"���ñ���", LVCFMT_LEFT, 230);
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
			m_AutoList.SetItemText(i, 1, L"����");
			m_AutoList.SetItemText(i, 2, L"Model");
		}
		else if (i == 1)
		{
			m_AutoList.SetItemText(i, 1, L"����汾");
			m_AutoList.SetItemText(i, 2, L"Version");
		}
		else
		{
			m_AutoList.SetItemText(i, 1, L"");
			m_AutoList.SetItemText(i, 2, L"Other" + str);
		}
	}
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_LaserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_LaserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//���水��Enter���󲻻��˳�
void CMFC_LaserDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}

//�رճ���ʱ
void CMFC_LaserDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	INT_PTR nRes;
	nRes = MessageBox(_T("��ȷ��Ҫ�˳���"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;
	CDialogEx::OnClose();
}

/*��ʼ������ģ��*/
//���ݿ����ð�ť
void CMFC_LaserDlg::OnBnClickedDbconfigButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ 
	DbConfig dbconfigDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = dbconfigDlg.DoModal();  // �����Ի���
	if (IDCANCEL == nRes)
		return;
}

//��䶩���ŵ�������
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

//ˢ���Ƶ�
void CMFC_LaserDlg::OnBnClickedRefreshzhidanButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_ImeiStart_EDIT, L"");
	SetDlgItemText(IDC_ImeiEnd_EDIT, L"");
	SetDlgItemText(IDC_ImeiCurrent_EDIT, L"");
	SetDlgItemText(IDC_LdTemplate_EDIT, L"");
	ZhiDanCombo.ResetContent();
	FillZhidanToComBo();
	SetRicheditText(L"ˢ�³ɹ���������ѡ�񶩵�", 0);
}

//ѡ�񶩵��Ŵ����¼�
void CMFC_LaserDlg::OnSelchangeZhidanCombo()
{
	UpdateData(TRUE);
	try {
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		_variant_t imeitemp;//������getcollect������
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
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
}

//ѡ��ģ�尴ť
void CMFC_LaserDlg::OnBnClickedChoosetemplateButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("�ص�ģ���ļ�(*.ezd)|*.ezd|�����ļ�(*.*)|*.*||");
	//������ļ��Ի���
	CFileDialog fileDlg(TRUE, _T("ezd"), NULL, 0, szFilter, this);

	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_LdTemplate_EDIT, strFilePath);
	}
}

//��ģ�庯��
void CMFC_LaserDlg::OnBnClickedOpentemplateButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;

	GetDlgItemText(IDC_LdTemplate_EDIT, strFilePath);
	if (strFilePath == "") {
		MessageBox(_T("����ѡ��ģ��"), _T("��ʾ"), NULL);
		return;
	}
	ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOWNORMAL);
}

//��ȡ����IP�͵�ַ
int CMFC_LaserDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
{
	char szLocalName[256];//����
	WSADATA wsaData;//�����׽��ִ洢����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)//<span class="con">windows��ʼ��socket����⣬����2��0�İ汾��windows socket��̱�<img class="selectsearch-hide" id="selectsearch-icon" alt="����" src="http://img.baidu.com/img/iknow/qb/select-search.png" />���ȳ�ʼ�����������,�򷵻�0</span>
	{
		if (gethostname(szLocalName, sizeof(szLocalName)) != 0)//��������
		{
			sLocalName = _T("û��ȡ��");
			WSACleanup();
			return GetLastError();
		}
		else
		{
			sLocalName = szLocalName;//��sLocalName�����洢��õļ��������
			struct hostent FAR * lpHostEnt = gethostbyname(CStringA(sLocalName));
			if (lpHostEnt == NULL)//����
			{
				sIpAddress = _T("");
				WSACleanup();
				return GetLastError();
			}
			LPSTR lpAddr = lpHostEnt->h_addr_list[0];//��ȡIP��ַ
			if (lpAddr)
			{
				struct in_addr inAddr;
				memmove(&inAddr, lpAddr, 4);
				sIpAddress = inet_ntoa(inAddr);//ת��Ϊ��׼��ʽ
				if (sIpAddress.IsEmpty())
				{
					sIpAddress = _T("û��ȡ��");
					WSACleanup();
					return 1;
				}
			}
		}
	}
	WSACleanup();//����׽��ֳ�ʼ������
	return 0;
}

//������ť
void CMFC_LaserDlg::OnBnClickedTolockButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UpdateWindow();
	CString LockText;
	CString ZhuanHuanChar;
	GetDlgItem(IDC_ToLock_BUTTON)->GetWindowText(LockText);
	if (LockText == "����") {
		//�ж��Ƶ�ѡ����û��
		GetDlgItemText(IDC_ZhiDan_COMBO, zhidan);
		if (zhidan == "") {
			MessageBox(_T("����ѡ�񶩵�"), _T("��ʾ"), NULL);
			return;
		}
		//�ж�ѡ��ģ����û
		CString strFilePath;
		GetDlgItemText(IDC_LdTemplate_EDIT, strFilePath);
		if (strFilePath == "") {
			MessageBox(_T("����ѡ��ģ��"), _T("��ʾ"), NULL);
			return;
		}
		//��¼��ǰSN�ı���
		CString SnCurrent;
		GetDlgItemText(IDC_SnCurrent_EDIT, SnCurrent);
		//�жϵ�ǰλ�ڲ��ڷ�Χ��
		CString  ImeiCurrent;
		GetDlgItemText(IDC_ImeiStart_EDIT, FImeiStart);
		GetDlgItemText(IDC_ImeiEnd_EDIT, FImeiEnd);
		GetDlgItemText(IDC_ImeiCurrent_EDIT, ImeiCurrent);
		//�ж�IMEIλ��
		/*if (HaveCheckCode){
			if (ImeiCurrent.GetLength() != 14){
				MessageBox(_T("IMEI�ű���Ϊ14λ"), _T("��ʾ"), NULL);
				return;
			}
		}
		else
		{
			if (ImeiCurrent.GetLength() != 15){
				MessageBox(_T("IMEI�ű���Ϊ15λ"), _T("��ʾ"), NULL);
				return;
			}
		}*/
		/*	if (ImeiCurrent.GetLength() != FImeiStart.GetLength()){
				MessageBox(_T("IMEI��ǰλ���ڷ�Χ��"), _T("��ʾ"), NULL);
				return;
			}
			if (ImeiCurrent<FImeiStart || ImeiCurrent>FImeiEnd){
				MessageBox(_T("IMEI��ǰλ���ڷ�Χ��"), _T("��ʾ"), NULL);
				return;
			}*/
			//��������Ϣ����ȫ�ֱ�����
		int NumSel;
		NumSel = m_IncreasingNumber.GetCurSel();
		m_IncreasingNumber.GetLBText(NumSel, IncreasingNumberStr);
		IncreasingnumberInt = _ttoi(IncreasingNumberStr);
		CString localIP, localname;
		GetDlgItemText(IDC_LocalPcIP_EDIT, localIP);
		GetDlgItemText(IDC_LocalPcName_EDIT, localname);
		ComputerIp = localIP + "," + localname;
		//����д���ص�IMEI����ȫ�ֱ���
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

		SetDlgItemText(IDC_ToLock_BUTTON, TEXT("����"));
		ADOManage adomanage;
		adomanage.ConnSQL();
		adomanage.InsetrLdRecordParam(zhidan, strFilePath, ImeiCurrent, checkcodeflag, SnCurrent, LdImei);
		adomanage.GetSoftModelAndVersion(LDsoftmodel, LDversion, zhidan);
		adomanage.CloseAll();
	}
	else
	{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		CString BurningButton, LdButton, CompareButton, ScanningGunButton;
		GetDlgItemText(IDC_BurningConn_BUTTON, BurningButton);
		GetDlgItemText(IDC_OpenLdSystem_BUTTON, LdButton);
		GetDlgItemText(IDC_CompareConn_BUTTON, CompareButton);
		GetDlgItemText(IDC_BurningScanningGun_Button, ScanningGunButton);
		if (BurningButton == "һ���ر�") {
			MessageBox(_T("���ȹر���д����"), _T("��ʾ"), NULL);
			return;
		}
		if (LdButton == "�ر��ص�ϵͳ") {
			MessageBox(_T("���ȹر��ص�ϵͳ"), _T("��ʾ"), NULL);
			return;
		}
		if (CompareButton == "һ���ر�") {
			MessageBox(_T("���ȹرնԱȴ���"), _T("��ʾ"), NULL);
			return;
		}
		if (ScanningGunButton == "�ر�ɨ��ǹģʽ") {
			MessageBox(_T("���ȹر�ɨ��ǹģʽ"), _T("��ʾ"), NULL);
			return;
		}
		INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ 
		UnLock unlockDlg;           // ����Ի�����CTipDlg��ʵ��   
		nRes = unlockDlg.DoModal();  // �����Ի���
		if (IDCANCEL == nRes)
			return;
	}
}



//����������
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

	SetDlgItemText(IDC_ToLock_BUTTON, TEXT("����"));
}

//�����У��λ��ѡ��
void CMFC_LaserDlg::OnBnClickedNocheckcodeCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!((CButton *)GetDlgItem(IDC_NoCheckCode_CHECK))->GetCheck())
	{
		HaveCheckCode = true;
	}
	else
	{
		HaveCheckCode = false;
	}
}


/*��Ϣ��־ģ��*/
//����ֵ������Ϣ��־����ʾ��ͬ����Ϣ
void CMFC_LaserDlg::SetRicheditText(CString strMsg, int No)
{
	m_currentRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

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
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	if (strMsg == "127.0.0.1>>������Ϣ")
	{
	cf.crTextColor = RGB(255, 0, 0);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf); //�����richedit��ר�÷�����richedit��������Ϊ��־���
	m_currenttcp1RichEdit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("����\r\n"));
	}
	cf.crTextColor = RGB(255, 200, 100);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf);*/
}

//��ȡϵͳʱ��
CString CMFC_LaserDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}


/*���ڳ�ʼ��*/
//��ʼ������
HANDLE CMFC_LaserDlg::InitCom(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"���ڳ�ʼ��ʧ�ܣ�", 1);
		//MessageBox(L"���ڳ�ʼ��ʧ�ܣ�", L"��ʾ��Ϣ", NULL);
		return NULL;
	}
	SetupComm(hCom, 100, 100); //���뻺����������������Ĵ�С����100
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 115200; //������
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

//��ʼ������
HANDLE CMFC_LaserDlg::InitCom1(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"���ڳ�ʼ��ʧ�ܣ�", 1);
		//MessageBox(L"���ڳ�ʼ��ʧ�ܣ�", L"��ʾ��Ϣ", NULL);
		return NULL;
	}
	SetupComm(hCom, 100, 100); //���뻺����������������Ĵ�С����100
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600; //������Ϊ9600
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

//��ʼ����PLCͨ�ŵĴ���
HANDLE CMFC_LaserDlg::InitCom2(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"���ڳ�ʼ��ʧ�ܣ�", 1);
		//MessageBox(L"���ڳ�ʼ��ʧ�ܣ�", L"��ʾ��Ϣ", NULL);
		return NULL;
	}
	SetupComm(hCom, 1024, 1024); //���뻺����������������Ĵ�С����100
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600; //������Ϊ9600
	dcb.ByteSize = 7; //ÿ���ֽ���8λ
	dcb.Parity = EVENPARITY; //żУ��λ
	dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

//�رմ���
int CMFC_LaserDlg::CloseCom(HANDLE handler)
{
	return CloseHandle(handler);
}



/************************************�ص�ϵͳģ��*********************************/
//���ص�ϵͳ
void CMFC_LaserDlg::OnBnClickedOpenldsystemButton()
{
	CString LaserSystemTest;
	GetDlgItem(IDC_OpenLdSystem_BUTTON)->GetWindowText(LaserSystemTest);
	if (LaserSystemTest == "���ص�ϵͳ") {
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		if (NULL == m_pCLDStartDlg)
		{
			m_pCLDStartDlg = new LDStartDlg();
			m_pCLDStartDlg->Create(IDD_LDSTART_DIALOG, this);
		}
		m_pCLDStartDlg->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_OpenLdSystem_BUTTON)->EnableWindow(false);
		//WindowTimeClose(L"�ص�ϵͳ���������Ե�...", 100);

		//���ܹ�
		//DllFlag = markezd.lmc1_Initial(L"F:\\��������\\��ά��\\EzCad", TRUE, AfxGetMainWnd()->m_hWnd);
		DllFlag = markezd.lmc1_Initial(L"", FALSE, AfxGetMainWnd()->m_hWnd);
		//������Ϻ�رյ������Ǹ�ģ̬�Ի���
		CWnd* hWnd = FindWindow(NULL, _T("��ʾ��Ϣ"));
		if (hWnd)
		{
			hWnd->PostMessage(WM_CLOSE, NULL, NULL);
		}
		DllStr.Format(_T("%d"), DllFlag);

		if (DllFlag == 0)
		{
			SetRicheditText(L"�ص�ϵͳ�����ɹ�", 0);
			WritetoTxt(L"Laser_" + zhidan + _T("_"), L"�ص�ϵͳ�����ɹ�\r\n");
		}
		else
		{
			SetRicheditText(L"�ص�ϵͳ����ʧ�ܣ��������" + DllStr, 1);
			WritetoTxt(L"Laser_" + zhidan + _T("_"), L"�ص�ϵͳ����ʧ�ܣ��������" + DllStr + L"\r\n");
			return;
		}
		//���ص�ģ��
		CString Templatepath;
		GetDlgItemText(IDC_LdTemplate_EDIT, Templatepath);
		DllFlag = markezd.lmc1_LoadEzdFile((LPTSTR)(LPCTSTR)(Templatepath));//��CStringת����TCHAR*���ͣ�Ҫת����
		DllStr.Format(_T("%d"), DllFlag);
		if (DllFlag == 0)
		{
			SetRicheditText(L"�ص�ģ���ȡ�ɹ�", 0);
			WritetoTxt(L"Laser_" + zhidan + _T("_"), L"�ص�ģ���ȡ�ɹ�\r\n");
		}
		else
		{
			SetRicheditText(L"�ص�ģ���ȡʧ��,����ģ��·���Ƿ����󣬴������" + DllStr, 1);
			CloseLaserSystem();
			return;
		}
		//����ص�ģ�����Ƿ����ָ���������ƣ��˺����ҵõ��������ƻ᷵��0���Ҳ����ͷ���1��ֱ�ӹر��ص�ϵͳ
		if (FindLdTemplateObject())
		{
			CloseLaserSystem();
			return;
		}
		SetDlgItemText(IDC_OpenLdSystem_BUTTON, TEXT("�ر��ص�ϵͳ"));
		GetDlgItem(IDC_LdOnceTime_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(true);
	}
	else
	{
		CloseLaserSystem();
	}
}

//�ر��ص�ϵͳ
void CMFC_LaserDlg::CloseLaserSystem() {
	DllFlag = markezd.lmc1_Close();

	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"�ر��ص�ϵͳ�ɹ�", 0);
	}
	else
	{
		SetRicheditText(L"�ر��ص�ϵͳʧ�ܣ��������" + DllStr, 1);
		return;
	}
	SetDlgItemText(IDC_OpenLdSystem_BUTTON, TEXT("���ص�ϵͳ"));
	GetDlgItem(IDC_LdOnceTime_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(false);
}

//Ѱ���ص�ģ���������
int CMFC_LaserDlg::FindLdTemplateObject()
{
	double a, b, c, d, e;
	CString ObIMEI = _T("IMEI");
	CString ObSN = _T("SN");
	//һ��һ��ȥ���Ƿ���ڶ������ƣ��Ҳ����ͷ���1���ҵõ��ͷ���0
	DllFlag = markezd.lmc1_GetEntSize((LPTSTR)(LPCTSTR)ObIMEI, a, b, c, d, e);
	if (DllFlag != 0)
	{
		DllStr.Format(_T("%d"), DllFlag);
		SetRicheditText(L"�ص�ϵͳ����ʧ�ܣ��������" + DllStr, 1);
		MessageBox(L"�Ҳ���IMEI����", _T("������Ϣ"), NULL);
		return 1;
	}
	/*DllFlag = markezd.lmc1_GetEntSize((LPTSTR)(LPCTSTR)ObSN, a, b, c, d, e);
	if (DllFlag != 0)
	{
		DllStr.Format(_T("%d"), DllFlag);
		SetRicheditText(L"�ص�ϵͳ����ʧ�ܣ��������" + DllStr, 1);
		MessageBox(L"�Ҳ���SN����", _T("������Ϣ"), NULL);
		return 1;
	}*/
	return 0;
}

//����IMEIУ��λ
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

//������N������Զ��ر�
void CMFC_LaserDlg::WindowTimeClose(CString str, int strtime)
{
	SetRicheditText(str, 1);
	SetTimer(1, strtime, NULL);//������ʱ��1,��ʱʱ����1.5��
	MessageBox(str + L"(�˵�������Զ��ر�)", L"��ʾ��Ϣ", NULL);
}

//��ʱ������
void CMFC_LaserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd* hWnd = FindWindow(NULL, _T("��ʾ��Ϣ"));
	if (hWnd)
	{
		hWnd->PostMessage(WM_CLOSE, NULL, NULL);
	}
	KillTimer(1);   //�رն�ʱ��
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
		//ShowDZLaserMsg(L"���ݿ��޹�������",1);
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

//��ʼ�ص���
int CMFC_LaserDlg::LdCore()
{
	UpdateData(TRUE);
	int sendSucceedFlag;
	CString StrImeiLd, StrSnLd; //�ص��õı���
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
			SetRicheditText(_T("�ص���������[ size:") + SizeStr + _T(" , LdIndex:") + LdIndexStr + _T("]"), 1);
			SendToPlc(8);
			SendToPlc(80);
			return 0;
		}
		SetDlgItemText(IDC_LdImei_EDIT, BurnVec[LdIndex]);
		UpdateWindow();
	}
	//���ж�IMEI�Ŷ��Ƿ��Ѿ�ȫ���ص���
	if (m_LdRandom)
	{
		if (TxTResult.empty())
		{
			SetRicheditText(_T("��TXT�ļ��е�IMEI��ȫ���ص����"), 0);
			MessageBox(L"��TXT�ļ��е�IMEI��ȫ���ص���ϣ�", L"��ʾ��Ϣ", NULL);
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
		SetRicheditText(_T("IMEI��") + CurrentLdImei + _T("�ںŶ��⣬�����Ƿ����ص���˺Ŷ�"), 1);
		MessageBox(L"����IMEI�Ŷ���ȫ���ص���ϣ�", L"��ʾ��Ϣ", NULL);
		LdFinishFlag = TRUE;
		SendToPlc(8);
		SendToPlc(80);
		return 0;
	}
	if (m_RelativeSN)//�ص����Snʱ���ж��Ƿ񳬳��Ŷ�
	{
		CString SnPre, Snlaf, endSN, justSN;
		GetDlgItemText(IDC_LdSn_EDIT, SnPre);
		justSN = SnPre.Right(SN2.GetLength());
		if (justSN > SN3 || justSN < SN2)
		{
			SetRicheditText(_T("��SN���ںŶ��⣬�����Ƿ����ص��걾�Ŷ�"), 1);
			MessageBox(L"����SN�Ŷ���ȫ���ص���ϣ�", L"��ʾ��Ϣ", NULL);
			LdFinishFlag = TRUE;
			SendToPlc(8);
			SendToPlc(80);
			return 0;
		}
	}
	//����У��λҪ���ֿ�
	if (!m_NoCheckCodeCheckControl.GetCheck())
	{
		StrImeiLd = CreateIMEI15(CurrentLdImei);
	}
	else
	{
		StrImeiLd = CurrentLdImei;
	}
	if (m_ReLD)  //���ش�
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
				SetRicheditText(L"��IMEI��" + StrImeiLd + L"���ص�����Զ�����", 1);
				UpdateWindow();
				WritetoTxt(L"Laser_" + zhidan + _T("_"), L"��IMEI��" + StrImeiLd + L"���ص��\r\n");
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
				SetRicheditText(L"��IMEI��" + StrImeiLd + L"���ص�����Զ�����", 1);
				UpdateWindow();
				WritetoTxt(L"Laser_" + zhidan + _T("_"), L"��IMEI��" + StrImeiLd + L"���ص��\r\n");
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
		SetRicheditText(L"�ص�ʧ��", 1);
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
			SetRicheditText(L"�ص�ʧ��", 1);
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
			SetRicheditText(L"�޹������ݣ���", 1);
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
				SetRicheditText(L"�滻����IMEI2ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI3ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI4ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI5ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI6ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI7ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI8ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI9ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI10ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI11ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI12ʱ���ִ����ص�ʧ��", 1);
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
				SetRicheditText(L"�滻����IMEI13ʱ���ִ����ص�ʧ��", 1);
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
					SetRicheditText(L"�ص�ʧ��", 1);
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
					SetRicheditText(L"�ص�ʧ��", 1);
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
	SetRicheditText(L"�ص��С�����", 0);
	char szIpAdd[256];
	USES_CONVERSION; //��������ʹ��T2A
	sprintf_s(szIpAdd, 256, "%s", T2A(StrImeiLd));
	send(clientSock, szIpAdd, 256, 0);
	DllFlag = markezd.lmc1_Mark(TRUE);
	DllStr.Format(_T("%d"), DllFlag);
	if (DllFlag == 0)
	{
		SetRicheditText(L"IMEI��:" + StrImeiLd + L"���ص����", 0);
		WritetoTxt(L"Laser_" + zhidan + _T("_"), L"IMEI��:" + StrImeiLd + L"���ص����\r\n");
		//adomanage.UpdateLdData(StrImeiLd, StrSnLd);����һ��
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
			//						//�ص����¼���ݺ�IMEI��+1
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
		if (m_RelativeSN)//�ص����SN��+1
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
		SetRicheditText(L"�ص�ʧ�ܣ��������" + DllStr, 1);
		WritetoTxt(L"Laser_" + zhidan + _T("_"), L"�ص�ʧ�ܣ��������" + DllStr + L"\r\n");
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

//�滻ָ���������Ƶ�ֵ
int CMFC_LaserDlg::ChangeLdName(CString LdVariable, CString strld)
{
	DllFlag = markezd.lmc1_ChangeTextByName((LPTSTR)(LPCTSTR)LdVariable, (LPTSTR)(LPCTSTR)strld);
	DllStr.Format(_T("%d"), DllFlag);
	if (DllFlag == 0)
	{

	}
	else
	{
		SetRicheditText(L"�滻����" + LdVariable + L"��ֵʧ�ܣ��������" + DllStr, 1);
		WritetoTxt(L"Laser_" + zhidan + _T("_"), L"�滻�����ֵʧ�ܣ��������" + DllStr + L"\r\n");
		return 0;
	}
	return 1;
}

//�ص�һ��
void CMFC_LaserDlg::OnBnClickedLdoncetimeButton()
{

	LdCore();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//��ת�ص�ģʽ
void CMFC_LaserDlg::OnBnClickedStartlaserButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString buttontext;
	GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(FALSE);
	Sleep(200);
	GetDlgItemText(IDC_StartLaser_BUTTON, buttontext);
	if (buttontext == "������ת�ص�ģʽ")
	{
		GetDlgItem(IDC_LdOnceTime_BUTTON)->EnableWindow(FALSE);
		SetDlgItemText(IDC_StartLaser_BUTTON, _T("�ر���ת�ص�ģʽ"));
		//�������߳�
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
		SetDlgItemText(IDC_StartLaser_BUTTON, _T("������ת�ص�ģʽ"));
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPENRELAY)->EnableWindow(TRUE);
		m_LdPortRun = false;
		LDIsOpenFlag = FALSE;
		ThreadReadLdPort = NULL;
		return;
	}
}

//��ת�ص��߳�
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
			//Sleep(200);//ֹͣ0.5��Ȼ�������
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

//PLC����
CString CMFC_LaserDlg::ReadFromPLCPort()
{
	//���ڱ���
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
		/*0 - 9 ��ʮ����*/
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

/*�򿪼̵�������*/
void CMFC_LaserDlg::OnBnClickedOpenrelay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString MyConnButtonText;
	GetCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);
	GetDlgItem(IDC_OPENRELAY)->GetWindowText(MyConnButtonText);
	if (MyConnButtonText == L"��PLC����")
	{

		Port3handler = InitCom2(Port3No);
		if (Port3handler == NULL)
		{
			SetRicheditText(L"��PLC����ʧ��", 1);
			return;
		}
		SetRicheditText(L"PLC���ڳ�ʼ���ɹ�", 0);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(false);
		GetDlgItem(IDC_OPENRELAY)->SetWindowText(L"�ر�PLC����");
	}
	else
	{
		if (!CloseCom(Port3handler))
		{
			SetRicheditText(L"�رռ�PLC����ʧ��", 1);
			MessageBox(L"�ر�PLC����ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}
		SetRicheditText(L"�ر�PLC���ڳɹ�", 0);
		GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(true);
		GetDlgItem(IDC_OPENRELAY)->SetWindowText(L"��PLC����");
	}

	//ShowBurningMsg(L"�̵������ڳ�ʼ���ɹ�!");
}

/*�Ƿ�˫���ص�*/
void CMFC_LaserDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

/*tcpServer�����߳�*/
UINT listenThread(LPVOID lparam)
{
	CMFC_LaserDlg * pServer = (CMFC_LaserDlg *)lparam;;
	CString ipstr;
	int port;
	if (INVALID_SOCKET == (pServer->m_SockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox(_T("����socketʧ��"));
		return 0;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	pServer->GetDlgItemText(IDC_LocalPcName_EDIT, ipstr);
	port = pServer->GetDlgItemInt(ID_PORT);
	char szIpAdd[32];
	USES_CONVERSION; //��������ʹ��T2A
	sprintf_s(szIpAdd, 32, "%s", T2A(ipstr));
	service.sin_addr.S_un.S_addr = inet_addr(szIpAdd);
	//service.sin_addr.s_addr = inet_addr(cstr);
	//inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);
	service.sin_port = htons(port);
	if (0 != bind(pServer->m_SockListen, (sockaddr *)&service, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("�󶨶˿�ʧ��"));
		return 0;
	}
	if (0 != listen(pServer->m_SockListen, 5))
	{
		AfxMessageBox(_T("�����˿�ʧ��"));
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
			//pServer->MessageBox(L"�ص�������ӳɹ�");
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
			readFromClient = A2T(szRev); //���ĳ������룬Ӣ������
			dlg->SetRicheditText(readFromClient, 0);
			//dlg->SetDlgItemTextW(IDC_LocalPcName_EDIT, readFromClient);

		}
	}
	return 0;
}

//�����ص�
void CMFC_LaserDlg::OnBnClickedReld()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//���ù���
void CMFC_LaserDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetRelative SetRelativeDlg;
	INT_PTR res = SetRelativeDlg.DoModal();
}

//�����ĵ�
void CMFC_LaserDlg::OnBnClickedLodatxt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString   FilePathName, txtValue;//�ļ�����������
	CStdioFile file;
	CFileDialog  Dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"TXT Files(*.txt)|*.txt|All Files(*.*)|*.*", this, NULL, TRUE);
	//���ļ�
	if (Dlg.DoModal() == IDOK)//�Ƿ�򿪳ɹ�
	{
		FilePathName = Dlg.GetPathName();//ȡ���ļ�·�����ļ���
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
	MessageBox(L"����ɹ�", L"��ʾ");
	file.Close();
}


//�ص񴮿�
void CMFC_LaserDlg::OnDropdownJdqCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FindCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);
}



/*****************************��дIMEIģ��******************************/
//��ע�����Ѱ���Ѿ�ע�ᴮ�ں�Ȼ��չʾ����
void CMFC_LaserDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo)
{
	//�Ȼ�ȡ��ǰ���ں�
	int cur = 0;
	int nSel;
	BOOL curflag = TRUE;
	nSel = pComboBox->GetCurSel();
	if (nSel >= 0)
	{
		pComboBox->GetLBText(nSel, ComNo);
	}

	//�����һ���б�
	pComboBox->ResetContent();

	HKEY hKey;
#ifdef _DEBUG
	ASSERT(pComboBox != NULL);
	pComboBox->AssertValid();
#endif
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // �򿪴���ע���
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPWSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // ö�ٴ���
			{
				break;
			}
			//if (nSel >= 0 && ComNo == LPWSTR(commName))//������ϴ�ѡ�������ǾͲ��õ�ǰѡ��䶯
			//{
			//	/*cur = i - PortNO+1;*/
			//	cur = i;
			//	curflag = FALSE;
			//}
			////���������ǾͰ�˳���Զ�����һ�£����һ����־λʹ�ø������ڲ����cur�ظ���ֵ
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
			//SetRicheditText(L"�Ҳ������ڣ����⴮���豸�Ƿ�������⣡", 1);
			//MessageBox(L"�Ҳ������ڣ����⴮���豸�Ƿ�������⣡", L"��ʾ��Ϣ", NULL);
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//�������������ʱˢ��ע�����
void CMFC_LaserDlg::OnDropdownBurningportCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	FindCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);
}

//��ȡ��ǰ���ں�
void CMFC_LaserDlg::GetCommPort(CComboBox *pComboBox, CString &ComNo)
{
	int nSel;
	nSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel, ComNo);

	if (ComNo == "")
	{
		MessageBox(L"����ѡ�񴮿ںţ�", L"��ʾ��Ϣ", NULL);
	}
}

//һ�����Ӱ�ť
void CMFC_LaserDlg::OnBnClickedBurningconnButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//if (IniTestCommand.IsEmpty())
	//{
	//	MessageBox(L"���Ȼ�ȡ����ָ��",L"��ʾ");
	//	return;
	//}
	UpdateData(TRUE);
	GetCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);//�Ȼ�ȡ��ǰ���ں�
	//GetCommPort((CComboBox*)GetDlgItem(IDC_JDQ_COMBO), Port3No);//�Ȼ�ȡ��ǰ���ں�
	//if (Port1No == Port3No){
	//	MessageBox(L"��ѡ����ȷ����д�ͼ̵�������", L"��ʾ��Ϣ", NULL);
	//	return;
	//}
	CString ConnButtonText;
	GetDlgItem(IDC_BurningConn_BUTTON)->GetWindowText(ConnButtonText);
	if (ConnButtonText == "һ����ʼ")
	{
		//�жϴ�����û�п����ɹ�
		if (StationValVector.size() == 0)
		{
			MessageBox(_T("��ȡָ��ʧ�ܣ������Ƿ����ϴ�ָ����������"), _T("��ʾ"), NULL);
			return;
		}
		Port1handler = InitCom(Port1No);
		if (Port1handler == NULL)
		{
			ShowBurningMsg(L"�򿪴���ʧ��", 1);
			return;
		}
		ShowBurningMsg(L"��д���ڳ�ʼ���ɹ�!", 0);
		BurnIsOpenFlag = TRUE;
		WritetoTxt(L"Burn_" + zhidan + _T("_"), L"����д���ڳɹ�\r\n");
		//�򿪼̵�������
		/*Port3handler = InitCom1(Port3No);
		if (Port3handler == NULL)
		{
			ShowBurningMsg(L"�򿪼̵�������ʧ��");
			return;
		}
		ShowBurningMsg(L"�̵������ڳ�ʼ���ɹ�!");	*/
		int MyDataCount = 0;
		MyDataCount = _ttoi64(m_EndIMEIVal) - _ttoi64(m_BurningEditVal);
		WriteThread = AfxBeginThread(SendDBCommand, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_BurningConrtolFlag = true;
		BurnLastSendTestFlag = true;
		SetDlgItemText(IDC_BurningConn_BUTTON, TEXT("һ���ر�"));
		SetDlgItemText(IDC_BurningRemind_STATIC, L"����");
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(false);

		//GetDlgItem(IDC_BurningScanningGun_Button)->EnableWindow(false);
		GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(false);
		//GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(false);
	}
	else
	{
		if (!CloseCom(Port1handler))
		{
			ShowBurningMsg(L"�رմ���ʧ��", 1);
			MessageBox(L"�رմ���1ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}
		ShowBurningMsg(L"�ر���д���ڳɹ�", 0);
		BurnIsOpenFlag = FALSE;
		WritetoTxt(L"Burn_" + zhidan + _T("_"), L"�ر���д���ڳɹ�\r\n");
		//�رռ̵�������
		/*if (!CloseCom(Port3handler))
		{
			ShowBurningMsg(L"�رռ̵�������ʧ��");
			MessageBox(L"�رռ̵�������ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}
		ShowBurningMsg(L"�رռ̵������ڳɹ�");*/
		m_BurningConrtolFlag = false; //ֹͣ�߳�
		BurnLastSendTestFlag = false;
		//SendTestFlag = false;
		Sleep(2000);
		SetDlgItemText(IDC_BurningConn_BUTTON, TEXT("һ����ʼ"));
		SetDlgItemText(IDC_BurningRemind_STATIC, L"δ����");
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(true);
		//GetDlgItem(IDC_BurningScanningGun_Button)->EnableWindow(true);
		GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(true);
		//GetDlgItem(IDC_JDQ_COMBO)->EnableWindow(true);
	}
}

//��ʾ��д��־
int CMFC_LaserDlg::ShowBurningMsg(CString strMsg, int MsgNum)
{
	CString str = GetTime() + _T("\r\n ");
	str += strMsg + _T("\r\n");
	m_BurnLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

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

//��ȡ���ڷ���ֵ
CString CMFC_LaserDlg::ReadFromBurningPort()
{

	//���ڱ���
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

//�ж�IMEI�Ƿ����
int CMFC_LaserDlg::CheckIMEI(CString IMEI)
{
	ADOManage adomanageJudge;
	int judgeflag;
	adomanageJudge.ConnSQL();
	judgeflag = adomanageJudge.CheckIMEIExit(IMEI);
	adomanageJudge.CloseAll();
	return judgeflag;
}

//��chipid�Ƿ�������imei��
CString CMFC_LaserDlg::getBandIMEIByChipID(CString chipid)
{
	ADOManage adomanageJudge;
	CString judgeflag;
	adomanageJudge.ConnSQL();
	judgeflag = adomanageJudge.checkAndGetIMEIbyID(chipid);
	adomanageJudge.CloseAll();
	return judgeflag;
}

//��ȡ��RID�󶨵�IMEI��
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

//������д��Ϣ
BOOL CMFC_LaserDlg::InsertBurningMes(CString zhidan, CString Imei, CString ComputerIp, CString burningresult, CString Rid)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.InsertBurningData(zhidan, Imei, ComputerIp, burningresult, Rid);
	adomanageInsert.CloseAll();
	return TRUE;
}

//������д��Ϣ
BOOL CMFC_LaserDlg::UpdateBurningMes(CString Imei, CString Rid, CString computerIp, CString BurnResult)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.UpdateBurningData(Imei, Rid, BurnResult, computerIp);
	adomanageInsert.CloseAll();
	return TRUE;
}

/*���¶Ա���Ϣ*/
BOOL CMFC_LaserDlg::UpdateCompareMes(CString Imei, CString BurnResult)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.UpdateCompareData(Imei, BurnResult);
	adomanageInsert.CloseAll();
	return TRUE;
}

//����IMEI��ǰλ
void CMFC_LaserDlg::UpdataBurningMes(CString zhidan, CString Imei)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.UpdateCurrnetImei(Imei, zhidan);
	adomanageInsert.CloseAll();
}

//����ɨ��ǹģʽ��ť
void CMFC_LaserDlg::OnBnClickedBurningscanninggunButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString scanninggunButtonText;
	GetDlgItemText(IDC_BurningScanningGun_Button, scanninggunButtonText);
	//GetCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);//�Ȼ�ȡ��ǰ���ں�
	if (scanninggunButtonText == "����ɨ��ǹģʽ") {
		//�жϴ�����û�п����ɹ�
		/*Port1handler = InitCom(Port1No);
		if (Port1handler == NULL)
		{
			ShowBurningMsg(L"�򿪴���ʧ��", 1);
			return;
		}
		ShowBurningMsg(L"���ڳ�ʼ���ɹ�!", 0);*/
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
		SetDlgItemText(IDC_BurningScanningGun_Button, L"�ر�ɨ��ǹģʽ");
		SetDlgItemText(IDC_BurningRemind_STATIC, L"����");
		SetDlgItemText(IDC_BurningImei_EDIT, L"");
	}
	else {
		/*if (!CloseCom(Port1handler))
		{
			ShowBurningMsg(L"�رմ���ʧ��", 1);
			MessageBox(L"�رմ���1ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}
		ShowBurningMsg(L"�رմ��ڳɹ�", 0);*/
		EnterScanFlag = FALSE;
		/*GetDlgItem(IDC_BurningConn_BUTTON)->EnableWindow(true);
		GetDlgItem(IDC_BurningPort_COMBO)->EnableWindow(true);
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_SendCommand1_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_SendCommand2_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_ReciveCommand_EDIT)->EnableWindow(true);*/
		//GetDlgItem(IDC_NoNet_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(false);
		SetDlgItemText(IDC_BurningScanningGun_Button, L"����ɨ��ǹģʽ");
		SetDlgItemText(IDC_BurningRemind_STATIC, L"δ����");
		CString CurrentImei;
		GetDlgItemText(IDC_ImeiCurrent_EDIT, scanninggunButtonText);
		SetDlgItemText(IDC_BurningImei_EDIT, scanninggunButtonText);
	}
}

// ��ȡMachineName�ֶ�����
CString CMFC_LaserDlg::getMachineName(CString zhidan)
{
	ADOManage ado;
	ado.ConnSQL();
	CString subName;
	subName = ado.adoGetMachineName(zhidan);
	ado.CloseAll();
	return subName;
}

//����id�����ݿ⣬�Ƿ�����дIMEI
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

//��ȡ����д������
CString CMFC_LaserDlg::getRelativeFata(CString param, CString paramIMEI)
{
	ADOManage myadomanage;
	CString relativeData;
	myadomanage.ConnSQL();
	relativeData = myadomanage.getRelativeIMEIX(param, paramIMEI);
	myadomanage.CloseAll();
	return relativeData;
}

//�����ȡ�����ݵ����ݿ�
void CMFC_LaserDlg::saveDataToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.saveData(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//��ȡ�±������
CString CMFC_LaserDlg::getNewSheetData(CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	CString dbdata = L"";
	myadomanage.ConnSQL();
	dbdata = myadomanage.getNewSheetDataDB(IMEI, ziduan);
	myadomanage.CloseAll();
	return dbdata;
}

//�ѻ�ȡ�����±����ݸ��µ��ɱ�
void CMFC_LaserDlg::saveNewDataToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.saveData(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//�ѻ�ȡ��������insert���ɱ�
void CMFC_LaserDlg::insertNewDataToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.insertToRelative(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//��ɹ������Ƿ����IMEI
BOOL CMFC_LaserDlg::checkIMEIInRelative(CString strIMEI)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	if (myadomanage.CheckIMEIInRelative(strIMEI))
	{
		myadomanage.CloseAll();
		return TRUE;               //����true��˵������IMEI����Ѷ��������ݸ��µ�������
	}
	myadomanage.CloseAll();
	return FALSE;                  //����false��˵��û��IMEI����Ѷ��������ݺ͵�ǰ��д��IMEI���뵽������
}

//���¹������Ƿ����IMEI
BOOL CMFC_LaserDlg::checkIMEIInNewRelative(CString strIMEI)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	if (myadomanage.CheckIMEIInRelative(strIMEI))
	{
		myadomanage.CloseAll();
		return TRUE;               //����true��˵������IMEI����Ѷ��������ݸ��µ�������
	}
	myadomanage.CloseAll();
	return FALSE;                  //����false��˵��û��IMEI����Ѷ��������ݺ͵�ǰ��д��IMEI���뵽������
}

//����IMEI���¹�����
void CMFC_LaserDlg::UpdateIMEIToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.UpdateIMEIToRelative(data, IMEI, ziduan);
	myadomanage.CloseAll();
}
//����ICCID��SIM��MAC���¹�����
void CMFC_LaserDlg::UpdateICCIDToDB(CString data, CString IMEI, CString ziduan)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.UpdateICCIDToRelative(data, IMEI, ziduan);
	myadomanage.CloseAll();
}

//����IMEI����ϱ�
void  CMFC_LaserDlg::updateToCoupleTest(CString IMEI, CString chipid)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.updateDatatoCoupleDB(IMEI, chipid);
	myadomanage.CloseAll();
}

//����ɹ���Ϣ���ܱ�
void CMFC_LaserDlg::insertToMainSheet(CString IMEI, CString result, CString chipid, CString version, CString machinename)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.insertToMainSheetDB(IMEI, result, chipid, version, machinename);
	myadomanage.CloseAll();
}

//���оƬID����IMEI�󶨣������
void CMFC_LaserDlg::UpdateToMainSheet(CString IMEI, CString result, CString chipid, CString version, CString machinename)
{
	ADOManage myadomanage;
	myadomanage.ConnSQL();
	myadomanage.UpdateToMainSheetDB(IMEI, result, chipid, version, machinename);
	myadomanage.CloseAll();
}

//��дָ����̣߳��������ݿ��е�ָ�
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
			dlg->ShowBurningMsg(L"IMEI:" + JudgeIMEI + L"�ڵ�ǰ�Ŷ��⣬����˶����Ŷ��Ƿ�����д���", 1);
			dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI��" + JudgeIMEI + L"�ڵ�ǰ�Ŷ���\r\n");
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
			dlg->ShowBurningMsg(L"��:" + DbTestcommand, 0);
			Sleep(400);
			readPort = dlg->ReadFromBurningPort();
			if (readPort.Find(commandReceive) >= 0)
			{
				//dlg->BatOnAndOff(10);
				dlg->ShowBurningMsg(L"��:" + readPort, 0);
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + DbTestcommand + L"\r\n");
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
				dlg->SendToPlc(3);  //���̵���������Ϣ����plc��д�е��ź�
				BurnFinishFlag = FALSE;
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
				dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("��д��"));
				pcEnterTestFlag = false;
				break;
			}
		} while (pcEnterTestFlag);
		for (j = 3; j <= StationValVector.size() - 1; j++)
		{
			dlg->GetCommandAndRec(j, commandName, commandReceive, tryTime, commandVal);
			if (commandName.Find(L"оƬID") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int k = 0; k <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");
					if (readPort.Find(commandReceive) != -1)
					{
						chipID = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						chipID = chipID.Left(32);
						dlg->ShowBurningMsg(L"��ȡоƬID�ɹ�:" + chipID, 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��ȡоƬ�ɹ���" + chipID + L"\r\n");
						k = tryTimeInt + 1;
					}
					else
					{
						k++;
						if (k > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"��ȡоƬIDʧ��", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��ȡоƬʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"����汾") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int p = 0; p <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						Allversion = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						Allversion = Allversion.Left(Allversion.Find(L"\r\n"));
						dlg->ShowBurningMsg(L"��ȡ����汾�ɹ���" + Allversion, 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��ȡ����汾�ɹ���" + Allversion + L"\r\n");
						p = tryTimeInt + 1;
					}
					else
					{
						p++;
						if (p > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"��ȡ����汾ʧ��", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��ȡ����汾ʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"����д��") != -1)
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
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						dlg->ShowBurningMsg(commandName + L"�ɹ�", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"�ɹ�\r\n");
						L = tryTimeInt + 1;
					}
					else
					{
						L++;
						if (L > tryTimeInt)
						{
							dlg->ShowBurningMsg(commandName + L"ʧ��", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"ʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"дIMEI1") != -1)
			{
				commandVal = commandVal + L"\r\n";
				for (int R = 0; R <= tryTimeInt;)
				{
					if (m_BurningConrtolFlag == false)
					{
						goto CLOSETHREAD;
					}
					bWriteStat = WriteFile(dlg->Port1handler, CT2A(commandVal), commandVal.GetLength(), &dwBytesWrite, NULL);
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						Allversion = readPort.Mid(readPort.Find(commandReceive) + commandReceive.GetLength());
						dlg->ShowBurningMsg(L"��һ����дָ��ͳɹ�", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��һ����дָ��ͳɹ�\r\n");
						R = tryTimeInt + 1;
					}
					else
					{
						R++;
						if (R > tryTimeInt)
						{
							dlg->ShowBurningMsg(L"��һ����дָ���ʧ��", 0);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��һ����дָ���ʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"дIMEI") != -1)
			{
				CString NoCheckCodeIMEI;
				CString chipBandIMEI;
				CString haveCheckCodeIMEI;
				chipBandIMEI = dlg->getBandIMEIByChipID(chipID);
				if (chipBandIMEI != L"") //���а�IMEI
				{
					if (chipBandIMEI.GetLength() == 15)
					{
						haveCheckCodeIMEI = chipBandIMEI;
						chipBandIMEI = chipBandIMEI.Left(chipBandIMEI.GetLength() - 1);
					}
					dlg->ShowBurningMsg(L"оƬID��" + chipID + L"����IMEI��" + haveCheckCodeIMEI + L"��", 1);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"оƬID��" + chipID + L"����IMEI��" + haveCheckCodeIMEI + L"��\r\n");
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
					if (dlg->CheckIMEI(CurrentBurningImei) == 1) //����IMEI�Ƿ��ѱ��ù���==1�����ù�����IMEI+1���ٽ����ж�
					{
						dlg->ShowBurningMsg(L"IMEI��" + CurrentBurningImei + L"�غţ��Զ�����", 1);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", CurrentBurningImei + L"�غ�\r\n");
						if (EnterScanFlag)    //ɨ��ģʽ������غ���ֱ���ж�λʧ��
						{
							checkRidFlag = FALSE;
							goto FALILDONE;
						}
						unsigned long long Ridimeiint = 0;
						Ridimeiint = _atoi64(CStringA(NoCheckCodeIMEI)) + IncreasingnumberInt;
						NoCheckCodeIMEI = _ui64toa(Ridimeiint, CT2A(NoCheckCodeIMEI), 10);
						if (NoCheckCodeIMEI.GetLength() != FImeiStart.GetLength() || NoCheckCodeIMEI<FImeiStart || NoCheckCodeIMEI.GetLength() != FImeiEnd.GetLength() || NoCheckCodeIMEI>FImeiEnd)
						{
							dlg->ShowBurningMsg(L"IMEI:" + CurrentBurningImei + L"�ڵ�ǰ�Ŷ��⣬����˶����Ŷ��Ƿ�����д���", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI��" + CurrentBurningImei + L"�ڵ�ǰ�Ŷ���\r\n");
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
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
					commandReceive.Replace(L" ", L"");
					readPort.Replace(L" ", L"");
					/*readPort.Replace(L"\r", L"");
					readPort.Replace(L"\n", L"");*/
					if (readPort.Find(commandReceive) != -1)
					{
						unsigned long long imeiint;
						CString imeistr;
						dlg->ShowBurningMsg(L"IMEI:" + CurrentBurningImei + L"��д�ɹ�", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI��" + CurrentBurningImei + L"��д�ɹ�\r\n");
						dlg->updateToCoupleTest(CurrentBurningImei, chipID); //����IMEI�ŵ���ϱ�
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
							dlg->ShowBurningMsg(L"IMEI:" + CurrentBurningImei + L"��дʧ��", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"IMEI��" + CurrentBurningImei + L"��дʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"��ȡ����") != -1)
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
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burning_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
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

						//�ж�ǰ׺
						if (commandName.Find(L"(PRE)") != -1)
						{
							CString PreStr = commandName.Left(commandName.Find(L"(PRE)"));

							if (NumberStr.Find(PreStr) != 0)//������ҳ���������0���Ǿʹ���ǰ׺�϶�����
							{
								dlg->ShowBurningMsg(L"�ֶ�ǰ׺����", 1);
								dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"�ֶ�ǰ׺����\r\n");
								goto FALILDONE;
							}

							commandName = commandName.Right(commandName.GetLength() - commandName.Find(L"(PRE)") - 5);//��ȡ��PRE����
						}

						//�жϳ���
						if (commandName.Find(L"(LENGTH)") != -1)
						{
							CString LengthStr = commandName.Left(commandName.Find(L"(LENGTH)"));

							if (NumberStr.GetLength() != _ttoi(LengthStr))//���Ȳ��Ծͷ��ش���
							{
								dlg->ShowBurningMsg(L"�ֶγ��ȴ���", 1);
								dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"�ֶγ��ȴ���\r\n");
								goto FALILDONE;
							}


							commandName = commandName.Right(commandName.GetLength() - commandName.Find(L"(LENGTH)") - 8);//��ȡ��LENGTH����
						}

						//�ж��Ƿ��ڷ�Χ��
						if (commandName.Find(L"(SectionNumber)") != -1)
						{
							CString SectionNumberStr = commandName.Left(commandName.Find(L"(SectionNumber)"));

							CString SectionNumberHighStr = SectionNumberStr.Left(SectionNumberStr.Find(L"-"));

							CString SectionNumberLowStr = SectionNumberStr.Right(SectionNumberStr.GetLength() - SectionNumberStr.Find(L"-") - 1);

							if (NumberStr.GetLength() != SectionNumberHighStr.GetLength() || NumberStr < SectionNumberHighStr || NumberStr.GetLength() != SectionNumberLowStr.GetLength() || NumberStr >SectionNumberLowStr)//�������ֵ����С����Сֵ�ͷ��ش���
							{
								dlg->ShowBurningMsg(L"�ֶβ��ںŶ���", 1);
								dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"�ֶβ��ںŶ���\r\n");
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
						if (dlg->checkIMEIInRelative(CurrentBurningImei))  //��IMEI����
						{
							dlg->saveDataToDB(NumberStr, CurrentBurningImei, DBziduan);
						}
						else
						{
							dlg->insertNewDataToDB(NumberStr, CurrentBurningImei, DBziduan);   //û�������
						}
						dlg->ShowBurningMsg(commandName + L"�ɹ�", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"�ɹ�\r\n");
						M = tryTimeInt + 1;
					}
					else
					{
						M++;
						if (M > tryTimeInt)
						{
							dlg->ShowBurningMsg(commandName + L"ʧ��", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"ʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
			else if (commandName.Find(L"��������"))
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
					dlg->ShowBurningMsg(L"��:" + commandVal, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + commandVal + L"\r\n");
					Sleep(500);
					readPort = dlg->ReadFromBurningPort();
					dlg->ShowBurningMsg(L"��:" + readPort, 0);
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
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
						newSheetData = dlg->getNewSheetData(NumberStr, ziduan);//��ȡ�¹�����Ĺ�������
						if (dlg->checkIMEIInRelative(CurrentBurningImei))   //��������IMEI�������
						{
							dlg->saveNewDataToDB(newSheetData, CurrentBurningImei, ziduan);
						}
						else
						{
							dlg->insertNewDataToDB(newSheetData, CurrentBurningImei, ziduan);  //�������
						}
						dlg->ShowBurningMsg(commandName + L"�ɹ�", 0);
						dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"�ɹ�\r\n");
						N = tryTimeInt + 1;
					}
					else
					{
						N++;
						if (N > tryTimeInt)
						{
							dlg->ShowBurningMsg(commandName + L"ʧ��", 1);
							dlg->WritetoTxt(L"Burn_" + zhidan + L"_", commandName + L"ʧ��\r\n");
							goto FALILDONE;
						}
					}
				}
			}
		}
		dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("�ɹ�"));
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
		dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"��дʧ��\r\n");
		dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("ʧ��"));
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
			//dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + DbTestcommand + L"\r\n");
			Sleep(500);
			readPort = dlg->ReadFromBurningPort();
			//dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
			if (readPort == "")
			{
				WriteFile(dlg->Port1handler, CT2A(DbTestcommand), DbTestcommand.GetLength(), &dwBytesWrite, NULL);
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���ͣ�" + DbTestcommand + L"\r\n");
				Sleep(500);
				readPort = dlg->ReadFromBurningPort();
				dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"���գ�" + readPort + L"\r\n");
				if (readPort == "")
				{
					if (EnterScanFlag)
					{
						dlg->SetDlgItemTextW(IDC_BurningImei_EDIT, L"");
					}
					dlg->SetDlgItemText(IDC_BurningRemind_STATIC, _T("����"));
					dlg->WritetoTxt(L"Burn_" + zhidan + L"_", L"����\r\n");
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


/**************************************�Ա�IMEIģ��**************************************/
//�������������ʱˢ��ע�����
void CMFC_LaserDlg::OnDropdownCompareportCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FindCommPort((CComboBox*)GetDlgItem(IDC_ComparePort_COMBO), Port2No);
}

//��ʾ�Ա���Ϣ
int CMFC_LaserDlg::ShowCompareMsg(CString strMsg, int MsgNum)
{
	CString str = GetTime() + _T("\r\n ");
	str += strMsg + _T("\r\n");
	m_CpLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

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

//һ����ʼ
void CMFC_LaserDlg::OnBnClickedCompareconnButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*if (IniTestCommand.IsEmpty())
	{
		MessageBox(L"���Ȼ�ȡ����ָ��", L"��ʾ");
		return;
	}*/
	GetCommPort((CComboBox*)GetDlgItem(IDC_ComparePort_COMBO), Port2No);//�Ȼ�ȡ��ǰ���ں�
	CString CompareConnButtonText;
	GetDlgItem(IDC_CompareConn_BUTTON)->GetWindowText(CompareConnButtonText);
	if (CompareConnButtonText == "һ����ʼ") {
		//�жϴ�����û�п����ɹ�
		if (StationValVector.size() == 0)
		{
			MessageBox(_T("��ȡָ��ʧ�ܣ������Ƿ����ϴ�ָ����������"), _T("��ʾ"), NULL);
			return;
		}
		Port2handler = InitCom(Port2No);
		if (Port2handler == NULL)
		{
			SetDlgItemText(IDC_CompareRemind_STATIC, L"�򿪴���ʧ��");
			return;
		}
		ShowCompareMsg(L"�Աȴ��ڳ�ʼ���ɹ�", 0);
		WriteComThread = AfxBeginThread(SendComparePortThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_CompareConrtolFlag = true;
		SetDlgItemText(IDC_CompareRemind_STATIC, L"����");
		WritetoTxt(L"Compare_", L"�򿪶Աȴ��ڳɹ�\r\n");
		SetDlgItemText(IDC_CompareConn_BUTTON, TEXT("һ���ر�"));
		GetDlgItem(IDC_ComparePort_COMBO)->EnableWindow(false);
	}
	else
	{
		if (!CloseCom(Port2handler))
		{
			MessageBox(L"�رնԱȴ���ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}
		ShowCompareMsg(L"�رնԱȴ��ڳɹ�", 0);
		WritetoTxt(L"Compare_", L"�رնԱȴ��ڳɹ�\r\n");
		CompareLastSendTestFlag = false;
		m_CompareConrtolFlag = false; //ֹͣ�߳�
		Sleep(500);
		SetDlgItemText(IDC_CompareConn_BUTTON, TEXT("һ����ʼ"));
		SetDlgItemText(IDC_CompareRemind_STATIC, L"δ����");
		GetDlgItem(IDC_ComparePort_COMBO)->EnableWindow(true);
	}
}

//�Աȴ����߳�
UINT SendComparePortThread(LPVOID lpParam)
{
	CMFC_LaserDlg* dlg;
	dlg = (CMFC_LaserDlg*)lpParam;
	PurgeComm(dlg->Port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//���ڱ���
	DWORD dwBytesWrite;
	BOOL bWriteStat;
	CString CIMEI1;
	CString CIMEI2;
	int ComparetryTimeInt;
	BOOL FuncCompareSendTestFlag = TRUE;
	BOOL FuncFuncCompareLastSendTestFlag = TRUE;
	BOOL checkBurnandLd = TRUE;
	CString strcommand; //��ָ���ñ���
	CString StrRead; //�Ŵ��ڷ���ֵ��
	CString IMEIStr; //��Ŷ�������IMEI��
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
			dlg->ShowCompareMsg(L"��:" + Teststrcommand, 0);
			//dlg->](1);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���ͣ�" + Teststrcommand + L"\r\n");
			Sleep(400);
			StrRead = dlg->ReadFromComparePort();
			if (StrRead.Find(compareRev) >= 0)
			{
				//dlg->BatOnAndOff(10);
				dlg->ShowCompareMsg(L"��:" + StrRead, 0);
				dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���գ�" + StrRead + L"\r\n");
				dlg->SendToPlc(6);
				dlg->SetDlgItemText(IDC_CompareRemind_STATIC, _T("�Ա���"));
				if (LDIsOpenFlag)
				{
					/*dlg->GetDlgItemText(IDC_NEXTCOMPAREIMEI, midChangeCpIMEI);
					CIMEI2 = midChangeCpIMEI;*/
					if (dlg->LdVec.size() == 0 || dlg->CompareIndex >= dlg->LdVec.size())
					{
						CString SizeStr, LdIndexStr;
						SizeStr.Format(L"%d", dlg->LdVec.size());
						LdIndexStr.Format(L"%d", dlg->CompareIndex);
						dlg->SetRicheditText(_T("�Ա���������[ size:") + SizeStr + _T(" , LdIndex:") + LdIndexStr + _T("]"), 1);
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
							dlg->SetRicheditText(_T("�Ա���������[ size:") + SizeStr + _T(" , LdIndex:") + LdIndexStr + _T("]"), 1);
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
			dlg->ShowCompareMsg(L"��:" + compareCommand, 0);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���ͣ�" + compareCommand + L"\r\n");
			Sleep(400);
			StrRead = dlg->ReadFromComparePort();
			dlg->ShowCompareMsg(L"��:" + StrRead, 0);
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���գ�" + StrRead + L"\r\n");
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
					dlg->ShowCompareMsg(L"����IMEI��" + CIMEI1 + L" �����IMEI��" + CIMEI2 + L"�Ա�ͨ��", 0);
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"�Ա�ͨ��");
					dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"IMEI��" + CIMEI1 + L"�Ա�ͨ��\r\n");
					dlg->UpdateCompareMes(CIMEI2, L"1");
					goto SUCCEED;
				}
				else
				{
					dlg->ShowCompareMsg(L"����IMEI��" + CIMEI1 + L" �����IMEI��" + CIMEI2 + L"�Ա�ͨ��", 1);
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"�Ա�ʧ��");
					dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"IMEI��" + CIMEI1 + L"�Ա�ʧ��\r\n");
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
					dlg->ShowCompareMsg(L"ָ��ظ�ʧ��", 1);
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"�Ա�ʧ��");
					dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"ָ��ظ�ʧ��\r\n");
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
		//�Ա�IMEI��ɺ���û����Ƿ��ѶϿ�
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
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���ͣ�" + Teststrcommand + L"\r\n");
			Sleep(500);
			StrRead = dlg->ReadFromComparePort();
			dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���գ�" + StrRead + L"\r\n");
			if (StrRead == "")
			{
				WriteFile(dlg->Port2handler, CT2A(Teststrcommand), Teststrcommand.GetLength(), &dwBytesWrite, NULL);
				dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���ͣ�" + Teststrcommand + L"\r\n");
				Sleep(500);
				StrRead = dlg->ReadFromComparePort();
				dlg->WritetoTxt(L"Compare_" + zhidan + _T("_"), L"���գ�" + StrRead + L"\r\n");
				if (StrRead == "") {
					dlg->SetDlgItemText(IDC_CompareRemind_STATIC, L"����");
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

//�����ں���
CString CMFC_LaserDlg::ReadFromComparePort()
{

	//���ڱ���
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

//����������д
BOOL CMFC_LaserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���


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

	////if (tempStr == "һ���ر�"){
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

//���������С��ɫ
HBRUSH CMFC_LaserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	CString Fontstr1 = L"";
	CString Fontstr2 = L"";
	if (pWnd->GetDlgCtrlID() == IDC_BurningRemind_STATIC)
	{
		GetDlgItemText(IDC_BurningRemind_STATIC, Fontstr1);
		if (Fontstr1 == "δ����" || Fontstr1 == "����" || Fontstr1 == "��д��")
		{
			m_brush.CreateSolidBrush(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
		else if (Fontstr1 == "IMEI�غ�" || Fontstr1 == "ʧ��") {
			m_brush.CreateSolidBrush(RGB(255, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
		else if (Fontstr1 == "�ɹ�")
		{
			m_brush.CreateSolidBrush(RGB(0, 128, 0));
			pDC->SetBkColor(RGB(0, 128, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_CompareRemind_STATIC)
	{
		GetDlgItemText(IDC_CompareRemind_STATIC, Fontstr2);
		if (Fontstr2 == "δ����" || Fontstr2 == "����" || Fontstr2 == "�Ա���")
		{
			m_brush.CreateSolidBrush(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
		else if (Fontstr2 == "IMEI�ѶԱȹ�" || Fontstr2 == "�Ա�ʧ��") {
			m_brush.CreateSolidBrush(RGB(255, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
		else if (Fontstr2 == "�Ա�ͨ��")
		{
			m_brush.CreateSolidBrush(RGB(0, 128, 0));
			pDC->SetBkColor(RGB(0, 128, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//��ʼ������
void CMFC_LaserDlg::fontinit()
{
	staticHint1font.CreatePointFont(500, L"����");
	editfont.CreatePointFont(300, L"����");

	GetDlgItem(IDC_BurningRemind_STATIC)->SetFont(&editfont);
	GetDlgItem(IDC_CompareRemind_STATIC)->SetFont(&editfont);
}


/***************************************������־��¼***********************************/
/*��ȡ��ǰexe·��*/
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

/*д���ݵ�txt�ļ�*/
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

			file.SeekToEnd(); // �ƶ��ļ�ָ�뵽ĩβ
			BYTE byUnicode[] = { 0xFF, 0xFE };
			file.Write(byUnicode, sizeof(byUnicode));
			file.Write(sValue, wcslen(sValue) * sizeof(WCHAR));
			file.Close();
		}
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
	return FALSE;
}

/*��ȡ����*/
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


/*************************************����***************************************/
//һ��ȫ����
void CMFC_LaserDlg::OnBnClickedAllautoButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*if (IniTestCommand.IsEmpty())
	{
		MessageBox(L"���Ȼ�ȡ����ָ��", L"��ʾ");
		return;
	}*/
	CString autobuttontext;
	GetDlgItemText(IDC_ALLAUTO_BUTTON, autobuttontext);
	if (autobuttontext == "һ��ȫ����")
	{
		CString burnbuttontext, laserbuttontext, comparebuttontext, JDQButtinText;
		GetDlgItemText(IDC_BurningConn_BUTTON, burnbuttontext);
		GetDlgItemText(IDC_OpenLdSystem_BUTTON, laserbuttontext);
		GetDlgItemText(IDC_CompareConn_BUTTON, comparebuttontext);
		GetDlgItemText(IDC_OPENRELAY, JDQButtinText);
		if (burnbuttontext == "һ���ر�" || laserbuttontext == "�ر��ص�ϵͳ" || comparebuttontext == "һ���ر�" || JDQButtinText == L"�رռ̵�������")
		{
			MessageBox(_T("����ʧ�ܣ����ȹر���������ģ��"), _T("��ʾ"), NULL);
			return;
		}
		CString porttext1, porttext2, porttext3;
		GetDlgItemText(IDC_BurningPort_COMBO, porttext1);
		GetDlgItemText(IDC_ComparePort_COMBO, porttext2);
		GetDlgItemText(IDC_JDQ_COMBO, porttext3);
		if (porttext1 == "" || porttext2 == "" || porttext3 == "" || porttext1 == porttext2 || porttext1 == porttext3 || porttext2 == porttext3)
		{
			MessageBox(_T("����ʧ�ܣ�������д���Աȡ��̵�������"), _T("��ʾ"), NULL);
			return;
		}
		//�жϴ����ܲ��ܿ����ɹ�
		if (StationValVector.size() == 0)
		{
			MessageBox(_T("��ȡָ��ʧ�ܣ������Ƿ����ϴ�ָ����������"), _T("��ʾ"), NULL);
			return;
		}
		Port1handler = InitCom(porttext1);
		if (Port1handler == NULL)
		{
			MessageBox(_T("����д����ʧ��"), _T("��ʾ"), NULL);
			return;
		}
		Port2handler = InitCom(porttext2);
		if (Port2handler == NULL)
		{
			MessageBox(_T("�򿪶Աȴ���ʧ��"), _T("��ʾ"), NULL);
			return;
		}
		Port3handler = InitCom1(porttext3);
		if (Port3handler == NULL)
		{
			MessageBox(_T("�򿪼̵�������ʧ��"), _T("��ʾ"), NULL);
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
		SetDlgItemText(IDC_ALLAUTO_BUTTON, L"һ��ȫ�ر�");
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
		SetDlgItemText(IDC_ALLAUTO_BUTTON, L"һ��ȫ����");
		GetDlgItem(IDC_StartLaser_BUTTON)->EnableWindow(true);
	}
}

//�����水ť
void CMFC_LaserDlg::OnBnClickedNonetCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetCommPort((CComboBox*)GetDlgItem(IDC_BurningPort_COMBO), Port1No);//�Ȼ�ȡ��ǰ���ں�
	//�жϴ�����û�п����ɹ�
	Port1handler = InitCom(Port1No);
	if (Port1handler == NULL)
	{
		ShowBurningMsg(L"�򿪴���ʧ��", 1);
		return;
	}
	ShowBurningMsg(L"���ڳ�ʼ���ɹ�!", 0);
	if (((CButton *)GetDlgItem(IDC_NoNet_CHECK))->GetCheck()) {
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(true);
		m_BurningImeiEdit.SetFocus();
	}
	else {
		GetDlgItem(IDC_BurningImei_EDIT)->EnableWindow(false);
	}
}

//ת16����
BYTE * CMFC_LaserDlg::CStrToByte(int len, CString str)
{
	BYTE *Buf = new BYTE[len];
	CString StrSub;
	for (int iSrc = 0, iDst = 0; iDst < len; iSrc += 2, iDst++)
	{
		StrSub = str.Mid(iSrc, 2);
		Buf[iDst] = (BYTE)_tcstoul(StrSub, NULL, 16);//ʮ������
	}
	return Buf;
}

//16�����ַ���ת2�����ַ���
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

/*�������*/
void CMFC_LaserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		myDlgManage.ResizeWindow();
	}
}

/*��ȡ����ָ��*/
void CMFC_LaserDlg::OnBnClickedGetcommand()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	MessageBox(L"��ȡ�ɹ�", L"��ʾ");
}

//��@@�ָ�ָ������
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

//��ȡStationz�ֶ�ָ��
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

//����
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

//��PLC�����ݽ���
void CMFC_LaserDlg::SendToPlc(int MsgNum)
{
	EnterCriticalSection(&cs);
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	CString ErrorNumStr;
	CString strcommand; //��ָ���ñ���
	BYTE * strcommand1;
	switch (MsgNum)
	{
		//��д
	case 1://ok
		strcommand = "023735393038033130";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"����ָ��" + strcommand, 0);
		break;
	case 2:
		strcommand = "023735413038033138";//no
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"����ָ��" + strcommand, 0);
		break;
	case 3:
		strcommand = "023735423038033139";//testing
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"����ָ��" + strcommand, 0);
		break;
	case 10:
		strcommand = "023835393038033131";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"����ָ��" + strcommand, 0);
		break;
	case 20:
		strcommand = "023835413038033139";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"����ָ��" + strcommand, 0);
		break;
	case 30:
		strcommand = "023835423038033141";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowBurningMsg(L"����ָ��" + strcommand, 0);
		break;

		//�Ա�
	case 4://ok
		strcommand = "023736313038033039";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"����ָ��" + strcommand, 0);
		break;
	case 5://no
		strcommand = "023736323038033041";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"����ָ��" + strcommand, 0);
		break;
	case 6:
		strcommand = "023736333038033042";//testing
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"����ָ��" + strcommand, 0);
		break;
	case 40:
		strcommand = "023836313038033041";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"����ָ��" + strcommand, 0);
		break;
	case 50:
		strcommand = "023836323038033042";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"����ָ��" + strcommand, 0);
		break;
	case 60:
		strcommand = "023836333038033043";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		ShowCompareMsg(L"����ָ��" + strcommand, 0);
		break;

		//�ص�
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
		SetRicheditText(L"����ָ��" + strcommand, 0);
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
		SetRicheditText(L"����ָ��" + strcommand, 0);
		break;
	case 11://��PLC	
		strcommand = "0230303130423031033637";
		strcommand1 = CStrToByte(16, strcommand);
		bWriteStat = WriteFile(Port3handler, strcommand1, strcommand.GetLength() / 2, &dwBytesWrite, NULL);
		//SetRicheditText(L"����ָ��" + strcommand, 0);
		break;
	}
	if (bWriteStat)
	{
		SetRicheditText(L"����ָ��ɹ�" + strcommand, 0);
	}
	else
	{
		SetRicheditText(L"����ָ��ʧ��" + strcommand, 1);
		DWORD ErrorNum = GetLastError();
		ErrorNumStr.Format(L"%d", ErrorNum);
		SetRicheditText(L"�����룺" + ErrorNumStr, 1);
	}
	Sleep(400);
	LeaveCriticalSection(&cs);
}

