// TPtable.h: CTPtable �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPTABLE_H__3C3B0241_E9DE_4A6D_9562_D0CDA2C58E05__INCLUDED_)
#define AFX_TPTABLE_H__3C3B0241_E9DE_4A6D_9562_D0CDA2C58E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CppSQLite3.h"

// �e�[�u���A�N�Z�X��{�N���X
class CTblBase
{
public:
	CTblBase(CString strDbPath);
	~CTblBase();

private:
	CString GetDateStr();
	BOOL TransactionExe(CString strSql);	// �g�����U�N�V�������g�p����SQL���s

protected:
	static CppSQLite3DB m_pDB;	// Database����N���X
	static int m_linkDB;		// Database����N���X�I�u�W�F�N�g�����N��
	CString m_TblName;			// ���݂̑���Ώۃe�[�u������
	CppSQLite3Query m_Query;	// �N�G���[���s�N���X

	virtual CString GetFldsString();	// "ID"�������t�B�[���h������
	virtual CString GetValsString();	// "ID"�������t�B�[���h�l������
	virtual CString GetSetsString();	// "ID"�������t�B�[���h�l�ݒ蕶����

public:
// ���ʃ��R�[�h�t�B�[���h ----- start -----
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
// ���ʃ��R�[�h�t�B�[���h ----- end -----

	BOOL m_blTransaction;	// �����I�g�����U�N�V�����g�p�t���O

//	BOOL OpenDb(CString strDbPath);
//	BOOL CloseDb();
	BOOL OpenTbl(CString strTblName);
	BOOL CloseTbl();
	BOOL TransactionStart();	// �g�����U�N�V�����J�n
	BOOL TransactionEnd();		// �g�����U�N�V�����I��

	virtual void InitRec();
	virtual double MaxVal(CString strFld, CString strWhere);	// �t�B�[���h��MAX�l
	virtual double MinVal(CString strFld, CString strWhere);	// �t�B�[���h��MIN�l
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
	virtual BOOL ExistTbl();		// �e�[�u�����݊m�F
};

// RSFL���R�[�h�N���X
class CTblRsfl :
  public CTblBase
{
public:
    CTblRsfl(CString strDbPath);
    ~CTblRsfl();

private:

public:
// �ŗL���R�[�h�t�B�[���h ----- start -----
	CString strSenderId;
	CString strSenderName;
	CString strReceiverId;
	CString strReceiverName;
	long lngRespType;
	long lngDefaultFlag;
	long lngFolderId;
	long lngLinkId;		// �i�\��j�ݒ�̋��p�i�O���[�v���Ɏg�p�j
	long lngSqlId;		// �i�\��j���������̐ݒ�p�i�����͕ʃe�[�u���j
// �ŗL���R�[�h�t�B�[���h ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};

// folder���R�[�h�N���X
class CTblFolder :
  public CTblBase
{
public:
    CTblFolder(CString strDbPath);
    ~CTblFolder();

private:

public:
// �ŗL���R�[�h�t�B�[���h ----- start -----
	CString strReceiverId;
	CString strTitle;
	long lngFolderType;
// �ŗL���R�[�h�t�B�[���h ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};

// RDFL���R�[�h�N���X
class CTblRdfl :
  public CTblBase
{
public:
    CTblRdfl(CString strDbPath);
    ~CTblRdfl();

private:

public:
// �ŗL���R�[�h�t�B�[���h ----- start -----
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
	long lngViewFlag;		// �Đ��σt���O
// �ŗL���R�[�h�t�B�[���h ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};

// person���R�[�h�N���X
class CTblPerson :
  public CTblBase
{
public:
    CTblPerson(CString strDbPath);
    ~CTblPerson();

private:

public:
// �ŗL���R�[�h�t�B�[���h ----- start -----
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
// �ŗL���R�[�h�t�B�[���h ----- end -----

	void InitRec();
	BOOL GetRec();
	CString GetFldsString();
	CString GetValsString();
	CString GetSetsString();
};


#endif // !defined(AFX_TPTABLE_H__3C3B0241_E9DE_4A6D_9562_D0CDA2C58E05__INCLUDED_)
