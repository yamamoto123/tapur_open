#if !defined(AFX_MAINTOOL_H__D4101CEA_8F09_498A_ABBE_81F0B717582C__INCLUDED_)
#define AFX_MAINTOOL_H__D4101CEA_8F09_498A_ABBE_81F0B717582C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainTool.h : ヘッダー ファイル
#include "Label.h"
#include "Audio.h"
#include "Video.h"

/////////////////////////////////////////////////////////////////////////////
// CMainTool ウィンドウ

class CMainTool : public CToolBar
{
// コンストラクション
public:
	CMainTool();

// アトリビュート
private:
	CAudio *m_pAudio;	// 音声操作クラス
	CVideo *m_pVideo;	// ビデオ操作クラス
	long m_lngRecordTime;	// 録音・録画時間(sec)
	//DWORD m_dwRecMod;	// 録音対象状態

public:
	CLabel m_wndStatic;

// オペレーション
private:
	BOOL UpdateToolBar();		// ツールバーの更新

public:
	BOOL CreateStatic(CLabel& txtStatic,CString strText,UINT nIndex,UINT nID,int nWidth);
	BOOL Init(CWnd *pWnd);			// 初期化
	BOOL ReplaceIcon(int intPos, WORD iconId);	// ツールバーアイコンの変更
	BOOL UpadateRecordMod();		// 録音対象選択ボタンの更新
	BOOL VRecordStart(CString strDate, BOOL blChat, BOOL blSilent = FALSE);	// 録画開始イベント
	BOOL RecordStart(CString strDate, BOOL blChat, BOOL blSilent = FALSE);	// 録音開始イベント
	BOOL VRecordPause();			// 録画一時停止イベント
	BOOL RecordPause();				// 録音一時停止イベント
	BOOL VRecordStop(BOOL blChat);	// 録画終了イベント
	BOOL RecordStop(BOOL blChat);	// 録音終了イベント
	void RecordMod();				// 録音対象選択イベント

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainTool)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainTool();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainTool)
	afx_msg void OnUpdateVRecordStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordPause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordStop(CCmdUI* pCmdUI);
//	afx_msg void OnRecordModBoth();
//	afx_msg void OnRecordModRcv();
//	afx_msg void OnRecordModSnd();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINTOOL_H__D4101CEA_8F09_498A_ABBE_81F0B717582C__INCLUDED_)
