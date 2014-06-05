// @(h) TPsetup.h        ver 1.0 ( '99.2.28  )

// @(s)
// �ݒ�t�@�C���A�\���̃n���h�����O
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

// �������p�����[�^�N���X�iUI <-> TPsetup�j
// �ʏ�͉������̔z��œn��
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
{	// �G���[�n���h�����O�N���X
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
	CString m_strCallerId;				// skype���A"0":default
	char caller_id[MAX_USRNAME_LEN];	// same as strCallerId
	long m_lngRespId;					// RSFL�e�[�u����̑Ή����R�[�hID

private:
	void Init();			// �R���X�g���N�^���ʏ���	
	BOOL MakeRsfResp(CArray<CRsfPrm, CRsfPrm> &prmArray);	// RSF�\���̂̃T�u�v�f����
	BOOL MakeRdfRcv();		// RDF�\���̂̃T�u�v�f����
	BOOL CopyST_UST_RESP(ST_RSF_RESP *pst_resp, UST_RSF_RESP *pust_resp);	// ST_RSF_RESP����UST_RSF_RESP�ւ̃R�s�[

public:
	CTPsetup();	
	CTPsetup(CString strPath);	
	CTPsetup(CString strPath, CString strTelno);	
	~CTPsetup();	

	CString GetDateStr();						// ����ID������
	long GetRespId() { return m_lngRespId; }	// RSFL���R�[�hID�Q��
	BOOL SetDataDir(CString strPath);			// �f�[�^�t�H���_
	CString GetDataFolder();					// �f�[�^�p�X������
	BOOL SetCallerId(CString strTelno);			// ���M��ID�o�^
	CString GetCallerId() { return m_strCallerId; }	// ���M��ID�Q��
	CString RsfFileName();						// RSF�t�@�C��������
	CString RsfBeepFileName(CString strSuffix = _T(""));	// BEEP�����t�@�C��������
	CString RsfAudioFileName(CString strProc);	// RSF�����t�@�C��������
	CString RsfChatFileName(CString strProc);	// RSF�`���b�g�t�@�C��������
	CString RsfVideoFileName(CString strProc);	// RSF�r�f�I�t�@�C��������
	CString RsfImgFileName(CString strProc, CString strExt);// RSF�摜�t�@�C��������
	CString FindRsfImgFile(CString strProc);	// RSF�摜�t�@�C������
	CString RsfPrgFileName(CString strProc, CString strPrgId);	// RSF�A�g�v���O����CSV�t�@�C��������
	CString RsfTmpFileName(CString strExt);		// RSF�ꎞ�t�@�C��������
	CString RsfTmpFolder();						// RSF�ꎞ�t�H���_�Q��
	CString RsfTmpAudioFileName() { return RsfTmpFileName(CString("wav")); }	// RSF�ꎞ�����t�@�C��������
	CString RsfTmpChatFileName() { return RsfTmpFileName(CString("txt")); }	// RSF�ꎞ�`���b�g�t�@�C��������
	CString RsfTmpVideoFileName() { return RsfTmpFileName(CString("avi")); }	// RSF�ꎞ�r�f�I�t�@�C��������
	CString RsfTmpImgFileName(CString strExt) { return RsfTmpFileName(strExt); }	// RSF�ꎞ�摜�t�@�C��������
	CString RdfFileName(CString strDate);		// RDF�@�C��������
	CString RdfWavFolder();										// RDF����(WAV)�t�@�C���t�H���_
	CString RdfWavFileName(CString strFolder, CString strDate, CString strProc);	// RDF����(WAV)�t�@�C��������
	CString RdfWavFileName(CString strDate, CString strProc);	// RDF����(WAV)�t�@�C��������
	CString RdfWavFileName(CString strProc);					// RDF����(WAV)�t�@�C��������
	CString FindRdfWavFileName(CString strFolder, CString strDate, CString strProc);	// RDF����(WAV)�t�@�C������
	CString RdfMp3Folder();										// RDF����(MP3)�t�@�C���t�H���_
	CString RdfMp3FileName(CString strFolder, CString strDate, CString strProc);	// RDF����(MP3)�t�@�C��������
	CString RdfMp3FileName(CString strDate, CString strProc);	// RDF����(MP3)�t�@�C��������
	CString RdfMp3FileName(CString strProc);					// RDF����(MP3)�t�@�C��������
	CString FindRdfMp3FileName(CString strFolder, CString strDate, CString strProc);	// RDF����(MP3)�t�@�C������
	CString GetAudioFileDate(CString strFile);					// RDF�����t�@�C������������擾
	CString GetAudioFileCallerId(CString strFile);				// RDF�����t�@�C�����瑗�M�Җ��i�X�J�C�v���j���擾	
	CString RdfVideoFolder();									// RDF�r�f�I�t�@�C���t�H���_
	CString RdfVideoFileName(CString strFolder, CString strDate, CString strProc);	// RDF�r�f�I�t�@�C��������
	CString RdfVideoFileName(CString strDate, CString strProc);	// RDF�r�f�I�t�@�C��������
	CString RdfVideoFileName(CString strProc);					// RDF�r�f�I�t�@�C��������
	CString FindRdfVideoFileName(CString strFolder, CString strDate, CString strProc);	// RDF�r�f�I�t�@�C������
	CString RdfTmpAudioFileName();				// RDF�ꎞ�����t�@�C��������
	CString RdfTmpVideoFileName();				// RDF�ꎞ�r�f�I�t�@�C��������
	BOOL SaveRsfFile();							// RSF�t�@�C������(�����l)
	BOOL SaveRsfFile(CArray<CRsfPrm, CRsfPrm> &prmArray);// RSF�t�@�C������
	BOOL SaveRsfFile(UST_RSF *pust_rsf);		// RSF�t�@�C������
	BOOL SaveRdfFile(CString strDate);			// RDF�t�@�C������
	UST_RSF *LoadRSF();							// RSF�t�@�C���̓��e���������Ƀ��[�h����
	UST_RDF *LoadRDF(CString strDate);			// RDF�t�@�C���̓��e���������Ƀ��[�h����
	BOOL GetResponse(CString &strProc, UST_RSF_RESP *pust_resp);	// �����R�[�h�ɑΉ����������ݒ���擾
};

#endif // !defined(_TPSETUP_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
