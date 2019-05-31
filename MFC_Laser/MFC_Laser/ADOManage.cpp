#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>

//ȫ�ֱ�������������г�ʼ��

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


//���������ݿ����Ӻ��ͷ���Դ����

//�������ݿ�
bool ADOManage::ConnSQL()
{
	CoInitialize(NULL); //��ʼ��COM����           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//��ʼ��Connectionָ�� 
	GetDBINFO();//��ȡ���ݿ���Ϣ
	try
	{
		/*�����ݿ⡰SQLServer����������Ҫ�������ݿ����� */
		CString m_ConnectStr;//�����ַ���

		//Ҫע�������ַ���������ģʽ������Ӧ��ѡ������ģʽ
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_SourceIp + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=GPSTest");
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		return 0;
	}
	return 1;
}

//����ָ�����ݿ�

//�����������ݿ�
bool ADOManage::ConntestdbSQL()
{
	CoInitialize(NULL); //��ʼ��COM����           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//��ʼ��Connectionָ�� 
	try
	{
		/*�����ݿ⡰SQLServer����������Ҫ�������ݿ����� */
		CString m_ConnectStr;//�����ַ���

		//Ҫע�������ַ���������ģʽ������Ӧ��ѡ������ģʽ
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_SourceIp + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=GPSTest");
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		return 0;
	}
	return 1;
}

//�ر����ݿ�
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

//�ͷ�ȫ�������ݿ���Դ
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

//�ͷ�ȫ�������ݿ���Դ1
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

//��ȡ���ݿ���Ϣ
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


//�����Ƿ�ͨ�ú���

//�����ģ�溯�����ܽ�һ���÷�
_RecordsetPtr ADOManage::MOBAN(CString str1, CString str2)
{
	////��ʼ��Recordsetָ��
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));

	////����
	//_variant_t affectline, strnull;//�����������ȡӰ�������õ�
	//CString strSql;//������������ִ������õ�

	////�������
	//strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + str1 + _T("] =") + _T("'") + str2 + _T("'");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	////Ϊ��ʹ���û����κ�����
	//if (m_pRecordSet->adoEOF)
	//{
	//	return m_pRecordSet;
	//}

	////���Դ����ݼ���������Ȼ��ŵ�ĳ������
	//m_pRecordSet->GetCollect("CH_PrintTime");

	////������ж�NULLҲҪ��_variant_t����
	//strnull = m_pRecordSet->GetCollect("CH_PrintTime");

	////�����Ϳ����ж�ȡ�����������Ƿ�ΪNULL
	//if (strnull.vt == VT_NULL)
	//{
	//	return m_pRecordSet;
	//}

	////�������
	//strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan])values('") + str1 + _T("', '") + str2 + _T("')");//����ִ�е�SQL���

	////�������ļ�¼����Ψһֵ�����Ǿ͵���try��catch����֤����ը������ͨ��affectline��ʹ�����ǿ��Խ����������������������update
	//try{
	//	m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//ֱ��ִ�����
	//}
	//catch (_com_error &e)
	//{
	//	e.Description();
	//	//AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	//}


	////�������
	////�����Ǽ������ʧ�ܺ�Ž���
	//if (affectline.vt == 0)
	//{
	//	strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',IMEI2='") + str1 + _T("',SECTIONNO1='") + str2 + _T("'");
	//	m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//ֱ��ִ�����
	//}

	return m_pRecordSet;//���涼û����ͷ���2����ɹ�

}


/*������������ǳ�ʼ������ģ�����õĺ���*/
//��ȡ��������
_RecordsetPtr ADOManage::GetZhiDan()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));//��ʼ��Recordsetָ��

	CString strSql = _T("SELECT [ZhiDan] FROM [GPSTest].[dbo].[Gps_ManuOrderParam] where Status='0' or Status='1' order by ZhiDan");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//���ݶ����Ż�ȡIMEI�ŷ�Χ
