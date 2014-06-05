// TPtable.h: CTPtable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPTABLE_H__3C3B0241_E9DE_4A6D_9562_D0CDA2C58E05__INCLUDED_)
#define AFX_TPTABLE_H__3C3B0241_E9DE_4A6D_9562_D0CDA2C58E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CppSQLite3.h"

// テーブルアクセス基本クラス
class CTblBase
{
public:
	CTblBase(CString strDbPath);
	~CTblBase();

private:
	CString GetDateStr();
	BOOL TransactionExe(CString strSql);	// トランザクションを使用したSQL実行

protected:
	static CppSQLite3DB m_pDB;	// Database操作クラス
	static int m_linkDB;		// Database操作クラスオブジェクトリンク数
	CString m_TblName;			// 現在の操作対象テーブル名称
	CppSQLite3Query m_Query;	// クエリー実行クラス

	virtual CString GetFldsString();	// "ID"を除くフィールド文字列
	virtual CString GetValsString();	// "ID"を除くフィールド値文字列
	virtual CString GetSetsString();	// "ID"を除くフィールド値設定文字列

public:
// 共通レコードフィールド ----- start -----
	long ID;
	long lngClsCode1;
	long lngClsCode2;
	long lngClsCode3;
	CString strRemarks1;
	CString strRemarks2;
	CString dlngSystemNew;	// 64bit long
	CString dlngSystemUpdate;	// 64bit long
	long lngSystemFlag;
	CString strSystemOperator;
	CString strSystemName;
	CString strSystemVersion;
// 共通レコードフィールド ----- end -----

	BOOL m_blTransaction;	// 明示的トランザクション使用フラグ

//	BOOL OpenDb(CString strDbPath);
//	BOOL CloseDb();
	BOOL OpenTbl(CString strTblName);
	BOOL CloseTbl();
	BOOL TransactionStart();	// トランザクション開始
	BOOL TransactionEnd();		// トランザクション終了

	virtual void InitRec();
	virtual double MaxVal(CString strFld, CString strWhere);	// フィールドのMAX値
	virtual double MinVal(CString strFld, CString strWhere);	// フィールドのMIN値
	virtual long CountRec(CString strWhere);
	virtual long SelectRec(long lngID);
	virtual long SelectRec(CString strWhere);
	virtual BOOL GetRec();
	virtual BOOL NextRec();
	virtual BOOL InsertRec();
	virtual BOOL InsertRec(CString strDate);
	virtual BOOL UpdateRec(long lngID);
	virtual BOOL UpdateRec(CString strSets, CString strWhere);
	virtual BOOL DeleteRec(long lngID);
	virtual BOOL DeleteRec(CString strWhere);
	virtual BOOL ExistTbl();		// テーブル存在確認
};

// RSFLレコードクラス
class CTblRsfl :
  public CTblBase
{
public:
    CTblRsfl(CString strDbPath);
    ~CTblRsfl();

private:

public:
// 固有レコードフィールド ----- start -----
	CString strSenderId;
	CString strSenderName;
	CString strReceiverId;
	CString strReceiverName;
	long lngRespType;
	long lngDefaultFlag;
	long lngFolderId;
	long lngLinkId;		// （予約）設定の共用（グループ化に使用）
	long lngSqlId;		// （予約）応答条件の設定用（条件は別テーブル）
// 固有レコードフィールド ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};

// folderレコードクラス
class CTblFolder :
  public CTblBase
{
public:
    CTblFolder(CString strDbPath);
    ~CTblFolder();

private:

public:
// 固有レコードフィールド ----- start -----
	CString strReceiverId;
	CString strTitle;
	long lngFolderType;
// 固有レコードフィールド ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};

// RDFLレコードクラス
class CTblRdfl :
  public CTblBase
{
public:
    CTblRdfl(CString strDbPath);
    ~CTblRdfl();

private:

public:
// 固有レコードフィールド ----- start -----
	long lngMsgType;
	CString strSenderId;
	CString strSenderName;
	CString strReceiverId;
	CString strReceiverName;
	long lngRespType;
	long lngFolderId;
	long lngTermId;
	long lngRespBaseId;
	CString strTitle;
	CString strKeyword1;
	CString strKeyword2;
	CString strKeyword3;
	CString strKeyword4;
	CString strKeyword5;
	long lngViewFlag;		// 再生済フラグ
// 固有レコードフィールド ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};

// personレコードクラス
class CTblPerson :
  public CTblBase
{
public:
    CTblPerson(CString strDbPath);
    ~CTblPerson();

private:

public:
// 固有レコードフィールド ----- start -----
	CString strId;
	CString strName;
	CString strAbbName;
	CString strReadName;
	CString strPosition;
	CString strSubName1;
	CString strSubName2;
	long lngBirthday;
	long lngSex;
	CString strLanguage;
	CString strLanguageCode;
	CString strPostCodeA;
	CString strPostCodeB;
	CString strCountry;
	CString strCountryCode;
	CString strProvince;
	CString strProvinceCode;
	CString strCity;
	CString strCityCode;
	CString strSection;
	CString strSectionCode;
	CString strAddressRem;
	CString strAddressCode;
	CString strAddressDet;
	CString strTel;
	CString strTelA;
	CString strTelB;
	CString strTelC;
	CString strTelExt;
	CString strWkTel;
	CString strExTel;
	CString strMbTel;
	CString strFax;
	CString strMail;
	CString strWeb;
	CString strAbout;
	CString strMood;
	long lngTimeZone;
// 固有レコードフィールド ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};


#endif // !defined(AFX_TPTABLE_H__3C3B0241_E9DE_4A6D_9562_D0CDA2C58E05__INCLUDED_)
