/*########################################################################
	���ļ�����: 	ado.h
	����  �ơ�:		ADO ��װ��.
	����  ����:		0.20
	����  �ߡ�:		����
	��E-mail��:		anyou@sina.com
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADORECORDSET_H)
#define _ANYOU_COOL_ADORECORDSET_H
#include "Ado.h"

/*########################################################################
			 ------------------------------------------------
						  CAdoRecordSet class
			 ------------------------------------------------
  ########################################################################*/

//##ModelId=437D26420357
class CAdoRecordSet 
{
// ����/�۹� --------------------------------------------
public:
	//##ModelId=437D26420358
	CAdoRecordSet();
	//##ModelId=437D26430006
	CAdoRecordSet(CAdoConnection *pConnection);

	//##ModelId=437D26430092
	virtual ~CAdoRecordSet();
	
protected:
	//##ModelId=437D2643011E
	void Release();
	
// ���� ------------------------------------------------	
public:
	// ��ǰ�༭״̬ ----------------------------
	//##ModelId=437D2643018C
	EditModeEnum GetEditMode();
	
	// ��ǰ״̬ --------------------------------
	//##ModelId=437D264301C8
	BOOL IsEOF();
	//##ModelId=437D2643020E
	BOOL IsBOF();
	//##ModelId=437D26430254
	BOOL IsOpen();
	//##ModelId=437D2643029A
	long GetState();
	//##ModelId=437D264302EB
	long GetStatus();

	// �����ص�����¼�� --------------------
	//##ModelId=437D2643033B
	long GetMaxRecordCount();
	//##ModelId=437D26430381
	BOOL SetMaxRecordCount(long count);
	
	// ���λ�� --------------------------------
	//##ModelId=437D264303C7
	CursorLocationEnum GetCursorLocation();
	//##ModelId=437D26440011
	BOOL SetCursorLocation(CursorLocationEnum CursorLocation = adUseClient);
	
	// ������� --------------------------------
	//##ModelId=437D26440043
	CursorTypeEnum GetCursorType();
	//##ModelId=437D26440075
	BOOL SetCursorType(CursorTypeEnum CursorType = adOpenStatic);
	
	// ��ǩ --------------------------------
	//##ModelId=437D264400B1
	_variant_t GetBookmark();
	//##ModelId=437D264400B2
	BOOL SetBookmark(_variant_t varBookMark = _variant_t((long)adBookmarkFirst));
	
	// ��ǰ��¼λ�� ------------------------
	//##ModelId=437D264400BC
	long GetAbsolutePosition();
	//##ModelId=437D264400ED
	BOOL SetAbsolutePosition(int nPosition);

	//##ModelId=437D26440129
	long GetAbsolutePage();
	//##ModelId=437D26440165
	BOOL SetAbsolutePage(int nPage);

	// ÿҳ�ļ�¼�� ------------------------
	//##ModelId=437D264401A2
	long GetPageSize();
	//##ModelId=437D264401DE
	BOOL SetCacheSize(const long& lCacheSize);	

	// ҳ�� --------------------------------
	//##ModelId=437D26440224
	long GetPageCount();

	// ��¼�����ֶ��� ----------------------
	//##ModelId=437D26440260
	long GetRecordCount();
	//##ModelId=437D264402B0
	long GetFieldsCount();
	
	// ��ѯ�ַ��� --------------------------
	//##ModelId=437D264402F6
	CString GetSQLText() {return m_strSQL;}
	//##ModelId=437D264402F7
	void SetSQLText(LPCTSTR strSQL) {m_strSQL = strSQL;}
	
	// ���Ӷ��� ----------------------------
	//##ModelId=437D26440300
	CAdoConnection* GetConnection() {return m_pConnection;}
	//##ModelId=437D26440301
	void SetAdoConnection(CAdoConnection *pConnection);

	// ��¼������ --------------------------
	//##ModelId=437D26440303
	_RecordsetPtr& GetRecordset();

	//##ModelId=437D2644030A
	CString GetLastError();

// �ֶ����� ----------------------------------------------
public:
	// �ֶμ� -------------------------------
	//##ModelId=437D2644030B
	FieldsPtr GetFields();

	// �ֶζ��� -----------------------------
	//##ModelId=437D2644033C
	FieldPtr  GetField(long lIndex);
	//##ModelId=437D26440364
	FieldPtr  GetField(LPCTSTR lpszFieldName);
	
	// �ֶ��� -------------------------------
	//##ModelId=437D26440382
	CString GetFieldName(long lIndex);
	
	// �ֶ��������� -------------------------
	//##ModelId=437D264403B4
	DataTypeEnum GetFieldType(long lIndex);
	//##ModelId=437D264403DC
	DataTypeEnum GetFieldType(LPCTSTR lpszFieldName);

	// �ֶ����� -----------------------------
	//##ModelId=437D2645001C
	long  GetFieldAttributes(long lIndex);
	//##ModelId=437D2645004F
	long  GetFieldAttributes(LPCTSTR lpszFieldName);