_RecordsetPtr ADOManage::GetIMEIByZhiDan(CString strzhidan)
{
	try{
		m_pRecordSet.CreateInstance(__uuidof(Recordset));
		//��ʼ��Recordsetָ��
		CString strSql = _T("SELECT SN1,SN2,SN3,IMEIStart,IMEIEnd  FROM [GPSTest].[dbo].[Gps_ManuOrderParam] WHERE ZhiDan ='") + strzhidan + _T("'");

		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		return m_pRecordSet;
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
}

//��ȡ��ǰ�ص��imei��
int ADOManage::GetIMEICurrentAndTemplate(CString strzhidan)
{
	try{
		m_pRecordSet1.CreateInstance(__uuidof(Recordset));
		//��ʼ��Recordsetָ��
		CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE ZhiDan ='") + strzhidan + _T("'");
		m_pRecordSet1 = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		if (m_pRecordSet1->adoEOF){
			return 0;
		}
		return 1;
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
}

//��ȡ�汾
void ADOManage::GetSoftModelAndVersion(CString &model,CString &version,CString zhidan)
{
	try {
		m_pRecordSet1.CreateInstance(__uuidof(Recordset));
		//��ʼ��Recordsetָ��
		_variant_t modleVal, versionVal;
		CString strSql = _T("SELECT SoftModel,Version  FROM [GPSTest].[dbo].[Gps_ManuOrderParam] WHERE ZhiDan ='") + zhidan + _T("'");
		m_pRecordSet1 = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
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
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
}


/*Gps_ManuLdRecordParam��*/
//����ʱ��¼�ص�ģ��͵�ǰIMEI��
int ADOManage::InsetrLdRecordParam(CString strzhidan, CString strtemplate, CString strcurrentimei,CString checkcode,CString sncurrent,CString LdImei)
{
	try{
		//��ʼ��Recordsetָ��
		m_pRecordSet.CreateInstance(__uuidof(Recordset));

		//����
		_variant_t Affectline;
		CString strSql;

		strSql = _T("SELECT ID FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE ZhiDan ='") + strzhidan + _T("'");
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		if (m_pRecordSet->adoEOF){
			//�����ݲ������
			strSql = _T("insert into[GPSTest].[dbo].[Gps_ManuLdRecordParam](Zhidan, LdTemplate, LdCurrentImei,CheckCode,ldSn,LdImei)values('") + strzhidan + _T("', '") + strtemplate + _T("', '") + strcurrentimei + _T("', '") + checkcode + _T("','") + sncurrent + _T("','")+LdImei+_T("')");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			return 1;
		}
		else
		{
			strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET [LdTemplate] = '") + strtemplate + _T("', [LdCurrentImei] = '") + strcurrentimei + _T("', CheckCode ='") + checkcode + _T("',ldSn='") + sncurrent + _T("' WHERE Zhidan = '") + strzhidan + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			return 1;
		}
		return 0;
	}
	catch (_com_error &e){
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
}

//������¶�����Ϣ
int ADOManage::InsetrDZLdRecordParam(CString strzhidan, CString strtemplate, CString checkcode,
	CString LdImei, CString increasingnum, CString haverelative,
	CString relativeziduan, CString bits, CString imeipre)
{
	try {
		//��ʼ��Recordsetָ��
		m_pRecordSet.CreateInstance(__uuidof(Recordset));

		//����
		_variant_t Affectline;
		CString strSql;

		strSql = _T("SELECT ID FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE ZhiDan ='") + strzhidan + _T("'");
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		if (m_pRecordSet->adoEOF) {
			//�����ݲ������
			strSql = _T("insert into[GPSTest].[dbo].[Gps_ManuLdRecordParam](Zhidan, LdTemplate, DZhaHavecheckCode,IncreasingNum,HaveRelativeZiduan,RelativeZiduan,Bits,IMEIPre,LdImei)values('") \
				+ strzhidan + _T("', '") + strtemplate + _T("', '") + checkcode + _T("','") + increasingnum \
				+ _T("','") + haverelative + _T("','") + relativeziduan + _T("','") + bits + _T("','") + imeipre+L"','"+LdImei + _T("')");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			return 1;
		}
		else
		{
			strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET [LdTemplate] = '") + strtemplate + _T("', [DZhaHavecheckCode] = '") + checkcode + _T("',IncreasingNum='") + increasingnum \
				+ _T("',HaveRelativeZiduan='") + haverelative + _T("',RelativeZiduan='") + relativeziduan + _T("',Bits='") + bits + _T("',IMEIPre='") + imeipre + _T("' WHERE Zhidan = '") + strzhidan + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			return 1;
		}
		return 0;
	}
	catch (_com_error &e) {
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
}


//��д�����µ�ǰλ����¼��
_RecordsetPtr ADOManage::UpdateCurrnetImei(CString strimei, CString zhidan){
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET LdCurrentImei = '") + strimei + _T("' where Zhidan='")+zhidan+_T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//�ص�������IMEI��ǰλ����¼��
_RecordsetPtr ADOManage::UpdateLdSnAndImei(CString strsn, CString strimei, CString zhidan){
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET LdImei = '") + strimei + _T("' where Zhidan='") + zhidan + _T("'");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/

	}

	return m_pRecordSet;
}

//���µ�ǰSN
_RecordsetPtr ADOManage::UpdateLdSn(CString strsn, CString zhidan) {
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET ldSn = '") + strsn + _T("' where Zhidan='") + zhidan + _T("'");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/

	}

	return m_pRecordSet;
}


//�����ش��¼
_RecordsetPtr ADOManage::UpdateReLdData(CString strimei) {
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[LPrintMarkData] SET TestTime = '") +GetTime()+ _T("' ")+_T("where IMEI = '")+ strimei +_T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/

	}

	return m_pRecordSet;
}

//���뵱ǰ��д��IMEI���´���ѡ��˶���ʱ��ֱ�ӻ�ȡ
_RecordsetPtr ADOManage::insertLdSnAndImei(CString strsn, CString strimei, CString zhidan) {
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	//strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdRecordParam] SET ldSn = '") + strsn + _T("',LdImei = '") + strimei + _T("' where Zhidan='") + zhidan + _T("'");
	strSql=_T("insert into [GPSTest].[dbo].[Gps_ManuLdRecordParam] (LdImei,Zhidan) values('")+strimei+_T("','")+ zhidan+_T("')");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//���Ƿ���ڶ���
int ADOManage::CheckLdzhidanExit(CString zhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdRecordParam] WHERE [ZhiDan] ='") + zhidan+ _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//��ȡIMEI������
//int ADOManage::GetImeicount(CString strzhidan, CString imeistart, CString imeiend)
//{
//	int imeicount;
//
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//unsigned long long b = 0;
	//b = _atoi64(CStringA(imeiend)) + 1;
	//imeiend = _ui64toa(b, CT2A(imeiend), 10);
	////��ʼ��Recordsetָ��
	//CString strSql = _T("SELECT count(*) FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'") + _T("AND IMEI >=") + _T("'") + imeistart + _T("'") + _T("AND IMEI <=") + _T("'") + imeiend + _T("'");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//imeicount = m_pRecordSet->GetCollect("");
//
//	return imeicount;
//}


//�ж϶������Ƿ����
//_RecordsetPtr ADOManage::JudgeZhiDan(CString strzhidan)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////��ʼ��Recordsetָ��

	//CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//return m_pRecordSet;
//}

//��дģ���¼����
//�ж�IMEI�Ƿ����
int ADOManage::CheckIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [IMEI] ='") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}
	return 1;
}

//���chipid�Ƿ�������IMEI�󶨣������ȡ
CString ADOManage::checkAndGetIMEIbyID(CString chipid)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT IMEI FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [SN] ='") + chipid + _T("'");
	CString chipidBandIMEI = L"";
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (!m_pRecordSet->adoEOF)
	{
		chipidBandIMEI = m_pRecordSet->GetCollect(L"IMEI");
	}
	return chipidBandIMEI;
}

/*��RID�Ƿ����*/
CString ADOManage::CheckRIDExit(CString strRID)
{
	CString IMEI;
	_variant_t IMEIVAl;
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [BasebandID] ='") + strRID + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
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
		IMEI = "RID������";
	}
	return IMEI;
}

//������д���ݵ���
_RecordsetPtr ADOManage::InsertBurningData(CString strzhidan, CString strimei, CString ComputerIp,CString burningresult,CString Rid)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("insert into[GPSTest].[dbo].[Gps_ManuLdParam](ZhiDan, IMEI, ProgrammeTime, ProgrammeResult, ComputerIp,BasebandID)values('") + strzhidan + _T("', '") + strimei + _T("', '") + GetTime() + _T("', '") + burningresult + _T("', '") + ComputerIp + _T("', '") +Rid+ _T("')");

	//IMEI�Ŵ���Ψһֵ��һ����˵��������Ѿ������˷������ظ����жϣ����������һ�����գ�����������ظ���IMEI�ͱ��쳣
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/

	}

	return m_pRecordSet;
}

