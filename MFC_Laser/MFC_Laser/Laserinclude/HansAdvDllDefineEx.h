#ifndef HANS_ADVINTERFACEDLLEX_6332F9B4_82E7_4403_B791_4F7210CD211B
#define HANS_ADVINTERFACEDLLEX_6332F9B4_82E7_4403_B791_4F7210CD211B

//���뵼������
#ifndef  HANSADVINTERFACE_EXPORT
	#define  HANSADVDLL_API	extern "C" __declspec(dllimport)
#else
	#define  HANSADVDLL_API	extern "C" __declspec(dllexport)
#endif


/****************************************************************
* B���߼�Ӧ�ã�����Ӧ����Ҫ��Ȩ*
****************************************************************/


HANSADVDLL_API int __stdcall HS_SendCustomCmd(__int64 inCmd,__int64 outCmd);
/*���ã���λ�������ͺ���λ��ͨѶ������ڲ�ʹ���ĵ���
[ע]�����ӿڽ�����������ڲ�ʹ�ã��������Ų��迪�š�*/

typedef void (*HS_USERDEFINE_MSG_CALLBACK)(DWORD dwCode,__int64 value);
HANSADVDLL_API int __stdcall HS_SetCustomMsgCall(HS_USERDEFINE_MSG_CALLBACK pCallBack);
/*���ã���λ����������λ��ͨѶ�á�
[ע]�����ӿڽ�����������ڲ�ʹ�ã��������Ų��迪�š�*/

/*���½ӿ�Ϊ���ƽӿڣ������⿪��*/
HANSADVDLL_API int __stdcall  HS_GetTextExtInfo(TCHAR *szObjName, LPVOID lpTextExtInfo);
HANSADVDLL_API int __stdcall  HS_SetTextExtInfo(TCHAR *szObjName, LPVOID lpTextExtInfo);
HANSADVDLL_API int __stdcall  HS_GetVarTextList(TCHAR *szObjList, int* pCount);
HANSADVDLL_API int __stdcall  HS_GetDocExtInfo( LPVOID lpDocExtInfo );
HANSADVDLL_API int __stdcall  HS_SetDocExtInfo( LPVOID lpDocExtInfo );


/*�������ӿ�,�����⿪��*/
HANSADVDLL_API int __stdcall HS_GetCardNumber(DWORD* pNumber);
HANSADVDLL_API int __stdcall HS_GetLayerParaType(int &nType);
HANSADVDLL_API int __stdcall HS_GetCardKey(TCHAR* pDogStr, int &nBufSize);

#endif //HANS_ADVINTERFACEDLLEX_6332F9B4_82E7_4403_B791_4F7210CD211B

