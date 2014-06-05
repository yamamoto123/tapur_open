/*

	TPsetup.cpp

*/

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include "Tapur.h"
#include "TPsetup.h"
#include "TPtable.h"
#include "TPutil.h"
#include "FileSpec.h"
#include "file_access.h"
#include "response_main.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//
//  �@�\     : �R���X�g���N�^
//  
//  �@�\���� : ��������
//  
//  �Ԃ�l   : �Ȃ�
//  
//  ���l     : 
//  
void CTPsetup::Init()	
{
	pst_xml = &st_xml;
	pst_rsf = &st_rsf;
	pust_rsf = &ust_rsf;
	pst_rdf = &st_rdf;
	pust_rdf = &ust_rdf;
	m_lngRespId = 0;
	m_strDataFolder.Empty();
	m_strCallerId.Empty();
}
CTPsetup::CTPsetup()	
{
	Init();
}
CTPsetup::CTPsetup(
CString strPath)	// �f�[�^�t�H���_
{
	Init();

	SetDataDir(strPath);
}
CTPsetup::CTPsetup(
CString strPath,	// �f�[�^�t�H���_
CString strTelno)	// ���M��ID
{
	Init();

	SetDataDir(strPath);
	SetCallerId(strTelno);
}

//
//  �@�\     : �f�X�g���N�^
//  
//  �@�\���� : �I������
//  
//  �Ԃ�l   : �Ȃ�
//  
//  ���l     : 
//  
 CTPsetup::~CTPsetup()	
{
	pst_xml = NULL;
	pst_rsf = NULL;
	pust_rsf = NULL;
	pst_rdf = NULL;
	pust_rdf = NULL;
	m_lngRespId = 0;
	m_strDataFolder.Empty();
	m_strCallerId.Empty();
}

//
//  �@�\     : �f�[�^�t�H���_�o�^
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::SetDataDir(
CString strPath)			// �f�[�^�t�H���_
{
	m_strDataFolder = strPath;
	return TRUE;
}

//
//  �@�\     : �f�[�^�p�X������
//  
//  �@�\���� : RDF�p
//  
//  �Ԃ�l   : �f�[�^�p�X��
//  
//  ���l     : 
//  
CString CTPsetup::GetDataFolder()
{
	CString str;
	DWORD dw;
	CRegDWORD rgEnableDataPath;
	CRegString rgDataPath;
	CFileSpec fs;

	// �K��̃f�[�^�p�X���g�p�i�����l�j
	str = m_strDataFolder;

	/// ���W�X�g���ǂݍ���
	rgEnableDataPath = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableDataPath"), 0, TRUE);
	dw = rgEnableDataPath;
	if (dw > 0) {
		/// ���[�U�ݒ�̃f�[�^�p�X���g�p
		// �w�肵���f�[�^�t�H���_���g�p�ł��Ȃ��ꍇ�A�K��̃f�[�^�p�X���g�p
		rgDataPath = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\DataPath"), _T(""), TRUE);
		str = rgDataPath;
		if (!str.IsEmpty()) {
			CFileSpec fs(str + "\\dummy.file");
			if (fs.MkFolder()) {
				str += _T(TAPUR_DATA_PATH) + ChkFolder(theApp.m_strUsrId);
				//str += _T(TAPUR_DATA_PATH) + theApp.m_strUsrId;
			}
		}
	}

	return str;
}

//
//  �@�\     : ���M��ID�o�^
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::SetCallerId(
CString strTelno)			// ���M��ID
{
	if (strTelno.IsEmpty()) {
		// �s���ȑ��M�ҁiSkypeIn�j
		m_strCallerId = _T("0");
	} else {
		// �t�@�C�����Ƃ��ĕs���ȕ�����"_"�u������
		m_strCallerId = strTelno;
		m_strCallerId.Replace(_T("/"),_T("_"));
		m_strCallerId.Replace(_T("*"),_T("_"));
		m_strCallerId.Replace(_T("?"),_T("_"));
		m_strCallerId.Replace(_T(":"),_T("_"));
		m_strCallerId.Replace(_T("<"),_T("_"));
		m_strCallerId.Replace(_T(">"),_T("_"));
		m_strCallerId.Replace(_T("|"),_T("_"));
	}
	
	size_t sz;
	wcstombs_s(&sz, caller_id, sizeof(caller_id), m_strCallerId, MAX_USRNAME_LEN);
	return TRUE;
}

