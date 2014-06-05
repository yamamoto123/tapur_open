#if !defined(AFX_RSFVIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFVIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfVideoDialog.h : ヘッダー ファイル
//
#include <imm.h> 
#include "Video.h"
#include "Label.h"
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfVideoDialog ダイアログ

class CRsfVideoDialog : public CDialog
{
// コンストラクション
public:
	CRsfVideoDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CRsfVideoDialog();      

// ダイアログ データ
	//{{AFX_DATA(CRsfVideoDialog)
	enum { IDD = IDD_RSF_VIDEO };
	CLabel	m_Time;
	CBitmapButton m_Play;
	CBitmapButton m_Pause;
	CBitmapButton m_Stop;
	CButton	m_File;
	CProgressCtrl m_Prog;
	//}}AFX_DATA

private:
	CTPerror te;
	CVideo *m_pVideo;		// ビデオ操作クラス
	CString m_strPlayFile;	// 応答ビデオファイル名
	CString m_strTmpFile;	// バックアップビデオファイル名
	long m_lngPlayTime;		// 演奏時間(sec)
	ST_TIMER m_stPlayChk;	// 演奏時間管理タイマ 

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfVideoDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfVideoDialog)
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnPause();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnFile();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL StartPlayChkTimer(long lngPlayTime);	// 演奏時間管理タイマ開始
	BOOL StopPlayChkTimer();	// 演奏時間管理タイマ終了

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	BOOL UpdateForm();	// 画面の更新
	BOOL Play();		// ビデオ再生
	BOOL PlayStop();	// ビデオ再生停止
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFVIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
