
// MFC_Laser.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_LaserApp: 
// �йش����ʵ�֣������ MFC_Laser.cpp
//

class CMFC_LaserApp : public CWinApp
{
public:
	CMFC_LaserApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_LaserApp theApp;