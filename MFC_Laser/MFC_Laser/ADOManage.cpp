#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>

//全局变量需在类外进行初始化

ADOManage::ADOManage() :
m_SourceIp(_T("")),
m_Port(_T("")),
m_UserName(_T("")),
m_Password(_T(""))
{
}


ADOManage::~ADOManage()
{
}


//以下是数据库连接和释放资源函数

//连接数据库
bool ADOManage::ConnSQL()
{
	CoInitialize(NULL); //初始化COM环境           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//初始化Connection指针 
	GetDBINFO();//获取数据库信息
	try
	{
		/*打开数据库“SQLServer”，这里需要根据数据库的情况 */
		CString m_ConnectStr;//连接字符串

		//要注意连接字符串有三种模式，这里应该选择网络模式
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_SourceIp + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=GPSTest");
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		return 0;
	}
	return 1;
}

//连接指令数据库

//测试连接数据库
bool ADOManage::ConntestdbSQL()
{
	CoInitialize(NULL); //初始化COM环境           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//初始化Connection指针 
	try
	{
		/*打开数据库“SQLServer”，这里需要根据数据库的情况 */
		CString m_ConnectStr;//连接字符串

		//要注意连接字符串有三种模式，这里应该选择网络模式
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_SourceIp + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=GPSTest");
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		return 0;
	}
	return 1;
}

//关闭数据库
bool ADOManage::CloseDB()
{
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return 1;
}

//释放全部的数据库资源
bool ADOManage::CloseAll()
{
	if (m_pRecordSet->GetState() == adStateOpen)
	{
		m_pRecordSet->Close();
	}
	if (m_pRecordSet)
	{
		m_pRecordSet.Release();
		m_pRecordSet = NULL;
	}
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return 1;
}

//释放全部的数据库资源1
bool ADOManage::CloseAll1()
{
	if (m_pRecordSet1->GetState() == adStateOpen)
	{
		m_pRecordSet1->Close();
	}
	if (m_pRecordSet1)
	{
		m_pRecordSet1.Release();
		m_pRecordSet1 = NULL;
	}
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return 1;
}

//获取数据库信息
void ADOManage::GetDBINFO()
{
	CString str;

	GetPrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_SourceIp = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Port"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Port = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("UserName"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_UserName = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Password = str;
	str.ReleaseBuffer();
}


//以下是非通用函数

//这个是模版函数，总结一下用法
_RecordsetPtr ADOManage::MOBAN(CString str1, CString str2)
{
	////初始化Recordset指针
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));

	////参数
	//_variant_t affectline, strnull;//这个是用来获取影响行数用的
	//CString strSql;//这个是用来存放执行语句用的

	////查找语句
	//strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + str1 + _T("] =") + _T("'") + str2 + _T("'");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	////为真就代表没查出任何数据
	//if (m_pRecordSet->adoEOF)
	//{
	//	return m_pRecordSet;
	//}

	////可以从数据集中拿数据然后放到某个变量
	//m_pRecordSet->GetCollect("CH_PrintTime");

	////如果是判断NULL也要用_variant_t类型
	//strnull = m_pRecordSet->GetCollect("CH_PrintTime");

	////这样就可以判断取出来的数据是否为NULL
	//if (strnull.vt == VT_NULL)
	//{
	//	return m_pRecordSet;
	//}

	////插入语句
	//strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan])values('") + str1 + _T("', '") + str2 + _T("')");//具体执行的SQL语句

	////如果插入的记录存在唯一值，我们就得用try，catch来保证不会炸掉，并通过affectline来使得我们可以进行其它操作，比如下面的update
	//try{
	//	m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//直接执行语句
	//}
	//catch (_com_error &e)
	//{
	//	e.Description();
	//	//AfxMessageBox(e.Description());/*打印出异常原因*/
	//}


	////更新语句
	////这里是假设插入失败后才进行
	//if (affectline.vt == 0)
	//{
	//	strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',IMEI2='") + str1 + _T("',SECTIONNO1='") + str2 + _T("'");
	//	m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//直接执行语句
	//}

	return m_pRecordSet;//上面都没问题就返回2代表成功

}