	// �ֶζ��峤�� -------------------------
	//##ModelId=437D26450081
	long  GetFieldDefineSize(long lIndex);
	//##ModelId=437D264500B3
	long  GetFieldDefineSize(LPCTSTR lpszFieldName);

	// �ֶ�ʵ�ʳ��� -------------------------
	//##ModelId=437D264500DB
	long  GetFieldActualSize(long lIndex);
	//##ModelId=437D2645010D
	long  GetFieldActualSize(LPCTSTR lpszFieldName);

	// �ֶ��Ƿ�ΪNULL -----------------------
	//##ModelId=437D26450135
	BOOL  IsFieldNull(long index);
	//##ModelId=437D2645015D
	BOOL  IsFieldNull(LPCTSTR lpFieldName);

// ��¼���� --------------------------------------------
public:
	//##ModelId=437D2645017B
	BOOL AddNew();
	//##ModelId=437D264501E9
	BOOL Update();
	//##ModelId=437D26450257
	BOOL UpdateBatch(AffectEnum AffectRecords = adAffectAll); 
	//##ModelId=437D264502C5
	BOOL CancelUpdate();
	//##ModelId=437D26450320
	BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
	//##ModelId=437D26450384
	BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);
	
	// ˢ�¼�¼���е����� ------------------
	//##ModelId=437D26460000
	BOOL Requery(long Options = adConnectUnspecified);
	//##ModelId=437D26460082
	BOOL Resync(AffectEnum AffectRecords = adAffectAll, ResyncEnum ResyncValues = adResyncAllValues);   

	//##ModelId=437D26460104
	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	//##ModelId=437D2646010E
	BOOL AddNew(CADORecordBinding &pAdoRecordBinding);
	
// ��¼���������� --------------------------------------
public:
	//##ModelId=437D26460110
	BOOL MoveFirst();
	//##ModelId=437D26460168
	BOOL MovePrevious();
	//##ModelId=437D264601C2
	BOOL MoveNext();
	//##ModelId=437D2646021D
	BOOL MoveLast();
	//##ModelId=437D26460277
	BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));
	
	// ����ָ���ļ�¼ ----------------------
	//##ModelId=437D264602C7
	BOOL Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
	//##ModelId=437D264602D3
	BOOL FindNext();

// ��ѯ ------------------------------------------------
public:
	//##ModelId=437D264602D4
	BOOL Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	//##ModelId=437D2646035D
	BOOL Cancel();
	//##ModelId=437D264603D5
	void Close();

	// ����/����־����ļ� -----------------
	//##ModelId=437D26470065
	BOOL Save(LPCTSTR strFileName = _T(""), PersistFormatEnum PersistFormat = adPersistXML);
	//##ModelId=437D264700E8
	BOOL Load(LPCTSTR strFileName);
	
