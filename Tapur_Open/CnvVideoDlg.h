// CnvVideoDlg.h : header file
//

#if !defined(AFX_CNVVIDEODLG_H__7D80A48A_4EB5_4411_8561_8477DD310C84__INCLUDED_)
#define AFX_CNVVIDEODLG_H__7D80A48A_4EB5_4411_8561_8477DD310C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AviManager.h"

/////////////////////////////////////////////////////////////////////////////
// CCnvVideoDlg dialog

class CCnvVideoDlg : public CDialog
{
// Construction
public:
   CCnvVideoDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
   //{{AFX_DATA(CCnvVideoDlg)
	enum { IDD = IDD_CONVERT_VIDEO };
    BOOL	m_blAudio;
	CString m_strTarget;
	//}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCnvVideoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CCnvVideoDlg)
	afx_msg void OnBrowsetarget();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
	BOOL CheckTargetFile(CString &sTargetFile);

private:
	CString m_strAudioFile;	// source audio file
	CString m_strSrcFile;	// source video file
	CString m_strDstFile;	// target video file
	CAviManager *m_pAviManager;
	BOOL CnvAviAvi();

public:
	BOOL SetSrcFile(CString strAViFile, CString strWavFile);	// 入力ファイルの指定
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNVVIDEODLG_H__7D80A48A_4EB5_4411_8561_8477DD310C84__INCLUDED_)
