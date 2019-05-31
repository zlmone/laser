#ifndef HANS_ADVINTERFACEDLL_6332F9B4_82E7_4403_B791_4F7210CD211B
#define HANS_ADVINTERFACEDLL_6332F9B4_82E7_4403_B791_4F7210CD211B

//导入导出定义
#ifndef  HANSADVINTERFACE_EXPORT
	#define  HANSADVDLL_API	extern "C" __declspec(dllimport)
#else
	#define  HANSADVDLL_API	extern "C" __declspec(dllexport)
#endif

#define HL_ERR_SUCCESS		0 			//成功
#define HL_ERR_PROGRUN		1 			//另一个程序在运行
#define HL_ERR_PATH			2			//路径不正确,可能程序不在软件安装目录下运行
#define HL_ERR_FAILOPEN		3 			//初始化失败，请检查狗和卡的连接情况
#define HL_ERR_NOINITIAL	4 			//未初始化
#define HL_ERR_WARNING		5 			//报警
#define HL_ERR_OVERTIME		6 			//命令超时
#define HL_ERR_READFILE		7 			//无法读取文件
#define HL_ERR_NOFINDFONT	8 			//指定的字体不存在
#define HL_ERR_NOPEN		9 			//指定的层号不存在
#define HL_ERR_NOOBJECT		10 			//没找到指定的对象
#define HL_ERR_PARAM		11 			//参数非法
#define HL_ERR_STATUS		12 			//错误的状态,当前状态下不能执行此操作
#define HL_ERR_ALLOCMEMORY	13 			//分配内存失败
#define HL_ERR_OVERRANGE	14 			//打标范围超限
#define HL_ERR_BUFFERSIZE	15			//缓冲区不够
#define HL_ERR_NULLPTR		16			//空指针
#define HL_ERR_NODOC		17			//未找到指定文档
#define HL_ERR_REOPEN		18			//指定文档已经被打开
#define HL_ERR_EXECFAIL     19          //下位机命令执行失败
#define HL_ERR_DOWNFAIL		20			// 多文档数据下载失败
#define HL_ERR_MULDOWNIO	21			// 不能存相同的文档IO托管值
#define HL_ERR_ALREADYDOWN  22			// 数据已下载
#define HL_ERR_CALLFUN		23			// 调用函数顺序不正确
#define HL_ERR_PREPROCESS   24			// 生成线条失败
#define HL_ERR_NOCALIFILE   25			// 找不到校正文件，请先启动cali.exe进行校正

#define HL_ERR_UNKNOWN		100			//未知错误

#define  WM_TOUCH_MSG    (WM_USER+1095)  //主动检测脚踏触发消息

#define DFT_DOC_NAME _T("default")

///////////////////文档数据下载过程消息响应////////////////////
#define WM_MARK_END				( WM_USER+5 )
#define MARK_TEND	100
#define MARK_INFO		103

// 多文档脱机下载回调，下位机标记完文档后回传标记的文档号(从0开始对下载的文档编号)，和标记时间
typedef void (*HS_MARKMSG_CALLBACK)(int nID, UINT64 nMarkTime);
/////////////////////////////////////////////////////////////////
/****************************************************************
* A、通用部分，所有用户均可使用*
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
// 以下几个为华为订制接口
/*****************************************************************
名称: HS_GetLayerByObjName
作用: 根据对象名称获取图层号	
参数说明:
			szObjName(输入):对象名
			nLayer(输出):图层号
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetLayerByObjName(TCHAR *szObjName,int *nLayer);


/*****************************************************************
名称: HS_SetLayerByObjName
作用: 根据对象名称设置对象图层	
参数说明:
			szObjName(输入):对象名
			nLayer(输入):图层号
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
******************************************************************/
HANSADVDLL_API int __stdcall HS_SetLayerByObjName(TCHAR *szObjName,int nLayer);


/*****************************************************************
名称: HS_GetFillParaByObjName
作用: 根据对象名称获取填充参数	
参数说明:
			szObjName(输入):对象名
			lpPara(输出):填充结构体HATCHPARA的指针(详细定义见baseHatch.h)
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetFillParaByObjName(TCHAR *szObjName,LPVOID lpPara);

/*****************************************************************
名称: HS_SetFillParaByObjName
作用: 根据对象名称设置填充参数	
参数说明:
			szObjName(输入):对象名
			lpPara(输入):填充结构体HATCHPARA的指针(详细定义见baseHatch.h)
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
******************************************************************/
HANSADVDLL_API int __stdcall HS_SetFillParaByObjName(TCHAR *szObjName,LPVOID lpPara);

