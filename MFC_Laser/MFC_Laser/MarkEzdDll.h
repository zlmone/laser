#pragma once
class MarkEzdDll
{
public:
	MarkEzdDll();
	~MarkEzdDll();

	//HINSTANCE hEzdDLL = ::LoadLibraryEx(L"MarkEzd.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	HINSTANCE hEzdDLL = ::LoadLibrary(L"MarkEzd.dll");

	/*
	if (hEzdDLL == NULL)
	{
	MessageBox(_T("无法载入dll"), NULL);
	}
	*/

	//初始化lmc1控制卡
	//输入参数: strEzCadPath  EzCad软件的执行路径
	//          bTestMode = TRUE 表示测试模式  bTestMode = FALSE 表示正常模式
	//          pOwenWnd      表示父窗口对象，如果需要实现停止打标，则系统会从此窗口截取消息 
	typedef int(*LMC1_INITIAL)(TCHAR * strEzCadPath, BOOL bTestMode, HWND hOwenWnd); //定义函数指针

	LMC1_INITIAL lmc1_Initial = (LMC1_INITIAL)GetProcAddress(hEzdDLL, "lmc1_Initial");

	//关闭lmc1控制卡
	typedef int(*LMC1_CLOSE)();

	LMC1_CLOSE lmc1_Close = (LMC1_CLOSE)GetProcAddress(hEzdDLL, "lmc1_Close");

	//载入ezd文件，并清除数据库所有对象
	//输入参数: strFileName  EzCad文件名称
	typedef int(*LMC1_LOADEZDFILE)(TCHAR* strFileName);

	LMC1_LOADEZDFILE lmc1_LoadEzdFile = (LMC1_LOADEZDFILE)GetProcAddress(hEzdDLL, "lmc1_LoadEzdFile");

	//标刻当前数据库里的所有数据
	//输入参数: bFlyMark = TRUE 使能飞动打标  bFlyMark = FALSE 使能飞动打标
	typedef int(*LMC1_MARK)(BOOL bFlyMark);

	LMC1_MARK lmc1_Mark = (LMC1_MARK)GetProcAddress(hEzdDLL, "lmc1_Mark");

	//更改当前数据库里的指定文本对象的文本
	//输入参数: strTextName     要更改内容的文本对象的名称
	//          strTextNew      新的文本内容
	typedef int(*LMC1_CHANGETEXTBYNAME)(TCHAR* strTextName, TCHAR*strTextNew);

	LMC1_CHANGETEXTBYNAME lmc1_ChangeTextByName = (LMC1_CHANGETEXTBYNAME)GetProcAddress(hEzdDLL, "lmc1_ChangeTextByName");

	// 读lmc1的输入端口
	//输入参数: 读入的输入端口的数据
	typedef int(*LMC1_READPORT)(WORD& data);

	LMC1_READPORT lmc1_ReadPort = (LMC1_READPORT)GetProcAddress(hEzdDLL, "lmc1_ReadPort");




	typedef int(*LMC1_WRITEPORT)(WORD data);

	LMC1_WRITEPORT lmc1_WritePort = (LMC1_WRITEPORT)GetProcAddress(hEzdDLL, "lmc1_WritePort");

	//得到指定对象的最大最小坐标,如果pEntName==NULL表示读取数据库所有对象的最大最小坐标
	typedef int(*LMC1_GETENTSIZE)(TCHAR* pEntName, double& dMinx, double& dMiny, double& dMaxx, double& dMaxy, double& dZ);

	LMC1_GETENTSIZE lmc1_GetEntSize = (LMC1_GETENTSIZE)GetProcAddress(hEzdDLL, "lmc1_GetEntSize");

};

