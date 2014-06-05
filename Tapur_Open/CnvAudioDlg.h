// CnvAudioDlg.h : header file
//

#if !defined(AFX_CNVAUDIODLG_H__7D80A48A_4EB5_4411_8561_8477DD310C84__INCLUDED_)
#define AFX_CNVAUDIODLG_H__7D80A48A_4EB5_4411_8561_8477DD310C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCnvAudioDlg dialog

//#include "DSEncoder.h"
#include "AcmMgr.h"
#include "ProgressWnd.h"

class CCnvAudioDlg : public CDialog
{
// Construction
public:
   CCnvAudioDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
   //{{AFX_DATA(CCnvAudioDlg)
	enum { IDD = IDD_CONVERT_AUDIO };
	CString m_strTarget;
	int		m_idxCodecName;
	int		m_idxFormatName;
	//}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCnvAudioDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CCnvAudioDlg)
	afx_msg void OnBrowsetarget();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnRdoType();
	afx_msg void OnCmbCodec();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
	//CDSEncoder	m_encoder;
	BOOL CheckTargetFile(CString TargetFile);

private:
	CString m_strSrcFile;			// 入力ファイル
	CString m_strDstFile;			// 出力ファイル
	CStringArray m_aryCodecName;	// MP3コーデック形式
	CStringArray m_aryFormatName;	// MP3フォーマット形式
	CAcmMgr m_oAcmMgr;				// ACMマネージャ
	WAVEFORMATEX m_wfx;				// 入力ファイルのフォーマットに対応したWAVEFORMATEX

public:
	BOOL SetSrcFile(CString strFile);	// 入力ファイルの指定
	BOOL CopyFile(BOOL blMsg = TRUE);
	BOOL CnvWavMp3(BOOL blMsg = TRUE);
	BOOL CnvWavOgg(BOOL blMsg = TRUE);
	BOOL CnvMp3Wav(BOOL blMsg = TRUE);
	BOOL CnvMp3Ogg(BOOL blMsg = TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNVAUDIODLG_H__7D80A48A_4EB5_4411_8561_8477DD310C84__INCLUDED_)