// �ֶδ�ȡ --------------------------------------------
public:
	//##ModelId=437D26470160
	BOOL PutCollect(long index, const _variant_t &value);
	//##ModelId=437D2647017E
	BOOL PutCollect(long index, const CString &value);
	//##ModelId=437D2647019C
	BOOL PutCollect(long index, const double &value);
	//##ModelId=437D264701BA
	BOOL PutCollect(long index, const float  &value);
	//##ModelId=437D264701D8
	BOOL PutCollect(long index, const long   &value);
	//##ModelId=437D264701F6
	BOOL PutCollect(long index, const DWORD  &value);
	//##ModelId=437D2647021E
	BOOL PutCollect(long index, const int    &value);
	//##ModelId=437D2647023C
	BOOL PutCollect(long index, const short  &value);
	//##ModelId=437D2647025A
	BOOL PutCollect(long index, const BYTE   &value);
	//##ModelId=437D26470282
	BOOL PutCollect(long index, const bool   &value);
	//##ModelId=437D264702AA
	BOOL PutCollect(long index, const COleDateTime &value);
	//##ModelId=437D264702BE
	BOOL PutCollect(long index, const COleCurrency &value);

	//##ModelId=437D264702DC
	BOOL PutCollect(LPCTSTR strFieldName, const _variant_t &value);
	//##ModelId=437D264702E6
	BOOL PutCollect(LPCTSTR strFieldName, const CString &value);
	//##ModelId=437D264702FA
	BOOL PutCollect(LPCTSTR strFieldName, const double &value);
	//##ModelId=437D26470318
	BOOL PutCollect(LPCTSTR strFieldName, const float  &value);
	//##ModelId=437D26470336
	BOOL PutCollect(LPCTSTR strFieldName, const long   &value);
	//##ModelId=437D26470355
	BOOL PutCollect(LPCTSTR strFieldName, const DWORD  &value);
	//##ModelId=437D26470373
	BOOL PutCollect(LPCTSTR strFieldName, const int    &value);
	//##ModelId=437D26470391
	BOOL PutCollect(LPCTSTR strFieldName, const short  &value);
	//##ModelId=437D264703AF
	BOOL PutCollect(LPCTSTR strFieldName, const BYTE   &value);
	//##ModelId=437D264703CD
	BOOL PutCollect(LPCTSTR strFieldName, const bool   &value);
	//##ModelId=437D2648000D
	BOOL PutCollect(LPCTSTR strFieldName, const COleDateTime &value);
	//##ModelId=437D2648002B
	BOOL PutCollect(LPCTSTR strFieldName, const COleCurrency &value);

	// ---------------------------------------------------------

	//##ModelId=437D2648003F
	BOOL GetCollect(long index, CString &value);
	//##ModelId=437D2648004A
	BOOL GetCollect(long index, double  &value);
	//##ModelId=437D2648005D
	BOOL GetCollect(long index, float   &value);
	//##ModelId=437D26480067
	BOOL GetCollect(long index, long    &value);
	//##ModelId=437D2648007B
	BOOL GetCollect(long index, DWORD   &value);
	//##ModelId=437D2648008F
	BOOL GetCollect(long index, int     &value);
	//##ModelId=437D26480099
	BOOL GetCollect(long index, short   &value);
	//##ModelId=437D264800AD
	BOOL GetCollect(long index, BYTE    &value);
	//##ModelId=437D264800CB
	BOOL GetCollect(long index, bool   &value);
	//##ModelId=437D264800DF
	BOOL GetCollect(long index, COleDateTime &value);
	//##ModelId=437D264800F3
	BOOL GetCollect(long index, COleCurrency &value);

	//##ModelId=437D26480107
	BOOL GetCollect(LPCSTR strFieldName, CString &strValue);
	//##ModelId=437D26480112
	BOOL GetCollect(LPCSTR strFieldName, double &value);
	//##ModelId=437D2648011B
	BOOL GetCollect(LPCSTR strFieldName, float  &value);
	//##ModelId=437D26480125
	BOOL GetCollect(LPCSTR strFieldName, long   &value);
	//##ModelId=437D26480139
	BOOL GetCollect(LPCSTR strFieldName, DWORD  &value);
	//##ModelId=437D2648014D
	BOOL GetCollect(LPCSTR strFieldName, int    &value);
	//##ModelId=437D26480157
	BOOL GetCollect(LPCSTR strFieldName, short  &value);
	//##ModelId=437D2648016B
	BOOL GetCollect(LPCSTR strFieldName, BYTE   &value);
	//##ModelId=437D26480189
	BOOL GetCollect(LPCSTR strFieldName, bool   &value);
	//##ModelId=437D2648019D
	BOOL GetCollect(LPCSTR strFieldName, COleDateTime &value);
	//##ModelId=437D264801A0
	BOOL GetCollect(LPCSTR strFieldName, COleCurrency &value);

	// BLOB ���ݴ�ȡ ------------------------------------------
	//##ModelId=437D264801A9
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
	//##ModelId=437D264801B1
	BOOL AppendChunk(long index, LPVOID lpData, UINT nBytes);
	//##ModelId=437D264801BC
	BOOL AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes);
	//##ModelId=437D264801C7
	BOOL AppendChunk(long index, LPCTSTR lpszFileName);
	//##ModelId=437D264801D1
	BOOL AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName);

	//##ModelId=437D264801DB
	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	//##ModelId=437D264801E5
	BOOL GetChunk(long index, LPVOID lpData);
	//##ModelId=437D264801EE
	BOOL GetChunk(LPCSTR strFieldName, LPVOID lpData);
	//##ModelId=437D264801F7
	BOOL GetChunk(long index, CBitmap &bitmap);
	//##ModelId=437D26480201
	BOOL GetChunk(LPCSTR strFieldName, CBitmap &bitmap);

// �������� --------------------------------------------
public:
	// ���˼�¼ ---------------------------------
	//##ModelId=437D26480204
	BOOL SetFilter(LPCTSTR lpszFilter);

	// ���� -------------------------------------
	//##ModelId=437D2648020C
	BOOL SetSort(LPCTSTR lpszCriteria);

	// �����Ƿ�֧��ĳ���� -----------------------
	//##ModelId=437D2648020E
	BOOL Supports(CursorOptionEnum CursorOptions = adAddNew);

	// ��¡ -------------------------------------
	//##ModelId=437D26480216
	BOOL Clone(CAdoRecordSet &pRecordSet);
	//##ModelId=437D26480218
	_RecordsetPtr operator = (_RecordsetPtr &pRecordSet);
	
	// ��ʽ�� _variant_t ����ֵ -----------------
	
//��Ա���� --------------------------------------------
protected:
	//##ModelId=437D26480222
	CAdoConnection     *m_pConnection;
	//##ModelId=437D2648022B
	_RecordsetPtr		m_pRecordset;
	//##ModelId=437D2648022F
	CString				m_strSQL;
	//##ModelId=437D26480234
	CString				m_strFind;
	//##ModelId=437D2648023E
	CString				m_strFileName;
	//##ModelId=437D26480240
	IADORecordBinding	*m_pAdoRecordBinding;
	//##ModelId=437D26480249
	SearchDirectionEnum m_SearchDirection;
public:
	//##ModelId=437D26480253
	_variant_t			m_varBookmark;
};
//________________________________________________________________________

#endif //_ANYOU_COOL_ADORECORDSET_H
