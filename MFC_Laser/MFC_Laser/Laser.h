#pragma once
//�ص������
#include "HansAdvDllDefine.h"
#include "HansAdvDllDefineEx.h"
#include "baseInterface.h"
#include "baseHatch.h"
class CLaser
{
public:
	CLaser();
	~CLaser();
public:
	TCHAR* ShowLastError();
	CString GetDLLVer();											//��ȡDLL�汾
	BOOL SysIni();													//��ʼ������ϵͳ
	BOOL SysClose();												//�رտ���ϵͳ

	BOOL LoadMarkFile(CString path);

	BOOL CloseMarkFile(CString  path);								//�رմ򿪵��ļ�

	BOOL Querytouch();												//��ѯ��̤��״̬
	BOOL QuerytouchActive(HWND m_hWnd);								//���ٽ�̤��⣬����λ��ʵʱ���
	
	BOOL StartMark(BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime);	//��ʼ���
	BOOL PreviewMark(BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime);//���Ԥ��

	BOOL ChangeTextByName(TCHAR *szName, TCHAR *szTextValue);
};