//
//  �@�\     : ����ID������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : ����ID������
//  
//  ���l     : 
//  
CString CTPsetup::GetDateStr()
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return strDate;
}

//
//  �@�\     : RSF�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfFileName()
{
	CString str = m_strDataFolder + _T(RSF_PATH) + _T("rsf-") + m_strCallerId + _T(".ref");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF BEEP�����t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BEEP�����t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfBeepFileName(
CString strSuffix)	// �T�t�B�b�N�X�iex, "1"�j
{
	CString str = m_strDataFolder + _T(RSF_WAV_PATH) + _T("beep") + strSuffix + _T(".wav");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF�����t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�����t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfAudioFileName(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str = m_strDataFolder + _T(RSF_WAV_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T(".wav");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF�`���b�g�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�`���b�g�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfChatFileName(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str = m_strDataFolder + _T(RSF_TXT_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T(".txt");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF�r�f�I�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�r�f�I�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfVideoFileName(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str = m_strDataFolder + _T(RSF_AVI_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T(".avi");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF�摜�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�摜�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfImgFileName(
CString strProc,	// �����R�[�h�iex, "$"�j
CString strExt)		// �g���q
{
	CString str = m_strDataFolder + _T(RSF_IMG_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc;
	if (!strExt.IsEmpty()) {
		str = str + _T(".") + strExt;
	}
	
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF�摜�t�@�C���̌���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�摜�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::FindRsfImgFile(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str;
	CString strFile;
	CFileSpec fs;

	/// RSF�t�@�C�����猟�o
	UST_RSF *pst = LoadRSF();
	strFile.Format(_T("%s"), pst->resp->play_image_file.filename);
	if (!strFile.IsEmpty()) {
		str = m_strDataFolder + _T(RSF_IMG_PATH) + strFile;
		fs.SetFullSpec(str);
		if (fs.Exist()) return str;
	}

	/// RSF�t�@�C���ɋL�ڂ��Ȃ��ꍇ�A�t�H���_���猟��
	strFile = m_strDataFolder + _T(RSF_IMG_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc;
	str = strFile + _T(".jpg");
	fs.SetFullSpec(str);
	if (fs.Exist()) return str;
	str = strFile + _T(".gif");
	fs.SetFullSpec(str);
	if (fs.Exist()) return str;
	str = strFile + _T(".bmp");
	fs.SetFullSpec(str);
	if (fs.Exist()) return str;

	str.Empty();
	return str;
}

//
//  �@�\     : RSF�A�g�v���O����CSV�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�A�g�v���O����CSV�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfPrgFileName(
CString strProc,	// �����R�[�h�iex, "$"�j
CString strPrgId)	// �v���O����ID�i"" or 1-999�j
{
	CString str = m_strDataFolder + _T(RSF_CSV_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T("-") + strPrgId + _T(".csv");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}


//
//  �@�\     : RSF�ꎞ�t�H���_�Q��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RSF�ꎞ�t�H���_�Q��
//  
//  ���l     : 
//  
CString CTPsetup::RsfTmpFolder()
{
	CString str = m_strDataFolder + _T(RSF_TMP_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RSF�ꎞ�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�ꎞ�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RsfTmpFileName(
CString strExt)		// �g���q
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	CString str = RsfTmpFolder() + _T("tmp-") + strDate;
	str = str + _T("-$.") + strExt;
	
	return str;
}

//
//  �@�\     : RDF�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�t�@�C����
//  
//  ���l     : �ۑ���͏�ɋK��̃f�[�^�t�H���_�i�f�[�^�t�H���_�̕ύX�ɂ͒Ǐ]���Ȃ��j
//  
CString CTPsetup::RdfFileName(
CString strDate)	// ����
{
	CString str = m_strDataFolder + _T(RDF_PATH) + _T("rdf-") + m_strCallerId + _T("-") + strDate + _T(".ref");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RDF����(WAV)�t�@�C���t�H���_
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF����(WAV)�t�@�C���t�H���_��
//  
//  ���l     : 
//  
CString CTPsetup::RdfWavFolder()
{
	CString str = GetDataFolder() + _T(RDF_WAV_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RDF����(WAV)�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF����(WAV)�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RdfWavFileName(
CString strFolder,	// �f�[�^�t�H���_
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str = strFolder + _T("rdf-") + m_strCallerId + _T("-") + strDate;
	str += _T("-") + strProc + _T(".wav");

	return str;
}
CString CTPsetup::RdfWavFileName(
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	return RdfWavFileName(RdfWavFolder(), strDate, strProc);
}
CString CTPsetup::RdfWavFileName(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return RdfWavFileName(strDate, strProc);
}

//
//  �@�\     : RDF����(WAV)�t�@�C���̌���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF����(WAV)�t�@�C�����AEmpty=������Ȃ��ꍇ
//  
//  ���l     : 
//  
CString CTPsetup::FindRdfWavFileName(
CString strFolder,	// �f�[�^�t�H���_
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str;

	if (strFolder.IsEmpty()) {
		str = m_strDataFolder;
	} else { 
		str = strFolder;
	}
	str += _T(RDF_WAV_PATH);
	CString strFile = RdfWavFileName(str, strDate, strProc);

	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		strFile.Empty();
	} 

	return strFile;
}

//
//  �@�\     : RDF����(MP3)�t�@�C���t�H���_
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF����(MP3)�t�@�C���t�H���_��
//  
//  ���l     : 
//  
CString CTPsetup::RdfMp3Folder()
{
	CString str = GetDataFolder() + _T(RDF_MP3_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RDF����(MP3)�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF����(MP3)�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RdfMp3FileName(
CString strFolder,	// �f�[�^�t�H���_
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str = strFolder + _T("rdf-") + m_strCallerId + _T("-") + strDate;
	str += _T("-") + strProc + _T(".mp3");

	return str;
}
CString CTPsetup::RdfMp3FileName(
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	return RdfMp3FileName(RdfMp3Folder(), strDate, strProc);
}
CString CTPsetup::RdfMp3FileName(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return RdfMp3FileName(strDate, strProc);
}

//
//  �@�\     : RDF����(MP3)�t�@�C���̌���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF����(MP3)�t�@�C�����AEmpty=������Ȃ��ꍇ
//  
//  ���l     : 
//  
CString CTPsetup::FindRdfMp3FileName(
CString strFolder,	// �f�[�^�t�H���_
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str;

	if (strFolder.IsEmpty()) {
		str = m_strDataFolder;
	} else { 
		str = strFolder;
	}
	str += _T(RDF_MP3_PATH);
	CString strFile = RdfMp3FileName(str, strDate, strProc);

	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		strFile.Empty();
	} 

	return strFile;
}

//
//  �@�\     : RDF�����t�@�C������������擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : ����������
//  
//  ���l     : 
//  
CString CTPsetup::GetAudioFileDate(
CString strFile)	// RDF�����t�@�C��
{
	CStringArray aryToken;
	TCHAR string[MAX_PATH];
	TCHAR seps[] = _T("-");
	LPTSTR token;
	TCHAR *next_token = NULL;
	int cnt;
    
	if (strFile.IsEmpty()) return _T("");

	aryToken.RemoveAll();
	_tcscpy_s(string, MAX_PATH, strFile);
	token = _tcstok_s( string, seps, &next_token );
	while( token != NULL )
	{
		aryToken.Add(token);
		token = _tcstok_s( NULL, seps, &next_token );
	}

	cnt = aryToken.GetSize();
	return aryToken.GetAt(cnt - 2);
}

//
//  �@�\     : RDF�����t�@�C�����瑗�M�Җ��i�X�J�C�v���j���擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : ���M�Җ��i�X�J�C�v���j������
//  
//  ���l     : 
//  
CString CTPsetup::GetAudioFileCallerId(
CString strFile)	// RDF�����t�@�C��
{
	CStringArray aryToken;
	TCHAR string[MAX_PATH];
	TCHAR seps[] = _T("-");
	LPTSTR token;
	TCHAR *next_token = NULL;
	int i, cnt;
	CString str;
    
	CFileSpec fs(strFile);
	_tcscpy_s(string, MAX_PATH, fs.FileName());

	aryToken.RemoveAll();
	token = _tcstok_s( string, seps, &next_token );
	while( token != NULL )
	{
		aryToken.Add(token);
		token = _tcstok_s( NULL, seps, &next_token );
	}

	// ���M�Җ����Z�p���[�^�������܂ޏꍇ������
	cnt = aryToken.GetSize();
	str.Empty();
	for(i = 1; i <= cnt - 3; i++) {
		str.Append(aryToken.GetAt(i));
		if (i != cnt - 3) {
			str.Append(_T("-"));
		}
	}

	return str;
}

//
//  �@�\     : RDF�r�f�I�t�@�C���t�H���_
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�r�f�I�t�@�C���t�H���_��
//  
//  ���l     : 
//  
CString CTPsetup::RdfVideoFolder()
{
	CString str = GetDataFolder() + _T(RDF_AVI_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  �@�\     : RDF�r�f�I�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�r�f�I�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RdfVideoFileName(
CString strFolder,	// �f�[�^�t�H���_
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str = strFolder + _T("rdf-") + m_strCallerId + _T("-") + strDate;
	str += _T("-") + strProc + _T(".avi");

	return str;
}
CString CTPsetup::RdfVideoFileName(
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	return RdfVideoFileName(RdfVideoFolder(), strDate, strProc);
}
CString CTPsetup::RdfVideoFileName(
CString strProc)	// �����R�[�h�iex, "$"�j
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return RdfVideoFileName(strDate, strProc);
}

//
//  �@�\     : RDF�����t�@�C���̌���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�����t�@�C�����AEmpty=������Ȃ��ꍇ
//  
//  ���l     : 
//  
CString CTPsetup::FindRdfVideoFileName(
CString strFolder,	// �f�[�^�t�H���_
CString strDate,	// ����
CString strProc)	// �����R�[�h�iex, "$"�j
{
	CString str;

	if (strFolder.IsEmpty()) {
		str = m_strDataFolder;
	} else { 
		str = strFolder;
	}
	str += _T(RDF_AVI_PATH);
	CString strFile = RdfVideoFileName(str, strDate, strProc);

	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		strFile.Empty();
	} 

	return strFile;
}

//
//  �@�\     : RDF�ꎞ�����t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�ꎞ�����t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RdfTmpAudioFileName()
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	CString str = GetDataFolder() + _T(RDF_TMP_PATH) + _T("tmp-") + strDate;
	str += _T("-$.wav");
	return str;
}

//
//  �@�\     : RDF�ꎞ�r�f�I�t�@�C��������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RDF�ꎞ�r�f�I�t�@�C����
//  
//  ���l     : 
//  
CString CTPsetup::RdfTmpVideoFileName()
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	CString str = GetDataFolder() + _T(RDF_TMP_PATH) + _T("tmp-") + strDate;
	str += _T("-$.avi");
	return str;
}

//
//  �@�\     : Make each RSF Response.
//  
//  �@�\���� : SaveRsfFile()�̉���
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::MakeRsfResp(
CArray<CRsfPrm, CRsfPrm> &prmArray)	// �����ݒ�̔z��
{
	CRsfPrm prm;
	ST_RSF_RESP st_rsf_resp;
	ST_RSF_RESP *pst_rsf_resp = &st_rsf_resp;
	ST_BIN_FILE *pbinf;
	CFileSpec fs;
	//_TCHAR fn[MAX_PATH];
	//_TCHAR buf[MAX_PATH];
	CString strFn;
	CString strBuf;
	char order[MAX_RSF_RESP];
	//size_t sz;

	memset((void *)order, '\0', sizeof(MAX_RSF_RESP));

	for(int i = 0; i < prmArray.GetSize(); i++) {
		prm = prmArray.GetAt(i);
		// ���ʐݒ�
		pst_rsf->resp_cnt++;
		pst_rsf_resp = pst_rsf->resp + (pst_rsf->resp_cnt-1);
		memset((void *)pst_rsf_resp, '\0', sizeof(ST_RSF_RESP));
		pst_rsf_resp->type = prm.type;
		strcat_s(order, MAX_RSF_RESP, "$");
		strcpy_s(pst_rsf_resp->order, MAX_RDF_RESP, order);
		if (prm.beep > 0) {
			pst_rsf_resp->beep = 1;
		}
		if (pst_rsf_resp->type != RESP_TYPE_DISCONNECT) {
			strcpy_s(pst_rsf_resp->term, MAX_DTMFTERM_LEN, "#");
		}

		// �eResponse�ݒ�
		switch(pst_rsf_resp->type) {
		case RESP_TYPE_PLAY:
			pst_rsf_resp->play_type = prm.play_type;
			if (prm.play_type & RESP_PLAY_TYPE_AUDIO) {
				pbinf = &(pst_rsf_resp->play_audio_file);
				strcpy_s(pbinf->bin_type, MAX_TYPE_LEN, "audio/x-wav");
				sprintf_s(pbinf->filename, MAX_FILE, "rsf-%s-%s.wav", caller_id, pst_rsf_resp->order);
				//mbstowcs_s(&sz, buf, MAX_PATH, pbinf->filename, MAX_PATH);
				//_stprintf_s(fn, MAX_FILE, _T("%s%s%s"), m_strDataFolder, _T(RSF_WAV_PATH), buf);
				//fs.SetFullSpec(fn);
				strFn = pbinf->filename;	// mb -> unicode
				strFn = m_strDataFolder + _T(RSF_WAV_PATH) + strFn;
				fs.SetFullSpec(strFn);
				pst_rsf_resp->play_audio_file.size = (long)fs.FileSize();
			}
			if (prm.play_type & RESP_PLAY_TYPE_VIDEO) {
				pbinf = &(pst_rsf_resp->play_video_file);
				strcpy_s(pbinf->bin_type, MAX_TYPE_LEN, "video/x-msvideo");
				sprintf_s(pbinf->filename, MAX_FILE, "rsf-%s-%s.avi", caller_id, pst_rsf_resp->order);
				//mbstowcs_s(&sz, buf, MAX_PATH, pbinf->filename, MAX_PATH);
				//_stprintf_s(fn, MAX_FILE, _T("%s%s%s"), m_strDataFolder, _T(RSF_AVI_PATH), buf);
				//fs.SetFullSpec(fn);
				strFn = pbinf->filename;	// mb -> unicode
				strFn = m_strDataFolder + _T(RSF_AVI_PATH) + strFn;
				fs.SetFullSpec(strFn);
				pst_rsf_resp->play_video_file.size = (long)fs.FileSize();
			}
			if (prm.play_type & RESP_PLAY_TYPE_IMAGE) {
				pbinf = &(pst_rsf_resp->play_image_file);
				if (prm.file_type == RESP_FILE_TYPE_BMP) {
					strcpy_s(pbinf->bin_type, MAX_TYPE_LEN, "image/bmp");
					sprintf_s(pbinf->filename, MAX_FILE, "rsf-%s-%s.bmp", caller_id, pst_rsf_resp->order);
				} else if (prm.file_type == RESP_FILE_TYPE_JPEG) {
					strcpy_s(pbinf->bin_type, MAX_TYPE_LEN, "image/jpeg");
					sprintf_s(pbinf->filename, MAX_FILE, "rsf-%s-%s.jpg", caller_id, pst_rsf_resp->order);
				} else if (prm.file_type == RESP_FILE_TYPE_GIF) {
					strcpy_s(pbinf->bin_type, MAX_TYPE_LEN, "image/gif");
					sprintf_s(pbinf->filename, MAX_FILE, "rsf-%s-%s.gif", caller_id, pst_rsf_resp->order);
				}
				//mbstowcs_s(&sz, buf, MAX_PATH, pbinf->filename, MAX_PATH);
				//_stprintf_s(fn, MAX_FILE, _T("%s%s%s"), m_strDataFolder, _T(RSF_IMG_PATH), buf);
				//fs.SetFullSpec(fn);
				strFn = pbinf->filename;	// mb -> unicode
				strFn = m_strDataFolder + _T(RSF_IMG_PATH) + strFn;
				fs.SetFullSpec(strFn);
				pst_rsf_resp->play_image_file.size =(long) fs.FileSize();
			}
			if (prm.play_type & RESP_PLAY_TYPE_CHAT) {
				pbinf = &(pst_rsf_resp->play_chat_file);
				strcpy_s(pbinf->bin_type, MAX_TYPE_LEN, "text/plain");
				sprintf_s(pbinf->filename, MAX_FILE, "rsf-%s-%s.txt", caller_id, pst_rsf_resp->order);
				//mbstowcs_s(&sz, buf, MAX_PATH, pbinf->filename, MAX_PATH);
				//_stprintf_s(fn, MAX_FILE, _T("%s%s%s"), m_strDataFolder, _T(RSF_TXT_PATH), buf);
				//fs.SetFullSpec(fn);
				strFn = pbinf->filename;	// mb -> unicode
				strFn = m_strDataFolder + _T(RSF_TXT_PATH) + strFn;
				fs.SetFullSpec(strFn);
				pst_rsf_resp->play_chat_file.size = (long)fs.FileSize();
			}
			break;
		case RESP_TYPE_RECORD:
			break;
		case RESP_TYPE_VRECORD:
			break;
		case RESP_TYPE_EXEC:
			pst_rsf_resp->exec_id = prm.exec_id;
			break;
		case RESP_TYPE_DISCONNECT:
			break;
		default:
			return FALSE;
		}
	}

	return TRUE;
}

//
//  �@�\     : Make each RDF Recieve.
//  
//  �@�\���� : SaveRdfFile()�̉���
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::MakeRdfRcv()
{
	ST_RDF_RESP st_rdf_resp;
	ST_RDF_RESP *pst_rdf_resp = &st_rdf_resp;
	CFileSpec fs;

	if (pst_rdf->resp_cnt >= MAX_RDF_RESP) return FALSE;

	// Recieve:RECORD
	pst_rdf->resp_cnt++;
	pst_rdf_resp = pst_rdf->data.call_resp + (pst_rdf->resp_cnt-1);
	memset((void *)pst_rdf_resp, '\0', sizeof(ST_RDF_RESP));
	strcpy_s(pst_rdf_resp->order, MAX_RDF_RESP, "$$");
	pst_rdf_resp->type = RESP_TYPE_RECORD;
	strcpy_s(pst_rdf_resp->term, sizeof(pst_rdf_resp->term)/sizeof(pst_rdf_resp->term[0]), "#");

	return TRUE;
}

//
//  �@�\     : RSF�ݒ�t�@�C���쐬
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::SaveRsfFile(
CArray<CRsfPrm, CRsfPrm> &prmArray)	// �����ݒ�̔z��
{
	char sfn[MAX_PATH];
	BOOL ret = FALSE;

	// Reset RSF structure
	memset((void *)pst_rsf, '\0', sizeof(ST_RSF));
	strcpy_s(pst_rsf->telno, MAX_TEL_LEN, caller_id);
	strcpy_s(pst_rsf->version, MAX_VER_LEN, POTM_VERSION);
	pst_rsf->resp_cnt = 0;

	// Make each RSF Response.
	MakeRsfResp(prmArray);

	// Reset XML structure
	memset((void *)pst_xml, '\0', sizeof(ST_XmlFile));
	if (newXML(pst_xml) < 0) return FALSE;

	// save RSF file
	memset(sfn, '\0', MAX_PATH);
	CString fn = RsfFileName();

#ifdef _UNICODE
	// wcstombs()�͓��{��𐳏�ɏ������Ȃ�
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,fn,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, fn, -1, sfn, dwLen, NULL, NULL);
#else
	wcstombs(sfn, fn, MAX_PATH);
#endif

	if (cnvRSF_XML(pst_rsf, pst_xml) < 0) {
		ret = FALSE;
		goto PROC_END;
	}
	if (writeXML(sfn, pst_xml) < 0) {
		ret = FALSE;
		goto PROC_END;
	}

	ret = TRUE;
PROC_END:
	freeXML(pst_xml);
	return ret;
}

//
//  �@�\     : RSF�ݒ�t�@�C���쐬
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::SaveRsfFile(
UST_RSF *pust)	// UNICODE �ݒ�\����
{
	CArray<CRsfPrm, CRsfPrm> prmArray;
	CRsfPrm prm;
	UST_RSF_RESP *resp;

	for (int i = 0; i < pust->resp_cnt; i++) {
		resp = pust->resp + i;
		prm.type = resp->type;
		prm.play_type = resp->play_type;
		prm.beep = resp->beep;
		prm.exec_id = resp->exec_id;
		prmArray.Add(prm);
	}

	return SaveRsfFile(prmArray);
}

//
//  �@�\     : RSF�ݒ�t�@�C���쐬(�����l)
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::SaveRsfFile()
{
	CArray<CRsfPrm, CRsfPrm> prmArray;
	prmArray.SetSize(3);
	CRsfPrm prm;
	memset(&prm, 0, sizeof(CRsfPrm));
	prm.type = RESP_TYPE_PLAY;
	prm.play_type = RESP_PLAY_TYPE_AUDIO;
	prmArray.SetAt(0, prm);
	memset(&prm, 0, sizeof(CRsfPrm));
	prm.type = RESP_TYPE_RECORD;
	prm.beep = 1;
	prm.exec_id = 0;
	prmArray.SetAt(1, prm);
	memset(&prm, 0, sizeof(CRsfPrm));
	prm.type = RESP_TYPE_DISCONNECT;
	prmArray.SetAt(2, prm);

	return SaveRsfFile(prmArray);
}

//
//  �@�\     : RDF��M�t�@�C���쐬
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::SaveRdfFile(
CString strDate)	// ����
{
	char sfn[MAX_PATH];
	BOOL ret = FALSE;

	// Reset RDF structure
	memset((void *)pst_rdf, '\0', sizeof(ST_RDF));
	strcpy_s(pst_rdf->telno, MAX_TEL_LEN, caller_id);
	strcpy_s(pst_rdf->version, MAX_VER_LEN, POTM_VERSION);
	pst_rdf->resp_cnt = 0;

	// Make each RDF Receive.
	MakeRdfRcv();

	// Reset XML structure
	memset((void *)pst_xml, '\0', sizeof(ST_XmlFile));
	if (newXML(pst_xml) < 0) return FALSE;

	// save RDF file
	memset(sfn, '\0', MAX_PATH);
	CString fn = RdfFileName(strDate);
#ifdef _UNICODE
	// wcstombs()�͓��{��𐳏�ɏ������Ȃ�
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,fn,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, fn, -1, sfn, dwLen, NULL, NULL);
#else
	wcstombs(sfn, fn, MAX_PATH);
#endif
	if (cnvRDF_XML(pst_rdf, pst_xml) < 0) {
		ret = FALSE;
		goto PROC_END;
	}
	if (writeXML(sfn, pst_xml) < 0) {
		ret = FALSE;
		goto PROC_END;
	}

	ret = TRUE;
PROC_END:
	freeXML(pst_xml);
	return ret;
}

//
//  �@�\     : ST_RSF_RESP����UST_RSF_RESP�ւ̃R�s�[
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::CopyST_UST_RESP(
ST_RSF_RESP *pst_resp,
UST_RSF_RESP *pust_resp)
{
	_TCHAR buf[MAX_LEN];
	size_t sz;

	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->order, MAX_LEN);
	pust_resp->order.Format(_T("%s"), (LPCTSTR)buf);
	pust_resp->type = pst_resp->type;
	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->term, MAX_LEN);
	pust_resp->term.Format(_T("%s"), (LPCTSTR)buf);
	pust_resp->beep = pst_resp->beep;
	pust_resp->exec_id = pst_resp->exec_id;
	pust_resp->record_flag = pst_resp->record_flag;
	pust_resp->play_type = pst_resp->play_type;

	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_audio_file.filename, MAX_LEN);
	pust_resp->play_audio_file.filename.Format(_T("%s"), (LPCTSTR)buf);
	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_audio_file.bin_type, MAX_LEN);
	pust_resp->play_audio_file.bin_type.Format(_T("%s"), (LPCTSTR)buf);
	pust_resp->play_audio_file.size = pst_resp->play_audio_file.size;

	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_video_file.filename, MAX_LEN);
	pust_resp->play_video_file.filename.Format(_T("%s"), (LPCTSTR)buf);
	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_video_file.bin_type, MAX_LEN);
	pust_resp->play_video_file.bin_type.Format(_T("%s"), (LPCTSTR)buf);
	pust_resp->play_video_file.size = pst_resp->play_video_file.size;

	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_image_file.filename, MAX_LEN);
	pust_resp->play_image_file.filename.Format(_T("%s"), (LPCTSTR)buf);
	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_image_file.bin_type, MAX_LEN);
	pust_resp->play_image_file.bin_type.Format(_T("%s"), (LPCTSTR)buf);
	pust_resp->play_image_file.size = pst_resp->play_image_file.size;

	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_chat_file.filename, MAX_LEN);
	pust_resp->play_chat_file.filename.Format(_T("%s"), (LPCTSTR)buf);
	mbstowcs_s(&sz, buf, MAX_LEN, pst_resp->play_chat_file.bin_type, MAX_LEN);
	pust_resp->play_chat_file.bin_type.Format(_T("%s"), (LPCTSTR)buf);
	pust_resp->play_chat_file.size = pst_resp->play_chat_file.size;

	return TRUE;
}

//
//  �@�\     : �����R�[�h�ɑΉ����������ݒ���擾
//  
//  �@�\���� : �f�t�H���g�������l��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTPsetup::GetResponse(
CString &strProc,		// �����R�[�h�A�Y�����Ȃ��ꍇ�f�t�H���g�l���܂񂾒l�ɕύX�����(input/return)
UST_RSF_RESP *pust_resp)	// ���ʎ擾�o�b�t�@
{
	ST_RSF_RESP st_resp;
	ST_RSF_RESP *pst_resp = &st_resp;
	char sorder[MAX_RDF_RESP];
	size_t sz;

	wcstombs_s(&sz, sorder, sizeof(sorder), strProc, MAX_RDF_RESP);
	if (getRSF_RESP(pst_rsf, pst_resp, sorder) < 0) {
		return NULL;
	}

	_TCHAR buf[MAX_LEN];
	mbstowcs_s(&sz, buf, MAX_LEN, sorder, MAX_LEN);
	strProc.Format(_T("%s"), (LPCTSTR)buf);

	return CopyST_UST_RESP(pst_resp, pust_resp);
}

//
//  �@�\     : RSF�t�@�C���̓��e���������Ƀ��[�h����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : UST_RSF, NULL:error
//  
//  ���l     : 
//  
UST_RSF *CTPsetup::LoadRSF()
{
	int i;
	char sfn[MAX_PATH];
	ST_RSF_RESP *presp;
	UST_RSF_RESP *puresp;
	size_t sz;

	//// RSFL�e�[�u���̑Ώۃ��R�[�h�Q��
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strSenderId") + _T("='") + m_strCallerId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
	if (tbl.SelectRec(strWhere) > 0) {
		m_lngRespId = tbl.ID; 	
	} else {
		//// �Y�����R�[�h���Ȃ����߁A�f�t�H���g�ݒ���g�p
		m_lngRespId = 1;
		SetCallerId(_T("0"));
	}

	/// RSF�̃��[�h
	CString fn = RsfFileName();
#ifdef _UNICODE
	// wcstombs()�͓��{��𐳏�ɏ������Ȃ�
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,fn,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, fn, -1, sfn, dwLen, NULL, NULL);
#else
	wcstombs(sfn, fn, MAX_PATH);
#endif
	if (getRSF(pst_rsf, sfn) < 0) return NULL;

	/// RSF��URSF�ɃR�s�[
	_TCHAR buf[MAX_LEN];
	mbstowcs_s(&sz, buf, MAX_LEN, pst_rsf->telno, MAX_LEN);
	pust_rsf->telno.Format(_T("%s"), (LPCTSTR)buf);
	mbstowcs_s(&sz, buf, MAX_LEN, pst_rsf->version, MAX_LEN);
	pust_rsf->version.Format(_T("%s"), (LPCTSTR)buf);
	pust_rsf->resp_cnt = pst_rsf->resp_cnt;
	for (i = 0; i < pust_rsf->resp_cnt; i++) {
		presp = pst_rsf->resp + i;
		puresp = pust_rsf->resp + i;
		CopyST_UST_RESP(presp, puresp);
	}

	return pust_rsf;
}

