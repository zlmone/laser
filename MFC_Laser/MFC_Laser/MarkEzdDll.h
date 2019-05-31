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
	MessageBox(_T("�޷�����dll"), NULL);
	}
	*/

	//��ʼ��lmc1���ƿ�
	//�������: strEzCadPath  EzCad�����ִ��·��
	//          bTestMode = TRUE ��ʾ����ģʽ  bTestMode = FALSE ��ʾ����ģʽ
	//          pOwenWnd      ��ʾ�����ڶ��������Ҫʵ��ֹͣ��꣬��ϵͳ��Ӵ˴��ڽ�ȡ��Ϣ 
	typedef int(*LMC1_INITIAL)(TCHAR * strEzCadPath, BOOL bTestMode, HWND hOwenWnd); //���庯��ָ��

	LMC1_INITIAL lmc1_Initial = (LMC1_INITIAL)GetProcAddress(hEzdDLL, "lmc1_Initial");

	//�ر�lmc1���ƿ�
	typedef int(*LMC1_CLOSE)();

	LMC1_CLOSE lmc1_Close = (LMC1_CLOSE)GetProcAddress(hEzdDLL, "lmc1_Close");

	//����ezd�ļ�����������ݿ����ж���
	//�������: strFileName  EzCad�ļ�����
	typedef int(*LMC1_LOADEZDFILE)(TCHAR* strFileName);

	LMC1_LOADEZDFILE lmc1_LoadEzdFile = (LMC1_LOADEZDFILE)GetProcAddress(hEzdDLL, "lmc1_LoadEzdFile");

	//��̵�ǰ���ݿ������������
	//�������: bFlyMark = TRUE ʹ�ܷɶ����  bFlyMark = FALSE ʹ�ܷɶ����
	typedef int(*LMC1_MARK)(BOOL bFlyMark);

	LMC1_MARK lmc1_Mark = (LMC1_MARK)GetProcAddress(hEzdDLL, "lmc1_Mark");

	//���ĵ�ǰ���ݿ����ָ���ı�������ı�
	//�������: strTextName     Ҫ�������ݵ��ı����������
	//          strTextNew      �µ��ı�����
	typedef int(*LMC1_CHANGETEXTBYNAME)(TCHAR* strTextName, TCHAR*strTextNew);

	LMC1_CHANGETEXTBYNAME lmc1_ChangeTextByName = (LMC1_CHANGETEXTBYNAME)GetProcAddress(hEzdDLL, "lmc1_ChangeTextByName");

	// ��lmc1������˿�
	//�������: ���������˿ڵ�����
	typedef int(*LMC1_READPORT)(WORD& data);

	LMC1_READPORT lmc1_ReadPort = (LMC1_READPORT)GetProcAddress(hEzdDLL, "lmc1_ReadPort");




	typedef int(*LMC1_WRITEPORT)(WORD data);

	LMC1_WRITEPORT lmc1_WritePort = (LMC1_WRITEPORT)GetProcAddress(hEzdDLL, "lmc1_WritePort");

	//�õ�ָ������������С����,���pEntName==NULL��ʾ��ȡ���ݿ����ж���������С����
	typedef int(*LMC1_GETENTSIZE)(TCHAR* pEntName, double& dMinx, double& dMiny, double& dMaxx, double& dMaxy, double& dZ);

	LMC1_GETENTSIZE lmc1_GetEntSize = (LMC1_GETENTSIZE)GetProcAddress(hEzdDLL, "lmc1_GetEntSize");

};

