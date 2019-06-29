#pragma once

#import "msado15.dll"  no_namespace rename("EOF", "adoEOF")


class ADOManage
{
public:
	ADOManage();
	~ADOManage();

	//自定义参数
public:
	_ConnectionPtr m_pConnection;    //连接参数
	_CommandPtr m_pCommand;          //SQL命令参数
	_RecordsetPtr m_pRecordSet;      //记录集参数
	_RecordsetPtr m_pRecordSet1;      //记录集参数1

	//获取数据库信息，当我们连接数据库的时候就先获取这些数据再连接,如果没有从外部获取那就在类初始化那里自定义一下
public:
	CString m_SourceIp;  //服务器IP
	CString m_Port;        //服务器端口
	CString m_UserName;    //用户名
	CString m_Password;    //密码
	void GetDBINFO();//获取数据库信息函数


	//以下都是自定义函数
public:
	//数据库连接和资源释放函数
	bool ConnSQL();
	//连接数据库函数
	bool ConntestdbSQL();//数据库配置那里用来测试连接用
	bool CloseDB();//关闭数据库
	bool CloseAll();//释放数据库资源
	bool CloseAll1();//释放数据库资源1

	//非通用函数
	_RecordsetPtr MOBAN(CString str1, CString str2);//模版函数

	//初始化配置模块中所用的函数
	_RecordsetPtr GetZhiDan();//获取订单号
	_RecordsetPtr GetIMEIByZhiDan(CString strzhidan);//根据订单号获取IMEI号
	int GetIMEICurrentAndTemplate(CString strzhidan);
	void GetSoftModelAndVersion(CString & model, CString & version, CString zhidan);
	//获取当前镭雕的imei号和模版路径
	int InsetrLdRecordParam(CString strzhidan, CString strtemplate, CString strcurrentimei, CString checkcode, CString sncurrent, CString LdImei);
	int InsetrDZLdRecordParam(CString strzhidan, CString strtemplate, CString checkcode, CString LdImei, CString increasingnum, CString haverelative, CString relativeziduan, CString bits, CString imeipre);

	//记录当前IMEI号和镭雕模板
	_RecordsetPtr JudgeZhiDan(CString strzhidan);//判断订单号是否存在
	int GetImeicount(CString strzhidan, CString imeistart, CString imeiend);//获取IMEI计数
	
	//烧写模块函数
	//插入烧写数据到表
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

	//烧写完后更新当前位到记录表
	_RecordsetPtr UpdateCurrnetImei(CString strimei, CString zhidan);

	//镭雕系统模块所用函数
	_RecordsetPtr UpdateIMEICurrent(CString strzhidan, CString imeicurrent);//存储当前镭雕的imei号
	_RecordsetPtr AddNew(CString strpicp, CString strzhidan, CString strimei);//插入镭雕输出数据到镭雕数据表
	_RecordsetPtr UpdateLdData(CString strimei, CString StrSn);
	_RecordsetPtr InsertLdData(CString strimei, CString Computerip, CString zhidan, CString softmodel, CString version);

	CString DZGetRelateData(CString strImei, CString ziduan);

	_RecordsetPtr GetRela1_13(CString IMEI);

	
	//镭雕后更新数据操作
	int CheckIMEIExit(CString strImei);
	CString checkAndGetIMEIbyID(CString chipid);
	CString CheckRIDExit(CString strImei);
	_RecordsetPtr InsertBurningData(CString strzhidan, CString strimei, CString ComputerIp, CString burningresult, CString Rid);
	//判断IMEI是否存在
	
	int CheckLdIMEIExitLM(CString strImei);
	int CheckLdIMEIExitTR(CString strImei);
	int CheckLdIMEIExit(CString strImei);
	CString GetRelateSN(CString strImei);
	void GetRelativeIMEI2_IMEI13(CString strImei);
	int CheckLdSNExit(CString strImei, CString zhidan);
	//判断镭雕IMEI是否存在
	_RecordsetPtr UpdateLdSnAndImei(CString strsn, CString strimei, CString zhidan); //镭雕完更新SN和IMEI到记录表

	_RecordsetPtr UpdateSubIMEISegment(CString zhidan, CString val);


	_RecordsetPtr UpdateLdSn(CString strsn, CString zhidan);

	_RecordsetPtr UpdateReLdData(CString strimei);
	_RecordsetPtr insertLdSnAndImei(CString strsn, CString strimei, CString zhidan);
	int CheckLdzhidanExit(CString zhidan);
	//对比模块函数
	_RecordsetPtr UpdateCompareData(CString strimei, CString result);//对比后更新数据操作
	int CheckCompareIMEIExit(CString strImei);//判断对比IMEI是否存在

	//数据库查询模块所用的函数
	_RecordsetPtr ShowInsertImeiByOrderNumber(CString ordernumber);//将对应的订单号中已经插入的IMEI展示出来
	_RecordsetPtr ShowRepeatImeiByOrderNumber(CString ordernumber);//将对应的订单号中已经重复的IMEI展示出来
	_RecordsetPtr ShowUnImeiByOrderNumber(CString ordernumber, CString imeistrat, CString imeiend);//将对应的订单号中未插入的IMEI展示出来
	//通用函数
	_RecordsetPtr GetRst();//干掉某张表数据用的
	CString GetTime();//获取当前系统时间
	CString CreateIMEI15(CString imei);
	CString GetSubSegment(CString zhidan);
	//用来算IMEI校验位用的
public:
	CString IMEI2Str, IMEI3Str, IMEI4Str, IMEI5Str, IMEI6Str, IMEI7Str, IMEI8Str, IMEI9Str, IMEI10Str, IMEI11Str\
		, IMEI12Str, IMEI13Str;
};

