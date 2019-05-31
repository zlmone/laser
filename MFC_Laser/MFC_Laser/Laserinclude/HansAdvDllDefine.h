#ifndef HANS_ADVINTERFACEDLL_6332F9B4_82E7_4403_B791_4F7210CD211B
#define HANS_ADVINTERFACEDLL_6332F9B4_82E7_4403_B791_4F7210CD211B

//���뵼������
#ifndef  HANSADVINTERFACE_EXPORT
	#define  HANSADVDLL_API	extern "C" __declspec(dllimport)
#else
	#define  HANSADVDLL_API	extern "C" __declspec(dllexport)
#endif

#define HL_ERR_SUCCESS		0 			//�ɹ�
#define HL_ERR_PROGRUN		1 			//��һ������������
#define HL_ERR_PATH			2			//·������ȷ,���ܳ����������װĿ¼������
#define HL_ERR_FAILOPEN		3 			//��ʼ��ʧ�ܣ����鹷�Ϳ����������
#define HL_ERR_NOINITIAL	4 			//δ��ʼ��
#define HL_ERR_WARNING		5 			//����
#define HL_ERR_OVERTIME		6 			//���ʱ
#define HL_ERR_READFILE		7 			//�޷���ȡ�ļ�
#define HL_ERR_NOFINDFONT	8 			//ָ�������岻����
#define HL_ERR_NOPEN		9 			//ָ���Ĳ�Ų�����
#define HL_ERR_NOOBJECT		10 			//û�ҵ�ָ���Ķ���
#define HL_ERR_PARAM		11 			//�����Ƿ�
#define HL_ERR_STATUS		12 			//�����״̬,��ǰ״̬�²���ִ�д˲���
#define HL_ERR_ALLOCMEMORY	13 			//�����ڴ�ʧ��
#define HL_ERR_OVERRANGE	14 			//��귶Χ����
#define HL_ERR_BUFFERSIZE	15			//����������
#define HL_ERR_NULLPTR		16			//��ָ��
#define HL_ERR_NODOC		17			//δ�ҵ�ָ���ĵ�
#define HL_ERR_REOPEN		18			//ָ���ĵ��Ѿ�����
#define HL_ERR_EXECFAIL     19          //��λ������ִ��ʧ��
#define HL_ERR_DOWNFAIL		20			// ���ĵ���������ʧ��
#define HL_ERR_MULDOWNIO	21			// ���ܴ���ͬ���ĵ�IO�й�ֵ
#define HL_ERR_ALREADYDOWN  22			// ����������
#define HL_ERR_CALLFUN		23			// ���ú���˳����ȷ
#define HL_ERR_PREPROCESS   24			// ��������ʧ��
#define HL_ERR_NOCALIFILE   25			// �Ҳ���У���ļ�����������cali.exe����У��

#define HL_ERR_UNKNOWN		100			//δ֪����

#define  WM_TOUCH_MSG    (WM_USER+1095)  //��������̤������Ϣ

#define DFT_DOC_NAME _T("default")

///////////////////�ĵ��������ع�����Ϣ��Ӧ////////////////////
#define WM_MARK_END				( WM_USER+5 )
#define MARK_TEND	100
#define MARK_INFO		103

