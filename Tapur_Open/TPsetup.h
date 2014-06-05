// @(h) TPsetup.h        ver 1.0 ( '99.2.28  )

// @(s)
// 設定ファイル、構造体ハンドリング
//

#if !defined(_TPSETUP_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
#define _TPSETUP_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_

#include <afxtempl.h> // CArray 
#include "file_access.h"
#include "TPerror.h"

typedef struct {
	CString filename;		/* binary filename */
	CString bin_type;		/* binary type string */
	long size;				/* file size */
} UST_BIN_FILE;

typedef struct {
	CString order;			/* response order, unique key */
	int type;				/* response type -> see definitions */
	CString name;			/* response name or id */
	CString term;			/* termination tone character list */
	int play_type;			/* play type -> see definitions */
	UST_BIN_FILE play_audio_file;	/* play audio file */
	UST_BIN_FILE play_video_file;	/* play video file */
	UST_BIN_FILE play_image_file;	/* play image file */
	UST_BIN_FILE play_chat_file;	/* play chat file */
	int beep;				/* beep before response flag on(1), off(0) */
	int exec_id;			/* id for "exec" response type */
	int record_flag;		/* flag to record(1) or not(0).(when use handset only) */
	CString link_order;		/* linked response order */
} UST_RSF_RESP;

typedef struct {
	CString telno;			/* telephone number, unique key */
	CString version;		/* RSF format version */
	int resp_cnt;			/* count of response */
	UST_RSF_RESP resp[MAX_RSF_RESP];	/* response setting list */
} UST_RSF;

typedef struct {
	CString order;			/* response order, unique key */
	int type;				/* response type -> see definitions */
	CString rsf_order;		/* RSF response order (if linked response used, not same with order) */
	CString term;			/* termination tone character */
	UST_BIN_FILE record_file;	/* recorded voice file */
	CString dtmf;			/* dtmf tone string */
} UST_RDF_RESP;

typedef struct {
	int file_cnt;			/* count of fax file */
	UST_BIN_FILE fax_file[MAX_FAX_PAGE];	/* fax image file list */
} UST_FAX_DATA;

typedef struct {
	CString id_date;		/* Date received data, unique key */
	CString telno;			/* caller's telephone number */
	CString version;		/* RDF format version */
	int type;				/* received data type -> see definitions */
	UST_RSF *st_rsf;		/* RSF setting used by call response */
	int resp_cnt;			/* count of response */
	UST_RDF_RESP call_resp[MAX_RDF_RESP];	/* call response list */
//	UST_FAX_DATA fax_data;	/* fax data */
} UST_RDF;

// 応答情報パラメータクラス（UI <-> TPsetup）
// 通常は応答順の配列で渡す
class CRsfPrm {
public:
	CRsfPrm() { Init(); }
	void Init() { type = 0; play_type = 0; beep = 0; exec_id = 0; }
	int type;		/* response type -> see definitions */
	int play_type;	/* play type -> see definitions */
	int beep;		/* beep before response flag on(1), off(0) */
	int exec_id;	/* id for "exec" response type */
	int file_type;	/* play file type -> see definitions */
};

class CTPsetup
{	// エラーハンドリングクラス
private:
	CTPerror te;
	ST_XmlFile st_xml;
	ST_XmlFile *pst_xml;
	ST_RSF st_rsf;
	ST_RSF *pst_rsf;
	UST_RSF ust_rsf;
	UST_RSF *pust_rsf;
	ST_RDF st_rdf;
	ST_RDF *pst_rdf;
	UST_RDF ust_rdf;
	UST_RDF *pust_rdf;
	CString m_strDataFolder;
	CString m_strCallerId;				// skype名、"0":default
	char caller_id[MAX_USRNAME_LEN];	// same as strCallerId
	long m_lngRespId;					// RSFLテーブル上の対応レコードID

private:
	void Init();			// コンストラクタ共通処理	
	BOOL MakeRsfResp(CArray<CRsfPrm, CRsfPrm> &prmArray);	// RSF構造体のサブ要素生成
	BOOL MakeRdfRcv();		// RDF構造体のサブ要素生成
	BOOL CopyST_UST_RESP(ST_RSF_RESP *pst_resp, UST_RSF_RESP *pust_resp);	// ST_RSF_RESPからUST_RSF_RESPへのコピー

public:
	CTPsetup();	
	CTPsetup(CString strPath);	
	CTPsetup(CString strPath, CString strTelno);	
	~CTPsetup();	

