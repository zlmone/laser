#include "stdafx.h"
#include "Laser.h"
//#include "HansAdvDllDefine.h"

CLaser::CLaser()
{
}


CLaser::~CLaser()
{
}
TCHAR* CLaser::ShowLastError()
{
	int  nErr(0);
	TCHAR szBuff[200] = { 0 };
	HS_GetLastError(&nErr, szBuff, 200);
	return szBuff;
}

CString CLaser::GetDLLVer()
{
	WORD wMainVer, wSubVer;
	HS_GetDllVersion(&wMainVer, &wSubVer);

	CString strVersion;
	strVersion.Format(_T("%d.%d.%d - %d.%d.%d"), wMainVer >> 12, wMainVer >> 7 & 0x001f, wMainVer & 0x007f,
		wSubVer >> 12, wSubVer >> 7 & 0x001f, wSubVer & 0x007f);
	return strVersion;
}
BOOL CLaser::SysIni()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	*(_tcsrchr(szPath, _T('\\')) + 1) = NULL;
	//AfxMessageBox(szPath);
	int nRet = HS_InitialMachine(L".\\");
	if (0 == nRet)
	{
		//AfxMessageBox(L"1");
		TCHAR* pszFile = new TCHAR[_tcslen(DFT_DOC_NAME) + 1];
		_tcscpy(pszFile, DFT_DOC_NAME);
		int nRet = HS_SetCurDoc(pszFile);	
		//AfxMessageBox(pszFile);//设置当前工作文档
		if (nRet == 0)
		{
			//AfxMessageBox(L"2");
			double x(0), y(0);
			HS_GetMarkRange(&x, &y);						//获取打标范围
			CString str;
			str.Format(_T("Mark range:%.0f*%.0fmm"), x, y);
			return TRUE;
		}
		else
		{
			//AfxMessageBox(L"3");
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}
BOOL CLaser::SysClose()
{
	//HS_FreeStrokeList(&m_listCustom);
	int nRet = HS_CloseMachine();
	if (0 == nRet)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CLaser::LoadMarkFile(CString  path)
{
	//AfxMessageBox(path);
	CString strShow;
	int nRet = HS_LoadMarkFile((LPTSTR)(LPCTSTR)path);						//int nRet = HS_SaveMarkFile(szFileName);
	if (0 == nRet)
	{
		//AfxMessageBox(L"11");
		int nRet = HS_SetCurDoc((LPTSTR)(LPCTSTR)path);					//设置当前工作文档
		if (nRet == 0)
		{
			//AfxMessageBox(L"12");
			return TRUE;
		}
		else
		{
			//strShow.Format(L"2%d", nRet);
			AfxMessageBox(strShow);
			return FALSE;
		}
	}
	else
	{
		//strShow.Format(L"1%d", nRet);
		AfxMessageBox(strShow);
		return FALSE;
	}
}

BOOL CLaser::CloseMarkFile(CString  path)
{
	int nRet = HS_CloseMarkFile((LPTSTR)(LPCTSTR)path, FALSE);
	if (nRet == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL CLaser::Querytouch()
{
	int nTouchFlag(0);
	UINT nRet = HS_IsTouch(&nTouchFlag);
	if (nRet == 0)
	{
		if (nTouchFlag)
			return TRUE;
	}
	return FALSE;
}

BOOL CLaser::QuerytouchActive(HWND m_hWnd)
{
	int nRet = HS_CheckTouch(m_hWnd, TRUE);//激活主动检测
	if (nRet==0)
	{
		//Check touch succeed!
		return TRUE;
	}
	else
	{
		ShowLastError();
		return FALSE;
	}
}

BOOL CLaser::StartMark(BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime)
{
	int nRet = HS_Mark(0, bWaitTouch, bWaitEnd, nOverTime, FALSE);
	if (0 == nRet)
	{		
		if (bWaitEnd == FALSE)
			return TRUE;

		int nFlag=0;
		BOOL Result = FALSE;
		while (bWaitEnd == TRUE)
		{
			int nRet = HS_IsMarkEnd(&nFlag);//0---正在打标；1---正常结束；2---打标中止结束；3---设备异常打标异常
			if (0 == nRet)
			{
				//Mark doing...
				if (nFlag==1)
				{
					UINT nTime;
					HS_GetMarkTime(&nTime);
					CString str;
					str.Format(_T("Mark finish...! Last time %d ms"), nTime);
					Result=TRUE;
					break;
				}
				else if (nFlag == 0)
				{
					Sleep(1000);
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
		} 

		if (Result == TRUE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
	}
	else
	{
		return FALSE;
	}
}

BOOL CLaser::PreviewMark(BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime)
{
	int nRet = HS_Mark(1, bWaitTouch, bWaitEnd, nOverTime, FALSE);
	if (0 == nRet)
	{
		return TRUE;
	}
	//int nRet = HS_MarkPause();
	//int nRet = HS_Remark();
	//int nRet = HS_MarkStop();//==0
	return FALSE;
}

BOOL CLaser::ChangeTextByName(TCHAR *szName, TCHAR *szTextValue)
{
	int nRet = HS_ChangeTextByName(szName, szTextValue);
	if (0 == nRet)
	{
		//Change text successfully!
		TCHAR szTextVal[200] = { 0 };
		int  nSize = 200;
		nRet = HS_GetTextByName(szName, szTextVal, &nSize);
		if (nRet == 0)
		{
			if(wcscmp(szTextValue, szTextVal)==0)
				return TRUE;
		}
		else
		{
			AfxMessageBox(_T("Error get Text!"));
		}
		
	}
	else
	{
		ShowLastError();
	}
	return FALSE;
}