// ���ĵ��ѻ����ػص�����λ��������ĵ���ش���ǵ��ĵ���(��0��ʼ�����ص��ĵ����)���ͱ��ʱ��
typedef void (*HS_MARKMSG_CALLBACK)(int nID, UINT64 nMarkTime);
/////////////////////////////////////////////////////////////////
/****************************************************************
* A��ͨ�ò��֣������û�����ʹ��*
****************************************************************/
HANSADVDLL_API int __stdcall HS_GetDllVersion(WORD* pMainVer,WORD *pDllVer);
HANSADVDLL_API int __stdcall HS_InitialMachine(TCHAR *szPath);
HANSADVDLL_API int __stdcall HS_CloseMachine();
HANSADVDLL_API int __stdcall HS_GetMarkRange(double *pX,double *pY);
HANSADVDLL_API int __stdcall HS_LoadMarkFile(TCHAR *szFile);
HANSADVDLL_API int __stdcall HS_GetDocList(TCHAR szDocList[][50], int* pCount);
HANSADVDLL_API int __stdcall HS_SetCurDoc(TCHAR *szDocName);
HANSADVDLL_API int __stdcall HS_SaveMarkFile(TCHAR *szFile);
HANSADVDLL_API int __stdcall HS_CloseMarkFile(TCHAR *szFile,BOOL bSave);
HANSADVDLL_API int __stdcall HS_IsTouch(int* pTouchFlag);	
HANSADVDLL_API int __stdcall HS_CheckTouch(HWND hWnd, BOOL bActive);
HANSADVDLL_API int __stdcall HS_Mark(int nType, BOOL bWaitTouch, BOOL bWaitEnd, int nOverTime, BOOL bMarkAll);
HANSADVDLL_API int __stdcall HS_IsMarkEnd(int* pFlag);
HANSADVDLL_API int __stdcall HS_MarkPause();
HANSADVDLL_API int __stdcall HS_Remark();
HANSADVDLL_API int __stdcall HS_MarkStop();
HANSADVDLL_API int __stdcall HS_GetMarkTime(UINT *pMarkTime);

HANSADVDLL_API int __stdcall HS_ChangeTextByName(TCHAR *szName, TCHAR *szTextValue);
HANSADVDLL_API int __stdcall HS_ChangeTextByNameW(TCHAR *szName, wchar_t *szTextValue);
HANSADVDLL_API int __stdcall HS_GetTextByName(TCHAR *szName, TCHAR *szText, int *pMaxCount);
HANSADVDLL_API int __stdcall HS_GetTextByNameW(TCHAR *szName, wchar_t *szText, int *pMaxCount);

