#pragma once

#import "msado15.dll"  no_namespace rename("EOF", "adoEOF")


class ADOManage
{
public:
	ADOManage();
	~ADOManage();

	//�Զ������
public:
	_ConnectionPtr m_pConnection;    //���Ӳ���
	_CommandPtr m_pCommand;          //SQL�������
	_RecordsetPtr m_pRecordSet;      //��¼������
	_RecordsetPtr m_pRecordSet1;      //��¼������1

	//��ȡ���ݿ���Ϣ���������������ݿ��ʱ����Ȼ�ȡ��Щ����������,���û�д��ⲿ��ȡ�Ǿ������ʼ�������Զ���һ��
public:
	CString m_SourceIp;  //������IP
	CString m_Port;        //�������˿�
	CString m_UserName;    //�û���
	CString m_Password;    //����
	void GetDBINFO();//��ȡ���ݿ���Ϣ����


	//���¶����Զ��庯��
public:
	//���ݿ����Ӻ���Դ�ͷź���
	bool ConnSQL();
	//�������ݿ⺯��
	bool ConntestdbSQL();//���ݿ�����������������������
	bool CloseDB();//�ر����ݿ�
	bool CloseAll();//�ͷ����ݿ���Դ
	bool CloseAll1();//�ͷ����ݿ���Դ1

	//��ͨ�ú���
	_RecordsetPtr MOBAN(CString str1, CString str2);//ģ�溯��

	//��ʼ������ģ�������õĺ���
	_RecordsetPtr GetZhiDan();//��ȡ������
	_RecordsetPtr GetIMEIByZhiDan(CString strzhidan);//���ݶ����Ż�ȡIMEI��
	int GetIMEICurrentAndTemplate(CString strzhidan);
	void GetSoftModelAndVersion(CString & model, CString & version, CString zhidan);
	//��ȡ��ǰ�ص��imei�ź�ģ��·��
	int InsetrLdRecordParam(CString strzhidan, CString strtemplate, CString strcurrentimei, CString checkcode, CString sncurrent, CString LdImei);
	int InsetrDZLdRecordParam(CString strzhidan, CString strtemplate, CString checkcode, CString LdImei, CString increasingnum, CString haverelative, CString relativeziduan, CString bits, CString imeipre);

	//��¼��ǰIMEI�ź��ص�ģ��
	_RecordsetPtr JudgeZhiDan(CString strzhidan);//�ж϶������Ƿ����
	int GetImeicount(CString strzhidan, CString imeistart, CString imeiend);//��ȡIMEI����
	
	//��дģ�麯��
	//������д���ݵ���
	_RecordsetPtr InsertBurningData(CString strzhidan, CString strimei, CString ComputerIp, CString burningresult);
	_RecordsetPtr UpdateBurningData(CString strimei, CString Rid, CString burnResult, CString ComputerIp);

	int checkIMEIbyID(CString chipid);

	CString getRelativeIMEIX(CString str, CString paramIMEI);

	_RecordsetPtr saveData(CString data, CString IMEI, CString ziduan);

	CString getNewSheetDataDB(CString paramIMEI, CString ziduan);

	int CheckIMEIInRelative(CString strImei);

	int CheckIMEIInNewRelative(CString strImei);

	_RecordsetPtr insertToRelative(CString data, CString IMEI, CString ziduan);

	_RecordsetPtr UpdateIMEIToRelative(CString data, CString IMEI, CString ziduan);

	_RecordsetPtr UpdateICCIDToRelative(CString data, CString IMEI, CString ziduan);









	
	
	_RecordsetPtr updateDatatoCoupleDB(CString IMEI, CString chipid);

	_RecordsetPtr insertToMainSheetDB(CString IMEI, CString result, CString chipid, CString version, CString machinename);

	_RecordsetPtr UpdateToMainSheetDB(CString IMEI, CString result, CString chipid, CString version, CString machinename);

	

	

	void GetTestCommandAndRec(CString softWare, CString TestCommand, CString CommandRec);

	CString adoGetStation(CString softWare);

	CString adoGetMachineName(CString zhidan);

	//��д�����µ�ǰλ����¼��
	_RecordsetPtr UpdateCurrnetImei(CString strimei, CString zhidan);

	//�ص�ϵͳģ�����ú���
	_RecordsetPtr UpdateIMEICurrent(CString strzhidan, CString imeicurrent);//�洢��ǰ�ص��imei��
	_RecordsetPtr AddNew(CString strpicp, CString strzhidan, CString strimei);//�����ص�������ݵ��ص����ݱ�
	_RecordsetPtr UpdateLdData(CString strimei, CString StrSn);
	_RecordsetPtr InsertLdData(CString strimei, CString Computerip, CString zhidan, CString softmodel, CString version);

	CString DZGetRelateData(CString strImei, CString ziduan);

	_RecordsetPtr GetRela1_13(CString IMEI);

	
	//�ص��������ݲ���
	int CheckIMEIExit(CString strImei);
	CString checkAndGetIMEIbyID(CString chipid);
	CString CheckRIDExit(CString strImei);
	_RecordsetPtr InsertBurningData(CString strzhidan, CString strimei, CString ComputerIp, CString burningresult, CString Rid);
	//�ж�IMEI�Ƿ����
	
	int CheckLdIMEIExitLM(CString strImei);
	int CheckLdIMEIExitTR(CString strImei);
	int CheckLdIMEIExit(CString strImei);
	CString GetRelateSN(CString strImei);
	void GetRelativeIMEI2_IMEI13(CString strImei);
	int CheckLdSNExit(CString strImei, CString zhidan);
	//�ж��ص�IMEI�Ƿ����
	_RecordsetPtr UpdateLdSnAndImei(CString strsn, CString strimei, CString zhidan); //�ص������SN��IMEI����¼��

	_RecordsetPtr UpdateSubIMEISegment(CString zhidan, CString val);


	_RecordsetPtr UpdateLdSn(CString strsn, CString zhidan);

	_RecordsetPtr UpdateReLdData(CString strimei);
	_RecordsetPtr insertLdSnAndImei(CString strsn, CString strimei, CString zhidan);
	int CheckLdzhidanExit(CString zhidan);
	//�Ա�ģ�麯��
	_RecordsetPtr UpdateCompareData(CString strimei, CString result);//�ԱȺ�������ݲ���
	int CheckCompareIMEIExit(CString strImei);//�ж϶Ա�IMEI�Ƿ����

	//���ݿ��ѯģ�����õĺ���
	_RecordsetPtr ShowInsertImeiByOrderNumber(CString ordernumber);//����Ӧ�Ķ��������Ѿ������IMEIչʾ����
	_RecordsetPtr ShowRepeatImeiByOrderNumber(CString ordernumber);//����Ӧ�Ķ��������Ѿ��ظ���IMEIչʾ����
	_RecordsetPtr ShowUnImeiByOrderNumber(CString ordernumber, CString imeistrat, CString imeiend);//����Ӧ�Ķ�������δ�����IMEIչʾ����
	//ͨ�ú���
	_RecordsetPtr GetRst();//�ɵ�ĳ�ű������õ�
	CString GetTime();//��ȡ��ǰϵͳʱ��
	CString CreateIMEI15(CString imei);
	CString GetSubSegment(CString zhidan);
	//������IMEIУ��λ�õ�
public:
	CString IMEI2Str, IMEI3Str, IMEI4Str, IMEI5Str, IMEI6Str, IMEI7Str, IMEI8Str, IMEI9Str, IMEI10Str, IMEI11Str\
		, IMEI12Str, IMEI13Str;
};

