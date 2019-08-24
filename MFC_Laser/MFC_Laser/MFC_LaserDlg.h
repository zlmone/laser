
// MFC_LaserDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "AutoCombox.h"
#include "MarkEzdDll.h"
#include "LDStartDlg.h"
#include <vector>
using namespace std;

//线程函数
UINT readFromLd2(LPVOID lparam);//tcp接收信息
UINT SendDBCommand(LPVOID lparam);//烧写
UINT SendComparePortThread(LPVOID lpParam);//对比串口写函数线程
UINT listenThread(LPVOID lparam);//tcp监听
UINT ReadLdPortThread(LPVOID pParam);//读镭雕机串口

//其它函数
void Sdk_Int2Char(int p_nNum, char * p_Ch);
void Sdk_Str2BcdStr(char * p_Str, int p_nLen, char * p_StrBcd);

// CMFC_LaserDlg 对话框
class CMFC_LaserDlg : public CDialogEx
{
private:
	LDStartDlg *m_pCLDStartDlg;

// 构造
public:
	CMFC_LaserDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_LASER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//获取本机信息变量和函数
	CString m_pcnameEdit;
	CString m_pcipEdit;
	SOCKET m_SockListen;
	SOCKET clientSock;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);

	//信息日志模块变量以及函数
	CRichEditCtrl m_currentRichedit;

	void SetRicheditText(CString strMsg, int No);//根据值来在信息日志中显示不同的信息
	CString GetTime();//获取当前系统时间

	//初始化配置模块变量以及函数
	CAutoCombox ZhiDanCombo;
	CString ZhiDanNO;//制单号
	CString IMEIStaStart, IMEIStaEnd;//IMEI号段范围的变量
	CString SN1, SN2, SN3;//SN号段范围的变量
	int SN1Len; //记录SN前缀位数

	//镭雕系统模块变量以及函数
	MarkEzdDll markezd;	//声明镭雕类对象
	int DllFlag;//DLL的返回值
	CString DllStr;//DLL的返回值转换成CString
	void UnlockContent();
	CString CreateIMEI15(CString imei);//生成IMEI校验位
	WORD LdPort;//镭雕串口变量

	//测试
	CString ceshiReadPort;


	CWinThread* ThreadReadLdPort;//镭雕线程句柄
	void WindowTimeClose(CString str, int strtime);//窗口在N毫秒后自动关闭
	int FindLdTemplateObject();//寻找镭雕模板对象名称
	int LdCore();  //开始镭雕函数
	int ChangeLdName(CString LdVariable, CString strld);//替换指定对象名称的值
	void CloseLaserSystem();


	//填充订单号到下拉框
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

	/*串口模块函数*/

	CWinThread* WriteThread;//下载写线程句柄
	CString Port1No;//串口名称变量
	CString Port4No;
	CString Port5No;
	HANDLE Port1handler;
	CString getBasebandID();
	//串口句柄
	CString ReadFromBurningPort();
	CString ReadFromPLCPort();
	HANDLE InitCom(CString comName);//初始化串口
	HANDLE InitCom1(CString comName);
	HANDLE InitCom2(CString comName);
	//初始化串口
	int CloseCom(HANDLE handler);//关闭串口
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);
	void LdCylinderPress(int argument);
	//获取当前串口号
	void FindCommPort(CComboBox *pComboBox, CString &ComNo);//从注册表中寻找串口号
	int CheckIMEI(CString IMEI); //判断IMEI是否存在

	CString getBandIMEIByChipID(CString chipid);

	CString CheckRID(CString IMEI);

	int checkCompareResult(CString IMEI);

	BOOL InsertBurningMes(CString zhidan, CString Imei, CString ComputerIp, CString burningresult, CString Rid);

	/*串口烧写IMEI函数*/
	BOOL UpdateBurningMes(CString Imei, CString Rid, CString computerIp, CString BurnResult);
	BOOL UpdateCompareMes(CString Imei, CString BurnResult);

	//插入烧写信息
	void UpdataBurningMes(CString zhidan, CString Imei);//更新IMEI当前位信息
	afx_msg void OnDropdownBurningportCombo();
	afx_msg void OnBnClickedBurningconnButton();
	int ShowBurningMsg(CString strMsg, int MsgNum);
	CEdit m_BurningeditLog;
	afx_msg void OnBnClickedNocheckcodeCheck();
	afx_msg void OnBnClickedLdoncetimeButton();
	CButton m_NoCheckCodeCheckControl;


	/*对比模块*/
	CWinThread* WriteComThread;//下载写线程句柄
	CString Port2No;//对比串口名称变量
	HANDLE Port2handler;//对比串口句柄
	afx_msg void OnDropdownCompareportCombo();
	int ShowCompareMsg(CString strMsg, int MsgNum);
	afx_msg void OnBnClickedCompareconnButton();
	CString ReadFromComparePort(); //对比串口读函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	/*字体函数*/
	CBrush m_brush;//刷子变量
	CFont staticHint1font, editfont;//字体变量
	void fontinit();
	afx_msg void OnBnClickedStartlaserButton();

	/*本地日志模块*/
	CString GetExePath();
	BOOL WritetoTxt(CString module, CString sValue);
	CString  GetDate();
	afx_msg void OnBnClickedAllautoButton();
	afx_msg void OnBnClickedBurningscanninggunButton();
	CEdit m_BurningImeiEdit;
	afx_msg void OnBnClickedNonetCheck();

	//继电器串口
	CString Port3No;//继电器串口名称变量
	HANDLE Port3handler;//继电器串口句柄
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
