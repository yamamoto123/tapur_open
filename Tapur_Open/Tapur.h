// Tapur.h : TAPUR アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_TAPUR_H__55A782FF_7AB5_41FF_BA17_12E16C5027BB__INCLUDED_)
#define AFX_TAPUR_H__55A782FF_7AB5_41FF_BA17_12E16C5027BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Tapur_i.h"
#include "registry.h"
#include "potm.h"
#include "MainFrm.h"
#include "SettingDialog.h"

// ウィンドウクラス名
#define MAIN_CLASS_NAME	"tTapurMain"

class CTapurApp;
extern CTapurApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTapurApp:
// このクラスの動作の定義に関しては Tapur.cpp ファイルを参照してください。
//

class CTapurApp : public CWinApp
{
public:
	CTapurApp();
	~CTapurApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTapurApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CTapurApp)
	afx_msg void OnAppHelp();
	afx_msg void OnLinkLame();
	afx_msg void OnLinkLicense();
	afx_msg void OnAppAbout();
	afx_msg void OnToolSetting();
	afx_msg void OnToolBackup();
	afx_msg void OnWavFolder();
	afx_msg void OnMp3Folder();
	afx_msg void OnVideoFolder();
	afx_msg void OnRepair();
	afx_msg void OnSoundRecoder();
	afx_msg void OnAudacity();
/*
	afx_msg void OnToolVRecord();
	afx_msg void OnToolRecord();
	afx_msg void OnToolRecPause();
	afx_msg void OnToolRecStop();
	afx_msg void OnToolRecMod();
*/
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
	BOOL LoadRegistry();	// レジストリのロード
	BOOL InitWinDev();		// Windowsデバイスの初期設定

private:
	HINSTANCE m_hResDLL;	// resource DLL handle

public:
// グローバル変数
	int m_intEnableLog;			// ログの有効/無効フラグ

	BOOL m_blSkypeConnect;		// 接続フラグ
	IAccessPtr m_pAccess;		// Skype操作クラス
//	BOOL m_blSenderCall;		// ユーザ発信フラグ
	long m_lngAppStatus;		// Tapur状態フラグ（1:会話録音、2:留守録、0:通常）

	CString m_strUsrId;			// ユーザ(スカイプID)
	CString m_strUsrName;		// ユーザ(スカイプ名)
//	long m_lngCallId;			// 通話ID
//	CString m_strPartnerId;		// 通話相手(スカイプID)
//	CString m_strPartnerName;	// 通話相手(スカイプ名)
	CString m_strDataFolder;	// データベースフォルダ
	CString m_strDBPath;		// データベースファイル
	CString m_strActiveFolder;	// カレントフォルダ
	long m_lngRdflId;			// カレント受信レコードID
	long m_lngRsflId;			// カレント設定レコードID

	BOOL OpenUrl(CString strUrl);	// URLのブラウザ表示
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TAPUR_H__55A782FF_7AB5_41FF_BA17_12E16C5027BB__INCLUDED_)