//������д����
_RecordsetPtr ADOManage::UpdateBurningData(CString strimei, CString Rid ,CString burnResult,CString ComputerIp)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',  ProgrammeTime = '%s',  ProgrammeResult = '%S' ,  ComputerIp = '%s' where BasebandID = '%s'", strimei, GetTime(), burnResult, ComputerIp, Rid);
	//IMEI�Ŵ���Ψһֵ��һ����˵��������Ѿ������˷������ظ����жϣ����������һ�����գ�����������ظ���IMEI�ͱ��쳣
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}


//���ܱ��ѯid
int ADOManage::checkIMEIbyID(CString chipid)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [SN] ='") + chipid + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (!m_pRecordSet->adoEOF)//�ǿ�
	{
		_variant_t val = m_pRecordSet->GetCollect(L"IMEI");
		if (val.vt!=NULL&&val.bstrVal!=L"")
		{
			return 0;
		}
	}
	return 1;//�ܱ�û�д�id������д
}

//��ȡ��������
CString ADOManage::getRelativeIMEIX(CString str,CString paramIMEI)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString myData = L"";
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + paramIMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (!m_pRecordSet->adoEOF)//�ǿ�
	{
		_variant_t val = m_pRecordSet->GetCollect((LPCTSTR)str);
		if (val.vt != NULL)
		{
			myData = val.bstrVal;
			return myData;
		}
	}
	return myData;//�ܱ�û�д�id������д
}

