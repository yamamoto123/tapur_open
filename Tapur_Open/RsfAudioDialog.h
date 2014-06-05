#if !defined(AFX_RSFAUDIODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFAUDIODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfAudioDialog.h : ヘッダー ファイル
//
#include <imm.h> 
#include "Audio.h"
#include "Label.h"
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfAudioDialog ダイアログ

class CRsfAudioDialog : public CDialog
{
// コンストラクション
public:
	CRsfAudioDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CRsfAudioDialog();      

// ダイアログ データ
	//{{AFX_DATA(CRsfAudioDialog)
	enum { IDD = IDD_RSF_AUDIO };
	CLabel	m_Time;
	CLabel	m_LinkBuy;
	CBitmapButton m_Play;
	CBitmapButton m_Pause;
	CBitmapButton m_Stop;
	CButton	m_File;
	CProgressCtrl m_Prog;
	//}}AFX_DATA

private:
	CTPerror te;
	CAudio *m_pAudio;		// 音声操作クラス
	CString m_strPlayFile;	// 応答音声ファイル名
	CString m_strTmpFile;	// バックアップ音声ファイル名
	long m_lngPlayTime;		// 演奏時間(sec)
	ST_TIMER m_stPlayChk;	// 演奏時間管理タイマ 

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfAudioDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfAudioDialog)
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
	BOOL Play();		// 音声再生
	BOOL PlayStop();	// 音声再生停止
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFAUDIODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