	CString GetDateStr();						// 日時ID文字列
	long GetRespId() { return m_lngRespId; }	// RSFLレコードID参照
	BOOL SetDataDir(CString strPath);			// データフォルダ
	CString GetDataFolder();					// データパス名生成
	BOOL SetCallerId(CString strTelno);			// 送信者ID登録
	CString GetCallerId() { return m_strCallerId; }	// 送信者ID参照
	CString RsfFileName();						// RSFファイル名生成
	CString RsfBeepFileName(CString strSuffix = _T(""));	// BEEP音声ファイル名生成
	CString RsfAudioFileName(CString strProc);	// RSF音声ファイル名生成
	CString RsfChatFileName(CString strProc);	// RSFチャットファイル名生成
	CString RsfVideoFileName(CString strProc);	// RSFビデオファイル名生成
	CString RsfImgFileName(CString strProc, CString strExt);// RSF画像ファイル名生成
	CString FindRsfImgFile(CString strProc);	// RSF画像ファイル検索
	CString RsfPrgFileName(CString strProc, CString strPrgId);	// RSF連携プログラムCSVファイル名生成
	CString RsfTmpFileName(CString strExt);		// RSF一時ファイル名生成
	CString RsfTmpFolder();						// RSF一時フォルダ参照
	CString RsfTmpAudioFileName() { return RsfTmpFileName(CString("wav")); }	// RSF一時音声ファイル名生成
	CString RsfTmpChatFileName() { return RsfTmpFileName(CString("txt")); }	// RSF一時チャットファイル名生成
	CString RsfTmpVideoFileName() { return RsfTmpFileName(CString("avi")); }	// RSF一時ビデオファイル名生成
	CString RsfTmpImgFileName(CString strExt) { return RsfTmpFileName(strExt); }	// RSF一時画像ファイル名生成
	CString RdfFileName(CString strDate);		// RDFァイル名生成
	CString RdfWavFolder();										// RDF音声(WAV)ファイルフォルダ
	CString RdfWavFileName(CString strFolder, CString strDate, CString strProc);	// RDF音声(WAV)ファイル名生成
	CString RdfWavFileName(CString strDate, CString strProc);	// RDF音声(WAV)ファイル名生成
	CString RdfWavFileName(CString strProc);					// RDF音声(WAV)ファイル名生成
	CString FindRdfWavFileName(CString strFolder, CString strDate, CString strProc);	// RDF音声(WAV)ファイル検索
	CString RdfMp3Folder();										// RDF音声(MP3)ファイルフォルダ
	CString RdfMp3FileName(CString strFolder, CString strDate, CString strProc);	// RDF音声(MP3)ファイル名生成
	CString RdfMp3FileName(CString strDate, CString strProc);	// RDF音声(MP3)ファイル名生成
	CString RdfMp3FileName(CString strProc);					// RDF音声(MP3)ファイル名生成
	CString FindRdfMp3FileName(CString strFolder, CString strDate, CString strProc);	// RDF音声(MP3)ファイル検索
	CString GetAudioFileDate(CString strFile);					// RDF音声ファイルから日時を取得
	CString GetAudioFileCallerId(CString strFile);				// RDF音声ファイルから送信者名（スカイプ名）を取得	
	CString RdfVideoFolder();									// RDFビデオファイルフォルダ
	CString RdfVideoFileName(CString strFolder, CString strDate, CString strProc);	// RDFビデオファイル名生成
	CString RdfVideoFileName(CString strDate, CString strProc);	// RDFビデオファイル名生成
	CString RdfVideoFileName(CString strProc);					// RDFビデオファイル名生成
	CString FindRdfVideoFileName(CString strFolder, CString strDate, CString strProc);	// RDFビデオファイル検索
	CString RdfTmpAudioFileName();				// RDF一時音声ファイル名生成
	CString RdfTmpVideoFileName();				// RDF一時ビデオファイル名生成
	BOOL SaveRsfFile();							// RSFファイル生成(初期値)
	BOOL SaveRsfFile(CArray<CRsfPrm, CRsfPrm> &prmArray);// RSFファイル生成
	BOOL SaveRsfFile(UST_RSF *pust_rsf);		// RSFファイル生成
	BOOL SaveRdfFile(CString strDate);			// RDFファイル生成
	UST_RSF *LoadRSF();							// RSFファイルの内容をメモリにロードする
	UST_RDF *LoadRDF(CString strDate);			// RDFファイルの内容をメモリにロードする
	BOOL GetResponse(CString &strProc, UST_RSF_RESP *pust_resp);	// 応答コードに対応した応答設定を取得
};

#endif // !defined(_TPSETUP_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
