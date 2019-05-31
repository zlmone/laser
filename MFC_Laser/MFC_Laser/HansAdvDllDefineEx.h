#ifndef HANS_ADVINTERFACEDLLEX_6332F9B4_82E7_4403_B791_4F7210CD211B
#define HANS_ADVINTERFACEDLLEX_6332F9B4_82E7_4403_B791_4F7210CD211B

//导入导出定义
#ifndef  HANSADVINTERFACE_EXPORT
	#define  HANSADVDLL_API	extern "C" __declspec(dllimport)
#else
	#define  HANSADVDLL_API	extern "C" __declspec(dllexport)
#endif


/****************************************************************
* B、高级应用，部分应用需要授权*
****************************************************************/


HANSADVDLL_API int __stdcall HS_SendCustomCmd(__int64 inCmd,__int64 outCmd);
/*作用：上位机主动和和下位机通讯，详见内部使用文档。
[注]：本接口仅供软件中心内部使用，其它部门不予开放。*/

typedef void (*HS_USERDEFINE_MSG_CALLBACK)(DWORD dwCode,__int64 value);
HANSADVDLL_API int __stdcall HS_SetCustomMsgCall(HS_USERDEFINE_MSG_CALLBACK pCallBack);
/*作用：下位机主动向上位机通讯用。
[注]：本接口仅供软件中心内部使用，其它部门不予开放。*/

/*以下接口为特制接口，不对外开放*/
HANSADVDLL_API int __stdcall  HS_GetTextExtInfo(TCHAR *szObjName, LPVOID lpTextExtInfo);
HANSADVDLL_API int __stdcall  HS_SetTextExtInfo(TCHAR *szObjName, LPVOID lpTextExtInfo);
HANSADVDLL_API int __stdcall  HS_GetVarTextList(TCHAR *szObjList, int* pCount);
HANSADVDLL_API int __stdcall  HS_GetDocExtInfo( LPVOID lpDocExtInfo );
HANSADVDLL_API int __stdcall  HS_SetDocExtInfo( LPVOID lpDocExtInfo );


/*卡操作接口,不对外开放*/
HANSADVDLL_API int __stdcall HS_GetCardNumber(DWORD* pNumber);
HANSADVDLL_API int __stdcall HS_GetLayerParaType(int &nType);
HANSADVDLL_API int __stdcall HS_GetCardKey(TCHAR* pDogStr, int &nBufSize);

#endif //HANS_ADVINTERFACEDLLEX_6332F9B4_82E7_4403_B791_4F7210CD211B