/*****************************************************************
名称: HS_GetFontFaceByObjName
作用: 根据对象名称获取字型(仅限于ttf文本)
参数说明:
			szObjName(输入):对象名
			szFontName(输出):字体名称如"宋体"等(详细定义见baseHatch.h),数组长度建议256,不能太小
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
******************************************************************/
HANSADVDLL_API int __stdcall HS_GetFontFaceByObjName(TCHAR *szObjName,TCHAR* szFontName);

/*****************************************************************
名称: HS_SetFontFaceByObjName
作用: 根据对象名称设置字型(仅限于ttf文本)
参数说明:
			szObjName(输入):对象名
			szFontName(输入):字体名称如"宋体"等(详细定义见baseHatch.h)
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
******************************************************************/
HANSADVDLL_API int __stdcall HS_SetFontFaceByObjName(TCHAR *szObjName,TCHAR* szFontName);

/*****************************************************************
名称: HS_GetObjPropertyByName
作用: 根据对象名称获取对象位置
参数说明:
			szObjName(输入):对象名
			pLeft(输出):左边位置
			pTop(输出):上面位置
			pRight(输出):右边位置
			pBottom(输出):下面位置
返回值:
			0:成功
			非0:失败(可通过HS_GetLastError获取错误信息)
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
名称： HS_SetMotionPara
作用：常用运动参数设置，立即生效，不保存到配置文件(调用时，对于没用到的参数建议传入配置文件默认值)
参数说明:
nAxis:			轴号
dbEmptySpeed:	空程速度(平移轴单位为mm/s,旋转轴单位为度/s)
dbWorkSpeed:	工作速度(平移轴单位为mm/s,旋转轴单位为度/s)
dbHomeHighSpeed:平移回零速度(mm/s)
dbHomeLowSpeed: 平移回零起动速度(mm/s)
dbEmptyTAcc:	空程加速时间(s)
dbWorkTAcc:		工作加速时间(s)
dbHomeTAcc:		回零减速时间(s)
\****************************************************************/
HANSADVDLL_API int  __stdcall HS_SetMotionPara(int nAxis, double dbEmptySpeed, double dbWorkSpeed, double dbHomeHighSpeed, double dbHomeLowSpeed, double dbEmptyTAcc, double dbWorkTAcc, double dbHomeTAcc);

/*****************************************************************\
名称： SetMotionSysPara
作用： 运动参数设置，立即生效，保存到配置文件
参数说明:
nAxis:			轴号
nPPR:			每转脉冲数
dbPitch:		螺间距
nDelay:			运动完成后延时
nAxisType:		运动轴类型 0 平移轴， 1 旋转轴
nMotorDire:		电机转动方向 1正向， -1 反向
dbEmptySpeed:	空程速度
dbWorkSpeed:	工作速度
dbEmptyTAcc:	空程加速时间
dbWorkTAcc:		工作加速时间
bHardOrgEnable:	机械原点使能
bAutoHome:		是否自动回零
bEnabled:		是否使能
\****************************************************************/
HANSADVDLL_API int  __stdcall HS_SetMotionSysPara(int nAxis,int nPPR, double dbPitch, int nDelay,int nAxisType,int nMotorDire,double	dbEmptySpeed,
												 double dbWorkSpeed, double dbEmptyTAcc,double dbWorkTAcc,BOOL bHardOrgEnable, BOOL bAutoHome, BOOL bEnabled);

/*****************************************************************\
名称： HS_SetMotionSysPara2
作用： 运动参数设置，立即生效，保存到配置文件
参数说明:
nAxis:  轴号
pParam: MOTIONPARA结构体指针
\****************************************************************/
HANSADVDLL_API int __stdcall HS_SetMotionSysPara2(int nAxis,LPVOID pPara);



HANSADVDLL_API int  __stdcall HS_GetLayerColor(int nLayerNo, COLORREF *pColor);

HANSADVDLL_API int __stdcall HS_GetStrokeList(TCHAR *szObjName, LPVOID pData);
/*获取当前指定对象的线条数据，DLL 内部分配内存，外部用完后用FreeStrokeList释放。*/
HANSADVDLL_API int __stdcall HS_PreprocessMark();

