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

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//
//  機能     : コンストラクタ
//  
//  機能説明 : 初期処理
//  
//  返り値   : なし
//  
//  備考     : 
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
CString strPath)	// データフォルダ
{
	Init();

	SetDataDir(strPath);
}
CTPsetup::CTPsetup(
CString strPath,	// データフォルダ
CString strTelno)	// 送信者ID
{
	Init();

	SetDataDir(strPath);
	SetCallerId(strTelno);
}

//
//  機能     : デストラクタ
//  
//  機能説明 : 終了処理
//  
//  返り値   : なし
//  
//  備考     : 
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
//  機能     : データフォルダ登録
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::SetDataDir(
CString strPath)			// データフォルダ
{
	m_strDataFolder = strPath;
	return TRUE;
}

//
//  機能     : データパス名生成
//  
//  機能説明 : RDF用
//  
//  返り値   : データパス名
//  
//  備考     : 
//  
CString CTPsetup::GetDataFolder()
{
	CString str;
	DWORD dw;
	CRegDWORD rgEnableDataPath;
	CRegString rgDataPath;
	CFileSpec fs;

	// 規定のデータパスを使用（初期値）
	str = m_strDataFolder;

	/// レジストリ読み込み
	rgEnableDataPath = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableDataPath"), 0, TRUE);
	dw = rgEnableDataPath;
	if (dw > 0) {
		/// ユーザ設定のデータパスを使用
		// 指定したデータフォルダが使用できない場合、規定のデータパスを使用
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
//  機能     : 送信者ID登録
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::SetCallerId(
CString strTelno)			// 送信者ID
{
	if (strTelno.IsEmpty()) {
		// 不明な送信者（SkypeIn）
		m_strCallerId = _T("0");
	} else {
		// ファイル名として不正な文字は"_"置換する
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
//  機能     : 日時ID文字列
//  
//  機能説明 : 
//  
//  返り値   : 日時ID文字列
//  
//  備考     : 
//  
CString CTPsetup::GetDateStr()
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return strDate;
}

//
//  機能     : RSFファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RSFファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfFileName()
{
	CString str = m_strDataFolder + _T(RSF_PATH) + _T("rsf-") + m_strCallerId + _T(".ref");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  機能     : RSF BEEP音声ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : BEEP音声ファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfBeepFileName(
CString strSuffix)	// サフィックス（ex, "1"）
{
	CString str = m_strDataFolder + _T(RSF_WAV_PATH) + _T("beep") + strSuffix + _T(".wav");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  機能     : RSF音声ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RSF音声ファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfAudioFileName(
CString strProc)	// 応答コード（ex, "$"）
{
	CString str = m_strDataFolder + _T(RSF_WAV_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T(".wav");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  機能     : RSFチャットファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RSFチャットファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfChatFileName(
CString strProc)	// 応答コード（ex, "$"）
{
	CString str = m_strDataFolder + _T(RSF_TXT_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T(".txt");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  機能     : RSFビデオファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RSFビデオファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfVideoFileName(
CString strProc)	// 応答コード（ex, "$"）
{
	CString str = m_strDataFolder + _T(RSF_AVI_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T(".avi");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  機能     : RSF画像ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RSF画像ファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfImgFileName(
CString strProc,	// 応答コード（ex, "$"）
CString strExt)		// 拡張子
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
//  機能     : RSF画像ファイルの検索
//  
//  機能説明 : 
//  
//  返り値   : RSF画像ファイル名
//  
//  備考     : 
//  
CString CTPsetup::FindRsfImgFile(
CString strProc)	// 応答コード（ex, "$"）
{
	CString str;
	CString strFile;
	CFileSpec fs;

	/// RSFファイルから検出
	UST_RSF *pst = LoadRSF();
	strFile.Format(_T("%s"), pst->resp->play_image_file.filename);
	if (!strFile.IsEmpty()) {
		str = m_strDataFolder + _T(RSF_IMG_PATH) + strFile;
		fs.SetFullSpec(str);
		if (fs.Exist()) return str;
	}

	/// RSFファイルに記載がない場合、フォルダから検索
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
//  機能     : RSF連携プログラムCSVファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RSF連携プログラムCSVファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfPrgFileName(
CString strProc,	// 応答コード（ex, "$"）
CString strPrgId)	// プログラムID（"" or 1-999）
{
	CString str = m_strDataFolder + _T(RSF_CSV_PATH) + _T("rsf-") + m_strCallerId + _T("-") + strProc + _T("-") + strPrgId + _T(".csv");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}


//
//  機能     : RSF一時フォルダ参照
//  
//  機能説明 : 
//  
//  返り値   : RSF一時フォルダ参照
//  
//  備考     : 
//  
CString CTPsetup::RsfTmpFolder()
{
	CString str = m_strDataFolder + _T(RSF_TMP_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  機能     : RSF一時ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDF一時ファイル名
//  
//  備考     : 
//  
CString CTPsetup::RsfTmpFileName(
CString strExt)		// 拡張子
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	CString str = RsfTmpFolder() + _T("tmp-") + strDate;
	str = str + _T("-$.") + strExt;
	
	return str;
}

//
//  機能     : RDFファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDFファイル名
//  
//  備考     : 保存先は常に規定のデータフォルダ（データフォルダの変更には追従しない）
//  
CString CTPsetup::RdfFileName(
CString strDate)	// 日時
{
	CString str = m_strDataFolder + _T(RDF_PATH) + _T("rdf-") + m_strCallerId + _T("-") + strDate + _T(".ref");
	
	CFileSpec fs(str);
	fs.MkFolder();
	return str;
}

//
//  機能     : RDF音声(WAV)ファイルフォルダ
//  
//  機能説明 : 
//  
//  返り値   : RDF音声(WAV)ファイルフォルダ名
//  
//  備考     : 
//  
CString CTPsetup::RdfWavFolder()
{
	CString str = GetDataFolder() + _T(RDF_WAV_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  機能     : RDF音声(WAV)ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDF音声(WAV)ファイル名
//  
//  備考     : 
//  
CString CTPsetup::RdfWavFileName(
CString strFolder,	// データフォルダ
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
{
	CString str = strFolder + _T("rdf-") + m_strCallerId + _T("-") + strDate;
	str += _T("-") + strProc + _T(".wav");

	return str;
}
CString CTPsetup::RdfWavFileName(
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
{
	return RdfWavFileName(RdfWavFolder(), strDate, strProc);
}
CString CTPsetup::RdfWavFileName(
CString strProc)	// 応答コード（ex, "$"）
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return RdfWavFileName(strDate, strProc);
}

//
//  機能     : RDF音声(WAV)ファイルの検索
//  
//  機能説明 : 
//  
//  返り値   : RDF音声(WAV)ファイル名、Empty=見つからない場合
//  
//  備考     : 
//  
CString CTPsetup::FindRdfWavFileName(
CString strFolder,	// データフォルダ
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
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
//  機能     : RDF音声(MP3)ファイルフォルダ
//  
//  機能説明 : 
//  
//  返り値   : RDF音声(MP3)ファイルフォルダ名
//  
//  備考     : 
//  
CString CTPsetup::RdfMp3Folder()
{
	CString str = GetDataFolder() + _T(RDF_MP3_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  機能     : RDF音声(MP3)ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDF音声(MP3)ファイル名
//  
//  備考     : 
//  
CString CTPsetup::RdfMp3FileName(
CString strFolder,	// データフォルダ
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
{
	CString str = strFolder + _T("rdf-") + m_strCallerId + _T("-") + strDate;
	str += _T("-") + strProc + _T(".mp3");

	return str;
}
CString CTPsetup::RdfMp3FileName(
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
{
	return RdfMp3FileName(RdfMp3Folder(), strDate, strProc);
}
CString CTPsetup::RdfMp3FileName(
CString strProc)	// 応答コード（ex, "$"）
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return RdfMp3FileName(strDate, strProc);
}

//
//  機能     : RDF音声(MP3)ファイルの検索
//  
//  機能説明 : 
//  
//  返り値   : RDF音声(MP3)ファイル名、Empty=見つからない場合
//  
//  備考     : 
//  
CString CTPsetup::FindRdfMp3FileName(
CString strFolder,	// データフォルダ
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
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
//  機能     : RDF音声ファイルから日時を取得
//  
//  機能説明 : 
//  
//  返り値   : 日時文字列
//  
//  備考     : 
//  
CString CTPsetup::GetAudioFileDate(
CString strFile)	// RDF音声ファイル
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
//  機能     : RDF音声ファイルから送信者名（スカイプ名）を取得
//  
//  機能説明 : 
//  
//  返り値   : 送信者名（スカイプ名）文字列
//  
//  備考     : 
//  
CString CTPsetup::GetAudioFileCallerId(
CString strFile)	// RDF音声ファイル
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

	// 送信者名がセパレータ文字を含む場合がある
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
//  機能     : RDFビデオファイルフォルダ
//  
//  機能説明 : 
//  
//  返り値   : RDFビデオファイルフォルダ名
//  
//  備考     : 
//  
CString CTPsetup::RdfVideoFolder()
{
	CString str = GetDataFolder() + _T(RDF_AVI_PATH);
	
	CFileSpec fs(str + "\\dummy.file");
	fs.MkFolder();
	return str;
}

//
//  機能     : RDFビデオファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDFビデオファイル名
//  
//  備考     : 
//  
CString CTPsetup::RdfVideoFileName(
CString strFolder,	// データフォルダ
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
{
	CString str = strFolder + _T("rdf-") + m_strCallerId + _T("-") + strDate;
	str += _T("-") + strProc + _T(".avi");

	return str;
}
CString CTPsetup::RdfVideoFileName(
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
{
	return RdfVideoFileName(RdfVideoFolder(), strDate, strProc);
}
CString CTPsetup::RdfVideoFileName(
CString strProc)	// 応答コード（ex, "$"）
{
	_TCHAR tmp[256];
	GetDateId(tmp);
	CString strDate = tmp;
	return RdfVideoFileName(strDate, strProc);
}

//
//  機能     : RDF音声ファイルの検索
//  
//  機能説明 : 
//  
//  返り値   : RDF音声ファイル名、Empty=見つからない場合
//  
//  備考     : 
//  
CString CTPsetup::FindRdfVideoFileName(
CString strFolder,	// データフォルダ
CString strDate,	// 日時
CString strProc)	// 応答コード（ex, "$"）
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
//  機能     : RDF一時音声ファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDF一時音声ファイル名
//  
//  備考     : 
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
//  機能     : RDF一時ビデオファイル名生成
//  
//  機能説明 : 
//  
//  返り値   : RDF一時ビデオファイル名
//  
//  備考     : 
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
//  機能     : Make each RSF Response.
//  
//  機能説明 : SaveRsfFile()の下位
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::MakeRsfResp(
CArray<CRsfPrm, CRsfPrm> &prmArray)	// 応答設定の配列
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
		// 共通設定
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

		// 各Response設定
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
//  機能     : Make each RDF Recieve.
//  
//  機能説明 : SaveRdfFile()の下位
//  
//  返り値   : BOOL
//  
//  備考     : 
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
//  機能     : RSF設定ファイル作成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::SaveRsfFile(
CArray<CRsfPrm, CRsfPrm> &prmArray)	// 応答設定の配列
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
	// wcstombs()は日本語を正常に処理しない
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
//  機能     : RSF設定ファイル作成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::SaveRsfFile(
UST_RSF *pust)	// UNICODE 設定構造体
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
//  機能     : RSF設定ファイル作成(初期値)
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
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
//  機能     : RDF受信ファイル作成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::SaveRdfFile(
CString strDate)	// 日時
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
	// wcstombs()は日本語を正常に処理しない
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
//  機能     : ST_RSF_RESPからUST_RSF_RESPへのコピー
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
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
//  機能     : 応答コードに対応した応答設定を取得
//  
//  機能説明 : デフォルト応答を考慮
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTPsetup::GetResponse(
CString &strProc,		// 応答コード、該当がない場合デフォルト値を含んだ値に変更される(input/return)
UST_RSF_RESP *pust_resp)	// 結果取得バッファ
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
//  機能     : RSFファイルの内容をメモリにロードする
//  
//  機能説明 : 
//  
//  返り値   : UST_RSF, NULL:error
//  
//  備考     : 
//  
UST_RSF *CTPsetup::LoadRSF()
{
	int i;
	char sfn[MAX_PATH];
	ST_RSF_RESP *presp;
	UST_RSF_RESP *puresp;
	size_t sz;

	//// RSFLテーブルの対象レコード参照
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strSenderId") + _T("='") + m_strCallerId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
	if (tbl.SelectRec(strWhere) > 0) {
		m_lngRespId = tbl.ID; 	
	} else {
		//// 該当レコードがないため、デフォルト設定を使用
		m_lngRespId = 1;
		SetCallerId(_T("0"));
	}

	/// RSFのロード
	CString fn = RsfFileName();
#ifdef _UNICODE
	// wcstombs()は日本語を正常に処理しない
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,fn,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, fn, -1, sfn, dwLen, NULL, NULL);
#else
	wcstombs(sfn, fn, MAX_PATH);
#endif
	if (getRSF(pst_rsf, sfn) < 0) return NULL;

	/// RSFをURSFにコピー
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
//  機能     : RDFファイルの内容をメモリにロードする
//  
//  機能説明 : 
//  
//  返り値   : UST_RDF, NULL:error
//  
//  備考     : 
//  
UST_RDF *CTPsetup::LoadRDF(
CString strDate)	// 日時
{
	int i;
	char sfn[MAX_PATH];
	ST_RDF_RESP *presp;
	UST_RDF_RESP *puresp;
	size_t sz;

	/// RDFのロード
	CString fn = RdfFileName(strDate);
#ifdef _UNICODE
	// wcstombs()は日本語を正常に処理しない
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,fn,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, fn, -1, sfn, dwLen, NULL, NULL);
#else
	wcstombs(sfn, fn, MAX_PATH);
#endif
	if (getRDF(pst_rdf, sfn) < 0) return NULL;

	/// RDFをURDFにコピー
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