/*下面五个函数是初始化配置模块所用的函数*/
//获取订单号码
_RecordsetPtr ADOManage::GetZhiDan()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));//初始化Recordset指针

	CString strSql = _T("SELECT [ZhiDan] FROM [GPSTest].[dbo].[Gps_ManuOrderParam] where Status='0' or Status='1' order by ZhiDan");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//根据订单号获取IMEI号范围
_RecordsetPtr ADOManage::GetIMEIByZhiDan(CString strzhidan)
{
	try{
		m_pRecordSet.CreateInstance(__uuidof(Recordset));
		//初始化Recordset指针
		CString strSql = _T("SELECT SN1,SN2,SN3,IMEIStart,IMEIEnd  FROM [GPSTest].[dbo].[Gps_ManuOrderParam] WHERE ZhiDan ='") + strzhidan + _T("'");

		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		return m_pRecordSet;
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}

//获取当前镭雕的imei号
int ADOManage::GetIMEICurrentAndTemplate(CString strzhidan)
{
	try{
		m_pRecordSet1.CreateInstance(__uuidof(Recordset));
		//初始化Recordset指针
		CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE ZhiDan ='") + strzhidan + _T("'");
		m_pRecordSet1 = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		if (m_pRecordSet1->adoEOF){
			return 0;
		}
		return 1;
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}

//获取版本
void ADOManage::GetSoftModelAndVersion(CString &model,CString &version,CString zhidan)
{
	try {
		m_pRecordSet1.CreateInstance(__uuidof(Recordset));
		//初始化Recordset指针
		_variant_t modleVal, versionVal;
		CString strSql = _T("SELECT SoftModel,Version  FROM [GPSTest].[dbo].[Gps_ManuOrderParam] WHERE ZhiDan ='") + zhidan + _T("'");
		m_pRecordSet1 = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		if (!m_pRecordSet1->adoEOF) 
		{
			modleVal = m_pRecordSet1->GetCollect(L"SoftModel");
			versionVal = m_pRecordSet1->GetCollect(L"Version");
			if (modleVal.vt!=NULL)
			{
				model = modleVal.bstrVal;
			}
			if (versionVal.vt != NULL)
			{
				version = versionVal.bstrVal;
			}
		}
		
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}


/*Gps_ManuLdRecordParam表*/
//锁定时记录镭雕模板和当前IMEI号
int ADOManage::InsetrLdRecordParam(CString strzhidan, CString strtemplate, CString strcurrentimei,CString checkcode,CString sncurrent,CString LdImei)
{
	try{
		//初始化Recordset指针
		m_pRecordSet.CreateInstance(__uuidof(Recordset));

		//参数
		_variant_t Affectline;
		CString strSql;

		strSql = _T("SELECT ID FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE ZhiDan ='") + strzhidan + _T("'");
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		if (m_pRecordSet->adoEOF){
			//将数据插入表中
			strSql = _T("insert into[GPSTest].[dbo].[Gps_ManuLdRecordParam](Zhidan, LdTemplate, LdCurrentImei,CheckCode,ldSn,LdImei)values('") + strzhidan + _T("', '") + strtemplate + _T("', '") + strcurrentimei + _T("', '") + checkcode + _T("','") + sncurrent + _T("','")+LdImei+_T("')");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			return 1;
		}
		else
		{
			strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET [LdTemplate] = '") + strtemplate + _T("', [LdCurrentImei] = '") + strcurrentimei + _T("', CheckCode ='") + checkcode + _T("',ldSn='") + sncurrent + _T("' WHERE Zhidan = '") + strzhidan + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			return 1;
		}
		return 0;
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}

//大族更新订单信息
int ADOManage::InsetrDZLdRecordParam(CString strzhidan, CString strtemplate, CString checkcode,
	CString LdImei, CString increasingnum, CString haverelative,
	CString relativeziduan, CString bits, CString imeipre)
{
	try {
		//初始化Recordset指针
		m_pRecordSet.CreateInstance(__uuidof(Recordset));

		//参数
		_variant_t Affectline;
		CString strSql;

		strSql = _T("SELECT ID FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE ZhiDan ='") + strzhidan + _T("'");
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		if (m_pRecordSet->adoEOF) {
			//将数据插入表中
			strSql = _T("insert into[GPSTest].[dbo].[Gps_ManuLdRecordParam](Zhidan, LdTemplate, DZhaHavecheckCode,IncreasingNum,HaveRelativeZiduan,RelativeZiduan,Bits,IMEIPre,LdImei)values('") \
				+ strzhidan + _T("', '") + strtemplate + _T("', '") + checkcode + _T("','") + increasingnum \
				+ _T("','") + haverelative + _T("','") + relativeziduan + _T("','") + bits + _T("','") + imeipre+L"','"+LdImei + _T("')");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			return 1;
		}
		else
		{
			strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET [LdTemplate] = '") + strtemplate + _T("', [DZhaHavecheckCode] = '") + checkcode + _T("',IncreasingNum='") + increasingnum \
				+ _T("',HaveRelativeZiduan='") + haverelative + _T("',RelativeZiduan='") + relativeziduan + _T("',Bits='") + bits + _T("',IMEIPre='") + imeipre + _T("' WHERE Zhidan = '") + strzhidan + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			return 1;
		}
		return 0;
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
}


//烧写完后更新当前位到记录表
_RecordsetPtr ADOManage::UpdateCurrnetImei(CString strimei, CString zhidan){
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET LdCurrentImei = '") + strimei + _T("' where Zhidan='")+zhidan+_T("'");

	//如果执行错误就会报异常原因
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//镭雕完后更新IMEI当前位到记录表
_RecordsetPtr ADOManage::UpdateLdSnAndImei(CString strsn, CString strimei, CString zhidan){
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET LdImei = '") + strimei + _T("' where Zhidan='") + zhidan + _T("'");
	//如果执行错误就会报异常原因
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/

	}

	return m_pRecordSet;
}

//更新当前SN
_RecordsetPtr ADOManage::UpdateLdSn(CString strsn, CString zhidan) {
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET ldSn = '") + strsn + _T("' where Zhidan='") + zhidan + _T("'");
	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/

	}

	return m_pRecordSet;
}


//更新重打记录
_RecordsetPtr ADOManage::UpdateReLdData(CString strimei) {
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[LPrintMarkData] SET TestTime = '") +GetTime()+ _T("' ")+_T("where IMEI = '")+ strimei +_T("'");

	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/

	}

	return m_pRecordSet;
}

//插入当前烧写的IMEI，下次再选择此订单时，直接获取
_RecordsetPtr ADOManage::insertLdSnAndImei(CString strsn, CString strimei, CString zhidan) {
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	//strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET ldSn = '") + strsn + _T("',LdImei = '") + strimei + _T("' where Zhidan='") + zhidan + _T("'");
	strSql=_T("insert into [GPSTest].[dbo].[Gps_ManuLdRecordParam] (LdImei,Zhidan) values('")+strimei+_T("','")+ zhidan+_T("')");
	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//查是否存在订单
int ADOManage::CheckLdzhidanExit(CString zhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE [ZhiDan] ='") + zhidan+ _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//获取IMEI计数量
//int ADOManage::GetImeicount(CString strzhidan, CString imeistart, CString imeiend)
//{
//	int imeicount;
//
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//unsigned long long b = 0;
	//b = _atoi64(CStringA(imeiend)) + 1;
	//imeiend = _ui64toa(b, CT2A(imeiend), 10);
	////初始化Recordset指针
	//CString strSql = _T("SELECT count(*) FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'") + _T("AND IMEI >=") + _T("'") + imeistart + _T("'") + _T("AND IMEI <=") + _T("'") + imeiend + _T("'");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//imeicount = m_pRecordSet->GetCollect("");
//
//	return imeicount;
//}


//判断订单号是否存在
//_RecordsetPtr ADOManage::JudgeZhiDan(CString strzhidan)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////初始化Recordset指针

	//CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//return m_pRecordSet;
//}

//烧写模块记录函数
//判断IMEI是否存在
int ADOManage::CheckIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [IMEI] ='") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}
	return 1;
}

//检查chipid是否与其他IMEI绑定，有则获取
CString ADOManage::checkAndGetIMEIbyID(CString chipid)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT IMEI FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [SN] ='") + chipid + _T("'");
	CString chipidBandIMEI = L"";
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (!m_pRecordSet->adoEOF)
	{
		chipidBandIMEI = m_pRecordSet->GetCollect(L"IMEI");
	}
	return chipidBandIMEI;
}

/*查RID是否存在*/
CString ADOManage::CheckRIDExit(CString strRID)
{
	CString IMEI;
	_variant_t IMEIVAl;
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [BasebandID] ='") + strRID + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	if (!m_pRecordSet->adoEOF)
	{
		IMEIVAl = m_pRecordSet->GetCollect("BasebandID");
		if (IMEIVAl.vt!=NULL)
		{
			IMEI = IMEIVAl.bstrVal;
		}
		else
		{
			IMEI = "";
		}
	}
	else
	{
		IMEI = "RID不存在";
	}
	return IMEI;
}

//插入烧写数据到表
_RecordsetPtr ADOManage::InsertBurningData(CString strzhidan, CString strimei, CString ComputerIp,CString burningresult,CString Rid)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("insert into[GPSTest].[dbo].[Gps_ManuLdParam](ZhiDan, IMEI, ProgrammeTime, ProgrammeResult, ComputerIp,BasebandID)values('") + strzhidan + _T("', '") + strimei + _T("', '") + GetTime() + _T("', '") + burningresult + _T("', '") + ComputerIp + _T("', '") +Rid+ _T("')");

	//IMEI号存在唯一值，一般来说在外面就已经做足了防呆防重复的判断，这里是最后一道保险，如果出现了重复的IMEI就报异常
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/

	}

	return m_pRecordSet;
}

//更新烧写数据
_RecordsetPtr ADOManage::UpdateBurningData(CString strimei, CString Rid ,CString burnResult,CString ComputerIp)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',  ProgrammeTime = '%s',  ProgrammeResult = '%S' ,  ComputerIp = '%s' where BasebandID = '%s'", strimei, GetTime(), burnResult, ComputerIp, Rid);
	//IMEI号存在唯一值，一般来说在外面就已经做足了防呆防重复的判断，这里是最后一道保险，如果出现了重复的IMEI就报异常
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}


