/*########################################################################
	【文件名】: 	ado.h
	【名  称】:		ADO 封装类.
	【版  本】:		0.20
	【作  者】:		成真
	【E-mail】:		anyou@sina.com
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADOCOMMAND_H)
#define _ANYOU_COOL_ADOCOMMAND_H
#include "Ado.h"

/*########################################################################
			------------------------------------------------
						   CAdoParameter class
			------------------------------------------------
  ########################################################################*/
//##ModelId=437D26480298
class CAdoParameter
{
// 构建/折构 -------------------------------------------
public:
	//##ModelId=437D264802A2
	CAdoParameter();
	//##ModelId=437D264802AC
	CAdoParameter(DataTypeEnum DataType, long lSize, ParameterDirectionEnum Direction, CString strName);
	//##ModelId=437D264802B6
	virtual ~CAdoParameter();
// 属性 ------------------------------------------------
public:
	//##ModelId=437D264802B8
	_ParameterPtr GetParameter() {return m_pParameter;}

	// 参数精度 ---------------------------
	//##ModelId=437D264802C0
	BOOL SetPrecision(char nPrecision);

	// 参数小数位数 -----------------------
	//##ModelId=437D264802C2
	BOOL SetNumericScale(int nScale);

	// 参数类型 ---------------------------
	//##ModelId=437D264802CB
	ParameterDirectionEnum GetDirection();
	//##ModelId=437D264802D4
	BOOL SetDirection(ParameterDirectionEnum Direction);
	
	// 参数名称 ---------------------------
	//##ModelId=437D264802D6
	CString GetName();
	//##ModelId=437D264802DE
	BOOL SetName(CString strName);

	// 参数长度 ---------------------------
	//##ModelId=437D264802E0
	int GetSize();
	//##ModelId=437D264802E8
	BOOL SetSize(int size);

	// 参数据类型 -------------------------
	//##ModelId=437D264802F2
	DataTypeEnum GetType();
	//##ModelId=437D264802F3
	BOOL SetType(DataTypeEnum DataType);

// 方法 ------------------------------------------------
public:	
	//##ModelId=437D264802FC
	BOOL GetValue(COleDateTime &value);
	//##ModelId=437D264802FE
	BOOL GetValue(CString &value);
	//##ModelId=437D26480306
	BOOL GetValue(double &value);
	//##ModelId=437D26480308
	BOOL GetValue(long &value);
	//##ModelId=437D2648030A
	BOOL GetValue(int &value);
	//##ModelId=437D26480311
	BOOL GetValue(short &value);
	//##ModelId=437D26480313
	BOOL GetValue(BYTE &value);
	//##ModelId=437D2648031A
	BOOL GetValue(bool &value);

	//##ModelId=437D2648031C
	BOOL SetValue(const float &value);
	//##ModelId=437D2648031E
	BOOL SetValue(const short &value);
	//##ModelId=437D26480325
	BOOL SetValue(const BYTE &value);
	//##ModelId=437D26480327
	BOOL SetValue(const COleDateTime &value);
	//##ModelId=437D2648032E
	BOOL SetValue(const CString &value);
	//##ModelId=437D26480330
	BOOL SetValue(const double &value);
	//##ModelId=437D26480338
	BOOL SetValue(const long &value);
	//##ModelId=437D2648033A
	BOOL SetValue(const int &value);
	//##ModelId=437D26480342
	BOOL SetValue(const bool &value);
	//##ModelId=437D26480344
	BOOL SetValue(const _variant_t &value);

//其他方法 ------------------------------
public:
	//##ModelId=437D2648034C
	_ParameterPtr& operator =(_ParameterPtr& pParameter);

// 数据 ------------------------------------------------
protected:
	//##ModelId=437D26480357
	_ParameterPtr m_pParameter;
	//##ModelId=437D2648035B
	CString m_strName;
	//##ModelId=437D26480361
	DataTypeEnum m_nType;
};


/*########################################################################
			------------------------------------------------
						   CAdoCommand class
			------------------------------------------------
  ########################################################################*/

//##ModelId=437D26480388
class CAdoCommand
{
// 构建/折构 -------------------------------------------
public:
	//##ModelId=437D26480389
	CAdoCommand();
	//##ModelId=437D2648039C
	CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText = _T(""), CommandTypeEnum CommandType = adCmdStoredProc);
	//##ModelId=437D264803B0
	virtual ~CAdoCommand();

// 属性 ------------------------------------------------
public:
	//##ModelId=437D264803BA
	_variant_t GetValue(LPCTSTR lpstrName);
	//##ModelId=437D264803CE
	_variant_t GetValue(long index);

	//##ModelId=437D264803D8
	_ParameterPtr GetParameter(long index);
	//##ModelId=437D264803E2
	_ParameterPtr GetParamter(LPCTSTR lpstrName);

	//##ModelId=437D26490004
	BOOL Append(_ParameterPtr param);

	//##ModelId=437D2649000E
	ParametersPtr GetParameters();
	
	//##ModelId=437D26490022
	BOOL SetCommandTimeOut(long lTime);

	//##ModelId=437D2649002C
	long GetState();

	//##ModelId=437D26490036
	BOOL SetCommandType(CommandTypeEnum CommandType);
	//##ModelId=437D26490040
	BOOL SetCommandText(LPCTSTR lpstrCommand);

	//##ModelId=437D26490054
	CAdoParameter operator [](int index);
	//##ModelId=437D2649005E
	CAdoParameter operator [](LPCTSTR lpszParamName);

// 实现 ------------------------------------------------
public:
	//##ModelId=437D26490068
	_ParameterPtr CreateParameter(LPCTSTR lpstrName, DataTypeEnum Type, 
								  ParameterDirectionEnum Direction, 
								  long Size, _variant_t Value);
	//##ModelId=437D2649007C
	_RecordsetPtr CAdoCommand::Execute(long Options = adCmdStoredProc);
	//##ModelId=437D26490090
	BOOL Cancel();
	
// 其他方法 --------------------------------------------
public:
	//##ModelId=437D2649009A
	_CommandPtr& GetCommand();
	//##ModelId=437D264900A4
	BOOL SetConnection(CAdoConnection *pConnect);

// 数据 ------------------------------------------------
protected:
	//##ModelId=437D264900B8
	void Release();
	//##ModelId=437D264900CD
	_CommandPtr		m_pCommand;
	//##ModelId=437D264900D1
	CString			m_strSQL;
};

#endif // !defined(_ANYOU_COOL_ADOCOMMAND_H)
