// TPtable.cpp: CTblBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

//#include "empty.h"
#include "stdafx.h"
#include "TPtable.h"
#include "potm.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////
// CTblBase
//////////////////////////////////////////////////////////////////////

CppSQLite3DB CTblBase::m_pDB;	// Database操作クラス
int CTblBase::m_linkDB = 0;		// Database操作クラスオブジェクトリンク数

CTblBase::CTblBase(CString strDbPath)
{
	m_blTransaction = FALSE;

	if (m_linkDB == 0) {
		m_pDB.open(strDbPath);
	}
	++m_linkDB;
}

CTblBase::~CTblBase()
{
	--m_linkDB;
	if (m_linkDB == 0) {
		m_Query.finalize();
		m_pDB.close();
	}
}

/*
//
//  機能     : DBオープン
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::OpenDb(CString strDbPath)
{
	try {
		m_pDB.open(strDbPath);
	} catch(CppSQLite3Exception *e) {
		blDbFlag = FALSE;
		return FALSE;
	}

	blDbFlag = TRUE;
	return TRUE;
}

//
//  機能     : DBクローズ
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::CloseDb()
{
	m_pDB.close();
	CloseTbl();
	blDbFlag = FALSE;
	return TRUE;
}
*/

//
//  機能     : Tblオープン
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::OpenTbl(CString strTblName)
{
	m_TblName = strTblName;
	return TRUE;
}

//
//  機能     : Tblクローズ
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::CloseTbl()
{
	m_TblName.Empty();
	return TRUE;
}

//
//  機能     : テーブル存在確認
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::ExistTbl()		
{
	BOOL blRet = TRUE;

	try {
		blRet = (BOOL)m_pDB.tableExists(m_TblName);
	} catch(CppSQLite3Exception *e) {
		blRet = FALSE;
	}

	return blRet;
}

//
//  機能     : レコード用領域の初期化(規定値)
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CTblBase::InitRec() 
{
	ID = 0;
	lngClsCode1 = 0;
	lngClsCode2 = 0;
	lngClsCode3 = 0;

	strRemarks1 = "";
	strRemarks2 = "";
	dlngSystemNew = "0";
	dlngSystemUpdate = "0";
	lngSystemFlag = 0;
	strSystemOperator = "";
	strSystemName = "";
	strSystemVersion = "";
}

//
//  機能     : "ID"を除くフィールド文字列(規定値)
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblBase::GetFldsString()
{
	CString str;

	/// 入力フィールドリスト
	str = _T("lngClsCode1,lngClsCode2,lngClsCode3");

	str = str + _T(",") + _T("strRemarks1,strRemarks2,lngSystemNew,lngSystemUpdate");
	str = str + _T(",") + _T("lngSystemFlag,strSystemOperator,strSystemName,strSystemVersion");

	return str;
}