//到总表查询id
int ADOManage::checkIMEIbyID(CString chipid)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [SN] ='") + chipid + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	if (!m_pRecordSet->adoEOF)//非空
	{
		_variant_t val = m_pRecordSet->GetCollect(L"IMEI");
		if (val.vt!=NULL&&val.bstrVal!=L"")
		{
			return 0;
		}
	}
	return 1;//总表没有此id，可烧写
}

//获取关联数据
CString ADOManage::getRelativeIMEIX(CString str,CString paramIMEI)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString myData = L"";
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + paramIMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	if (!m_pRecordSet->adoEOF)//非空
	{
		_variant_t val = m_pRecordSet->GetCollect((LPCTSTR)str);
		if (val.vt != NULL)
		{
			myData = val.bstrVal;
			return myData;
		}
	}
	return myData;//总表没有此id，可烧写
}

//保存读取到的数据到关联表
_RecordsetPtr ADOManage::saveData(CString data, CString IMEI,CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[DataRelativeSheet] SET ")+ziduan+_T(" = '") + data + _T("' where [IMEI1] = '") + IMEI + _T("'");

	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//获取新表数据
CString ADOManage::getNewSheetDataDB( CString paramIMEI,CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString myData = L"";
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeUpdate] WHERE [RelativeNum] ='") + paramIMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	if (!m_pRecordSet->adoEOF)//非空
	{
		_variant_t val = m_pRecordSet->GetCollect((LPCTSTR)ziduan);
		if (val.vt != NULL)
		{
			myData = val.bstrVal;
			return myData;
		}
	}
	return myData;//总表没有此id，可烧写
}

