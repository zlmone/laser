#pragma once
//镭雕机调用
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
	CString GetDLLVer();											//获取DLL版本
	BOOL SysIni();													//初始化控制系统
	BOOL SysClose();												//关闭控制系统

	BOOL LoadMarkFile(CString path);

	BOOL CloseMarkFile(CString  path);								//关闭打开的文件

	BOOL Querytouch();												//查询脚踏板状态
	BOOL QuerytouchActive(HWND m_hWnd);								//快速脚踏检测，由下位机实时检测
	
	BOOL StartMark(BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime);	//开始打标
	BOOL PreviewMark(BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime);//红光预览

	BOOL ChangeTextByName(TCHAR *szName, TCHAR *szTextValue);
};