HANSADVDLL_API int __stdcall HS_ReadPort(DWORD* pValue);
HANSADVDLL_API int __stdcall HS_WritePort(DWORD dwValue, DWORD dwMask);
HANSADVDLL_API int __stdcall HS_SetDevCfg();
HANSADVDLL_API int __stdcall HS_PreviewGraph(HWND hWnd,double dbCenterX,double dbCenterY,double dbScale);
HANSADVDLL_API int __stdcall HS_SetHatchPara(LPVOID lpPara);
HANSADVDLL_API int __stdcall HS_GetHatchPara(LPVOID  lpPara);
HANSADVDLL_API int __stdcall HS_SetFontPara(const LPVOID lpPara);
HANSADVDLL_API int __stdcall HS_GetFontPara(LPVOID lpPara);
HANSADVDLL_API int __stdcall HS_SetLayerPara(int nLayer, LPVOID lpPara);
HANSADVDLL_API int __stdcall HS_GetLayerPara(int nLayer, LPVOID lpPara);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ���¼���Ϊ��Ϊ���ƽӿ�
/*****************************************************************
����: HS_GetLayerByObjName
����: ���ݶ������ƻ�ȡͼ���	
����˵��:
			szObjName(����):������
			nLayer(���):ͼ���
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetLayerByObjName(TCHAR *szObjName,int *nLayer);


/*****************************************************************
����: HS_SetLayerByObjName
����: ���ݶ����������ö���ͼ��	
����˵��:
			szObjName(����):������
			nLayer(����):ͼ���
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_SetLayerByObjName(TCHAR *szObjName,int nLayer);


/*****************************************************************
����: HS_GetFillParaByObjName
����: ���ݶ������ƻ�ȡ������	
����˵��:
			szObjName(����):������
			lpPara(���):���ṹ��HATCHPARA��ָ��(��ϸ�����baseHatch.h)
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetFillParaByObjName(TCHAR *szObjName,LPVOID lpPara);

/*****************************************************************
����: HS_SetFillParaByObjName
����: ���ݶ�����������������	
����˵��:
			szObjName(����):������
			lpPara(����):���ṹ��HATCHPARA��ָ��(��ϸ�����baseHatch.h)
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_SetFillParaByObjName(TCHAR *szObjName,LPVOID lpPara);

/*****************************************************************
����: HS_GetFontFaceByObjName
����: ���ݶ������ƻ�ȡ����(������ttf�ı�)
����˵��:
			szObjName(����):������
			szFontName(���):����������"����"��(��ϸ�����baseHatch.h),���鳤�Ƚ���256,����̫С
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetFontFaceByObjName(TCHAR *szObjName,TCHAR* szFontName);

/*****************************************************************
����: HS_SetFontFaceByObjName
����: ���ݶ���������������(������ttf�ı�)
����˵��:
			szObjName(����):������
			szFontName(����):����������"����"��(��ϸ�����baseHatch.h)
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_SetFontFaceByObjName(TCHAR *szObjName,TCHAR* szFontName);

/*****************************************************************
����: HS_GetObjPropertyByName
����: ���ݶ������ƻ�ȡ����λ��
����˵��:
			szObjName(����):������
			pLeft(���):���λ��
			pTop(���):����λ��
			pRight(���):�ұ�λ��
			pBottom(���):����λ��
����ֵ:
			0:�ɹ�
			��0:ʧ��(��ͨ��HS_GetLastError��ȡ������Ϣ)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetObjPropertyByName(TCHAR* szObjName,double* pLeft,double *pTop,double *pRight,double *pBottom);
HANSADVDLL_API int __stdcall HS_SetObjPropertyByName(TCHAR* szObjName,double dbLeft,double dbTop,double dbRight,double dbBottom);
//////////////////////////////////////////////////////////////////////////////////////////

HANSADVDLL_API int __stdcall HS_ImportVectorFile(TCHAR *szFileName, double dbPosX, double dbPosY, 
																	  double dbWidth, double dbHeight, double dbRotate,
																	  BOOL bHath, int nHatchLayer, TCHAR *szObjName);

HANSADVDLL_API int __stdcall HS_ImportImageFile(TCHAR *szFileName, double dbPosX, double dbPosY, 
																	  double dbWidth, double dbHeight, double dbRotate,
																	  int nMarkType, double dbDotDistX, double dbDotDistY,
																	  int nAccDot, BOOL bReciprocate, int nRepairDot, TCHAR *szObjName);

HANSADVDLL_API int __stdcall HS_AddText(	TCHAR *szStr, 
															double dbHeight, 
															double dbWidthRatio,
															BOOL bModel,
															TCHAR *szTextName,
															double dbPosX, 
															double dbPosY, 
															int nAlign, 
															double dbAngle,
															int nLayer, 
															BOOL bHatch, 
															int nHatchLayer,
															TCHAR *szObjName
															);

HANSADVDLL_API int __stdcall HS_AddText2(TCHAR *szStr, 
														 double dbHeight, 
														 double dbWidthRatio,
														 BOOL bModel,
														 TCHAR *szTextName,
														 double dbPosX, 
														 double dbPosY, 
														 int nAlign, 
														 double dbAngle,
														 int nLayer, 
														 BOOL bHatch, 
														 int nHatchLayer,
														 double dbCharSpace,
														 double dbLineSpace,
														 TCHAR *szObjName
														 );

HANSADVDLL_API int __stdcall HS_AddBarcode(	TCHAR *szStr, 
																int			nBarType, 
																double	dbHeight, 
																double	dbPosX, 
																double	dbPosY,
																double	dbAngle, 
																int			nAlign, 
																int			nLayerNo, 
																double	dbBarRatio, 
																double	dbLineDist,
																double	dbShrink, 
																BOOL		bReverse, 
																double	dbRevWidth, 
																double	dbRevHeight, 
																BOOL		Model, 
																TCHAR *szTextName, 
																TCHAR *szObjName
															);

HANSADVDLL_API int  __stdcall HS_AddCurve(double ptBuf[][2], int nPtNum, int nLayer, BOOL bHatch, int nHatchLayer, TCHAR *szObjName);
HANSADVDLL_API int  __stdcall HS_AddDotMatrix(double ptBuf[][2], int nPtNum, int nLayer, TCHAR *szObjName);

//////////////
HANSADVDLL_API int  __stdcall HS_AddDelay(DWORD dwDelay, TCHAR *szObjName);
HANSADVDLL_API int  __stdcall HS_AddInput(DWORD  dwInput, DWORD  dwInputMask, TCHAR *szObjName);
HANSADVDLL_API int  __stdcall HS_AddOutput(DWORD  dwOutput, DWORD  dwOutputMask,DWORD dwOutputDelay, TCHAR *szObjName);
HANSADVDLL_API int  __stdcall HS_AddMotorPos(double* motorAxisPos,TCHAR *szObjName);
HANSADVDLL_API int  __stdcall HS_AddMotorMove(int nWorkType,WORD nAxis,WORD nMoveMode,double dbMoveValue,TCHAR *szObjName);
HANSADVDLL_API int  __stdcall HS_AddConditionStart(DWORD  dwInput, DWORD  dwInputMask,TCHAR* szObjName);
HANSADVDLL_API int  __stdcall HS_AddConditionEnd(TCHAR* szObjName);
HANSADVDLL_API int  __stdcall HS_AddBufferLaser(BOOL bLaserOn, double dbQFreq, double dbQRls, double dbCurrent, double dbPower, int nLaserMode,TCHAR* szObjName);
HANSADVDLL_API int  __stdcall HS_AddCCD(UINT nProgID,UINT nNGRepNum,UINT nOverTime,UINT nErrorMode,UINT	nBaudRate,BOOL bScanJump,double dbX,double dbY,UINT nDelay,TCHAR* szObjName);
HANSADVDLL_API int  __stdcall HS_AddPosCali(UINT nCaliType, TCHAR *szObjName);
/////////////

HANSADVDLL_API int  __stdcall HS_GetObjList(TCHAR szObjList[][50], int *pCount);
HANSADVDLL_API int  __stdcall HS_SelectObjects(TCHAR *szObjName, BOOL bSelected);

HANSADVDLL_API int  __stdcall HS_ClearObjects();
HANSADVDLL_API int  __stdcall HS_Move(double mx,double my);
HANSADVDLL_API int  __stdcall HS_Rotate(double x, double y,double a);
HANSADVDLL_API int  __stdcall HS_Shear(double x, double y, double ax, double ay);
HANSADVDLL_API int  __stdcall HS_Scale(double x, double y,double rx,double ry);
HANSADVDLL_API int  __stdcall HS_Array(double ptArray[][2],int nSize);
HANSADVDLL_API int  __stdcall HS_GetObjProperty(double* pLeft,double *pTop,double *pRight,double *pBottom);
HANSADVDLL_API int  __stdcall HS_IGotoXYZ(int nType, double dbX, double dbY, double dbZ);
HANSADVDLL_API int  __stdcall HS_IGotoXYZ2(int nType, double dbX,double dbY, double dbZ, BOOL bRed);
HANSADVDLL_API int  __stdcall HS_ILaserOn(double dbQFrq, double QRls,double dbCurrentA,double dbPower);
HANSADVDLL_API int  __stdcall HS_ILaserOn2(double dbQFrq,double dbQRls,double dbCurrentA,double dbPower,int nLaserMode);
HANSADVDLL_API int  __stdcall HS_ILaserOff();

HANSADVDLL_API void __stdcall HS_GetLastError(int* pError, TCHAR* szMsg, int nSize);
HANSADVDLL_API int  __stdcall HS_CheckDogFuncFlag(BYTE nBit, BOOL* bOpen);	
HANSADVDLL_API int  __stdcall HS_MoveMotor(LPVOID pPara);
HANSADVDLL_API int  __stdcall HS_HomeMove(LPVOID pPara,BOOL bSoftHome);
HANSADVDLL_API int  __stdcall HS_MoveMotorV(int nAxisNo, double dbMoveValue, int nMoveModel, int nMoveType);
HANSADVDLL_API int  __stdcall HS_HomeMoveV(int nAxisNo, BOOL bSoftHome);
HANSADVDLL_API int  __stdcall HS_GetCurMotorPos(int nAxis, double& dbPos);
HANSADVDLL_API int  __stdcall HS_SetAsSoftHome(int nAxis);

/*****************************************************************\
���ƣ� HS_SetMotionPara
���ã������˶��������ã�������Ч�������浽�����ļ�(����ʱ������û�õ��Ĳ������鴫�������ļ�Ĭ��ֵ)
����˵��:
nAxis:			���
dbEmptySpeed:	�ճ��ٶ�(ƽ���ᵥλΪmm/s,��ת�ᵥλΪ��/s)
dbWorkSpeed:	�����ٶ�(ƽ���ᵥλΪmm/s,��ת�ᵥλΪ��/s)
dbHomeHighSpeed:ƽ�ƻ����ٶ�(mm/s)
dbHomeLowSpeed: ƽ�ƻ������ٶ�(mm/s)
dbEmptyTAcc:	�ճ̼���ʱ��(s)
dbWorkTAcc:		��������ʱ��(s)
dbHomeTAcc:		�������ʱ��(s)
\****************************************************************/
HANSADVDLL_API int  __stdcall HS_SetMotionPara(int nAxis, double dbEmptySpeed, double dbWorkSpeed, double dbHomeHighSpeed, double dbHomeLowSpeed, double dbEmptyTAcc, double dbWorkTAcc, double dbHomeTAcc);