//查关联表是否已有IMEI
int ADOManage::CheckIMEIInRelative(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}
	return 1;
}

//查新关联表是否存在IMEI
int ADOManage::CheckIMEIInNewRelative(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeUpdate] WHERE [IMEI] ='") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}
	return 1;
}


//把读到的数据插入关联表
_RecordsetPtr ADOManage::insertToRelative(CString data, CString IMEI, CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("insert into [GPSTest].[dbo].[DataRelativeSheet] (IMEI1,") + ziduan + _T(",TestTime) values ('") + IMEI + _T("','") + data + _T("','") + GetTime() + _T("')");
	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//更新IMEI到新关联表
_RecordsetPtr ADOManage::UpdateIMEIToRelative(CString data, CString IMEI, CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[DataRelativeUpdate] SET IMEI1 = '") + IMEI + _T("' where ") + ziduan + _T(" = '") + data + _T("'");
	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//更新ICCID或sim卡号或蓝牙号到新关联表
_RecordsetPtr ADOManage::UpdateICCIDToRelative(CString data, CString IMEI, CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[DataRelativeUpdate] SET ") + ziduan + _T(" = '") + data + _T("' where IMEI1 ='") + IMEI + _T("'");
	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}


//更新IMEI到耦合表
_RecordsetPtr ADOManage::updateDatatoCoupleDB(CString IMEI, CString chipid)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_CoupleTest_Result] SET IMEI = '") + IMEI + _T("' where [SN] = '") + chipid + _T("'");

	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//插入成功信息到总表
_RecordsetPtr ADOManage::insertToMainSheetDB(CString IMEI, CString result,CString chipid,CString version,CString machinename)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("insert into [GPSTest].[dbo].[Gps_TestResult] (SN,IMEI,WriteImeiResult,SoftModel,Version) values('") +chipid+_T("','")+ IMEI + _T("','") + result+_T("','")+ machinename +_T("','")+ version + _T("')");

	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//如果chipid与imei绑定，则更新
_RecordsetPtr ADOManage::UpdateToMainSheetDB(CString IMEI, CString result, CString chipid, CString version, CString machinename)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_TestResult] Set IMEI = '") + IMEI + _T("',WriteImeiResult = '") + result + _T("',SoftModel = '") + machinename + _T("',Version = '") + version + _T("'")+_T(" where SN = '")+chipid+_T("'");

	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//获取指令和返回值