//
//  �@�\     : RDF�t�@�C���̓��e���������Ƀ��[�h����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : UST_RDF, NULL:error
//  
//  ���l     : 
//  
UST_RDF *CTPsetup::LoadRDF(
CString strDate)	// ����
{
	int i;
	char sfn[MAX_PATH];
	ST_RDF_RESP *presp;
	UST_RDF_RESP *puresp;
	size_t sz;

	/// RDF�̃��[�h
	CString fn = RdfFileName(strDate);
#ifdef _UNICODE
	// wcstombs()�͓��{��𐳏�ɏ������Ȃ�
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,fn,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, fn, -1, sfn, dwLen, NULL, NULL);
#else
	wcstombs(sfn, fn, MAX_PATH);
#endif
	if (getRDF(pst_rdf, sfn) < 0) return NULL;

	/// RDF��URDF�ɃR�s�[
	_TCHAR buf[MAX_LEN];
	mbstowcs_s(&sz, buf, MAX_LEN, pst_rdf->telno, MAX_LEN);
	pust_rdf->telno.Format(_T("%s"), (LPCTSTR)buf);
	mbstowcs_s(&sz, buf, MAX_LEN, pst_rdf->version, MAX_LEN);
	pust_rdf->version.Format(_T("%s"), (LPCTSTR)buf);
	pust_rdf->resp_cnt = pst_rdf->resp_cnt;
	for (i = 0; i < pust_rdf->resp_cnt; i++) {
		presp = pst_rdf->data.call_resp + i;
		puresp = pust_rdf->call_resp + i;
//		CopyST_UST_RESP(presp, puresp);
	}

	return pust_rdf;
}