/*****************************************************************\
���ƣ� SetMotionSysPara
���ã� �˶��������ã�������Ч�����浽�����ļ�
����˵��:
nAxis:			���
nPPR:			ÿת������
dbPitch:		�ݼ��
nDelay:			�˶���ɺ���ʱ
nAxisType:		�˶������� 0 ƽ���ᣬ 1 ��ת��
nMotorDire:		���ת������ 1���� -1 ����
dbEmptySpeed:	�ճ��ٶ�
dbWorkSpeed:	�����ٶ�
dbEmptyTAcc:	�ճ̼���ʱ��
dbWorkTAcc:		��������ʱ��
bHardOrgEnable:	��еԭ��ʹ��
bAutoHome:		�Ƿ��Զ�����
bEnabled:		�Ƿ�ʹ��
\****************************************************************/
HANSADVDLL_API int  __stdcall HS_SetMotionSysPara(int nAxis,int nPPR, double dbPitch, int nDelay,int nAxisType,int nMotorDire,double	dbEmptySpeed,
												 double dbWorkSpeed, double dbEmptyTAcc,double dbWorkTAcc,BOOL bHardOrgEnable, BOOL bAutoHome, BOOL bEnabled);

/*****************************************************************\
���ƣ� HS_SetMotionSysPara2
���ã� �˶��������ã�������Ч�����浽�����ļ�
����˵��:
nAxis:  ���
pParam: MOTIONPARA�ṹ��ָ��
\****************************************************************/
HANSADVDLL_API int __stdcall HS_SetMotionSysPara2(int nAxis,LPVOID pPara);