void ADOManage::GetTestCommandAndRec(CString softWare, CString TestCommand, CString CommandRec)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql = (L"select * [GPSTest].[dbo].[TestSystemSettingFunc] where SoftWare = '")+softWare+L"'";
	//IMEI号存在唯一值，一般来说在外面就已经做足了防呆防重复的判断，这里是最后一道保险，如果出现了重复的IMEI就报异常
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
	/*if (!m_pRecordSet->adoEOF)
	{
		val = m_pRecordSet->GetCollect(L"SN");
		if (val.vt != NULL)
		{
			GetSN = val.bstrVal;
		}
	}*/
	return;
}

//获取station字段数据
CString ADOManage::adoGetStation(CString softWare)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;
	CString getStation = L"";
	_variant_t val;
	//将数据插入表中
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql = (L"select Station from [NetMarkIMEI2].[dbo].[TestSystemSettingFunc] where SoftWare = '") + softWare + L"'";
	//IMEI号存在唯一值，一般来说在外面就已经做足了防呆防重复的判断，这里是最后一道保险，如果出现了重复的IMEI就报异常
	try {
		m_pRecordSet=m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
	if (!m_pRecordSet->adoEOF)
	{
		val = m_pRecordSet->GetCollect(L"Station");
		if (val.vt != NULL)
		{
			getStation = val.bstrVal;
		}
	}
	return getStation;
}