//
//  機能     : "ID"を除くフィールド値文字列(規定値)
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblBase::GetValsString()
{
	CString str;
	_TCHAR buf[32];

	//// 入力値リスト
	_ltot_s(lngClsCode1, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = buf;
	_ltot_s(lngClsCode2, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	_ltot_s(lngClsCode3, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;

	str = str + _T(",'") + strRemarks1 + _T("'");
	str = str + _T(",'") + strRemarks2 + _T("'");
	str = str + _T(",") + dlngSystemNew; 
	str = str + _T(",") + dlngSystemUpdate; 
	_ltot_s(lngSystemFlag, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	str = str + _T(",'") + strSystemOperator + _T("'");
	str = str + _T(",'") + strSystemName + _T("'");
	str = str + _T(",'") + strSystemVersion + _T("'");

	return str;
}

//
//  機能     : "ID"を除くフィールド値設定文字列(規定値)
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblBase::GetSetsString()
{
	CString str;
	_TCHAR buf[32];

	//// 入力値リスト
	str.Empty();
	_ltot_s(lngClsCode1, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T("lngClsCode1") + _T("=") + buf;
	_ltot_s(lngClsCode2, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngClsCode2") + _T("=") + buf; 
	_ltot_s(lngClsCode3, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngClsCode3") + _T("=") + buf; 

	str = str + _T(",") + _T("strRemarks1") + _T("='") + strRemarks1 + _T("'");
	str = str + _T(",") + _T("strRemarks2") + _T("='") + strRemarks2 + _T("'");
	str = str + _T(",") + _T("lngSystemNew") + _T("=") + dlngSystemNew; 
	str = str + _T(",") + _T("lngSystemUpdate") + _T("=") + dlngSystemUpdate; 
	_ltot_s(lngSystemFlag, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngSystemFlag") + _T("=") + buf;
	str = str + _T(",") + _T("strSystemOperator") + _T("='") + strSystemOperator + _T("'");
	str = str + _T(",") + _T("strSystemName") + _T("='") + strSystemName + _T("'");
	str = str + _T(",") + _T("strSystemVersion") + _T("='") + strSystemVersion + _T("'");

	return str;
}

//
//  機能     : トランザクション開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::TransactionStart()	
{
	BOOL blRet = TRUE;

	if (!m_blTransaction) return FALSE;

	/// クエリー中であれば解除する
	m_Query.finalize();

	try {
		m_pDB.execDML(_T("begin transaction;"));
	} catch(CppSQLite3Exception *e) {
		blRet = FALSE;
	}

	return blRet;
}

//
//  機能     : トランザクション終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::TransactionEnd()	
{
	BOOL blRet = TRUE;

	if (!m_blTransaction) return FALSE;

	try {
		m_pDB.execDML(_T("commit transaction;"));
	} catch(CppSQLite3Exception *e) {
		m_pDB.execDML(_T("rollback transaction;"));
		blRet = FALSE;
	}

	m_blTransaction = FALSE;
	return blRet;
}

//
//  機能     : トランザクションを使用したSQL実行
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::TransactionExe(
CString strSql)		// SQL
{
	BOOL blRet = TRUE;

	/// クエリー中であれば解除する
	m_Query.finalize();

	/// 実行
	m_pDB.execDML(_T("begin transaction;"));
	try {
		int nRows = m_pDB.execDML(strSql);
		if (nRows > 0) {
			m_pDB.execDML(_T("commit transaction;"));
		} else {
			m_pDB.execDML(_T("rollback transaction;"));
			blRet = FALSE;
		}
	} catch(CppSQLite3Exception *e) {
		m_pDB.execDML(_T("rollback transaction;"));
		blRet = FALSE;
	}

	return blRet;
}

//
//  機能     : RSFLテーブルから現在のレコードの選択結果取得(規定値)
//  
//  機能説明 : SelectRec()実行後に有効
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::GetRec()	
{
	if (m_linkDB == 0) return FALSE;
	if (m_Query.eof()) return FALSE;

	ID = m_Query.getIntField(_T("ID"));
	lngClsCode1 = m_Query.getIntField(_T("lngClsCode1"));
	lngClsCode2 = m_Query.getIntField(_T("lngClsCode2"));
	lngClsCode3 = m_Query.getIntField(_T("lngClsCode3"));

	strRemarks1 = m_Query.getStringField(_T("strRemarks1"));
	strRemarks2 = m_Query.getStringField(_T("strRemarks2"));
	dlngSystemNew = m_Query.getStringField(_T("lngSystemNew"));
	dlngSystemUpdate = m_Query.getStringField(_T("lngSystemUpdate"));
	lngSystemFlag = m_Query.getIntField(_T("lngSystemFlag"));
	strSystemOperator = m_Query.getStringField(_T("strSystemOperator"));
	strSystemName = m_Query.getStringField(_T("strSystemName"));
	strSystemVersion = m_Query.getStringField(_T("strSystemVersion"));

	return TRUE;
}

//
//  機能     : フィールドのMAX値
//  
//  機能説明 : 型が一定でないので、doubleで返す
//  
//  返り値   : MAX値文字列、Empty:該当データなし
//  
//  備考     : 
//  
double CTblBase::MaxVal(
CString strFld,		// 対象フィールド名
CString strWhere)	// 選択用where句、Empty:all record
{
	if (m_linkDB == 0) return -1;
	if (m_TblName.IsEmpty()) return -1;

	CString strSql;
	strSql.Empty();
	strSql = _T("select max(") + strFld + _T(") from ") + m_TblName;
	if (strWhere.IsEmpty()) {
		strSql = strSql + _T(";");
	} else {
		strSql = strSql + _T(" where ") + strWhere + _T(";");
	}

	m_Query.finalize();
	m_Query = m_pDB.execQuery(strSql);
	return m_Query.getFloatField(_T("max(") + strFld + _T(")"));
}

//
//  機能     : フィールドのMIN値
//  
//  機能説明 : 型が一定でないので、doubleで返す
//  
//  返り値   : MIN値文字列、Empty:該当データなし
//  
//  備考     : 
//  
double CTblBase::MinVal(
CString strFld,		// 対象フィールド名
CString strWhere)	// 選択用where句、Empty:all record
{
	if (m_linkDB == 0) return -1;
	if (m_TblName.IsEmpty()) return -1;

	CString strSql;
	strSql.Empty();
	strSql = _T("select min(") + strFld + _T(") from ") + m_TblName;
	if (strWhere.IsEmpty()) {
		strSql = strSql + _T(";");
	} else {
		strSql = strSql + _T(" where ") + strWhere + _T(";");
	}

	m_Query.finalize();
	m_Query = m_pDB.execQuery(strSql);
	return m_Query.getFloatField(_T("min(") + strFld + _T(")"));
}

//
//  機能     : テーブルからレコード数取得
//  
//  機能説明 : 
//  
//  返り値   : 選択レコード数、-1:error
//  
//  備考     : 
//  
long CTblBase::CountRec(
CString strWhere)		// 選択用where句、Empty:all record
{
	if (m_linkDB == 0) return -1;
	if (m_TblName.IsEmpty()) return -1;

	/// 選択レコード数
	CString strSql;
	strSql.Empty();
	strSql = _T("select count(*) from ") + m_TblName;
	if (strWhere.IsEmpty()) {
		strSql = strSql + _T(";");
	} else {
		strSql = strSql + _T(" where ") + strWhere + _T(";");
	}
TRACE(_T("%s\n"), strSql.Left(500));

	m_Query.finalize();
	m_Query = m_pDB.execQuery(strSql);
	long lngCnt = m_Query.getIntField(_T("count(*)"));
	if (lngCnt == 0) return 0;

	return lngCnt;
}

//
//  機能     : テーブルからレコードの選択(ID)
//  
//  機能説明 : 最初のレコード内容取得
//  
//  返り値   : 選択レコード数、-1:error
//  
//  備考     : 
//  
long CTblBase::SelectRec(
long lngID)		// ID
{
	CString strWhere;
	_TCHAR buf[32];

	strWhere.Empty();
	_ltot_s(lngID, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("ID=") + buf;

	return SelectRec(strWhere);
}

//
//  機能     : テーブルからレコードの選択(SQL)
//  
//  機能説明 : 最初のレコード内容取得
//  
//  返り値   : 選択レコード数、-1:error
//  
//  備考     : 
//  
long CTblBase::SelectRec(
CString strWhere)		// 選択用where句
{
	if (m_linkDB == 0) return -1;

	/// 選択レコード数確認
	long lngCnt = CountRec(strWhere);
	if (lngCnt <= 0) return 0;

	/// レコード選択
	CString strSql;
	strSql.Empty();
	strSql = _T("select * from ") + m_TblName + _T(" where ") + strWhere + _T(";");
TRACE(_T("%s\n"), strSql.Left(500));

	m_Query.finalize();
	m_Query = m_pDB.execQuery(strSql);
	if (m_Query.eof()) return 0;

	if (!GetRec()) return -1;
	return lngCnt;
}

//
//  機能     : テーブルから次レコードの選択結果取得
//  
//  機能説明 : SelectRec()実行後に有効
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::NextRec()	
{
	if (m_linkDB == 0) return FALSE;
	if (m_Query.eof()) return FALSE;

	m_Query.nextRow();
	return GetRec();
}

//
//  機能     : テーブルへレコード追加
//  
//  機能説明 : "ID"はAUTO設定
//  
//  返り値   : BOOL
//  
//  備考     : (???)transaction を使用するとエラー
//
BOOL CTblBase::InsertRec(CString strDate)
{
	/// insert時の規定値
	dlngSystemNew = strDate;
	dlngSystemUpdate = strDate;
	strSystemOperator = _T(POTM_OPERATOR);
	strSystemName = _T(POTM_APPNAME);
	strSystemVersion = _T(POTM_VERSION);

	/// 入力フィールドリスト
	CString strFlds = GetFldsString();
	/// 入力値リスト
	CString strVals = GetValsString();

	/// insert用SQL
	CString strSql;
	strSql.Empty();
	strSql = _T("insert into ") + m_TblName + 
		_T(" (") + strFlds + _T(") values (") + strVals + _T(");");
TRACE(_T("%s\n"), strSql.Left(500));
TRACE(_T("%s\n%s\n"), strFlds.Left(250), strVals.Left(250));

	/// 実行
	if (m_blTransaction) {
		if (m_pDB.execDML(strSql) < 0) return FALSE;
		return TRUE;
	} else {
		return TransactionExe(strSql);
	}
}
BOOL CTblBase::InsertRec()
{
	CString strDate = GetDateStr();
	return InsertRec(strDate);
}

//
//  機能     : テーブルのレコード更新(ID)
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::UpdateRec(
long lngID)	// 更新レコードの"ID"フィールド値
{
	_TCHAR buf[32];

	/// update時の規定値
	CString strDate = GetDateStr();
	dlngSystemUpdate = strDate;
	strSystemOperator = _T(POTM_OPERATOR);
	strSystemName = _T(POTM_APPNAME);
	strSystemVersion = _T(POTM_VERSION);

	/// 入力設定値リスト
	CString strSets = GetSetsString();

	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngID, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("ID=") + buf;

	return UpdateRec(strSets, strWhere);
}

//
//  機能     : テーブルのレコード更新(SQL)
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::UpdateRec(
CString strSets,	// 入力設定値リスト
CString strWhere)	// 更新レコード選択where句
{
	/// update用SQL
	CString strSql;
	strSql.Empty();
	strSql = _T("update ") + m_TblName + _T(" set ") + strSets + 
		_T(" where ") + strWhere + _T(";");
TRACE(_T("%s\n"), strSql.Left(500));

	/// 実行
	if (m_blTransaction) {
		if (m_pDB.execDML(strSql) < 0) return FALSE;
		return TRUE;
	} else {
		return TransactionExe(strSql);
	}
}

//
//  機能     : テーブルのレコード削除(ID)
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::DeleteRec(
long lngID)	// 削除レコードの"ID"フィールド値
{
	_TCHAR buf[32];
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngID, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("ID=") + buf;

	/// 実行
	return DeleteRec(strWhere);
}

//
//  機能     : テーブルのレコード削除(SQL)
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblBase::DeleteRec(
CString strWhere)	// 削除レコード選択where句
{
	/// delete用SQL
	CString strSql;
	strSql.Empty();
	strSql = _T("delete from ") + m_TblName + _T(" where ") + strWhere + _T(";");
TRACE(_T("%s\n"), strSql.Left(500));

	/// 実行
	if (m_blTransaction) {
		if (m_pDB.execDML(strSql) < 0) return FALSE;
		return TRUE;
	} else {
		return TransactionExe(strSql);
	}
}

//
//  機能     : 日時をロング整数形式の文字列で生成
//  
//  機能説明 : ex) "200501010100"
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblBase::GetDateStr()
{
	time_t ct = time(NULL);
	struct tm lst;
	localtime_s(&lst, &ct);

	_TCHAR str[64];
	_stprintf_s(str, sizeof(str)/sizeof(str[0]), _T("%04d%02d%02d%02d%02d%02d"), (1900+lst.tm_year), (1+lst.tm_mon),
		lst.tm_mday, lst.tm_hour, lst.tm_min, lst.tm_sec);

	return str;
}

//////////////////////////////////////////////////////////////////////
// CTblRsfl
//////////////////////////////////////////////////////////////////////

//
//  機能     : Constructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblRsfl::CTblRsfl(CString strDbPath) : CTblBase(strDbPath)
{
	m_TblName = _T("rsfl");
	InitRec(); 
}

//
//  機能     : Destructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblRsfl::~CTblRsfl()
{
	m_TblName.Empty();
}

//
//  機能     : レコード用領域の初期化
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CTblRsfl::InitRec() 
{
	CTblBase::InitRec();

	strSenderId = "";
	strSenderName = "";
	strReceiverId = "";
	strReceiverName = "";
	lngRespType = 0;
	lngDefaultFlag = 0;
	lngFolderId = 0;
	lngLinkId = 0;
	lngSqlId = 0;
}

//
//  機能     : "ID"を除くフィールド文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblRsfl::GetFldsString()
{
	CString str;

	str = CTblBase::GetFldsString();

	/// 入力フィールドリスト
	str = str + _T(",") + _T("strSenderId");
	str = str + _T(",") + _T("strSenderName");
	str = str + _T(",") + _T("strReceiverId");
	str = str + _T(",") + _T("strReceiverName");
	str = str + _T(",") + _T("lngRespType");
	str = str + _T(",") + _T("lngDefaultFlag");
	str = str + _T(",") + _T("lngFolderId");
	str = str + _T(",") + _T("lngLinkId");
	str = str + _T(",") + _T("lngSqlId");

	return str;
}

//
//  機能     : "ID"を除くフィールド値文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblRsfl::GetValsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetValsString();

	//// 入力値リスト
	str = str + _T(",'") + strSenderId + _T("'");
	str = str + _T(",'") + strSenderName + _T("'");
	str = str + _T(",'") + strReceiverId + _T("'");
	str = str + _T(",'") + strReceiverName + _T("'");
	_ltot_s(lngRespType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	_ltot_s(lngDefaultFlag, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf; 
	_ltot_s(lngLinkId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf; 
	_ltot_s(lngSqlId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf; 

	return str;
}

//
//  機能     : "ID"を除くフィールド値設定文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblRsfl::GetSetsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetSetsString();

	//// 入力値リスト
	str = str + _T(",") + _T("strSenderId") + _T("='") + strSenderId + _T("'");
	str = str + _T(",") + _T("strSenderName") + _T("='") + strSenderName + _T("'");
	str = str + _T(",") + _T("strReceiverId") + _T("='") + strReceiverId + _T("'");
	str = str + _T(",") + _T("strReceiverName") + _T("='") + strReceiverName + _T("'");
	_ltot_s(lngRespType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngRespType") + _T("=") + buf;
	_ltot_s(lngDefaultFlag, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngDefaultFlag") + _T("=") + buf; 
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngFolderId") + _T("=") + buf;
	_ltot_s(lngLinkId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngLinkId") + _T("=") + buf;
	_ltot_s(lngSqlId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngSqlId") + _T("=") + buf;

	return str;
}

//
//  機能     : RSFLテーブルから現在のレコードの選択結果取得
//  
//  機能説明 : SelectRec()実行後に有効
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblRsfl::GetRec()	
{
	if (m_linkDB == 0) return FALSE;
	if (m_Query.eof()) return FALSE;

	if (!CTblBase::GetRec()) return FALSE;

	strSenderId = m_Query.getStringField(_T("strSenderId"));
	strSenderName = m_Query.getStringField(_T("strSenderName"));
	strReceiverId = m_Query.getStringField(_T("strReceiverId"));
	strReceiverName = m_Query.getStringField(_T("strReceiverName"));
	lngRespType = m_Query.getIntField(_T("lngRespType"));
	lngDefaultFlag = m_Query.getIntField(_T("lngDefaultFlag"));
	lngFolderId = m_Query.getIntField(_T("lngFolderId"));
	lngLinkId = m_Query.getIntField(_T("lngLinkId"));
	lngSqlId = m_Query.getIntField(_T("lngSqlId"));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTblFolder
//////////////////////////////////////////////////////////////////////

//
//  機能     : Constructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblFolder::CTblFolder(CString strDbPath) : CTblBase(strDbPath)
{
	m_TblName = _T("folder");
	InitRec(); 
}

//
//  機能     : Destructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblFolder::~CTblFolder()
{
	m_TblName.Empty();
}

//
//  機能     : レコード用領域の初期化
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CTblFolder::InitRec() 
{
	CTblBase::InitRec();

	strReceiverId = "";
	strTitle = "";
	lngFolderType = 0;
}

//
//  機能     : "ID"を除くフィールド文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblFolder::GetFldsString()
{
	CString str;

	str = CTblBase::GetFldsString();

	/// 入力フィールドリスト
	str = str + _T(",") + _T("strReceiverId");
	str = str + _T(",") + _T("strTitle");
	str = str + _T(",") + _T("lngFolderType");

	return str;
}

//
//  機能     : "ID"を除くフィールド値文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblFolder::GetValsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetValsString();

	//// 入力値リスト
	str = str + _T(",'") + strReceiverId + _T("'");
	str = str + _T(",'") + strTitle + _T("'");
	_ltot_s(lngFolderType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;

	return str;
}

//
//  機能     : "ID"を除くフィールド値設定文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblFolder::GetSetsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetSetsString();

	//// 入力値リスト
	str = str + _T(",") + _T("strReceiverId") + _T("='") + strReceiverId + _T("'");
	str = str + _T(",") + _T("strTitle") + _T("='") + strTitle + _T("'");
	_ltot_s(lngFolderType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngFolderType") + _T("=") + buf;

	return str;
}

//
//  機能     : Folderテーブルから現在のレコードの選択結果取得
//  
//  機能説明 : SelectRec()実行後に有効
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblFolder::GetRec()	
{
	if (m_linkDB == 0) return FALSE;
	if (m_Query.eof()) return FALSE;

	if (!CTblBase::GetRec()) return FALSE;

	strReceiverId = m_Query.getStringField(_T("strReceiverId"));
	strTitle = m_Query.getStringField(_T("strTitle"));
	lngFolderType = m_Query.getIntField(_T("lngFolderType"));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTblRdfl
//////////////////////////////////////////////////////////////////////

//
//  機能     : Constructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblRdfl::CTblRdfl(CString strDbPath) : CTblBase(strDbPath)
{
	m_TblName = _T("rdfl");
	InitRec(); 
}

//
//  機能     : Destructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblRdfl::~CTblRdfl()
{
	m_TblName.Empty();
}

//
//  機能     : レコード用領域の初期化
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CTblRdfl::InitRec() 
{
	CTblBase::InitRec();

	lngMsgType = 0;
	strSenderId = "";
	strSenderName = "";
	strReceiverId = "";
	strReceiverName = "";
	lngRespType = 0;
	lngFolderId = 0;
	lngTermId = 0;
	lngRespBaseId = 0;
	strTitle = "";
	strKeyword1 = "";
	strKeyword2 = "";
	strKeyword3 = "";
	strKeyword4 = "";
	strKeyword5 = "";
	lngViewFlag = 0;
}

//
//  機能     : "ID"を除くフィールド文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblRdfl::GetFldsString()
{
	CString str;

	str = CTblBase::GetFldsString();

	/// 入力フィールドリスト
	str = str + _T(",") + _T("lngMsgType");
	str = str + _T(",") + _T("strSenderId");
	str = str + _T(",") + _T("strSenderName");
	str = str + _T(",") + _T("strReceiverId");
	str = str + _T(",") + _T("strReceiverName");
	str = str + _T(",") + _T("lngRespType");
	str = str + _T(",") + _T("lngFolderId");
	str = str + _T(",") + _T("lngTermId");
	str = str + _T(",") + _T("lngRespBaseId");
	str = str + _T(",") + _T("strTitle");
	str = str + _T(",") + _T("strKeyword1");
	str = str + _T(",") + _T("strKeyword2");
	str = str + _T(",") + _T("strKeyword3");
	str = str + _T(",") + _T("strKeyword4");
	str = str + _T(",") + _T("strKeyword5");
	str = str + _T(",") + _T("lngViewFlag");

	return str;
}

//
//  機能     : "ID"を除くフィールド値文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblRdfl::GetValsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetValsString();

	//// 入力値リスト
	_ltot_s(lngMsgType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	str = str + _T(",'") + strSenderId + _T("'");
	str = str + _T(",'") + strSenderName + _T("'");
	str = str + _T(",'") + strReceiverId + _T("'");
	str = str + _T(",'") + strReceiverName + _T("'");
	_ltot_s(lngRespType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf; 
	_ltot_s(lngTermId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	_ltot_s(lngRespBaseId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	str = str + _T(",'") + strTitle + _T("'");
	str = str + _T(",'") + strKeyword1 + _T("'");
	str = str + _T(",'") + strKeyword2 + _T("'");
	str = str + _T(",'") + strKeyword3 + _T("'");
	str = str + _T(",'") + strKeyword4 + _T("'");
	str = str + _T(",'") + strKeyword5 + _T("'");
	_ltot_s(lngViewFlag, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;

	return str;
}

//
//  機能     : "ID"を除くフィールド値設定文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblRdfl::GetSetsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetSetsString();

	//// 入力値リスト
	_ltot_s(lngMsgType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngMsgType") + _T("=") + buf;
	str = str + _T(",") + _T("strSenderId") + _T("='") + strSenderId + _T("'");
	str = str + _T(",") + _T("strSenderName") + _T("='") + strSenderName + _T("'");
	str = str + _T(",") + _T("strReceiverId") + _T("='") + strReceiverId + _T("'");
	str = str + _T(",") + _T("strReceiverName") + _T("='") + strReceiverName + _T("'");
	_ltot_s(lngRespType, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngRespType") + _T("=") + buf;
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngFolderId") + _T("=") + buf;
	_ltot_s(lngTermId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngTermId") + _T("=") + buf;
	_ltot_s(lngRespBaseId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngRespBaseId") + _T("=") + buf; 
	str = str + _T(",") + _T("strTitle") + _T("='") + strTitle + _T("'");
	str = str + _T(",") + _T("strKeyword1") + _T("='") + strKeyword1 + _T("'");
	str = str + _T(",") + _T("strKeyword2") + _T("='") + strKeyword2 + _T("'");
	str = str + _T(",") + _T("strKeyword3") + _T("='") + strKeyword3 + _T("'");
	str = str + _T(",") + _T("strKeyword4") + _T("='") + strKeyword4 + _T("'");
	str = str + _T(",") + _T("strKeyword5") + _T("='") + strKeyword5 + _T("'");
	_ltot_s(lngViewFlag, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngViewFlag") + _T("=") + buf; 

	return str;
}

//
//  機能     : Rdflテーブルから現在のレコードの選択結果取得
//  
//  機能説明 : SelectRec()実行後に有効
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblRdfl::GetRec()	
{
	if (m_linkDB == 0) return FALSE;
	if (m_Query.eof()) return FALSE;

	if (!CTblBase::GetRec()) return FALSE;

	lngMsgType = m_Query.getIntField(_T("lngMsgType"));
	strSenderId = m_Query.getStringField(_T("strSenderId"));
	strSenderName = m_Query.getStringField(_T("strSenderName"));
	strReceiverId = m_Query.getStringField(_T("strReceiverId"));
	strReceiverName = m_Query.getStringField(_T("strReceiverName"));
	lngRespType = m_Query.getIntField(_T("lngRespType"));
	lngFolderId = m_Query.getIntField(_T("lngFolderId"));
	lngTermId = m_Query.getIntField(_T("lngTermId"));
	lngRespBaseId = m_Query.getIntField(_T("lngRespBaseId"));
	strTitle = m_Query.getStringField(_T("strTitle"));
	strKeyword1 = m_Query.getStringField(_T("strKeyword1"));
	strKeyword2 = m_Query.getStringField(_T("strKeyword2"));
	strKeyword3 = m_Query.getStringField(_T("strKeyword3"));
	strKeyword4 = m_Query.getStringField(_T("strKeyword4"));
	strKeyword5 = m_Query.getStringField(_T("strKeyword5"));
	lngViewFlag = m_Query.getIntField(_T("lngViewFlag"));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTblFolder
//////////////////////////////////////////////////////////////////////

//
//  機能     : Constructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblPerson::CTblPerson(CString strDbPath) : CTblBase(strDbPath)
{
	m_TblName = _T("person");
	InitRec(); 
}

//
//  機能     : Destructor
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CTblPerson::~CTblPerson()
{
	m_TblName.Empty();
}

//
//  機能     : レコード用領域の初期化
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CTblPerson::InitRec() 
{
	CTblBase::InitRec();

	strId = "";
	strName = "";
	strAbbName = "";
	strReadName = "";
	strPosition = "";
	strSubName1 = "";
	strSubName2 = "";
	lngBirthday = 0;
	lngSex = 0;
	strLanguage = "";
	strLanguageCode = "";
	strPostCodeA = "";
	strPostCodeB = "";
	strCountry = "";
	strCountryCode = "";
	strProvince = "";
	strProvinceCode = "";
	strCity = "";
	strCityCode = "";
	strSection = "";
	strSectionCode = "";
	strAddressRem = "";
	strAddressCode = "";
	strAddressDet = "";
	strTel = "";
	strTelA = "";
	strTelB = "";
	strTelC = "";
	strTelExt = "";
	strWkTel = "";
	strExTel = "";
	strMbTel = "";
	strFax = "";
	strMail = "";
	strWeb = "";
	strAbout = "";
	strMood = "";
	lngTimeZone = 0;
}

//
//  機能     : "ID"を除くフィールド文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblPerson::GetFldsString()
{
	CString str;

	str = CTblBase::GetFldsString();

	/// 入力フィールドリスト
	str = str + _T(",") + _T("strId");
	str = str + _T(",") + _T("strName");
	str = str + _T(",") + _T("strAbbName");
	str = str + _T(",") + _T("strReadName");
	str = str + _T(",") + _T("strPosition");
	str = str + _T(",") + _T("strSubName1");
	str = str + _T(",") + _T("strSubName2");
	str = str + _T(",") + _T("lngBirthday");
	str = str + _T(",") + _T("lngSex");
	str = str + _T(",") + _T("strLanguage");
	str = str + _T(",") + _T("strLanguageCode");
	str = str + _T(",") + _T("strPostCodeA");
	str = str + _T(",") + _T("strPostCodeB");
	str = str + _T(",") + _T("strCountry");
	str = str + _T(",") + _T("strCountryCode");
	str = str + _T(",") + _T("strProvince");
	str = str + _T(",") + _T("strProvinceCode");
	str = str + _T(",") + _T("strCity");
	str = str + _T(",") + _T("strCityCode");
	str = str + _T(",") + _T("strSection");
	str = str + _T(",") + _T("strSectionCode");
	str = str + _T(",") + _T("strAddressRem");
	str = str + _T(",") + _T("strAddressCode");
	str = str + _T(",") + _T("strAddressDet");
	str = str + _T(",") + _T("strTel");
	str = str + _T(",") + _T("strTelA");
	str = str + _T(",") + _T("strTelB");
	str = str + _T(",") + _T("strTelC");
	str = str + _T(",") + _T("strTelExt");
	str = str + _T(",") + _T("strWkTel");
	str = str + _T(",") + _T("strExTel");
	str = str + _T(",") + _T("strMbTel");
	str = str + _T(",") + _T("strFax");
	str = str + _T(",") + _T("strMail");
	str = str + _T(",") + _T("strWeb");
	str = str + _T(",") + _T("strAbout");
	str = str + _T(",") + _T("strMood");
	str = str + _T(",") + _T("lngTimeZone");

	return str;
}

//
//  機能     : "ID"を除くフィールド値文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblPerson::GetValsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetValsString();

	//// 入力値リスト
	str = str + _T(",'") + strId + _T("'");
	str = str + _T(",'") + strName + _T("'");
	str = str + _T(",'") + strAbbName + _T("'");
	str = str + _T(",'") + strReadName + _T("'");
	str = str + _T(",'") + strPosition + _T("'");
	str = str + _T(",'") + strSubName1 + _T("'");
	str = str + _T(",'") + strSubName2 + _T("'");
	_ltot_s(lngBirthday, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	_ltot_s(lngSex, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;
	str = str + _T(",'") + strLanguage + _T("'");
	str = str + _T(",'") + strLanguageCode + _T("'");
	str = str + _T(",'") + strPostCodeA + _T("'");
	str = str + _T(",'") + strPostCodeB + _T("'");
	str = str + _T(",'") + strCountry + _T("'");
	str = str + _T(",'") + strCountryCode + _T("'");
	str = str + _T(",'") + strProvince + _T("'");
	str = str + _T(",'") + strProvinceCode + _T("'");
	str = str + _T(",'") + strCity + _T("'");
	str = str + _T(",'") + strCityCode + _T("'");
	str = str + _T(",'") + strSection + _T("'");
	str = str + _T(",'") + strSectionCode + _T("'");
	str = str + _T(",'") + strAddressRem + _T("'");
	str = str + _T(",'") + strAddressCode + _T("'");
	str = str + _T(",'") + strAddressDet + _T("'");
	str = str + _T(",'") + strTel + _T("'");
	str = str + _T(",'") + strTelA + _T("'");
	str = str + _T(",'") + strTelB + _T("'");
	str = str + _T(",'") + strTelC + _T("'");
	str = str + _T(",'") + strTelExt + _T("'");
	str = str + _T(",'") + strWkTel + _T("'");
	str = str + _T(",'") + strExTel + _T("'");
	str = str + _T(",'") + strMbTel + _T("'");
	str = str + _T(",'") + strFax + _T("'");
	str = str + _T(",'") + strMail + _T("'");
	str = str + _T(",'") + strWeb + _T("'");
	str = str + _T(",'") + strAbout + _T("'");
	str = str + _T(",'") + strMood + _T("'");
	_ltot_s(lngTimeZone, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + buf;

	return str;
}

//
//  機能     : "ID"を除くフィールド値設定文字列
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//  
CString CTblPerson::GetSetsString()
{
	CString str;
	_TCHAR buf[32];

	str = CTblBase::GetSetsString();

	//// 入力値リスト
	str = str + _T(",") + _T("strId") + _T("='") + strId + _T("'");
	str = str + _T(",") + _T("strName") + _T("='") + strName + _T("'");
	str = str + _T(",") + _T("strAbbName") + _T("='") + strAbbName + _T("'");
	str = str + _T(",") + _T("strReadName") + _T("='") + strReadName + _T("'");
	str = str + _T(",") + _T("strPosition") + _T("='") + strPosition + _T("'");
	str = str + _T(",") + _T("strSubName1") + _T("='") + strSubName1 + _T("'");
	str = str + _T(",") + _T("strSubName2") + _T("='") + strSubName2 + _T("'");
	_ltot_s(lngBirthday, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngBirthday") + _T("=") + buf;
	_ltot_s(lngSex, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngSex") + _T("=") + buf;
	str = str + _T(",") + _T("strLanguage") + _T("='") + strLanguage + _T("'");
	str = str + _T(",") + _T("strLanguageCode") + _T("='") + strLanguageCode + _T("'");
	str = str + _T(",") + _T("strPostCodeA") + _T("='") + strPostCodeA + _T("'");
	str = str + _T(",") + _T("strPostCodeB") + _T("='") + strPostCodeB + _T("'");
	str = str + _T(",") + _T("strCountry") + _T("='") + strCountry + _T("'");
	str = str + _T(",") + _T("strCountryCode") + _T("='") + strCountryCode + _T("'");
	str = str + _T(",") + _T("strProvince") + _T("='") + strProvince + _T("'");
	str = str + _T(",") + _T("strProvinceCode") + _T("='") + strProvinceCode + _T("'");
	str = str + _T(",") + _T("strCity") + _T("='") + strCity + _T("'");
	str = str + _T(",") + _T("strCityCode") + _T("='") + strCityCode + _T("'");
	str = str + _T(",") + _T("strSection") + _T("='") + strSection + _T("'");
	str = str + _T(",") + _T("strSectionCode") + _T("='") + strSectionCode + _T("'");
	str = str + _T(",") + _T("strAddressRem") + _T("='") + strAddressRem + _T("'");
	str = str + _T(",") + _T("strAddressCode") + _T("='") + strAddressCode + _T("'");
	str = str + _T(",") + _T("strAddressDet") + _T("='") + strAddressDet + _T("'");
	str = str + _T(",") + _T("strTel") + _T("='") + strTel + _T("'");
	str = str + _T(",") + _T("strTelA") + _T("='") + strTelA + _T("'");
	str = str + _T(",") + _T("strTelB") + _T("='") + strTelB + _T("'");
	str = str + _T(",") + _T("strTelC") + _T("='") + strTelC + _T("'");
	str = str + _T(",") + _T("strTelExt") + _T("='") + strTelExt + _T("'");
	str = str + _T(",") + _T("strWkTel") + _T("='") + strWkTel + _T("'");
	str = str + _T(",") + _T("strExTel") + _T("='") + strExTel + _T("'");
	str = str + _T(",") + _T("strMbTel") + _T("='") + strMbTel + _T("'");
	str = str + _T(",") + _T("strFax") + _T("='") + strFax + _T("'");
	str = str + _T(",") + _T("strMail") + _T("='") + strMail + _T("'");
	str = str + _T(",") + _T("strWeb") + _T("='") + strWeb + _T("'");
	str = str + _T(",") + _T("strAbout") + _T("='") + strAbout + _T("'");
	str = str + _T(",") + _T("strMood") + _T("='") + strMood + _T("'");
	_ltot_s(lngTimeZone, buf, sizeof(buf)/sizeof(buf[0]), 10);
	str = str + _T(",") + _T("lngTimeZone") + _T("=") + buf;

	return str;
}

//
//  機能     : Personテーブルから現在のレコードの選択結果取得
//  
//  機能説明 : SelectRec()実行後に有効
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTblPerson::GetRec()	
{
	if (m_linkDB == 0) return FALSE;
	if (m_Query.eof()) return FALSE;

	if (!CTblBase::GetRec()) return FALSE;

	strId = m_Query.getStringField(_T("strId"));
	strName = m_Query.getStringField(_T("strName"));
	strAbbName = m_Query.getStringField(_T("strAbbName"));
	strReadName = m_Query.getStringField(_T("strReadName"));
	strPosition = m_Query.getStringField(_T("strPosition"));
	strSubName1 = m_Query.getStringField(_T("strSubName1"));
	strSubName2 = m_Query.getStringField(_T("strSubName2"));
	lngBirthday = m_Query.getIntField(_T("lngBirthday"));
	lngSex = m_Query.getIntField(_T("lngSex"));
	strLanguage = m_Query.getStringField(_T("strLanguage"));
	strLanguageCode = m_Query.getStringField(_T("strLanguageCode"));
	strPostCodeA = m_Query.getStringField(_T("strPostCodeA"));
	strPostCodeB = m_Query.getStringField(_T("strPostCodeB"));
	strCountry = m_Query.getStringField(_T("strCountry"));
	strCountryCode = m_Query.getStringField(_T("strCountryCode"));
	strProvince = m_Query.getStringField(_T("strProvince"));
	strProvinceCode = m_Query.getStringField(_T("strProvinceCode"));
	strCity = m_Query.getStringField(_T("strCity"));
	strCityCode = m_Query.getStringField(_T("strCityCode"));
	strSection = m_Query.getStringField(_T("strSection"));
	strSectionCode = m_Query.getStringField(_T("strSectionCode"));
	strAddressRem = m_Query.getStringField(_T("strAddressRem"));
	strAddressCode = m_Query.getStringField(_T("strAddressCode"));
	strAddressDet = m_Query.getStringField(_T("strAddressDet"));
	strTel = m_Query.getStringField(_T("strTel"));
	strTelA = m_Query.getStringField(_T("strTelA"));
	strTelB = m_Query.getStringField(_T("strTelB"));
	strTelC = m_Query.getStringField(_T("strTelC"));
	strTelExt = m_Query.getStringField(_T("strTelExt"));
	strWkTel = m_Query.getStringField(_T("strWkTel"));
	strExTel = m_Query.getStringField(_T("strExTel"));
	strMbTel = m_Query.getStringField(_T("strMbTel"));
	strFax = m_Query.getStringField(_T("strFax"));
	strMail = m_Query.getStringField(_T("strMail"));
	strWeb = m_Query.getStringField(_T("strWeb"));
	strAbout = m_Query.getStringField(_T("strAbout"));
	strMood = m_Query.getStringField(_T("strMood"));
	lngTimeZone = m_Query.getIntField(_T("lngTimeZone"));

	return TRUE;
}

