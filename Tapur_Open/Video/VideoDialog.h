#if !defined(AFX_VIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_VIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoDialog.h : ヘッダー ファイル
//
#include <imm.h> 
#include "TPerror.h"
#include "videograbbervc6.h"
#include "videograbberConst.h"

// Recording Status
#define VIDEO_RECORD_RUNNING	1
#define VIDEO_RECORD_SUCCESS	2
#define VIDEO_RECORD_FAILED		3

// for videoecho123
#define VECHO_POSX 200
#define VECHO_POSY 10
#define VECHO_SZX  280
#define VECHO_SZY  240


/////////////////////////////////////////////////////////////////////////////
// CVideoDialog ダイアログ

class CVideoDialog : public CDialog
{
// コンストラクション
public:
	CVideoDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CVideoDialog();      

// ダイアログ データ
	//{{AFX_DATA(CVideoDialog)
	enum { IDD = IDD_VIDEO };
	CVideoGrabberVC6	m_VideoGrab;
	//}}AFX_DATA

private:
	CTPerror te;
	int m_intRecordingStatus;	// 録音状態
	CString m_strPlayFile;		// 再生ファイル名
	CString m_strStreamPath;	// ストリーム再生作業フォルダ
	CString m_strSkypeId;		// Skype Name or ID

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVideoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CVideoDialog)
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnResizeVideograbber();
	afx_msg void OnRecordingStartedVideograbber(LPCTSTR FileName);
	afx_msg void OnRecordingCompletedVideograbber(LPCTSTR FileName, BOOL Success);
	afx_msg void OnFrameCaptureCompletedVideograbber(long FrameBitmapHandle, long BitmapWidth, long BitmapHeight, long FrameNumber, double FrameTime, long DestType, LPCTSTR FileName, BOOL Success, long FrameId);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_blVSetTop;	// ビデオ画面を前面表示フラグ

	void SetSkypeId(CString strSkypeId) {m_strSkypeId = strSkypeId; }
	void SetStreamPath(CString strPath) {m_strStreamPath = strPath; }
	int GetRecordingStatus() { return m_intRecordingStatus; }
	HWND GetVideoWndLocal(LPRECT pRect);
	HWND GetVideoWndLarge(LPRECT pRect);
	BOOL SetVideoWndLarge(LPRECT pRect, BOOL blSizeOnly = TRUE);
	BOOL SetTopVideoWndLocal(BOOL blSetTop);
	BOOL SetTopVideoWndLarge(BOOL blSetTop);
	//BOOL IsPlayStatus(TxPlayerState status);	// VideoPlayerの再生状態
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