//获取MachineName字段数据
CString ADOManage::adoGetMachineName(CString zhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;
	CString getStation = L"";
	_variant_t val;
	//将数据插入表中
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql = (L"select MachineName from [NetMarkIMEI2].[dbo].[TestSystemSettingFunc] where SoftWare = '") + zhidan + L"'";
	//IMEI号存在唯一值，一般来说在外面就已经做足了防呆防重复的判断，这里是最后一道保险，如果出现了重复的IMEI就报异常
	try {
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
	if (!m_pRecordSet->adoEOF)
	{
		val = m_pRecordSet->GetCollect(L"MachineName");
		if (val.vt != NULL)
		{
			getStation = val.bstrVal;
		}
	}
	return getStation;
}

//查LPrintMarkData表中是否存在IMEI
int ADOManage::CheckLdIMEIExitLM(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[LPrintMarkData] WHERE [IMEI] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	int count = m_pRecordSet->GetRecordCount();
	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//查Gps_TestResult表是否存在IMEI
int ADOManage::CheckLdIMEIExitTR(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [IMEI] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//镭雕系统模块所用的函数
//判断IMEI是否存在
int ADOManage::CheckLdIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [IMEI] ='") + strImei +_T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//获取关联sn
CString ADOManage::GetRelateSN(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	_variant_t val;
	CString GetSN = L"";
	CString strSql = _T("SELECT SN FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (!m_pRecordSet->adoEOF)
	{
		val = m_pRecordSet->GetCollect(L"SN");
		if (val.vt!=NULL)
		{
			GetSN = val.bstrVal;
		}
	}
	return GetSN;
}

void ADOManage::GetRelativeIMEI2_IMEI13(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	_variant_t val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13;
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	if (!m_pRecordSet->adoEOF)
	{
		val2 = m_pRecordSet->GetCollect(L"IMEI2");
		val3 = m_pRecordSet->GetCollect(L"IMEI3");
		val4 = m_pRecordSet->GetCollect(L"IMEI4");
		val5 = m_pRecordSet->GetCollect(L"IMEI5");
		val6 = m_pRecordSet->GetCollect(L"IMEI6");
		val7 = m_pRecordSet->GetCollect(L"IMEI7");
		val8 = m_pRecordSet->GetCollect(L"IMEI8");
		val9 = m_pRecordSet->GetCollect(L"IMEI9");
		val10 = m_pRecordSet->GetCollect(L"IMEI10");
		val11 = m_pRecordSet->GetCollect(L"IMEI11");
		val12 = m_pRecordSet->GetCollect(L"IMEI12");
		val13 = m_pRecordSet->GetCollect(L"IMEI13");
		if (val2.vt != NULL)
		{
			IMEI2Str = val2.bstrVal;
		}
		if (val3.vt != NULL)
		{
			IMEI3Str = val3.bstrVal;
		}
		if (val4.vt != NULL)
		{
			IMEI4Str = val4.bstrVal;
		}
		if (val5.vt != NULL)
		{
			IMEI5Str = val5.bstrVal;
		}
		if (val6.vt != NULL)
		{
			IMEI6Str = val6.bstrVal;
		}
		if (val7.vt != NULL)
		{
			IMEI7Str = val7.bstrVal;
		}
		if (val8.vt != NULL)
		{
			IMEI8Str = val8.bstrVal;
		}
		if (val9.vt != NULL)
		{
			IMEI9Str = val9.bstrVal;
		}
		if (val10.vt != NULL)
		{
			IMEI10Str = val10.bstrVal;
		}
		if (val11.vt != NULL)
		{
			IMEI11Str = val11.bstrVal;
		}
		if (val12.vt != NULL)
		{
			IMEI12Str = val12.bstrVal;
		}
		if (val13.vt != NULL)
		{
			IMEI13Str = val13.bstrVal;
		}
	}
}


//查sn是否被镭雕过
int ADOManage::CheckLdSNExit(CString strImei,CString zhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [SN] ='") + strImei + _T("' AND ZhiDan ='")+zhidan+_T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//镭雕后更新数据操作
_RecordsetPtr ADOManage::UpdateLdData(CString strimei, CString StrSn)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdParam] SET SN='") + StrSn + _T("', LdTime = '") + GetTime() + _T("',LdResult='1' where [IMEI] = '") + strimei + _T("'");

	//如果执行错误就会报异常原因
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//插入镭雕数据
_RecordsetPtr ADOManage::InsertLdData(CString strimei, CString Computerip,CString zhidan,CString softmodel,CString version)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	//strSql.Format(_T("insert into [GPSTest].[dbo].[Gps_ManuLdParam] (IMEI,LdResult,ZhiDan,SN) values ('%S','%s','%s','%s')"), strimei,L"1", zhidan, StrSn);
	strSql = _T("insert into [GPSTest].[dbo].[LPrintMarkData] (IMEI,ZhiDan,Computer,TestTime,SoftModel,Version) values('") + strimei + _T("','") + zhidan + _T("','") + Computerip + _T("','") + GetTime()+_T("','")+ softmodel +_T("','")+version+_T("')");

	//strSql = _T("insert into [GPSTest].[dbo].[Gps_ManuLdParam] (IMEI,LdResult,ZhiDan,SN,LdTime) values('") + strimei + _T("','") + L"1" + _T("','") + zhidan + _T("','") + StrSn +_T("','")+GetTime()+ _T("')");
	//如果执行错误就会报异常原因
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}


CString ADOManage::DZGetRelateData(CString strImei,CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	_variant_t val;
	CString GetData = L"";
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (!m_pRecordSet->adoEOF)
	{
		val = m_pRecordSet->GetCollect((LPCTSTR)ziduan);
		if (val.vt != NULL)
		{
			GetData = val.bstrVal;
		}
		else
		{
			GetData = L"NULL";
		}
	}
	return GetData;
}

/*对比模块函数*/

//判断IMEI是否存在
int ADOManage::CheckCompareIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [IMEI] ='") + strImei + _T("' AND CompareResult ='1'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//对比后更新数据操作
_RecordsetPtr ADOManage::UpdateCompareData(CString strimei,CString result)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdParam] SET CompareTime = '") + GetTime() + _T("',CompareResult='") + result + _T("' where [IMEI] = '") + strimei + _T("'");

	//如果执行错误就会报异常原因
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}



//存储当前镭雕的imei号
//_RecordsetPtr ADOManage::UpdateIMEICurrent(CString strzhidan, CString imeicurrent)
//{
	////初始化Recordset指针
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//_variant_t Affectline;

	////把IMEI号+1
	//unsigned long long imeiint = 0;
	//imeiint = _atoi64(CStringA(imeicurrent)) + 1;
	//imeicurrent = _ui64toa(imeiint, CT2A(imeicurrent), 10);

	////更新一下最后一个扫入的IMEI号，一般来说不会更新失败，要是失败了，那肯定是订单号被干掉了
	//CString strSql = _T("UPDATE[") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("]") + _T("SET LDIMEICurrent = '") + imeicurrent + _T("' where ZhiDan = '") + strzhidan + _T("'");

	//try{
	//	m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	//}
	//catch (_com_error &e)
	//{
	//	AfxMessageBox(e.Description());/*打印出异常原因*/
	//}
	//return m_pRecordSet;
//}


//下面三个数据库查询模块所用的函数
//将对应的订单号中已经插入的IMEI展示出来
//_RecordsetPtr ADOManage::ShowInsertImeiByOrderNumber(CString ordernumber)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////初始化Recordset指针
	//CString strSql = _T("SELECT [IMEI],[InputTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//return m_pRecordSet;
//}

//将对应的订单号中已经重复的IMEI展示出来
//_RecordsetPtr ADOManage::ShowRepeatImeiByOrderNumber(CString ordernumber)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////初始化Recordset指针
	//CString strSql = _T("SELECT [IMEI],[ReLdImeiNum],[ReLdImeiTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'") + _T("AND [ReLdImeiNum]>0");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//return m_pRecordSet;
//}

//将对应的订单号中未插入的IMEI展示出来
//_RecordsetPtr ADOManage::ShowUnImeiByOrderNumber(CString ordernumber, CString imeistart, CString imeiend)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////初始化Recordset指针

	//CString strSql1, strSql2, strSql3, imeitemp;

	////先清除一遍临时表
	//strSql3 = _T("truncate table[testLD].[dbo].[Gps_ManuUnLdParam]");
	//m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//直接执行语句

	////然后将数据插入到一张临时表中
	//unsigned long long a, b;
	//a = _atoi64(CStringA(imeistart));
	//b = _atoi64(CStringA(imeiend));
	//for (; a <= b; a++)
	//{
	//	imeistart = _ui64toa(a, CT2A(imeistart), 10);
	//	imeitemp = CreateIMEI15(imeistart);
	//	strSql1 = _T("insert into[testLD].[dbo].[Gps_ManuUnLdParam](UNIMEI)values('") + imeitemp + _T("')");
	//	m_pConnection->Execute(_bstr_t(strSql1), NULL, adCmdText);
	//}

	////接着获取范围内没有的数据
	//strSql2 = _T("select[UNIMEI] FROM[testLD].[dbo].[Gps_ManuUnLdParam] where[UNIMEI] not in(select[IMEI] FROM[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] where[zhidan] = '") + ordernumber + _T("')");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql2), NULL, adCmdText);//直接执行语句

	////最后清除临时表
	//m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//直接执行语句

	//return m_pRecordSet;
//}


//以下是通用函数
//清除数据库某张表
_RecordsetPtr ADOManage::GetRst()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针 
	CString strSql;
	strSql = "Truncate table [testLD].[dbo].[testld]";
	//strSql = "select * from BLEStatus";//具体执行的SQL语句

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//将查询数据导入m_pRecordset数据容器
	return m_pRecordSet;
}

//获取当前时间函数
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//用来算IMEI校验位用的
CString ADOManage::CreateIMEI15(CString imei)
{
	long long a = 0;
	int j = 0;
	char imeiChar[14];
	strncpy(imeiChar, CStringA(imei), sizeof(imeiChar));
	//	std::string sz2 = CT2A(imei.GetBuffer());

	int resultInt = 0;
	for (int i = 0; i < 14; i++)
	{
		int a = (int)imeiChar[i] - '0';
		i++;
		int temp = ((int)imeiChar[i] - '0') * 2;
		int b = temp < 10 ? temp : temp - 9;
		resultInt += a + b;
	}
	resultInt %= 10;
	GetLastError();
	resultInt = (resultInt == 0 ? 0 : 10 - resultInt);
	CString resultStr, abc;
	resultStr.Format(_T("%d"), resultInt);
	abc = imei + resultStr;
	imei.ReleaseBuffer();
	return abc;
}