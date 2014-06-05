#if !defined(AFX_MsgForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
#define AFX_MsgForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgForm.h : header file
//

#include "VisualFx.h"
#include "TPerror.h"
#include "Label.h"
#include "TPerror.h"
#include "Audio.h"
#include "Video.h"
#include "CnvAudioDlg.h"
#include "CnvVideoDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMsgForm : public TVisualFormView
{
protected:
	CMsgForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMsgForm)

// Form Data
public:
	//{{AFX_DATA(CMsgForm)
	enum { IDD = IDD_MSGFORM };
	CString	m_EditTitle;
	CLabel	m_Title;
	CLabel	m_Title1;
	CLabel	m_Title2;
	CLabel	m_Title3;
	CLabel	m_Title4;
	CLabel	m_Date;
	CLabel	m_SenderName;
	CLabel	m_SenderId;
	CLabel	m_Time;
	CBitmapButton m_Play;
	CBitmapButton m_Pause;
	CBitmapButton m_Stop;
	CProgressCtrl m_Prog;
	CButton	m_File;
	CButton	m_VFile;
	CButton	m_PodCast;
	CButton	m_AppPlay;
	CButton	m_AppVPlay;
	//}}AFX_DATA

// Attributes
private:
	CTPerror te;
	CBrush m_brush;			// 背景色のブラシ(white)
	CAudio *m_pAudio;		// 音声操作クラス
	CVideo *m_pVideo;		// ビデオ操作クラス
	CString m_strAudioFile;	// 音声ファイル名
	CString m_strVideoFile;	// ビデオファイル名
	long m_lngPlayTime;		// 演奏時間(sec)
	ST_TIMER m_stPlayChk;	// 演奏時間管理タイマ 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgForm)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMsgForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMsgForm)
	afx_msg void OnKillfocusEditTitle();
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnPause();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnFile();
	afx_msg void OnBtnVFile();
	afx_msg void OnBtnAppPlay();
	afx_msg void OnBtnAppVPlay();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL StartPlayChkTimer(long lngPlayTime);	// 演奏時間管理タイマ開始
	BOOL StopPlayChkTimer();	// 演奏時間管理タイマ終了
	CCnvAudioDlg m_cnvADlg;		// 音声ファイル変換ツール
	CCnvVideoDlg m_cnvVDlg;		// ビデオファイル変換ツール

public:
//	BOOL UpdateForm();	// 画面の更新
	BOOL UpdateForm(BOOL blPlayStop = TRUE);	// 画面の更新
	BOOL Play();		// 音声再生
	BOOL PlayPause();	// 音声再生一時停止
	BOOL PlayStop();	// 音声再生停止
	BOOL ExportFile();	// 音声エクスポート
	BOOL ExportVFile();	// 動画エクスポート
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MsgForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
