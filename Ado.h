/*########################################################################
	���ļ�����: 	ado.h
	����  �ơ�:		ADO ��װ��.
	����  ����:		0.20
	����  �ߡ�:		����
	��E-mail��:		anyou@sina.com
	 ---------------------------------------------------------
	������ʱ�䡿:		20/1/2003 19:08
	���޸�ʱ�䡿:		09/4/2003
	 ---------------------------------------------------------
	���汾��ʷ��:
		[0.20]  :   ��װ�� CAdoCommand ��.
			[-15/4/2003-]
			
		[0.11]  :   ������һЩ bug.
					ɾ�� oleinit() ����, ��Ϊ���ò��ס�
					�����˼�¼����ȡ����.
			[-09/4/2003-]

		[0.10]	:	��Ҫ���� CAdoConnection �� CAdoRecordSet ������, ��װ��
			��ado�ĳ��õĲ�������.
			[-20/1/2003-]
	 ---------------------------------------------------------
	��ʹ��˵����:
		1. ����������ָ�� msado15.dll ��ȫ·��, һ���� "C:\Program Files\
		   Common Files\System\ADO\" Ŀ¼��.
		2. ��ʹ�ñ���֮ǰ����Ҫ��ʼ�� COM ����, ���Ե��� CoInitialize(NULL)  
		   ����ʼ��, �� CoUninitialize() �ͷ�;
		3. ��ʹ�ü�¼������ǰ�����ȵ��� CAdoConnection::Open() �������ݿ�,
		   ���Ӻ�ɸ���� CAdoRecordSet ����ʹ��, �������ر����ݿ�.
		4. ʹ�ü�¼��ִ��SQL���֮ǰ, Ҫʹ�ù������������ SetAdoConnection() 
		   ���������Ӷ���.
		5. �ڱ�������в����������ı��뾯��:
		   warning: unary minus operator applied to unsigned type, result
		   still unsigned
		   �������˾������,������ StdAfx.h �ļ��м�������һ�д����Խ�ֹ
		   �˾���: 	#pragma warning(disable:4146) 
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADO_H)
#define _ANYOU_COOL_ADO_H

#if _MSC_VER > 1000
#pragma once
#endif 
 
// ���� ado �� -----------------------------------------------------------
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;  

#include <icrsint.h>
class CAdoConnection;
#include "AdoRecordSet.h"
#include "AdoCommand.h"

// ��ֵ����ת�� -----------------------------------
COleDateTime vartodate(const _variant_t& var);
COleCurrency vartocy(const _variant_t& var);
bool vartobool(const _variant_t& var);
BYTE vartoby(const _variant_t& var);
short vartoi(const _variant_t& var);
long vartol(const _variant_t& var);
double vartof(const _variant_t& var);
CString vartostr(const _variant_t& var);

/*########################################################################
			  ------------------------------------------------
						    CAdoConnection class
			  ------------------------------------------------
  ########################################################################*/
//##ModelId=437D26490113
class CAdoConnection
{
// ����/���� ------------------------------------------
public:
	//##ModelId=437D26490114
	CAdoConnection();
	//##ModelId=437D2649011D
	virtual ~CAdoConnection();

protected:
	//##ModelId=437D26490127
	void Release();

// ���� -----------------------------------------------
public:
	// ���Ӷ��� ----------------------------------
	//##ModelId=437D26490131
	_ConnectionPtr& GetConnection() {return m_pConnection;};

	// �쳣��Ϣ ----------------------------------
	//##ModelId=437D26490132
	CString GetLastErrorText();
	//##ModelId=437D26490133
	ErrorsPtr GetErrors();
	//##ModelId=437D2649013B
	ErrorPtr  GetError(long index);

	// �����ִ� ----------------------------------
	//##ModelId=437D26490145
	CString GetConnectionText() {return m_strConnect;}

	// ������Ϣ ----------------------------------
	//##ModelId=437D26490146
	CString GetProviderName();
	//##ModelId=437D26490147
	CString GetVersion();
	//##ModelId=437D26490148
	CString GetDefaultDatabase();

	// ����״̬ ----------------------------------
	//##ModelId=437D2649014F
	BOOL IsOpen();
	//##ModelId=437D26490150
	long GetState();
	
	// ����ģʽ ----------------------------------
	//##ModelId=437D26490151
	ConnectModeEnum GetMode();
	//##ModelId=437D26490159
	BOOL SetMode(ConnectModeEnum mode);

	// ����ʱ�� ----------------------------------
	//##ModelId=437D2649015B
	long GetConnectTimeOut();
	//##ModelId=437D26490163
	BOOL SetConnectTimeOut(long lTime = 5);
	
	// ����Դ��Ϣ -------------------------------
	//##ModelId=437D26490165
	_RecordsetPtr OpenSchema(SchemaEnum QueryType);

// ���� -----------------------------------------------
public:
	// ���ݿ����� --------------------------------
	//##ModelId=437D2649016D
	BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
	//##ModelId=437D26490177
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	//##ModelId=437D26490182
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	//##ModelId=437D2649018D
	BOOL OpenUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
	//##ModelId=437D26490197
	void Close();

// ���� -----------------------------------------------
public:
	// ������ ----------------------------------
	//##ModelId=437D2649019F
	long BeginTrans();
	//##ModelId=437D264901A0
	BOOL RollbackTrans();
	//##ModelId=437D264901A9
	BOOL CommitTrans();

	// ִ�� SQL ��� ------------------------------
	//##ModelId=437D264901AA
	_RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
	//##ModelId=437D264901B3
	BOOL Cancel();

// ���� -----------------------------------------------
protected:
	//##ModelId=437D264901B4
	CString			m_strConnect;
	//##ModelId=437D264901BE
	_ConnectionPtr	m_pConnection;
};

#endif // !defined(_ANYOU_COOL_ADO_H)










































