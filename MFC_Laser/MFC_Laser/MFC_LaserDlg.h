
// MFC_LaserDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "AutoCombox.h"
#include "MarkEzdDll.h"
#include "LDStartDlg.h"
#include <vector>
using namespace std;

//�̺߳���
UINT readFromLd2(LPVOID lparam);//tcp������Ϣ
UINT SendDBCommand(LPVOID lparam);//��д
UINT SendComparePortThread(LPVOID lpParam);//�Աȴ���д�����߳�
UINT listenThread(LPVOID lparam);//tcp����
UINT ReadLdPortThread(LPVOID pParam);//���ص������

//��������
void Sdk_Int2Char(int p_nNum, char * p_Ch);
void Sdk_Str2BcdStr(char * p_Str, int p_nLen, char * p_StrBcd);

// CMFC_LaserDlg �Ի���
class CMFC_LaserDlg : public CDialogEx
{
private:
	LDStartDlg *m_pCLDStartDlg;

// ����
public:
	CMFC_LaserDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_LASER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnBnClickedDbconfigButton();

public:
	vector<CString>BurnVec;
	vector<CString>LdVec;
	int LdIndex = 0;
	int CompareIndex = 0;
public:
	//��ȡ������Ϣ�����ͺ���
	CString m_pcnameEdit;
	CString m_pcipEdit;
	SOCKET m_SockListen;
	SOCKET clientSock;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);

	//��Ϣ��־ģ������Լ�����
	CRichEditCtrl m_currentRichedit;

	void SetRicheditText(CString strMsg, int No);//����ֵ������Ϣ��־����ʾ��ͬ����Ϣ
	CString GetTime();//��ȡ��ǰϵͳʱ��

	//��ʼ������ģ������Լ�����
	CAutoCombox ZhiDanCombo;
	CString ZhiDanNO;//�Ƶ���
	CString IMEIStaStart, IMEIStaEnd;//IMEI�Ŷη�Χ�ı���
	CString SN1, SN2, SN3;//SN�Ŷη�Χ�ı���
	int SN1Len; //��¼SNǰ׺λ��

	//�ص�ϵͳģ������Լ�����
	MarkEzdDll markezd;	//�����ص������
	int DllFlag;//DLL�ķ���ֵ
	CString DllStr;//DLL�ķ���ֵת����CString
	void UnlockContent();
	CString CreateIMEI15(CString imei);//����IMEIУ��λ
	WORD LdPort;//�ص񴮿ڱ���

	//����
	CString ceshiReadPort;


	CWinThread* ThreadReadLdPort;//�ص��߳̾��
	void WindowTimeClose(CString str, int strtime);//������N������Զ��ر�
	int FindLdTemplateObject();//Ѱ���ص�ģ���������
	int LdCore();  //��ʼ�ص���
	int ChangeLdName(CString LdVariable, CString strld);//�滻ָ���������Ƶ�ֵ
	void CloseLaserSystem();


	//��䶩���ŵ�������
	void FillZhidanToComBo();
	afx_msg void OnBnClickedRefreshzhidanButton();
	afx_msg void OnSelchangeZhidanCombo();
	CEdit ImeiStart;
	CEdit ImeiEnd;
	afx_msg void OnBnClickedChoosetemplateButton();
	afx_msg void OnBnClickedOpentemplateButton();
	afx_msg void OnBnClickedTolockButton();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOpenldsystemButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	/*����ģ�麯��*/

	CWinThread* WriteThread;//����д�߳̾��
	CString Port1No;//�������Ʊ���
	CString Port4No;
	CString Port5No;
	HANDLE Port1handler;
	CString getBasebandID();
	//���ھ��
	CString ReadFromBurningPort();
	CString ReadFromPLCPort();
	HANDLE InitCom(CString comName);//��ʼ������
	HANDLE InitCom1(CString comName);
	HANDLE InitCom2(CString comName);
	//��ʼ������
	int CloseCom(HANDLE handler);//�رմ���
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);
	void LdCylinderPress(int argument);
	//��ȡ��ǰ���ں�
	void FindCommPort(CComboBox *pComboBox, CString &ComNo);//��ע�����Ѱ�Ҵ��ں�
	int CheckIMEI(CString IMEI); //�ж�IMEI�Ƿ����

	CString getBandIMEIByChipID(CString chipid);

	CString CheckRID(CString IMEI);

	int checkCompareResult(CString IMEI);

	BOOL InsertBurningMes(CString zhidan, CString Imei, CString ComputerIp, CString burningresult, CString Rid);

	/*������дIMEI����*/
	BOOL UpdateBurningMes(CString Imei, CString Rid, CString computerIp, CString BurnResult);
	BOOL UpdateCompareMes(CString Imei, CString BurnResult);

	//������д��Ϣ
	void UpdataBurningMes(CString zhidan, CString Imei);//����IMEI��ǰλ��Ϣ
	afx_msg void OnDropdownBurningportCombo();
	afx_msg void OnBnClickedBurningconnButton();
	int ShowBurningMsg(CString strMsg, int MsgNum);
	CEdit m_BurningeditLog;
	afx_msg void OnBnClickedNocheckcodeCheck();
	afx_msg void OnBnClickedLdoncetimeButton();
	CButton m_NoCheckCodeCheckControl;


	/*�Ա�ģ��*/
	CWinThread* WriteComThread;//����д�߳̾��
	CString Port2No;//�Աȴ������Ʊ���
	HANDLE Port2handler;//�Աȴ��ھ��
	afx_msg void OnDropdownCompareportCombo();
	int ShowCompareMsg(CString strMsg, int MsgNum);
	afx_msg void OnBnClickedCompareconnButton();
	CString ReadFromComparePort(); //�Աȴ��ڶ�����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	/*���庯��*/
	CBrush m_brush;//ˢ�ӱ���
	CFont staticHint1font, editfont;//�������
	void fontinit();
	afx_msg void OnBnClickedStartlaserButton();

	/*������־ģ��*/
	CString GetExePath();
	BOOL WritetoTxt(CString module, CString sValue);
	CString  GetDate();
	afx_msg void OnBnClickedAllautoButton();
	afx_msg void OnBnClickedBurningscanninggunButton();
	CEdit m_BurningImeiEdit;
	afx_msg void OnBnClickedNonetCheck();

	//�̵�������
	CString Port3No;//�̵����������Ʊ���
	HANDLE Port3handler;//�̵������ھ��
	HANDLE Port4handler;
	HANDLE Port5handler;
	BYTE * CStrToByte(int len, CString str);
	CString HexStr2BinStr(CString HexStr);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_CompareLog;
	CEdit myLdport;
	afx_msg void OnBnClickedGetcommand();
	afx_msg void OnBnClickedOpenrelay();
	afx_msg void OnBnClickedLdtwopart();
	CButton m_Check;
	afx_msg void OnBnClickedCheck1();
	BOOL m_checkState;
	CButton m_RelateSn;
	BOOL m_RelativeSN;
	BOOL m_ReLD;
	afx_msg void OnBnClickedReld();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedLodatxt();





	void GetCommandAndRec(int num, CString & str1, CString & str2, CString & str3, CString & str4);

	void GetStation(CString softWare);

	CString getMachineName(CString zhidan);

	BOOL checkIMEIbyID(CString chipID);

	CString getRelativeFata(CString param, CString paramIMEI);

	void saveDataToDB(CString data, CString IMEI, CString ziduan);

	

	CString getNewSheetData(CString IMEI, CString ziduan);

	void saveNewDataToDB(CString data, CString IMEI, CString ziduan);

	void insertNewDataToDB(CString data, CString IMEI, CString ziduan);

	BOOL checkIMEIInRelative(CString strIMEI);

	BOOL checkIMEIInNewRelative(CString strIMEI);

	void UpdateIMEIToDB(CString data, CString IMEI, CString ziduan);

	void UpdateICCIDToDB(CString data, CString IMEI, CString ziduan);



	void updateToCoupleTest(CString IMEI, CString chipid);

	void insertToMainSheet(CString IMEI, CString result, CString chipid, CString version, CString machinename);

	void UpdateToMainSheet(CString IMEI, CString result, CString chipid, CString version, CString machinename);

	vector<vector<CString>>BurLdCpVec;
	BOOL m_LdRandom;
	CRichEditCtrl m_BurnLog;
	CRichEditCtrl m_CpLog;
	CComboBox m_IncreasingNumber;
	afx_msg void OnBnClickedButton2();
	BOOL m_NocheckCode;
	CButton m_AutoCheckPrint;
	BOOL m_AutoCheckPrintVal;
	afx_msg void OnDropdownJdqCombo();
	void SendToPlc(int MsgNum);
	CComboBox m_BurnPort;
	CString m_BurningEditVal;
	CString m_EndIMEIVal;
};