HANSADVDLL_API int  __stdcall HS_GetLayerColor(int nLayerNo, COLORREF *pColor);

HANSADVDLL_API int __stdcall HS_GetStrokeList(TCHAR *szObjName, LPVOID pData);
/*��ȡ��ǰָ��������������ݣ�DLL �ڲ������ڴ棬�ⲿ�������FreeStrokeList�ͷš�*/
HANSADVDLL_API int __stdcall HS_PreprocessMark();

HANSADVDLL_API int __stdcall HS_MarkStorkeList(int nType,const LPVOID pData);
/*��괫�����������*/

HANSADVDLL_API int __stdcall HS_FreeStrokeList(LPVOID pData);
/*�ͷ��ڲ���������������*/




/*****************************************************************
���ĵ����ص�������
	HS_BeginMulDownload();
	HS_DownSingleDoc(_T("D:\\HS1.hs"), 0x10);
	HS_DownSingleDoc(_T("D:\\HS2.hs"), 0x20);
	..........................................
	..........................................
	HS_EndMulDownload();


	���ع�����HWND������Ϊ�գ���������:
	LRESULT CHansAdvInterfaceTestDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
	{
	switch(wParam)
	{
	case MARK_INFO: // ���ؽ���
	m_progressDown.SetPos(lParam);
	break;
	case MARK_TEND: // һ���ĵ����سɹ�,��ǰ�ռ�
	UINT uSpaceRate = lParam;
	CString str;
	str.Format(_T("��ǰ�ĵ��ɹ����ص���, �洢�ռ�ռ�� %d%% !"),uSpaceRate);
	AfxMessageBox(str);
	break;
	}

	return 0;
	}

*****************************************************************/