HANSADVDLL_API int __stdcall HS_MarkStorkeList(int nType,const LPVOID pData);
/*打标传入的线条数据*/

HANSADVDLL_API int __stdcall HS_FreeStrokeList(LPVOID pData);
/*释放内部创建的线条数据*/




/*****************************************************************
多文档下载调用流程
	HS_BeginMulDownload();
	HS_DownSingleDoc(_T("D:\\HS1.hs"), 0x10);
	HS_DownSingleDoc(_T("D:\\HS2.hs"), 0x20);
	..........................................
	..........................................
	HS_EndMulDownload();


	下载过程需HWND参数不为空，交互处理:
	LRESULT CHansAdvInterfaceTestDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
	{
	switch(wParam)
	{
	case MARK_INFO: // 下载进度
	m_progressDown.SetPos(lParam);
	break;
	case MARK_TEND: // 一个文档下载成功,当前空间
	UINT uSpaceRate = lParam;
	CString str;
	str.Format(_T("当前文档成功下载到卡, 存储空间占用 %d%% !"),uSpaceRate);
	AfxMessageBox(str);
	break;
	}

	return 0;
	}

*****************************************************************/

/*****************************************************************
名称： HS_BeginMulDownload
作用： 开始多文档数据下载
参数说明:
			hwnd用于下载过程接收消息,可使用默认参数NULL
****************************************************************/
HANSADVDLL_API int __stdcall HS_BeginMulDownload(HWND hWnd = NULL);


/*****************************************************************
名称： HS_DownSingleDoc
作用： 文档数据下载
参数说明:
			TCHAR *szFile：已打开的文档全名称（包含路径和后缀名）
			WORD  uIO：	文档对应IO信号,使用I4 I5 I6 I7
****************************************************************/
HANSADVDLL_API int __stdcall HS_DownSingleDoc( TCHAR *szFile, WORD uIO);

/*****************************************************************
名称： HS_EndMulDownload
作用：	多文档数据下载结束
			调用之前必须确保调用了HS_BeginMulDownload并返回成功
参数说明:
			nDownSuccess为返回成功下载的文档数量
			pCallMarkMsg 为NULL则结束时会切换到脱机状态，不为空则为半脱机，上位机回调有效
****************************************************************/
HANSADVDLL_API int __stdcall HS_EndMulDownload(UINT &nDownSuccess, HS_MARKMSG_CALLBACK pCallMarkMsg = NULL);


/*****************************************************************
名称：	HS_SetCurrentDocOffline
作用：	单文档数据下载
			此函数只下载当前文档数据，要下载打开的另一个文档必须先使用HS_SetCurDoc设置文档
参数说明:
			hwnd用于下载过程接收消息,可使用默认参数NULL
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetCurrentDocOffline(HWND hwnd = NULL);


/*****************************************************************
名称：	HS_SetWorkStatus
作用：	切换卡的工作状态
参数说明：
//0 切换到单文档脱机状态,pCallMarkMsg为NULL
//1 联机,pCallMarkMsg为NULL
//2 多文档脱机,pCallMarkMsg为NULL
//3 半脱机状态,结合pCallMarkMsg用
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetWorkStatus(UINT nStatus, HS_MARKMSG_CALLBACK pCallMarkMsg);


/*****************************************************************
名称：	HS_SetOnline
作用：	切换到联机状态
参数说明:
			无
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetOnline();


/*****************************************************************
名称：	HS_ClearFlashData
作用：	擦除卡数据
参数说明:
			无
****************************************************************/
HANSADVDLL_API int __stdcall HS_ClearFlashData();


/*****************************************************************
名称：	HS_SoftReset
作用：	软复位，可清除系统报警状态
参数说明:
			无
****************************************************************/
HANSADVDLL_API int __stdcall HS_SoftReset();

/*****************************************************************
名称：	HS_SetGalvHomeSts
作用：	设置打标完成后振镜是否跳转到指定HOME位置，TRUE-跳转，FALSE-不跳转
参数说明:
			无
****************************************************************/
HANSADVDLL_API int __stdcall HS_SetGalvHomeSts(BOOL bHome);

#endif //HANS_ADVINTERFACEDLL_6332F9B4_82E7_4403_B791_4F7210CD211B