#pragma once

#include "AutoCombox.h"
#include "Laser.h"
#include "ComboListCtrl.h"
// CDZ_Laser 对话框
using namespace std;
class CDZ_Laser : public CDialogEx
{
	DECLARE_DYNAMIC(CDZ_Laser)

public:
	CDZ_Laser(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDZ_Laser();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DZLASER };
#endif

protected:
	HICON ownicon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CAutoCombox m_DZZhiDanComBo;
	virtual BOOL OnInitDialog();
	void DZFillZhidanToComBo();
	CString getMachineName(CString zhidan);
	afx_msg void OnCbnSelchangeCombo1();
	CButton m_DZHaveCheckCode;
	CComboBox m_IncreasingNum;
	CComboBox m_DZRelativeZiduan;
	CButton m_DZHaveRelativeZiDuan;
	CEdit m_IMEISTAR;
	CEdit m_IMEIEND;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	CListCtrl m_DataShow;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	CString GetRelativeData(CString IMEI, CString ziduan);
	CString m_TagsCount;
	afx_msg void OnBnClickedButton4();
	CListCtrl m_NoUseData;
	afx_msg void OnBnClickedButton9();
	int CheckNoUseDataInDB(CString IMEI);
	CWinThread *CheckThread;
	CWinThread *MarkThread;
	afx_msg void OnBnClickedButton3();
	CString m_NoUseDataCount;
	afx_msg void OnBnClickedButton8();
	int GetLocalHostIPName(CString & sLocalName, CString & sIpAddress);
	CString DzIp, DzName;
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton1();
	int ShowDZLaserMsg(CString strMsg, int MsgNum);
	CLaser LaserMachine;
	CRichEditCtrl m_DzLaserLog;
	CString DzLaserFile;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	BOOL m_MarkType;
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	BOOL StartmarkFunc(BOOL OnceDone, CString InputData, BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime, int DataCount, int DataNo);
	BOOL ScanStartmarkFunc(BOOL OnceDone, CString InputData, CString relative, BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime, int DataCount, int DataNo);
	BOOL UpdateDzLDNextIMEI(CString IMEI);
	BOOL InsertDzLdData(CString IMEI);
	BOOL UpdateDzLdData(CString IMEI);
	LRESULT PopulateComboList2(WPARAM wParam, LPARAM lParam);
	BOOL writeDZTxt(CString value);
	BOOL PauseMark = FALSE;
	int FirstData = 0;
	BOOL StartMark = FALSE;
	BOOL CheckDBResult[30000] = {0};
	CComboListCtrl m_list2;
	BOOL FastCheck;
	BOOL EmptyDataCheck;
	CEdit m_MarkData;
	BOOL DataRelativeChoose;
	BOOL RemarkCheck;
	BOOL DBIFilterCheck;
	CString m_ZiDuanVal;
	CString m_delayTime;
	afx_msg void OnBnClickedButton16();
	CString m_ScanData;
	afx_msg void OnEnChangeEdit9();
	CString m_BitsCheck;
	CString m_ScanDataPre;
	BOOL CheckIMEIData;
	CString m_StartIMEI;
	CString m_EndIMEI;
	CEdit m_ScanCtrl;
	CString ScanRelativeData;
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UnLockSomething(BOOL flag);
	void LockSomething(BOOL flag);
	BOOL m_CheckCodeVal;
	CString m_IncreasingNumVal;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_FastCheclCtrl;
};

UINT CheckIMEIThread(LPVOID lparam);

UINT OutDataThread(LPVOID lparam);

UINT CheckNoUseData(LPVOID lparam);

UINT StartMarkThread(LPVOID lparam);