/*****************************************************************
���ƣ� HS_BeginMulDownload
���ã� ��ʼ���ĵ���������
����˵��:
			hwnd�������ع��̽�����Ϣ,��ʹ��Ĭ�ϲ���NULL
****************************************************************/
HANSADVDLL_API int __stdcall HS_BeginMulDownload(HWND hWnd = NULL);


/*****************************************************************
���ƣ� HS_DownSingleDoc
���ã� �ĵ���������
����˵��:
			TCHAR *szFile���Ѵ򿪵��ĵ�ȫ���ƣ�����·���ͺ�׺����
			WORD  uIO��	�ĵ���ӦIO�ź�,ʹ��I4 I5 I6 I7
****************************************************************/
HANSADVDLL_API int __stdcall HS_DownSingleDoc( TCHAR *szFile, WORD uIO);

/*****************************************************************
���ƣ� HS_EndMulDownload
���ã�	���ĵ��������ؽ���
			����֮ǰ����ȷ��������HS_BeginMulDownload�����سɹ�
����˵��:
			nDownSuccessΪ���سɹ����ص��ĵ�����
			pCallMarkMsg ΪNULL�����ʱ���л����ѻ�״̬����Ϊ����Ϊ���ѻ�����λ���ص���Ч
****************************************************************/
HANSADVDLL_API int __stdcall HS_EndMulDownload(UINT &nDownSuccess, HS_MARKMSG_CALLBACK pCallMarkMsg = NULL);


/*****************************************************************
���ƣ�	HS_SetCurrentDocOffline
���ã�	���ĵ���������
			�˺���ֻ���ص�ǰ�ĵ����ݣ�Ҫ���ش򿪵���һ���ĵ�������ʹ��HS_SetCurDoc�����ĵ�
����˵��:
			hwnd�������ع��̽�����Ϣ,��ʹ��Ĭ�ϲ���NULL
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetCurrentDocOffline(HWND hwnd = NULL);


/*****************************************************************
���ƣ�	HS_SetWorkStatus
���ã�	�л����Ĺ���״̬
����˵����
//0 �л������ĵ��ѻ�״̬,pCallMarkMsgΪNULL
//1 ����,pCallMarkMsgΪNULL
//2 ���ĵ��ѻ�,pCallMarkMsgΪNULL
//3 ���ѻ�״̬,���pCallMarkMsg��
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetWorkStatus(UINT nStatus, HS_MARKMSG_CALLBACK pCallMarkMsg);


/*****************************************************************
���ƣ�	HS_SetOnline
���ã�	�л�������״̬
����˵��:
			��
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetOnline();


/*****************************************************************
���ƣ�	HS_ClearFlashData
���ã�	����������
����˵��:
			��
****************************************************************/
HANSADVDLL_API int __stdcall HS_ClearFlashData();


/*****************************************************************
���ƣ�	HS_SoftReset
���ã�	��λ�������ϵͳ����״̬
����˵��:
			��
****************************************************************/
HANSADVDLL_API int __stdcall HS_SoftReset();

/*****************************************************************
���ƣ�	HS_SetGalvHomeSts
���ã�	���ô����ɺ����Ƿ���ת��ָ��HOMEλ�ã�TRUE-��ת��FALSE-����ת
����˵��:
			��
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetGalvHomeSts(BOOL bHome);

#endif //HANS_ADVINTERFACEDLL_6332F9B4_82E7_4403_B791_4F7210CD211B