//�����ȡ�������ݵ�������
_RecordsetPtr ADOManage::saveData(CString data, CString IMEI,CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[DataRelativeSheet] SET ")+ziduan+_T(" = '") + data + _T("' where [IMEI1] = '") + IMEI + _T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//��ȡ�±�����
CString ADOManage::getNewSheetDataDB( CString paramIMEI,CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString myData = L"";
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeUpdate] WHERE [RelativeNum] ='") + paramIMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (!m_pRecordSet->adoEOF)//�ǿ�
	{
		_variant_t val = m_pRecordSet->GetCollect((LPCTSTR)ziduan);
		if (val.vt != NULL)
		{
			myData = val.bstrVal;
			return myData;
		}
	}
	return myData;//�ܱ�û�д�id������д
}

//��������Ƿ�����IMEI
int ADOManage::CheckIMEIInRelative(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}
	return 1;
}

//���¹������Ƿ����IMEI
int ADOManage::CheckIMEIInNewRelative(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeUpdate] WHERE [IMEI] ='") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}
	return 1;
}


//�Ѷ��������ݲ��������
_RecordsetPtr ADOManage::insertToRelative(CString data, CString IMEI, CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("insert into [GPSTest].[dbo].[DataRelativeSheet] (IMEI1,") + ziduan + _T(",TestTime) values ('") + IMEI + _T("','") + data + _T("','") + GetTime() + _T("')");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//����IMEI���¹�����
_RecordsetPtr ADOManage::UpdateIMEIToRelative(CString data, CString IMEI, CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[DataRelativeUpdate] SET IMEI1 = '") + IMEI + _T("' where ") + ziduan + _T(" = '") + data + _T("'");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//����ICCID��sim���Ż������ŵ��¹�����
_RecordsetPtr ADOManage::UpdateICCIDToRelative(CString data, CString IMEI, CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[DataRelativeUpdate] SET ") + ziduan + _T(" = '") + data + _T("' where IMEI1 ='") + IMEI + _T("'");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}


//����IMEI����ϱ�
_RecordsetPtr ADOManage::updateDatatoCoupleDB(CString IMEI, CString chipid)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_CoupleTest_Result] SET IMEI = '") + IMEI + _T("' where [SN] = '") + chipid + _T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//����ɹ���Ϣ���ܱ�
_RecordsetPtr ADOManage::insertToMainSheetDB(CString IMEI, CString result,CString chipid,CString version,CString machinename)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("insert into [GPSTest].[dbo].[Gps_TestResult] (SN,IMEI,WriteImeiResult,SoftModel,Version) values('") +chipid+_T("','")+ IMEI + _T("','") + result+_T("','")+ machinename +_T("','")+ version + _T("')");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//���chipid��imei�󶨣������
_RecordsetPtr ADOManage::UpdateToMainSheetDB(CString IMEI, CString result, CString chipid, CString version, CString machinename)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_TestResult] Set IMEI = '") + IMEI + _T("',WriteImeiResult = '") + result + _T("',SoftModel = '") + machinename + _T("',Version = '") + version + _T("'")+_T(" where SN = '")+chipid+_T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//��ȡָ��ͷ���ֵ
void ADOManage::GetTestCommandAndRec(CString softWare, CString TestCommand, CString CommandRec)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql = (L"select * [GPSTest].[dbo].[TestSystemSettingFunc] where SoftWare = '")+softWare+L"'";
	//IMEI�Ŵ���Ψһֵ��һ����˵��������Ѿ������˷������ظ����жϣ����������һ�����գ�����������ظ���IMEI�ͱ��쳣
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
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

//��ȡstation�ֶ�����
CString ADOManage::adoGetStation(CString softWare)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;
	CString getStation = L"";
	_variant_t val;
	//�����ݲ������
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql = (L"select Station from [NetMarkIMEI2].[dbo].[TestSystemSettingFunc] where SoftWare = '") + softWare + L"'";
	//IMEI�Ŵ���Ψһֵ��һ����˵��������Ѿ������˷������ظ����жϣ����������һ�����գ�����������ظ���IMEI�ͱ��쳣
	try {
		m_pRecordSet=m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
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

//��ȡMachineName�ֶ�����
CString ADOManage::adoGetMachineName(CString zhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;
	CString getStation = L"";
	_variant_t val;
	//�����ݲ������
	//strSql.Format(L"update [GPSTest].[dbo].[Gps_ManuLdParam] set IMEI = '%s',set ProgrammeTime = '%s',set ProgrammeResult = '%S' ,set ComputerIp = '%s' where BasebandID = '%s'",strimei,GetTime(), burnResult, ComputerIp, Rid);
	strSql = (L"select MachineName from [NetMarkIMEI2].[dbo].[TestSystemSettingFunc] where SoftWare = '") + zhidan + L"'";
	//IMEI�Ŵ���Ψһֵ��һ����˵��������Ѿ������˷������ظ����жϣ����������һ�����գ�����������ظ���IMEI�ͱ��쳣
	try {
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
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

//��LPrintMarkData�����Ƿ����IMEI
int ADOManage::CheckLdIMEIExitLM(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[LPrintMarkData] WHERE [IMEI] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	int count = m_pRecordSet->GetRecordCount();
	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//��Gps_TestResult���Ƿ����IMEI
int ADOManage::CheckLdIMEIExitTR(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_TestResult] WHERE [IMEI] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//�ص�ϵͳģ�����õĺ���
//�ж�IMEI�Ƿ����
int ADOManage::CheckLdIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [IMEI] ='") + strImei +_T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//��ȡ����sn
CString ADOManage::GetRelateSN(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	_variant_t val;
	CString GetSN = L"";
	CString strSql = _T("SELECT SN FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
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
	//��ʼ��Recordsetָ��
	_variant_t val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13;
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
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


//��sn�Ƿ��ص��
int ADOManage::CheckLdSNExit(CString strImei,CString zhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [SN] ='") + strImei + _T("' AND ZhiDan ='")+zhidan+_T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//�ص��������ݲ���
_RecordsetPtr ADOManage::UpdateLdData(CString strimei, CString StrSn)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdParam] SET SN='") + StrSn + _T("', LdTime = '") + GetTime() + _T("',LdResult='1' where [IMEI] = '") + strimei + _T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//�����ص�����
_RecordsetPtr ADOManage::InsertLdData(CString strimei, CString Computerip,CString zhidan,CString softmodel,CString version)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	//strSql.Format(_T("insert into [GPSTest].[dbo].[Gps_ManuLdParam] (IMEI,LdResult,ZhiDan,SN) values ('%S','%s','%s','%s')"), strimei,L"1", zhidan, StrSn);
	strSql = _T("insert into [GPSTest].[dbo].[LPrintMarkData] (IMEI,ZhiDan,Computer,TestTime,SoftModel,Version) values('") + strimei + _T("','") + zhidan + _T("','") + Computerip + _T("','") + GetTime()+_T("','")+ softmodel +_T("','")+version+_T("')");

	//strSql = _T("insert into [GPSTest].[dbo].[Gps_ManuLdParam] (IMEI,LdResult,ZhiDan,SN,LdTime) values('") + strimei + _T("','") + L"1" + _T("','") + zhidan + _T("','") + StrSn +_T("','")+GetTime()+ _T("')");
	//���ִ�д���ͻᱨ�쳣ԭ��
	try {
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}


CString ADOManage::DZGetRelateData(CString strImei,CString ziduan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	_variant_t val;
	CString GetData = L"";
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[DataRelativeSheet] WHERE [IMEI1] ='") + strImei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
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

/*�Ա�ģ�麯��*/

//�ж�IMEI�Ƿ����
int ADOManage::CheckCompareIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [GPSTest].[dbo].[Gps_ManuLdParam] WHERE [IMEI] ='") + strImei + _T("' AND CompareResult ='1'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//�ԱȺ�������ݲ���
_RecordsetPtr ADOManage::UpdateCompareData(CString strimei,CString result)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE [GPSTest].[dbo].[Gps_ManuLdParam] SET CompareTime = '") + GetTime() + _T("',CompareResult='") + result + _T("' where [IMEI] = '") + strimei + _T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}



//�洢��ǰ�ص��imei��
//_RecordsetPtr ADOManage::UpdateIMEICurrent(CString strzhidan, CString imeicurrent)
//{
	////��ʼ��Recordsetָ��
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//_variant_t Affectline;

	////��IMEI��+1
	//unsigned long long imeiint = 0;
	//imeiint = _atoi64(CStringA(imeicurrent)) + 1;
	//imeicurrent = _ui64toa(imeiint, CT2A(imeicurrent), 10);

	////����һ�����һ��ɨ���IMEI�ţ�һ����˵�������ʧ�ܣ�Ҫ��ʧ���ˣ��ǿ϶��Ƕ����ű��ɵ���
	//CString strSql = _T("UPDATE[") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("]") + _T("SET LDIMEICurrent = '") + imeicurrent + _T("' where ZhiDan = '") + strzhidan + _T("'");

	//try{
	//	m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	//}
	//catch (_com_error &e)
	//{
	//	AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	//}
	//return m_pRecordSet;
//}


//�����������ݿ��ѯģ�����õĺ���
//����Ӧ�Ķ��������Ѿ������IMEIչʾ����
//_RecordsetPtr ADOManage::ShowInsertImeiByOrderNumber(CString ordernumber)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////��ʼ��Recordsetָ��
	//CString strSql = _T("SELECT [IMEI],[InputTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//return m_pRecordSet;
//}

//����Ӧ�Ķ��������Ѿ��ظ���IMEIչʾ����
//_RecordsetPtr ADOManage::ShowRepeatImeiByOrderNumber(CString ordernumber)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////��ʼ��Recordsetָ��
	//CString strSql = _T("SELECT [IMEI],[ReLdImeiNum],[ReLdImeiTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'") + _T("AND [ReLdImeiNum]>0");

	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//return m_pRecordSet;
//}

//����Ӧ�Ķ�������δ�����IMEIչʾ����
//_RecordsetPtr ADOManage::ShowUnImeiByOrderNumber(CString ordernumber, CString imeistart, CString imeiend)
//{
	//m_pRecordSet.CreateInstance(__uuidof(Recordset));
	////��ʼ��Recordsetָ��

	//CString strSql1, strSql2, strSql3, imeitemp;

	////�����һ����ʱ��
	//strSql3 = _T("truncate table[testLD].[dbo].[Gps_ManuUnLdParam]");
	//m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//ֱ��ִ�����

	////Ȼ�����ݲ��뵽һ����ʱ����
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

	////���Ż�ȡ��Χ��û�е�����
	//strSql2 = _T("select[UNIMEI] FROM[testLD].[dbo].[Gps_ManuUnLdParam] where[UNIMEI] not in(select[IMEI] FROM[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] where[zhidan] = '") + ordernumber + _T("')");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql2), NULL, adCmdText);//ֱ��ִ�����

	////��������ʱ��
	//m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//ֱ��ִ�����

	//return m_pRecordSet;
//}


//������ͨ�ú���
//������ݿ�ĳ�ű�
_RecordsetPtr ADOManage::GetRst()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ�� 
	CString strSql;
	strSql = "Truncate table [testLD].[dbo].[testld]";
	//strSql = "select * from BLEStatus";//����ִ�е�SQL���

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//����ѯ���ݵ���m_pRecordset��������
	return m_pRecordSet;
}

//��ȡ��ǰʱ�亯��
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//������IMEIУ��λ�õ�
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