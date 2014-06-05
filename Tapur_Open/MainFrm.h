// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CD11421F_56D8_4EB4_9104_11F0891E2316__INCLUDED_)
#define AFX_MAINFRM_H__CD11421F_56D8_4EB4_9104_11F0891E2316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPerror.h"
#include "VisualFx.h"
#include "SystemTray.h"
#include "ExtStatusControlBar.h"
#include "MainTool.h"
#include "Audio.h"
#include "TPerror.h"

//interface IAccess;
class CAccessEvents;

class CMainFrame : public CFrameWnd
{
	
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// アトリビュート
public:
	NOTIFYICONDATA m_stNtfyIcon;
	CSystemTray m_TrayIcon;

private:
	CTPerror te;
	BOOL InitSkypeAccess(void);	// スカイプアクセスオブジェクトの生成

// オペレーション
public:
	BOOL ConnectSkype(void);	// スカイプ接続

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CExtStatusControlBar  m_wndStatusBar;
	CReBar      m_wndReBar;
public:  // コントロール バー用メンバ
	CMainTool   m_wndToolBar;
	CDialogBar  m_wndDlgBar;

public:
	TVisualFramework m_Framework;
	TVisualObject *m_pMainTab;
	TVisualObject *m_pTabInbox;
	TVisualObject *m_pTabSetup;
	TVisualObject *m_pMsgTree;
	TVisualObject *m_pRsfTree;
	TVisualObject *m_pNestInbox;
	TVisualObject *m_pNestSetup;
	TVisualObject *m_pMsgList;
	TVisualObject *m_pRsfList;
	TVisualObject *m_pTabMsg;
	TVisualObject *m_pTabRsf;
	TVisualObject *m_pMsgForm;
	TVisualObject *m_pRsfSimpleForm;
	TVisualObject *m_pMsgFolderForm;
	TVisualObject *m_pRsfFolderForm;

	HICON m_hIcon;			// 通常のタスクトレイアイコン
	HICON m_hIconDis;		// 接続不可のタスクトレイアイコン
	HICON m_hIconNew;		// 新着ありのタスクトレイアイコン

	int m_flgEnable;		// 応答処理の有効/無効フラグ
	int m_flgAutoEnable;	// 自動録音処理の有効/無効フラグ
	ST_TIMER m_stApp;			// APP動作確認タイマ
	ST_TIMER m_stConnect;		// スカイプ接続待ちタイマ
	ST_TIMER m_stAnswerWait;	// 留守番応答待ちタイマ 
	ST_TIMER m_stRecordMax;		// 留守番録音MAX時間チェックタイマ 
	ST_TIMER m_stPlayChk;		// 応答メッセージ再生終了待ちタイマ 

// 生成されたメッセージ マップ関数
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPopupOpen();
	afx_msg void OnPopupStop();
	afx_msg void OnPopupEnable();
	afx_msg void OnPopupAutoEnable();
	afx_msg void OnPopupHotkey();
	afx_msg void OnPopupHelp();
	afx_msg void OnPopupSkype();
	afx_msg void OnPopupClose();
	afx_msg void OnClose();
	afx_msg void OnUpdatePopupStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupAutoEnable(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnVRecordStart();
	afx_msg void OnRecordStart();
	afx_msg void OnRecordPause();
	afx_msg void OnRecordStop();
	afx_msg void OnRecordMod();
	afx_msg void OnRecordModDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnRecordModBoth();
	afx_msg void OnRecordModRcv();
	afx_msg void OnRecordModSnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	DWORD m_dwCookie;

	BOOL m_blHotKey;			// HotKeyの有効無効
	long m_lngNewMsg;			// 新着メッセージ管理変数
	BOOL ClearTmpData();		// TMPフォルダクリア

public:
	CComObject<CAccessEvents>* m_pAccessEvents;
	CAudio *m_pAudio;	// 音声操作クラス
	CVideo *m_pVideo;	// ビデオ操作クラス

	void PushRecordStop() { OnRecordStop(); }

	void UpdateForm();			// 画面の更新
	void InitTooltip();			// ツールチップの初期化
	BOOL StartAppTimer();		// APP動作確認タイマ開始
	BOOL StopAppTimer();		// APP動作確認タイマ終了
	BOOL StartConnectTimer();	// スカイプ接続待ちタイマ開始
	BOOL StopConnectTimer();	// スカイプ接続待ちタイマ終了
	BOOL StartAnswerWaitTimer();// 留守番応答待ちタイマ開始
	BOOL StopAnswerWaitTimer();	// 留守番応答待ちタイマ終了
	BOOL StartRecordMaxTimer();	// 留守番録音MAX時間チェックタイマ開始
	BOOL StopRecordMaxTimer();	// 留守番録音MAX時間チェックタイマ終了
	BOOL StartPlayChkTimer(long lngPlayTime);	// 応答メッセージ再生終了待ちタイマ開始
	BOOL StopPlayChkTimer();	// 応答メッセージ再生終了待ちタイマ終了
	long RepairTmpData(long lngChkHour);	// 未登録の一時データの修復
	BOOL IncNewMsgCnt(long lngCnt);		// 新着メッセージ数更新
	BOOL SetStatusText(CString str, int idx = 0) { return m_wndStatusBar.SetPaneText(idx, str, TRUE); }
	void RegHotKey();			// HotKeyの登録
	void UnregHotKey();			// HotKeyの解除
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__CD11421F_56D8_4EB4_9104_11F0891E2316__INCLUDED_)
